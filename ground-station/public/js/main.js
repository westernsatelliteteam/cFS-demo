const ajax = (method, url, body) => {
    return new Promise((resolve, reject) => {
        let xmlhttp = new XMLHttpRequest();
        xmlhttp.onreadystatechange = function() {
            if (this.readyState == XMLHttpRequest.DONE) {
                resolve(this)
            }
        };
        xmlhttp.open(method, url, true);
        xmlhttp.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
        if(method.toLowerCase() === 'get') xmlhttp.send();
        else xmlhttp.send(JSON.stringify(body));
    });
}

const handle_command = async (event, payloads) => {
    const ip = document.getElementById('ip').value.trim();
    const port = document.getElementById('port').value.trim();
    let arguments = {};
    Object.assign(arguments, args[event.id])
    arguments.host = ip;
    arguments.port = port;
    if(payloads != undefined) {
        payloads.forEach(payload => {
            const value = document.getElementById(payload.id).value
            if(arguments[payload.type] === undefined) arguments[payload.type] = value;
            else if(Array.isArray(arguments[payload.type])) {
                arguments[payload.type].push(value);
            }
            else {
                const existing_value = arguments[payload.type];
                arguments[payload.type] = [existing_value, value]
            }
        });
        }
    const result = await ajax('POST', '/command', { arguments });
    if(result.status >= 200 && result.status < 400) {
        console.log('success')
    }
    else {
        console.log('error')
    }

}

const open_tlm = (event) => {
    window.open(`/telemetry?app=${event.id}`, '_blank');
}

let console_output = []
const console_output_elem = document.getElementById('console-output')

const clear_console = () => {
    console_output = [];
    console_output_elem.innerHTML = '';
}

let socket = new WebSocket('ws://localhost:5001');
socket.onopen = async (event) => {
    console.log('Connected to websocket')
}

socket.onclose = (event) => {
    console.log('Disconnected from websocket')
}

socket.onmessage = (event) => {
    const packet = JSON.parse(event.data)
    if(packet.application_id == 8) {
        console_output.push(`${(new Date()).toISOString()}: ${packet.data.replace('\n', '')}`)
        console_output_elem.innerHTML = console_output.join('<br />')
        console_output_elem.scrollTop = console_output_elem.scrollHeight;
    }
};