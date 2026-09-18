const fs = require("node:fs");
const vm = require("node:vm");
const assert = require("node:assert/strict");
const source = fs.readFileSync(
  require("node:path").join(__dirname, "../src/glfw_emscripten_touch.c"),
  "utf8",
);
function body(marker) {
  const at = source.indexOf(marker);
  const open = source.indexOf("{", at);
  return source.slice(open + 1, source.indexOf("\n});", open));
}
const pollBody = body("EM_JS(void, das_glfw_touch_poll_js");
const queryBody = body("EM_JS(double, das_glfw_touch_query_js");
const deviceBody = body("EM_JS(int, das_glfw_is_touch_device_js");

function fixture() {
  const listeners = {};
  const canvas = {
    addEventListener: (type, fn) => {
      listeners[type] = fn;
    },
    getBoundingClientRect: () => ({ left: 10, top: 20, width: 100, height: 50 }),
  };
  const Module = { canvas };
  const Browser = {
    calculateMouseCoords: (px, py) => ({ x: (px - 10) * 2, y: (py - 20) * 2 }),
  };
  const scope = { Module, Browser, document: {} };
  const poll = vm.runInNewContext(`(function() { ${pollBody} })`, scope);
  const query = vm.runInNewContext(
    `(function(field, i) { ${queryBody} })`,
    scope,
  );
  const fire = (type, touches) =>
    listeners[type]({
      changedTouches: touches.map(([identifier, pageX, pageY]) => ({
        identifier,
        pageX,
        pageY,
      })),
    });
  const snapshot = () => {
    const n = query(0, 0);
    const out = [];
    for (let i = 0; i < n; ++i)
      out.push({
        id: query(1, i),
        x: query(2, i),
        y: query(3, i),
        began: query(4, i),
        ended: query(5, i),
      });
    return out;
  };
  return { poll, query, fire, snapshot, listeners };
}

const f = fixture();
assert.equal(f.query(0, 0), 0, "no table before the first poll");
f.poll();
assert.deepEqual(
  Object.keys(f.listeners).sort(),
  ["touchcancel", "touchend", "touchmove", "touchstart"],
  "listeners installed by the first poll",
);

// Two fingers at once, both reported, in calculateMouseCoords space.
f.fire("touchstart", [
  [5, 20, 30],
  [6, 60, 40],
]);
f.poll();
assert.deepEqual(f.snapshot(), [
  { id: 5, x: 20, y: 20, began: 1, ended: 0 },
  { id: 6, x: 100, y: 40, began: 1, ended: 0 },
]);

// began is visible for exactly one frame; a move updates the position.
f.fire("touchmove", [[6, 70, 45]]);
f.poll();
assert.deepEqual(f.snapshot(), [
  { id: 5, x: 20, y: 20, began: 0, ended: 0 },
  { id: 6, x: 120, y: 50, began: 0, ended: 0 },
]);

// An ended finger is reported ended for one frame, then gone; the other stays.
f.fire("touchend", [[5, 20, 30]]);
f.poll();
assert.deepEqual(f.snapshot(), [
  { id: 5, x: 20, y: 20, began: 0, ended: 1 },
  { id: 6, x: 120, y: 50, began: 0, ended: 0 },
]);
f.poll();
assert.deepEqual(f.snapshot(), [
  { id: 6, x: 120, y: 50, began: 0, ended: 0 },
]);

// A tap that begins and ends between two polls is seen once, as began + ended.
f.fire("touchstart", [[7, 15, 25]]);
f.fire("touchend", [[7, 15, 25]]);
f.poll();
assert.deepEqual(f.snapshot(), [
  { id: 6, x: 120, y: 50, began: 0, ended: 0 },
  { id: 7, x: 10, y: 10, began: 1, ended: 1 },
]);
f.poll();
assert.deepEqual(f.snapshot(), [
  { id: 6, x: 120, y: 50, began: 0, ended: 0 },
]);

// An id reused by a new finger while the ended one is still reported starts a new record.
f.fire("touchend", [[6, 70, 45]]);
f.fire("touchstart", [[6, 30, 30]]);
f.poll();
assert.deepEqual(f.snapshot(), [
  { id: 6, x: 120, y: 50, began: 0, ended: 1 },
  { id: 6, x: 40, y: 20, began: 1, ended: 0 },
]);

// touchcancel ends like touchend; a move of an unknown id is ignored.
f.fire("touchmove", [[99, 1, 1]]);
f.fire("touchcancel", [[6, 30, 30]]);
f.poll();
assert.deepEqual(f.snapshot(), [
  { id: 6, x: 40, y: 20, began: 0, ended: 1 },
]);

function device(matches, points) {
  const scope = {
    window: { matchMedia: (q) => ({ matches: !!matches[q] }) },
    navigator: { maxTouchPoints: points },
  };
  return vm.runInNewContext(`(function() { ${deviceBody} })`, scope)();
}
assert.equal(device({ "(pointer: coarse)": true }, 5), 1, "phone");
assert.equal(device({ "(hover: none)": true }, 5), 1, "touch without hover");
assert.equal(device({ "(hover: hover)": true }, 0), 0, "desktop");
assert.equal(device({}, 10), 0, "touch laptop with a mouse is not a touch device");
assert.equal(
  vm.runInNewContext(`(function() { ${deviceBody} })`, {})(),
  0,
  "no window at all",
);

console.log(
  "PASS: install on first poll, two fingers, one-frame began/ended, same-frame tap, id reuse, cancel, device detection",
);
