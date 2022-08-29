"use strict";

const express  = require('express');
const EventEmitter = require("events").EventEmitter;
const addon = require("bindings")("piffero_addon");
const emitter = new EventEmitter();


//const app = express();

emitter.on("start", () => {
  console.log("### START ...");
});
emitter.on("data", (evt) => {
  console.log(evt);
});

emitter.on("end", () => {
  console.log("### END ###");
});

addon.callEmit(
  emitter.emit.bind(emitter),
  "ar"
);


/*app.listen(3000, () => {
  console.log("Open your browser");
}); */



