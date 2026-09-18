// Every finger on the canvas, polled. Emscripten's GLFW forwards only the first
// finger, as the left mouse button; this keeps all of them, in the same
// coordinate space as glfwGetCursorPos (Browser.calculateMouseCoords).
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <emscripten/emscripten.h>
#include "glfw_emscripten_touch.h"

EM_JS_DEPS(das_glfw_touch_deps, "$Browser");

// modules/dasGlfw/ARCHITECTURE.md#touch-list
EM_JS(void, das_glfw_touch_poll_js, (), {
    var M = (typeof Module !== 'undefined') ? Module : null;
    if (!M) return;
    var T = M.dasTouch;
    if (!T) {
        var canvas = M.canvas || ((typeof document !== 'undefined') && document.getElementById('canvas'));
        if (!canvas) return;
        T = M.dasTouch = { list: [] };
        var find = function (id) {
            for (var i = 0; i < T.list.length; ++i) if (T.list[i].id === id && !T.list[i].endedPending && !T.list[i].ended) return i;
            return -1;
        };
        var coords = function (t) {
            if (typeof Browser !== 'undefined' && Browser.calculateMouseCoords) return Browser.calculateMouseCoords(t.pageX, t.pageY);
            var r = canvas.getBoundingClientRect();
            return { x: t.clientX - r.left, y: t.clientY - r.top };
        };
        canvas.addEventListener('touchstart', function (e) {
            for (var k = 0; k < e.changedTouches.length; ++k) {
                var t = e.changedTouches[k];
                var c = coords(t);
                T.list.push({ id: t.identifier, x: c.x, y: c.y, beganPending: true, began: false, endedPending: false, ended: false });
            }
        }, true);
        canvas.addEventListener('touchmove', function (e) {
            for (var k = 0; k < e.changedTouches.length; ++k) {
                var t = e.changedTouches[k];
                var i = find(t.identifier);
                if (i < 0) continue;
                var c = coords(t);
                T.list[i].x = c.x;
                T.list[i].y = c.y;
            }
        }, true);
        var ended = function (e) {
            for (var k = 0; k < e.changedTouches.length; ++k) {
                var i = find(e.changedTouches[k].identifier);
                if (i >= 0) T.list[i].endedPending = true;
            }
        };
        canvas.addEventListener('touchend', ended, true);
        canvas.addEventListener('touchcancel', ended, true);
        return;
    }
    var keep = [];
    for (var i = 0; i < T.list.length; ++i) {
        var t = T.list[i];
        if (t.ended) continue;
        t.began = t.beganPending;
        t.beganPending = false;
        t.ended = t.endedPending;
        keep.push(t);
    }
    T.list = keep;
});

// field is a DasGlfwTouchField; the JS body cannot see the C enum, so it spells the same values.
EM_JS(double, das_glfw_touch_query_js, (int field, int i), {
    var T = (typeof Module !== 'undefined') && Module.dasTouch;
    if (!T) return 0;
    if (field == 0) return T.list.length;
    var t = T.list[i];
    if (!t) return 0;
    if (field == 1) return t.id;
    if (field == 2) return t.x;
    if (field == 3) return t.y;
    if (field == 4) return t.began ? 1 : 0;
    if (field == 5) return t.ended ? 1 : 0;
    return 0;
});

EM_JS(int, das_glfw_is_touch_device_js, (), {
    if (typeof window === 'undefined' || typeof navigator === 'undefined') return 0;
    var mq = function (q) { return !!(window.matchMedia && window.matchMedia(q).matches); };
    var points = navigator.maxTouchPoints || 0;
    return (mq('(pointer: coarse)') || (points > 0 && mq('(hover: none)'))) ? 1 : 0;
});

void das_glfw_touch_poll(void) { das_glfw_touch_poll_js(); }
double das_glfw_touch_query(int field, int i) { return das_glfw_touch_query_js(field, i); }
int das_glfw_is_touch_device(void) { return das_glfw_is_touch_device_js(); }
