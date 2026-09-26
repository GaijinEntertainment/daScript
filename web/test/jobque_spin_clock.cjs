// EMXX=/path/to/em++ node web/test/jobque_spin_clock.cjs
const assert = require("node:assert/strict");
const fs = require("node:fs");
const os = require("node:os");
const path = require("node:path");
const { execFileSync } = require("node:child_process");

(async () => {
  const root = fs.mkdtempSync(path.join(os.tmpdir(), "jobque-spin-clock-"));
  try {
    const output = path.join(root, "clock.cjs");
    execFileSync(
      process.env.EMXX || "em++",
      [
        path.join(__dirname, "jobque_spin_clock.cpp"),
        "-std=c++17",
        "-O2",
        "--no-entry",
        "-sMEMORY64=2",
        "-sMODULARIZE=1",
        "-sENVIRONMENT=node",
        "-sASSERTIONS=2",
        "-sSAFE_HEAP=2",
        "-sSTACK_OVERFLOW_CHECK=2",
        '-sEXPORTED_FUNCTIONS=["_spin_clock_now","_spin_clock_reset","_spin_clock_expired"]',
        "-o",
        output,
      ],
      { stdio: "inherit" },
    );
    const module = new WebAssembly.Module(
      fs.readFileSync(path.join(root, "clock.wasm")),
    );
    const imports = WebAssembly.Module.imports(module);
    assert.ok(
      imports.some((i) => i.name === "emscripten_get_now"),
      "spin clock must use the direct f64 browser clock",
    );
    assert.ok(
      !imports.some((i) => i.name === "clock_time_get"),
      "spin clock must not use the WASI integer/memory clock bridge",
    );
    let now = 1800000000000.125,
      calls = 0;
    const api = await require(output)({
      instantiateWasm(imports, ready) {
        imports.env.emscripten_get_now = () => {
          calls++;
          return now;
        };
        const instance = new WebAssembly.Instance(module, imports);
        ready(instance, module);
        return instance.exports;
      },
    });
    assert.equal(
      api._spin_clock_now(),
      now,
      "large epoch retains fractional milliseconds",
    );
    calls = 0;
    api._spin_clock_reset(0);
    assert.equal(api._spin_clock_expired(0), 1);
    assert.equal(api._spin_clock_expired(1), 1);
    assert.equal(calls, 0, "zero window makes no host clock calls");
    api._spin_clock_reset(750);
    assert.equal(calls, 1);
    now += 0.5;
    assert.equal(api._spin_clock_expired(0), 0);
    now += 0.25;
    assert.equal(api._spin_clock_expired(0), 1);
    now += 1;
    assert.equal(
      api._spin_clock_expired(1),
      0,
      "work renews even after prior expiry",
    );
    now += 0.5;
    assert.equal(api._spin_clock_expired(0), 0);
    now += 0.25;
    assert.equal(api._spin_clock_expired(0), 1);
    assert.equal(calls, 6, "one host read per positive-window check");
    console.log(
      "PASS WASM direct-clock imports, fractional epoch, zero window, expiry and renewal",
    );
  } finally {
    fs.rmSync(root, { recursive: true, force: true });
  }
})().catch((error) => {
  console.error(error);
  process.exitCode = 1;
});
