// node web/test/fileinfo_constructor.cjs <host-daslang> <threaded-wasm64-lib-dir>
const assert = require("node:assert/strict");
const fs = require("node:fs");
const os = require("node:os");
const path = require("node:path");
const { spawnSync } = require("node:child_process");
assert.ok(process.argv[2] && process.argv[3], "Pass the LLVM host compiler and threaded wasm64 library directory");
const compiler = path.resolve(process.argv[2]), libraries = path.resolve(process.argv[3]);
const temporary = fs.mkdtempSync(path.join(os.tmpdir(), "fileinfo-constructor-"));
function run(command, args) {
  const result = spawnSync(command, args, {cwd: temporary, encoding: "utf8", timeout: 180000});
  process.stdout.write(result.stdout || "");
  process.stderr.write(result.stderr || "");
  assert.equal(result.error, undefined);
  assert.equal(result.status, 0, `${path.basename(command)} must succeed`);
  return result.stdout;
}
try {
  const object = path.join(temporary, "fixture.o"), output = path.join(temporary, "fixture.js");
  const fixture = path.join(__dirname, "fileinfo_constructor", "constructor_fileinfo_threaded_startup_long_path.das");
  run(compiler, ["-no-module-cache", "-exe", "-output", object,
    fixture, "--",
    "--jit-target=wasm64-unknown-emscripten", "--jit-emit-object", "--jit-threads", "--jit-debug", "--jit-stack", "--jit-check-abi",
    `--jit-runtime-lib=${path.join(libraries, "liblibDaScript_runtime.a")}`]);
  run(process.env.EMXX || "em++", [object, path.join(libraries, "liblibDaScript_runtime.a"), path.join(libraries, "liblibUriParser.a"),
    "-o", output, "-O2", "-pthread", "-sMEMORY64=2", "-sPTHREAD_POOL_SIZE=2", "-sSAFE_HEAP=2", "-sASSERTIONS=2",
    "-fwasm-exceptions", "-sWASM_LEGACY_EXCEPTIONS=0", "-sEXIT_RUNTIME=1", "-Wl,--allow-multiple-definition"]);
  assert.match(run(process.execPath, [output]), /FILEINFO_CONSTRUCTOR_OK/);
  console.log("PASS optimized threaded startup initializes allocating file metadata safely");
} finally {
  fs.rmSync(temporary, {recursive: true, force: true});
}
