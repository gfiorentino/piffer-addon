"use strict";

const express  = require('express');
const EventEmitter = require("events").EventEmitter;
const addon = require("bindings")("piffero_addon");
const emitter = new EventEmitter();

const piffero = require("piffero");
const fs = require("fs");

// create read stream of json
const inputStream = fs.createReadStream("files/large-file.json");
// pass the stream to Piffero with the json path
/*const resultStream = piffero.Piffero.findByPath(
  inputStream,
  "$.a[500].payload.pull_request"
);

// trasform the stream to string and print into console
let resultString = "";
console.time("javascript");
resultStream.on("data", (chunk) => (resultString += chunk.toString()));
resultStream.on("end", () => {
  console.timeEnd("javascript");
 // console.log(resultString);
});*/


//const app = express();

console.time("c++");
let resultString2 = "";
emitter.on("start", () => {
  console.log("### START ...");
});
emitter.on("data", (evt) => {
  resultString2 += evt.toString();
 // console.log(evt);
});

emitter.on("end", () => {
  console.timeEnd("c++");
  console.log("### END ###");
   console.log(resultString2);
});

addon.callEmit(emitter.emit.bind(emitter), "a[5000].payload.pull_request");


/*app.listen(3000, () => {
  console.log("Open your browser");
}); */



