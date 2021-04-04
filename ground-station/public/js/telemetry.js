const params = new URLSearchParams(window.location.search)

const tlm_table_elem = document.getElementById('telem_table');
const packets_received_elem = document.getElementById('packets-received')

const hex_to_ascii = (input) => {
    const hex = input.toString();
    let str = '';
    for (let i = 0; (i < hex.length && hex.substr(i, 2) !== '00'); i += 2)
        str += String.fromCharCode(parseInt(hex.substr(i, 2), 16));
    return str;
}

let packets_received = 0

if(params.has('app') && telemetry[params.get('app')] != undefined) {
    const tlm_def = telemetry[params.get('app')];
    console.log(`Loading telem for ${params.get('app')}`)
    document.title  = `Telemetry: ${tlm_def.full_name}`
    tlm_def.def.forEach((row_def, i) => {
        if(row_def.show) {
            let row = document.createElement('tr');
            tlm_table_elem.appendChild(row)

            let label = document.createElement('td');
            row.appendChild(label);
            row.childNodes[0].innerHTML = row_def.name

            let value = document.createElement('td');
            row.appendChild(value);
        }
    })

    let socket = new WebSocket('ws://localhost:5001');
    socket.onopen = async (event) => {
        console.log('Connected to websocket')
    }
    
    socket.onclose = (event) => {
        console.log('Disconnected from websocket')
    }
    
    socket.onmessage = (event) => {
        const packet = JSON.parse(event.data)
        if(packet.application_id === tlm_def.application_id) {
            packets_received++;
            packets_received_elem.innerHTML = packets_received;
            let row = 0;
            tlm_def.def.forEach((tlm_row, i) => {
                if(tlm_row.show) {
                    const hex = packet.data.substring(tlm_row.offset*2, tlm_row.offset*2 + tlm_row.size*2);
                    let value = parseInt(hex.match(/../g).reverse().join(''), 16) // swap endianness
                    if(tlm_row.display === 'ascii') {
                        value = hex_to_ascii(value)
                    }
                    else {
                        value = value.toString(tlm_row.display).trim()
                        if(tlm_row.display === 2) value = `0b${value}`
                        else if(tlm_row.display === 16) value = `0x${value}`
                        else if(tlm_row.display === 8) value = `0 ${value}`
                    }
                    tlm_table_elem.childNodes[row+2].childNodes[1].innerHTML = value;
                    row++;
                }
            })
        }
    };
}
else {
    console.log('error: invalid app')
}