// EMXX=/path/to/em++ node web/test/performance_clock.cjs
const assert = require("node:assert/strict");
const fs = require("node:fs");
const os = require("node:os");
const path = require("node:path");
const { execFileSync } = require("node:child_process");
(async () => {
  const root = fs.mkdtempSync(path.join(os.tmpdir(), "performance-clock-"));
  try {
    const output = path.join(root, "clock.cjs");
    const fatal = path.join(root, "fatal.cpp");
    fs.writeFileSync(
      fatal,
      "#include <cstdlib>\nvoid das_fatal_log(const char*, ...) { std::abort(); }\nvoid os_debug_break() { std::abort(); }\n",
    );
    execFileSync(
      process.env.EMXX || "em++",
      [
        path.resolve(__dirname, "../../src/hal/performance_time.cpp"),
        fatal,
        "-I" + path.resolve(__dirname, "../../include"),
        "-std=c++17",
        "-D_EMSCRIPTEN_VER=1",
        "-DNDEBUG=1",
        "-O2",
        "--no-entry",
        "-sMEMORY64=2",
        "-sMODULARIZE=1",
        "-sENVIRONMENT=node",
        "-sASSERTIONS=2",
        "-sSAFE_HEAP=2",
        "-sSTACK_OVERFLOW_CHECK=2",
        '-sEXPORTED_FUNCTIONS=["_ref_time_ticks","_get_time_usec","_get_time_nsec","_ref_time_delta_to_usec"]',
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
      "timing uses the direct floating-point host clock",
    );
    assert.ok(
      !imports.some((i) => i.name === "clock_time_get"),
      "timing must not allocate through the integer/memory clock bridge",
    );
    let now = 0,
      reads = 0;
    const api = await require(output)({
      instantiateWasm(imports, ready) {
        imports.env.emscripten_get_now = () => {
          reads++;
          return now;
        };
        const instance = new WebAssembly.Instance(module, imports);
        ready(instance, module);
        return instance.exports;
      },
    });
    for (now of [
      0, 0.0000005, 1.0000005, 4503599627.370497, 1800000000000.125,
    ]) {
      assert.equal(
        api._ref_time_ticks(),
        BigInt(Math.round(now * 1e6)),
        "nanoseconds preserve the host clock's rounding",
      );
    }
    now = 1234.125;
    const start = api._ref_time_ticks();
    now += 0.75;
    assert.equal(api._get_time_nsec(start), 750000n);
    assert.equal(api._get_time_usec(start), 750);
    assert.equal(api._ref_time_delta_to_usec(1234567n), 1234n);
    assert.equal(
      reads,
      8,
      "each timestamp/elapsed query reads the host once; delta conversion reads none",
    );
    console.log(
      "PASS WASM timing imports, nanosecond rounding, large epoch and elapsed conversions",
    );
  } finally {
    fs.rmSync(root, { recursive: true, force: true });
  }
})().catch((e) => {
  console.error(e);
  process.exitCode = 1;
});
