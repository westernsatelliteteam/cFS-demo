let socket;
let console_output = []

const console_output_elem = document.getElementById('console-output')

const connect = () => {
    socket = new WebSocket('ws://localhost:5001');
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
}

connect()