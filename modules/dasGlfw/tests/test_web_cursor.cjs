const fs = require("node:fs");
const vm = require("node:vm");
const assert = require("node:assert/strict");
const source = fs.readFileSync(
  require("node:path").join(__dirname, "../src/glfw_emscripten_cursor.c"),
  "utf8",
);
const body = source.slice(source.indexOf("var id ="), source.indexOf("\n});"));
const CURSOR = 0x00033001,
  NORMAL = 0x00034001,
  DISABLED = 0x00034003;
function fixture(hasPointerLock) {
  const state = { inputModes: { [CURSOR]: NORMAL } },
    calls = [],
    removed = [];
  const canvas = { removeEventListener: (...args) => removed.push(args) };
  const document = {};
  if (hasPointerLock) {
    document.exitPointerLock = () => {};
    canvas.requestPointerLock = () => {};
  }
  const GLFW = {
    WindowFromId: (id) => (id === 7 ? state : null),
    onClickRequestPointerLock: () => {},
    setInputMode: (...args) => calls.push(args),
  };
  const fn = vm.runInNewContext(
    `(function(window, mode, value, cursor_mode, normal_mode, disabled_mode) { ${body} })`,
    { document, GLFW, Browser: { getCanvas: () => canvas } },
  );
  return {
    set: (window, mode, value) =>
      fn(window, mode, value, CURSOR, NORMAL, DISABLED),
    state,
    calls,
    removed,
    canvas,
  };
}
const unavailable = fixture(false);
unavailable.set(7n, CURSOR, NORMAL);
assert.equal(unavailable.state.inputModes[CURSOR], NORMAL);
assert.equal(unavailable.removed.length, 1);
unavailable.set(7n, CURSOR, DISABLED);
assert.equal(
  unavailable.state.inputModes[CURSOR],
  NORMAL,
  "unavailable capture is not reported as active",
);
assert.equal(unavailable.calls.length, 0);
unavailable.canvas.requestPointerLock = () => {};
unavailable.set(7n, CURSOR, DISABLED);
assert.equal(unavailable.calls.length, 0, "capture needs a release API too");
const missingRequest = fixture(true);
delete missingRequest.canvas.requestPointerLock;
missingRequest.set(7n, CURSOR, DISABLED);
assert.equal(missingRequest.calls.length, 0, "capture needs a request API too");
unavailable.set(7n, 4, 5);
assert.deepEqual(
  unavailable.calls,
  [[7, 4, 5]],
  "other modes delegate unchanged",
);
unavailable.set(99n, CURSOR, NORMAL);
assert.equal(unavailable.removed.length, 1, "invalid window is ignored");
const available = fixture(true);
available.set(7n, CURSOR, NORMAL);
available.set(7n, CURSOR, DISABLED);
assert.deepEqual(
  available.calls,
  [
    [7, CURSOR, NORMAL],
    [7, CURSOR, DISABLED],
  ],
  "supported Pointer Lock delegates unchanged",
);
console.log(
  "PASS: unavailable Pointer Lock APIs, normal cursor, desktop delegation, invalid window",
);
