const path = require('path');
const { exec } = require('child_process');

const express = require('express');
const morgan = require('morgan');

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

const server = app.listen(node_port, () => {
  console.log(`Server started on port ${process.env.EXTERNAL_PORT} in mode ${node_env}`);
});


const to_args = (options) => {
    let args = ''

    Object.keys(options).forEach(function (key) {
        var flag;
        var val = options[key];
        flag = key.replace(/[A-Z]/g, '_$&').toLowerCase();
        if(Array.isArray(val)) {
            val.forEach(item => {
                args += `--${flag}=${item} `
            })
        }
        else {
            args += `--${flag}=${val} `
        }
    });

    return args;
};