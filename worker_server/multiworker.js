const Worker = require("worker_threads").Worker;

const responseMap = new Map();
const workerPool = [];
const workerPool1 = [];
let last_index = 0;
let last_index1 = 0;
let response_id = 1;

for (let index = 0; index < 1; index++) {
  const worker = new Worker("./worker_server/piffero-worker.js");
  workerPool1.push(worker);
  worker.on("message", (result) => {
    handler(result);
  });
}

for (let index = 0; index < 1; index++) {
  const worker = new Worker("./worker_server/addon-worker.js");
  workerPool.push(worker);
  worker.on("message", (result) => {
    handler(result);
  });
}

function handler(msg) {
  let currentResponse = responseMap.get(msg.id);
  currentResponse.setHeader("Content-Type", "application/json");
  currentResponse.send(msg.result);
  responseMap.delete(msg.id);
}

function getWorker() {
  last_index++;
  if (last_index === workerPool.length) {
    last_index = 0;
  }
  return workerPool[last_index];
}

function getWorker1() {
  last_index1++;
  if (last_index1 === workerPool1.length) {
    last_index1 = 0;
  }
  return workerPool1[last_index];
}

function getPath(req, res, file) {
  const path = req.params.path;
  const id_ = response_id++;
  responseMap.set(id_, res);
  let worker = getWorker();
  worker.postMessage({ path, id: id_, file: file });
}

function getPathPiffero(req, res, file) {
  const path = req.params.path;
  console.log(path);
  const id_ = response_id++;
  responseMap.set(id_, res);
  let worker = getWorker1();
  worker.postMessage({ path, id: id_, file: file });
}

exports.getPath = getPath;
exports.getPathPiffero = getPathPiffero;
