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

const handle_command = async event => {
    const ip = document.getElementById('ip').value.trim();
    const port = document.getElementById('port').value.trim();
    let arguments = args[event.id];
    arguments.host = ip;
    arguments.port = port;
    const result = await ajax('POST', '/command', { arguments });
    if(result.status >= 200 && result.status < 400) {
        console.log('success')
    }
    else {
        console.log('error')
    }

}