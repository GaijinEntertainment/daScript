// Parent half of the playground's execution sandbox. Owns run-frame.html: one
// frame per program, thrown away afterwards.
//
// Why a frame at all — see the comment block in run-frame.html. Short version:
// runs used to share one wasm instance, so a program that threw inside init()
// left the daslang module registry un-torn-down and poisoned every later run in
// that page ("Module 'x' already created", a fatal). Restarting makes the clean
// slate structural instead of depending on every error path being tidy, gives a
// kill switch for a hung program, and disposes of the canvas + WebGL context.
//
// Loaded BEFORE main.js, which delegates its run path here.

(function () {
    "use strict";

    var FRAME_SRC = "run-frame.html";

    var host = null;        // element the frames live in
    var current = null;     // frame serving the run in flight (or the idle one)
    var spare = null;       // pre-warmed frame, so a Run click pays ~0 startup
    var onOutput = null;    // set by main.js: (text, color) => void
    var onExit = null;

    function makeFrame() {
        var rec = { el: document.createElement("iframe"), ready: false, used: false };
        rec.el.src = FRAME_SRC;
        rec.el.className = "pg-run-frame";
        rec.el.setAttribute("title", "daslang program output");
        rec.aspect = 3 / 4;     // until the program reports its drawing buffer
        // Hidden until the program asks for a WebGL context (or forever, for
        // text programs). Geometry is fit() 's job — see below.
        rec.el.style.cssText =
            "display:none;width:640px;height:480px;border:1px solid #444;" +
            "background:#000;margin-top:20px;margin-bottom:6px;";
        host.appendChild(rec.el);
        return rec;
    }

    // Height-led, the way the page has always actually presented the canvas:
    // take most of the viewport height and derive width from the program's
    // drawing-buffer aspect, giving back whatever the column cannot fit.
    //
    // Deliberately NOT clamped to 640. The old canvas code asked for a 640 box,
    // but the column's stylesheet outvoted it, so every visitor has been seeing
    // a ~1085px-wide render. A frame that honours the clamp is the first thing
    // to shrink it, and a 1024x768 program downscaled into 640 loses exactly the
    // thin, high-contrast detail — shadow edges, thin geometry — that reads as
    // "the render went washed out".
    var MAX_VIEWPORT_FRACTION = 0.8;

    function fit(rec) {
        if (!rec || !rec.el || rec.el.style.display === "none") return;
        // Measure the column, not the host — the host sizes to its content, so
        // asking it how wide it is would just echo the frame back.
        var column = host && host.parentElement;
        var maxHeight = Math.round(window.innerHeight * MAX_VIEWPORT_FRACTION);
        var width = Math.min(column ? column.clientWidth : 640, Math.round(maxHeight / rec.aspect));
        rec.el.style.setProperty("width", width + "px", "important");
        rec.el.style.setProperty("height", Math.round(width * rec.aspect) + "px", "important");
    }

    window.addEventListener("resize", function () { fit(current); });

    function destroy(rec) {
        if (!rec) return;
        // Removing the frame kills the wasm instance, its pthread pool, the
        // AudioContext, the canvas and its GL context in one move.
        if (rec.el && rec.el.parentNode) rec.el.parentNode.removeChild(rec.el);
    }

    function ensureSpare() {
        if (!spare) spare = makeFrame();
    }

    // Messages carry no frame identity, so match on source window to avoid
    // crosstalk between the outgoing frame and the pre-warmed one.
    window.addEventListener("message", function (ev) {
        if (ev.origin !== window.location.origin) return;
        var msg = ev.data;
        if (!msg || typeof msg !== "object") return;
        var rec = null;
        if (current && ev.source === current.el.contentWindow) rec = current;
        else if (spare && ev.source === spare.el.contentWindow) rec = spare;
        if (!rec) return;

        switch (msg.type) {
            case "ready":
                rec.ready = true;
                if (typeof window.updateButtonStates === "function") window.updateButtonStates();
                if (rec.pending) { var p = rec.pending; rec.pending = null; send(rec, p); }
                break;
            case "stdout":
                if (onOutput) onOutput(msg.text, "#ffffff");
                break;
            case "stderr":
                if (onOutput) onOutput(msg.text, "#ff9393");
                break;
            case "aborted":
                if (onOutput) onOutput("runtime aborted: " + msg.message, "#ff2d2d");
                break;
            case "canvas-visible":
                if (msg.width > 0 && msg.height > 0) rec.aspect = msg.height / msg.width;
                rec.el.style.display = "block";
                fit(rec);
                var out = document.getElementById("output");
                if (out) out.classList.add("with-canvas");
                break;
            case "exit":
                if (onExit) onExit(msg.code);
                break;
        }
    });

    function send(rec, payload) {
        if (!rec.ready) { rec.pending = payload; return; }
        rec.el.contentWindow.postMessage(payload, window.location.origin);
    }

    var PlaygroundRunner = {
        init: function (hostEl, outputFn, exitFn) {
            host = hostEl;
            onOutput = outputFn;
            onExit = exitFn || null;
            ensureSpare();
        },

        // Ready means "a frame is standing by", which is what gates the buttons.
        isReady: function () {
            return !!(spare && spare.ready) || !!(current && current.ready && !current.used);
        },

        // Throw away the frame that ran (if any) and promote the pre-warmed one.
        // Called before every run, and by Clear.
        reset: function () {
            if (current) { destroy(current); current = null; }
            var out = document.getElementById("output");
            if (out) out.classList.remove("with-canvas");
            ensureSpare();
        },

        // files: { "main.das": "...", ... }  args: argv for callMain
        // assets: URLs fetched into MEMFS before the program starts
        run: function (files, args, assets) {
            this.reset();
            current = spare;
            spare = null;
            current.used = true;
            send(current, { type: "run", files: files, args: args, assets: assets || [] });
            // Warm the next one while this program runs, so the following Run
            // click does not pay startup either.
            ensureSpare();
        },
    };

    window.PlaygroundRunner = PlaygroundRunner;
})();
