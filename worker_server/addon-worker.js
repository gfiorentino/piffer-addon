const { parentPort } = require("worker_threads");
const addon = require("bindings")("piffero_addon");
const EventEmitter = require("events").EventEmitter;

function get(jsonpath, id, file) {
  const emitter = new EventEmitter();
  emitter.on("end", () => {
    parentPort.postMessage({ result: "mamata", id: id });
  });
  addon.callEmit(emitter.emit.bind(emitter), jsonpath);
}

parentPort.on("message", (message) => {
  return get(message.path, message.id, message.file);
});
