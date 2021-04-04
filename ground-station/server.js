const path = require('path');
const { exec } = require('child_process');
const dgram = require('dgram');

const express = require('express');
const morgan = require('morgan');
const WebSocket = require('ws');

let docker_ip;

// app configuration
const DEBUG = false;
const REQUEST_DEBUG = false;
const node_port = 3000;
const node_env = process.env.NODE_ENV || 'development';
const app = express();

// middlewares
app.use(express.static(path.join(__dirname, 'public')));
app.use(express.urlencoded({ extended: true }));
app.use(express.json());
if(REQUEST_DEBUG) app.use(morgan('dev'));

const executable = path.join('.', 'tools', 'cFS-GroundSystem', 'Subsystems', 'cmdUtil', 'cmdUtil');

// routes
app.get('/', (req, res) => res.sendFile(path.join(__dirname, 'public', 'index.html')))
app.get('/telemetry', (req, res) => res.sendFile(path.join(__dirname, 'public', 'telemetry.html')))
app.post('/command', (req, res) => {
    const arguments = to_args(req.body.arguments);
    let command = `${executable} ${arguments}`
    if(DEBUG) console.log(`Command sent: ${JSON.stringify(arguments)}`)
    exec(command , (err, stdout, stderr) => {
            if (err) {
                console.log(err)
                return res.status(500).send()
            }
            else {
                return res.status(200).send();
            }
    });
})

const server = app.listen(node_port, async () => {
  console.log(`Server started on port ${process.env.EXTERNAL_PORT} in mode ${node_env}`);
  docker_ip = await get_docker_ip();
});


const to_args = (options) => {
    let args = ''

    Object.keys(options).forEach(async (key) => {
        var flag;
        var val = options[key];
        flag = key.replace(/[A-Z]/g, '_$&').toLowerCase();
        if(Array.isArray(val)) {
            val.forEach(item => {
                args += `--${flag}=${item} `
            })
        }
        else if(key.toLowerCase() === 'string') {
            args += `--${flag}="${val.length}:${val.value}" `
        }
        else if(key.toLowerCase() === 'ip') {
            val = val.trim()
            if(val.toLowerCase() === 'gnd') {
                val = docker_ip;
            }
            const ip = ip_to_hex(val)
            ip.forEach(long => args += `--long=0x${long} `)
        }
        else {
            args += `--${flag}=${val} `
        }
    });

    return args;
};

const ip_to_hex = (ip) => {
    let hex_equiv = [];
	for(let i = 0; i < ip.length; i++) {
        const hex = Number(ip.charCodeAt(i)).toString(16);
		hex_equiv.push(hex);
    }
    const hex_string = hex_equiv.join('')

    // convert to 8 8-byte (8 characters) array
    let output = hex_string.match(/.{1,8}/g);

    // pad final element to full 8 bytes
    for(let i = 0; i < 8 - output[output.length-1].length; i++) output[output.length-1] += '00';

	return output;
}

const get_docker_ip = () => {
    return new Promise((resolve, reject) => {
        exec(`awk 'END{print $1}' /etc/hosts`, (err, stdout, stderr) => {
            if(err) {
                reject(err);
            }
            else if(stderr) {
                reject(stderr);
            }
            else {
                resolve(stdout.trim());
            }
        });
    })
}

const tlm_in = dgram.createSocket('udp4');

tlm_in.on('message', (msg, rinfo) => {
    const decoded = decode_ccsds(msg)
    if(DEBUG) console.log(`Message received from ${decoded.application_id} (${rinfo.size})`)
    if(decoded.application_id === 8) decoded.data = decoded.data.toString('ascii')
    else decoded.data = decoded.data.toString('hex')
    wss.clients.forEach(function each(client) {
        if (client.readyState === WebSocket.OPEN) {
          client.send(JSON.stringify(decoded));
        }
    });
});

tlm_in.on('connect', (msg, rinfo) => {
    console.log(`Connected`)
});

tlm_in.on('listening', () => {
    const { address, port } = tlm_in.address();
    console.log(`Telemetry input listening on ${address}:${port}`)
  });
  
tlm_in.on('error', (err) => {
    console.error(err);
    process.exit(0);
});
  
tlm_in.bind('1235');


const wss = new WebSocket.Server({ port: 5001 });

wss.on('connection', (ws) => {
    console.log('Connected to frontend')
})

const decode_ccsds = (packet) => {
    // from https://public.ccsds.org/Pubs/133x0b2e1.pdf

    let decoded = {}

    // primary header
    const primary_header = packet.slice(0, 6);
    decoded.primary_header = primary_header.toString('hex')
    decoded.version = (primary_header[0] >>> 5) & 7;
    decoded.type = (primary_header[0] >>> 4) & 1;
    decoded.secondary_header_flag = (primary_header[0] >>> 3) & 1;
    decoded.application_id = primary_header.readUInt16BE(0) & 0x7FF;
    decoded.sequence_flags = primary_header[2] >>> 6;
    decoded.count = primary_header.readUInt16BE(2) & 0x3FFF;
    decoded.name = decoded.count;
    decoded.length = primary_header.readUInt16BE(4)

    // secondary header (implemented from CCSDS_ExtendedHeader_t defined in cFS' SB)
    if(decoded.secondary_header_flag == 1) {
        const secondary_header = packet.slice(6,16)
        decoded.secondary_header = secondary_header.toString('hex')
        // bytes 0-1
        decoded.subsystem_id = secondary_header.readUInt16BE(0) & 0x01FF;
        decoded.playback_flag = (secondary_header.readUInt16BE(0) & 0x200) >>> 9;
        decoded.little_endian_flag = (secondary_header.readUInt16BE(0) & 0x400) >>> 10;
        decoded.eds_version = (secondary_header[1] & 0xF800) >>> 11;
        decoded.time = secondary_header.readUIntBE(4, 6);

        // bytes 1-2
        decoded.system_id = secondary_header.readUInt16BE(1)

        decoded.data = packet.slice(16)
    }
    else {
        decoded.data = packet.slice(6)
    }
    return decoded;
}