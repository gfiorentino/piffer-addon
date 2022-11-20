const EventEmitter = require("events").EventEmitter;
const addon = require("bindings")("piffero_addon");

var Benchmark = require("benchmark");

var suite = new Benchmark.Suite();

const piffero = require("piffero");
const fs = require("fs");

// var JSON_PATH = '[5000].payload';
// var JSON_PATH = '[1].payload';

suite

  .add("c++ addon", {
    defer: true,
    maxTime: 5,
    fn: function (deferred) {
      const emitter = new EventEmitter();
      let resultString2 = "";

      emitter.on("data", (evt) => {
        resultString2 += evt.toString();
      });
      emitter.on("end", () => {
        deferred.resolve();
      });

      addon.callEmit(emitter.emit.bind(emitter), "a[5000].payload.pull_request.url");
    },
  })
  .add("Piffero (stream)", {
    defer: true,
    maxTime: 5,
    fn: function (deferred) {
      var result = piffero.Piffero.findByPath(
        fs.createReadStream("files/large-file.json"),
        "$.a[5000].payload.pull_request.url"
      );

      result.on("data", () => {});
      result.on("end", () => {
        deferred.resolve();
      });
    },
  })
  .on("complete", function () {
    console.log("###########################################");
    for (var i = 0; i < this.length; i++) {
      console.log(
        this[i].name +
          " " +
          this[i].hz +
          " ops/sec (" +
          this[i].stats.sample.length +
          " runs sampled)"
      );
    }
    console.log("Fastest is " + this.filter("fastest").map("name"));
    console.log("Slowest is " + this.filter("slowest").map("name"));
  })
  .run({ async: true });
