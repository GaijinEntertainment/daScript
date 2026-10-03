// Build web/test/idle_pop with daspkg release wasm, then pass its output directory.
// Counts actual WASI clock calls on the two waiting consumer threads. Node is
// sufficient for this regression; WebKit additionally retains the polling garbage.
const assert = require("node:assert/strict");
const fs = require("node:fs");
const os = require("node:os");
const path = require("node:path");
const { spawnSync } = require("node:child_process");
const input = process.argv[2];
assert.ok(
  input,
  "Usage: node web/test/idle_pop.cjs <compiled idle_pop directory>",
);
const root = fs.mkdtempSync(path.join(os.tmpdir(), "idle-pop-check-"));
try {
  let js = fs.readFileSync(path.join(input, "idle_pop.js"), "utf8");
  const needle = "function _clock_time_get(clk_id, ignored_precision, ptime) {";
  assert.ok(js.includes(needle), "WASI clock instrumentation point is present");
  js = js.replace(
    needle,
    needle +
      `
    if (ENVIRONMENT_IS_PTHREAD && ++idleClockCalls === 33) {
      console.error('FAIL idle consumer repeatedly polls the host clock');
    }
  `,
  );
  js =
    `var idleClockCalls = 0;
var Module = { print(text) {
  console.log(text);
  if (text === 'PASS idle pop wakes for payload and producer completion')
    setTimeout(() => process.exit(0), 0);
}};
` + js;
  fs.writeFileSync(path.join(root, "idle_pop.js"), js);
  fs.copyFileSync(
    path.join(input, "idle_pop.wasm"),
    path.join(root, "idle_pop.wasm"),
  );
  const result = spawnSync(process.execPath, [path.join(root, "idle_pop.js")], {
    encoding: "utf8",
    timeout: 30000,
  });
  process.stdout.write(result.stdout || "");
  process.stderr.write(result.stderr || "");
  assert.equal(
    result.error,
    undefined,
    "workers must complete without deadlock",
  );
  assert.equal(result.status, 0, "consumers complete successfully");
  assert.doesNotMatch(
    result.stdout + result.stderr,
    /FAIL idle consumer/,
    "blocking consumers must not poll the clock",
  );
  assert.match(
    result.stdout,
    /PASS idle pop wakes for payload and producer completion/,
  );
  console.log("PASS bounded idle clock calls and real channel/stream wakeups");
} finally {
  fs.rmSync(root, { recursive: true, force: true });
}
