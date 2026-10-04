// Run a compiled WASM assertion fixture and require its completion sentinel.
const assert = require("node:assert/strict");
const fs = require("node:fs");
const os = require("node:os");
const path = require("node:path");
const { spawnSync } = require("node:child_process");
module.exports = function runWasmAssertions(input, name, sentinel) {
  assert.ok(input, "Pass the compiled WASM package directory");
  const root = fs.mkdtempSync(path.join(os.tmpdir(), "wasm-assertions-"));
  try {
      const js = fs.readFileSync(path.join(input, `${name}.js`), "utf8");
      fs.writeFileSync(path.join(root, `${name}.js`), `var Module = { print(text) {
          console.log(text);
          if (text === ${JSON.stringify(sentinel)}) setTimeout(() => process.exit(0), 0);
      }};\n` + js);
      fs.copyFileSync(path.join(input, `${name}.wasm`), path.join(root, `${name}.wasm`));
      const result = spawnSync(process.execPath, [path.join(root, `${name}.js`)], {
          encoding: "utf8", timeout: 30000,
      });
      process.stdout.write(result.stdout || "");
      process.stderr.write(result.stderr || "");
      assert.equal(result.error, undefined, "fixture must finish before timeout");
      assert.equal(result.status, 0, "WASM assertions must succeed");
      assert.ok(result.stdout.includes(sentinel), "WASM assertions must reach completion");
  } finally {
      fs.rmSync(root, { recursive: true, force: true });
  }

};
