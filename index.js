"use strict";

const express  = require('express');
const EventEmitter = require("events").EventEmitter;
const addon = require("bindings")("piffero_addon");
const emitter = new EventEmitter();
const fs = require('fs');
const { emit } = require('process');
const piffero = require("piffero");



//const app = express();
let resultString2 = "";
  console.time("c++");
   console.log("### start ###");
emitter.on("start", () => {

});
emitter.on("data", (evt) => {
   resultString2 += evt.toString();
});

emitter.on("end", () => {
  console.timeEnd("c++");
  console.log(resultString2);
 console.log("### END ###");
});

let _stream =  fs.createReadStream("files/large-file.json");
 addon.callEmit(
   emitter.emit.bind(emitter),
   "a[7650].payload.pull_request",
   _stream
 );


// create read stream of json
const inputStream = fs.createReadStream("files/large-file.json");
// pass the stream to Piffero with the json path
const resultStream = piffero.Piffero.findByPath(
  inputStream,
  "$.a[7650].payload.pull_request.url"
);

// trasform the stream to string and print into console
 let resultString = "";
  console.time("javascript");
resultStream.on("data", (chunk) => (resultString += chunk.toString()));
resultStream.on("end", () => {
   console.timeEnd("javascript");
   console.log(resultString);
});






