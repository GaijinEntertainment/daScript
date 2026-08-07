// Browser-side probe, installed into every page and frame of a context via
// playwright's addInitScript — before any page script runs, so nothing races
// the program's own start-up.
//
// `installProbe` is serialized and evaluated in the browser: it must stay
// self-contained (no imports, no closure over module state) and touch no node
// API. Its only export to the runner is `window.__dwv`.

export function installProbe(opts) {
    const state = {
        raf: 0,          // the frame's own paint loop — 0 means wedged or throttled
        draws: 0,        // the PROGRAM's draw calls: a dead emscripten loop stops these
        gl: false,       // a WebGL context was created
        audio: false,    // an AudioContext was created
        ran: false,      // this frame is the one the playground handed a program to
        glErrors: [],    // only populated when opts.pollGl (pages with no watcher of their own)
    };
    window.__dwv = state;

    (function tick() {
        state.raf++;
        requestAnimationFrame(tick);
    })();

    // The playground hands the program to one run frame and pre-warms a spare;
    // both are run-frame.html, so the run message is what tells them apart.
    window.addEventListener('message', function (ev) {
        const m = ev.data;
        if (m && typeof m === 'object' && m.type === 'run') state.ran = true;
    });

    const DRAW_METHODS = [
        'drawArrays', 'drawElements', 'drawArraysInstanced',
        'drawElementsInstanced', 'drawRangeElements',
    ];

    // Patch the prototypes once rather than each returned context: emscripten
    // issues thousands of draws per frame and a per-instance wrapper chain is
    // the more expensive of the two.
    function patchProto(proto) {
        if (!proto || proto.__dwvPatched) return;
        proto.__dwvPatched = true;
        for (const name of DRAW_METHODS) {
            const original = proto[name];
            if (typeof original !== 'function') continue;
            proto[name] = function () {
                state.draws++;
                return original.apply(this, arguments);
            };
        }
    }
    patchProto(window.WebGLRenderingContext && window.WebGLRenderingContext.prototype);
    patchProto(window.WebGL2RenderingContext && window.WebGL2RenderingContext.prototype);

    const getContext = HTMLCanvasElement.prototype.getContext;
    HTMLCanvasElement.prototype.getContext = function (type) {
        const ctx = getContext.apply(this, arguments);
        if (ctx && /webgl/i.test(String(type))) {
            state.gl = true;
            // Artifact pages carry no glGetError watcher of their own, so poll
            // one here. NEVER on the playground: run-frame.html already polls,
            // and getError CLEARS the flag — two pollers would steal each
            // other's errors and both would under-report.
            if (opts && opts.pollGl) startGlPoll(ctx);
        }
        return ctx;
    };

    function startGlPoll(gl) {
        const NAMES = {
            0x0500: 'GL_INVALID_ENUM', 0x0501: 'GL_INVALID_VALUE', 0x0502: 'GL_INVALID_OPERATION',
            0x0505: 'GL_OUT_OF_MEMORY', 0x0506: 'GL_INVALID_FRAMEBUFFER_OPERATION',
        };
        (function poll() {
            try {
                const code = gl.getError();
                if (code) {
                    const text = 'GL error: ' + (NAMES[code] || '0x' + code.toString(16));
                    if (!state.glErrors.includes(text)) state.glErrors.push(text);
                }
            } catch (e) { /* context lost — the page's own errors will say so */ }
            requestAnimationFrame(poll);
        })();
    }

    for (const name of ['AudioContext', 'webkitAudioContext']) {
        const Original = window[name];
        if (typeof Original !== 'function') continue;
        const Patched = function () {
            state.audio = true;
            return Reflect.construct(Original, arguments, this.constructor || Patched);
        };
        Patched.prototype = Original.prototype;
        window[name] = Patched;
    }
}
