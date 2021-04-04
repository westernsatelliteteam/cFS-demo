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
        console.log(packet.application_id)
    };
}

connect()