.pragma library
function request(verb, BASE, endpoint, obj, cb) {
    var xhr = new XMLHttpRequest()
    xhr.onreadystatechange = function() {
        if(xhr.readyState === XMLHttpRequest.DONE) {
            if (xhr.status === 200) {
                if(cb) {
                    var res = JSON.parse(xhr.responseText.toString())
                    res.error = null;
                    cb(res)
                }
            } else {
                console.log('Ошибка в запросе: ' + verb + ' ' + BASE + (endpoint ? '/' + endpoint : ''));
                var errorResponse;
                try {
                    errorResponse = JSON.parse(xhr.responseText.toString());
                } catch (err) {
                    errorResponse = {error: "error"};
                }
                cb(errorResponse);
            }
        }
    }
    xhr.open(verb, BASE + (endpoint ? '/' + endpoint : ''))
    xhr.setRequestHeader('Content-Type', 'application/json')
    xhr.setRequestHeader('Accept', 'application/json')
    var data = obj ? JSON.stringify(obj) : ''
    xhr.send(data)
}