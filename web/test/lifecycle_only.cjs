// No script main: verify init, update and shutdown all run in order.
const assert = require("node:assert/strict");
const fs = require("node:fs");
const os = require("node:os");
const path = require("node:path");
const { spawnSync } = require("node:child_process");
require("./wasm_assertions.cjs")(process.argv[2], "lifecycle_only", "LIFECYCLE_ONLY_OK");
const compiler = process.argv[3];
assert.ok(compiler, "Pass the host daslang path to verify rejected entries too");
const temporary = fs.mkdtempSync(path.join(os.tmpdir(), "lifecycle-negative-"));
try {
  for (const [fixture, diagnostic] of [
    ["_missing_entry.das", /entrypoint `main\(\)` not found/],
    ["_unsafe_export.das", /local 'hold'.*collector must walk/],
  ]) {
    const artifact = path.join(temporary, fixture + ".o");
    const result = spawnSync(compiler, ["-exe", "-output", artifact,
      path.join(__dirname, "lifecycle_only", fixture), "--",
      "--jit-target=wasm64-unknown-emscripten", "--jit-emit-object"], {
      encoding: "utf8", timeout: 60000,
    });
    assert.equal(result.error, undefined);
    assert.match(result.stdout + result.stderr, diagnostic);
    assert.equal(fs.existsSync(artifact), false, "rejected entry must not emit an artifact");
  }
  console.log("PASS lifecycle selection retains native-entry and exported-function checks");
} finally {
  fs.rmSync(temporary, { recursive: true, force: true });
}
