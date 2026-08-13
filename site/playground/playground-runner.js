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

    var FRAME_SRC = "run-frame.html?v=3";

    var host = null;        // element the frames live in
    var current = null;     // frame serving the run in flight (or the idle one)
    var spare = null;       // pre-warmed frame, so a Run click pays ~0 startup
    var onOutput = null;    // set by main.js: (text, color) => void
    var onExit = null;

    // A spare that aborts never becomes ready, and readiness is what gates the Run
    // button — so a single failed runtime fetch would leave the page dead until
    // reload. Rebuild it instead, but bounded: if the runtime is genuinely
    // unreachable, retrying forever just spawns frames.
    var spareAborts = 0;
    var MAX_SPARE_ABORTS = 3;
    var lastReviveAt = 0;
    var REVIVE_COOLDOWN_MS = 8000;

    // ── the one compiled runtime ─────────────────────────────────────────────
    // The runtime wasm (~40MB) is compiled HERE, once per page. Every frame asks
    // for this module over postMessage and instantiates it — instances share the
    // compiled machine code, so a Run click costs an instantiation, not a
    // compile. Without this, every frame (one per run, plus the pre-warmed
    // spare) compiled the whole runtime again; browsers budget compiled wasm
    // per PROCESS and reclaim it lazily, so a session of runs walked the
    // process to "wasm streaming compile failed: out of memory" — a wedge that
    // survives reloads (same process) until the tab is closed.
    var WASM_URL = "daslang_static.wasm";
    var wasmModulePromise = null;

    // A download that stops flowing must reject rather than hang: once a frame
    // has been acked, this promise is the only thing it is waiting on — nothing
    // else times out, so a silent stall would wedge the page in the exact shape
    // this file exists to remove. Idle-based, not total-time-based, so a slow
    // link that is still moving bytes is never cut off.
    var STALL_MS = 30000;

    function fetchRuntimeWithStallGuard() {
        return fetch(WASM_URL).then(function (r) {
            if (!r.ok) throw new Error("HTTP " + r.status + " fetching " + WASM_URL);
            if (!r.body || typeof TransformStream === "undefined") return r;
            var timer = null;
            function arm(controller) {
                clearTimeout(timer);
                timer = setTimeout(function () {
                    controller.error(new Error("runtime download stalled"));
                }, STALL_MS);
            }
            var guard = new TransformStream({
                start: function (c) { arm(c); },
                transform: function (chunk, c) { arm(c); c.enqueue(chunk); },
                flush: function () { clearTimeout(timer); },
            });
            // Headers ride along so compileStreaming still sees the wasm MIME type.
            return new Response(r.body.pipeThrough(guard), { headers: r.headers });
        });
    }

    function compileRuntime() {
        if (!wasmModulePromise) {
            wasmModulePromise = fetchRuntimeWithStallGuard()
                .then(function (r) { return WebAssembly.compileStreaming(r); })
                .catch(function (e) {
                    // The ladder the emscripten glue always had: streaming compile
                    // hard-requires the application/wasm MIME type, and losing the
                    // ArrayBuffer arm turned a warn-and-work server into a dead
                    // page. One retry also covers a mid-stream stall.
                    console.warn("wasm streaming compile failed, retrying as ArrayBuffer:", e);
                    return fetchRuntimeWithStallGuard()
                        .then(function (r) { return r.arrayBuffer(); })
                        .then(function (bytes) { return WebAssembly.compile(bytes); });
                })
                .catch(function (e) {
                    // Drop the failed promise so a revive() retries the compile —
                    // an OOM here is often transient (the old page's module not
                    // yet collected), and a pinned rejection would make it final.
                    wasmModulePromise = null;
                    throw e;
                });
        }
        return wasmModulePromise;
    }

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

    // Exposed, because the wasm engine's page frame stands in the same host and
    // must be sized by the same rule — a second copy of this drifts, and the
    // failure mode is silent (a frame that renders, just wrong).
    function fitElement(el, aspect) {
        if (!el) return;
        // Measure the column, not the host — the host sizes to its content, so
        // asking it how wide it is would just echo the frame back.
        var column = host && host.parentElement;
        var maxHeight = Math.round(window.innerHeight * MAX_VIEWPORT_FRACTION);
        var width = Math.min(column ? column.clientWidth : 640, Math.round(maxHeight / aspect));
        el.style.setProperty("width", width + "px", "important");
        el.style.setProperty("height", Math.round(width * aspect) + "px", "important");
    }

    function fit(rec) {
        if (!rec || !rec.el || rec.el.style.display === "none") return;
        fitElement(rec.el, rec.aspect);
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

    // ── fps badge ────────────────────────────────────────────────────────────
    // One badge for both engines. The number is measured inside whichever frame is
    // running (run-frame.html interpreted, the artifact shell in wasm mode) and
    // posted here, because only the frame can see the program's draw calls — the
    // wasm artifact is cross-origin and cannot be inspected from this side at all.
    // It stands over the run host so it reads as part of the picture.
    var badge = null;
    var badgeVisible = true;
    var badgeHideTimer = null;

    function ensureBadge() {
        if (badge || !host) return;
        if (getComputedStyle(host).position === "static") host.style.position = "relative";
        badge = document.createElement("div");
        badge.className = "pg-fps";
        badge.style.cssText =
            "position:absolute;top:26px;right:6px;z-index:5;pointer-events:none;" +
            "padding:2px 6px;border-radius:3px;background:rgba(0,0,0,.62);" +
            "color:#e8dcc0;font:600 11px/1.4 'JetBrains Mono',monospace;" +
            "letter-spacing:.02em;display:none;";
        host.appendChild(badge);
    }

    function renderBadge(text) {
        ensureBadge();
        if (!badge) return;
        if (text === null) { badge.style.display = "none"; return; }
        badge.textContent = text;
        badge.style.display = badgeVisible ? "block" : "none";
    }

    // A program that stops presenting (finished, wedged, torn down) must not leave a
    // stale number on screen implying it is still running.
    function noteFps(value) {
        if (!(value >= 0)) return;
        renderBadge(value >= 10 ? Math.round(value) + " fps" : value.toFixed(1) + " fps");
        if (badgeHideTimer) clearTimeout(badgeHideTimer);
        badgeHideTimer = setTimeout(function () { renderBadge(null); }, 2000);
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
            case "need-wasm-module":
                // Ack immediately: the compile can take seconds, and without an
                // ack the frame cannot tell "parent is compiling" from "parent
                // predates this protocol" (where it falls back to self-compile).
                (function (target) {
                    function deliver(payload) {
                        try { target.postMessage(payload, window.location.origin); } catch (e) { /* frame gone */ }
                    }
                    deliver({ type: "wasm-module-pending" });
                    compileRuntime().then(
                        function (mod) { deliver({ type: "wasm-module", module: mod }); },
                        function (e) { deliver({ type: "wasm-module", module: null, error: String((e && e.message) || e) }); }
                    );
                })(ev.source);
                break;
            case "ready":
                rec.ready = true;
                if (rec === spare) spareAborts = 0;
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
                if (rec === spare) {
                    destroy(spare);
                    spare = null;
                    if (++spareAborts <= MAX_SPARE_ABORTS) ensureSpare();
                    if (typeof window.updateButtonStates === "function") window.updateButtonStates();
                }
                break;
            case "canvas-visible":
                if (msg.width > 0 && msg.height > 0) rec.aspect = msg.height / msg.width;
                rec.el.style.display = "block";
                fit(rec);
                var out = document.getElementById("output");
                if (out) out.classList.add("with-canvas");
                break;
            case "fps":
                // Only the frame actually running a program may drive the badge; the
                // pre-warmed spare also ticks rAF and would otherwise report 0.
                if (rec === current) noteFps(msg.value);
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
            // Start the one compile now — the spare is about to ask for it.
            // Errors surface through the frame protocol, not here.
            compileRuntime().catch(function () {});
            ensureSpare();
        },

        // Ready means "a frame is standing by", which is what gates the buttons.
        isReady: function () {
            return !!(spare && spare.ready) || !!(current && current.ready && !current.used);
        },

        // Dead means the runner gave up: initialized, yet no frame standing by
        // and none loading. Distinct from "loading" (a spare exists, not ready
        // yet) — the Run button stays clickable in THIS state only, so the user
        // has a way to trigger revive(); reporting it as "still loading" hid
        // exactly that. (No `current` term: a current frame is always mid-run
        // or finished, never standing by.)
        isDead: function () {
            return !!host && !spare;
        },

        // A page whose spare kept aborting (MAX_SPARE_ABORTS) has no frame and,
        // without this, no way back short of a reload — which does not help when
        // the abort was the process's wasm memory, since that outlives reloads.
        // A Run click is the user's explicit "try again": rebuild the spare and
        // reset the abort budget. Cheap — a new frame reuses the compiled
        // module, and a failed parent compile retries because compileRuntime()
        // drops its promise on rejection. Returns whether it revived anything;
        // false means a spare is already standing (loading or ready), or a
        // revive just ran — the cooldown keeps a click storm on a structurally
        // dead page (no runtime to fetch at all) from stacking abort budgets.
        revive: function () {
            if (!host || spare) return false;
            var now = Date.now();
            if (now - lastReviveAt < REVIVE_COOLDOWN_MS) return false;
            lastReviveAt = now;
            spareAborts = 0;
            ensureSpare();
            if (typeof window.updateButtonStates === "function") window.updateButtonStates();
            return true;
        },

        // Size an element the way a run frame is sized. The wasm engine's page
        // frame lives in this same host and would otherwise have to re-derive
        // the column-measuring rule.
        fitElement: fitElement,

        // The wasm engine's page frame posts its own fps (cross-origin, so main.js
        // receives it there and forwards); the badge itself stays owned here.
        noteFps: noteFps,

        setFpsVisible: function (on) {
            badgeVisible = !!on;
            if (badge && !badgeVisible) badge.style.display = "none";
        },

        // Throw away the frame that ran (if any) and promote the pre-warmed one.
        // Called before every run, and by Clear. Clear on a dead page is a "try
        // again" gesture like Run, so it grants the same fresh abort budget its
        // new spare needs — and the buttons re-gate, since aliveness may have
        // just flipped.
        reset: function () {
            if (current) { destroy(current); current = null; }
            var out = document.getElementById("output");
            if (out) out.classList.remove("with-canvas");
            renderBadge(null);   // the run it described is gone
            spareAborts = 0;
            ensureSpare();
            if (typeof window.updateButtonStates === "function") window.updateButtonStates();
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
