"use strict";
const fs = require("node:fs");
const path = require("node:path");
const http = require("node:http");

exports.createWasmServer = function (root) {
  return http.createServer((req, res) => {
    if (req.url === "/favicon.ico") {
      res.writeHead(204);
      res.end();
      return;
    }
    let file;
    try {
      const relative = decodeURIComponent(
        new URL(req.url, "http://localhost").pathname,
      );
      file = path.resolve(root, "." + relative);
    } catch {
      res.writeHead(400);
      res.end();
      return;
    }
    if (
      !file.startsWith(root + path.sep) ||
      !fs.existsSync(file) ||
      !fs.statSync(file).isFile()
    ) {
      res.writeHead(404);
      res.end();
      return;
    }
    const types = {
      ".html": "text/html",
      ".js": "application/javascript",
      ".wasm": "application/wasm",
      ".json": "application/json",
      ".txt": "text/plain",
      ".md": "text/plain",
    };
    res.writeHead(200, {
      "Content-Type": types[path.extname(file)] || "application/octet-stream",
      "Cross-Origin-Opener-Policy": "same-origin",
      "Cross-Origin-Embedder-Policy": "require-corp",
      "Cache-Control": "no-store",
    });
    fs.createReadStream(file).pipe(res);
  });
};
