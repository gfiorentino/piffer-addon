

const Piffero = require("piffero").Piffero;
const fs = require("fs");
const express = require("express");
const app = express();
const multi = require("./multiworker");

app.use((req, res, next) => {
  const start = Date.now();
  res.on("finish", () =>
    console.log(req.path + " in", Date.now() - start, "ms")
  );
  next();
});


app.get("/worker/:path", (req, res) => {
    multi.getPath(req, res, "large-file.json"); //11350
});

app.get("/worker-piffero/:path", (req, res) => {
    multi.getPathPiffero(req, res, "large-file.json"); //11350
});


app.listen(3000, () => {
  console.log("Open your browser at: http://localhost:3000/$[1].payload");
});