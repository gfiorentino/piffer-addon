const { parentPort} = require('worker_threads');
const Piffero = require('piffero').Piffero;

const fs  =  require('fs');
function get(jsonpath, id, file) {
    stream = fs.createReadStream('./files/' + file);
    const calback = result => parentPort.postMessage({result: result, id:id} );
    Piffero.findAsString(calback, stream, jsonpath);
}
 parentPort.on('message', (message) => {
     return get(message.path, message.id, message.file);
 })
 