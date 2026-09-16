/* examples.js — daslang apps compiled to WebAssembly, played in-browser.
 *
 * Each game ships as a standalone compiled build (cross-compiled as wasm64 so host and
 * target pointer widths match, then lowered to a 32-bit memory at link time, so every
 * engine runs it). The compiled artifacts live at
 *   examples/<id>/<id>.{html,js,wasm}
 * and are loaded into an <iframe> player on demand.
 *
 * ?force=interp runs the same example interpreted instead (slower) — a QA path, not a
 * fallback any browser needs. */
(function () {
    'use strict';

    // ─── Data ───────────────────────────────────────────────────────────
    var REPO_BLOB = 'https://github.com/GaijinEntertainment/daScript/blob/master/web/examples/ui/samples/examples/';
    var REPO_ROOT = 'https://github.com/GaijinEntertainment/daScript/blob/master/';

    var DAS_EXAMPLES = [
        {
            id: 'arcanoid', name: 'Arkanoid', kind: 'brick-breaker',
            description: 'The classic block-breaker — paddle, ball, breakable rows, ' +
                'rendered in 3D straight through the WebGL2 backend.',
            tags: ['game', 'opengl', 'wasm'],
            controls: '← → or A D  paddle · space launch · F3 F4 scanline pitch',
            poster: 'files/examples/arcanoid-poster.png',
            aspect: 1024 / 768,           // native window — sizes the player so the game fills it
        },
        {
            id: 'pacman', name: 'Pac-Man', kind: 'maze chase',
            description: 'Maze navigation with classic ghost targeting, pellets and ' +
                'power-ups — entities driven by the daslang ECS, compiled to wasm.',
            tags: ['game', 'decs', 'wasm'],
            controls: 'arrow keys  ·  move',
            poster: 'files/examples/pacman-poster.png',
            aspect: 900 / 760,
        },
        {
            id: 'boulder-dash', name: 'Boulder Dash', kind: 'cave physics',
            description: 'Dig for diamonds, dodge the boulders — fireflies, butterflies, ' +
                'amoeba and magic walls on an 8 Hz scan-line cave, every cave generated ' +
                'from a seed. A daslang-live game, compiled to wasm.',
            tags: ['game', 'opengl', 'audio', 'wasm'],
            controls: 'arrows move · z/ctrl grab · space start',
            poster: 'files/examples/boulder-dash-poster.jpg',
            aspect: 1280 / 720,
        },
        {
            id: 'river_run', name: 'River Run', kind: 'river shooter',
            description: 'Fly a gunship down a canyon river — gunboats, jets, bridges ' +
                'and fuel depots, across ten sections that each carry their own weather. ' +
                'Shadow-mapped, with ambient occlusion, bloom and sun shafts, all rendered ' +
                'through the WebGL2 backend.',
            tags: ['game', 'opengl', 'decs', 'audio', 'wasm'],
            controls: 'arrows / A D steer · W S throttle · space fire · esc pause',
            poster: 'files/examples/river_run-poster.jpg',
            aspect: 1280 / 720,
        },
        {
            id: 'furier', name: 'Fourier Series', kind: 'imgui showcase',
            description: 'Epicycle vectors tracing a Fourier curve — a live OpenGL ' +
                'visualization driven by Dear ImGui controls. The whole ImGui surface ' +
                '(daslang theme, font, widgets) is compiled to WebAssembly and runs here.',
            tags: ['imgui', 'opengl', 'wasm'],
            controls: 'mouse · imgui widgets',
            poster: 'files/examples/furier-poster.png',
            aspect: 1024 / 1024,
            compiledOnly: true,
            playgroundSlug: 'furier',
            src: 'examples/graphics/furier_opengl_imgui_example.das',
        },
        {
            id: 'path_tracer_lab', name: 'Path Tracer Lab', kind: 'jobque + opengl showcase',
            description: 'Watch a path tracer fill in — pick CPU single-thread, the job ' +
                'queue, OS threads, or the GPU, and see the same scene render in parallel. ' +
                'daslang threads map to real Web Workers; the whole OpenGL + Dear ImGui ' +
                'surface is compiled to WebAssembly and runs here.',
            tags: ['imgui', 'opengl', 'jobque', 'threads', 'wasm'],
            controls: 'mouse · imgui widgets · pick a render mode',
            poster: 'files/examples/path_tracer_lab-poster.jpg',
            aspect: 512 / 512,
            compiledOnly: true,
            playgroundSlug: 'path_tracer_lab',
            src: 'examples/graphics/path_tracer_lab_opengl_imgui_example.das',
        },
        {
            id: 'physarum_lab', name: 'Physarum Lab', kind: 'threads + audio showcase',
            description: 'Millions of slime-mold agents sense and steer along their own ' +
                'trails, building emergent transport networks — partitioned across worker ' +
                'threads you control live (watch agents/sec climb as you add threads). daslang ' +
                'threads map to real Web Workers, and a strudel soundtrack swells as you stir the ' +
                'slime — OpenGL + Dear ImGui + audio, all compiled to WebAssembly.',
            tags: ['imgui', 'opengl', 'threads', 'audio', 'wasm'],
            controls: 'drag to attract the slime · imgui widgets · worker-thread slider',
            poster: 'files/examples/physarum_lab-poster.jpg',
            aspect: 1024 / 1024,
            compiledOnly: true,
            playgroundSlug: 'physarum_lab',
            src: 'examples/graphics/physarum_lab_opengl_imgui_example.das',
        },
    ];

    DAS_EXAMPLES.forEach(function (ex) {
        ex.gameUrl = 'examples/' + ex.id + '/' + ex.id + '.html';
        ex.srcUrl = ex.src ? (REPO_ROOT + ex.src) : (REPO_BLOB + ex.id + '/main.das');
        // playgroundSlug must name a data.json sample. A compiled-only card can still carry one:
        // the playground's threaded interpreter binds the native module its in-page player bundles.
        var pgSlug = ex.playgroundSlug || (ex.compiledOnly ? null : ex.id);
        ex.playgroundUrl = pgSlug ? ('/playground/?example=' + pgSlug) : null;
    });

    var FORCE = new URLSearchParams(window.location.search).get('force');
    var USE_COMPILED = FORCE !== 'interp';

    function esc(s) {
        return String(s == null ? '' : s)
            .replace(/&/g, '&amp;').replace(/</g, '&lt;')
            .replace(/>/g, '&gt;').replace(/"/g, '&quot;');
    }

    // ─── Card grid ──────────────────────────────────────────────────────
    function cardHTML(ex) {
        var tags = ex.tags.map(function (t) {
            return '<span class="forge-ex-tag">' + esc(t) + '</span>';
        }).join('');
        return '' +
            '<article class="forge-ex-card" data-id="' + esc(ex.id) + '" tabindex="0" role="button" aria-label="play ' + esc(ex.name) + '">' +
              '<div class="forge-ex-card__screen">' +
                '<img class="forge-ex-card__poster" src="' + esc(ex.poster) + '" alt="' + esc(ex.name) + ' screenshot" loading="lazy" />' +
                '<div class="forge-ex-card__scanlines" aria-hidden="true"></div>' +
                '<div class="forge-ex-card__hover"><span class="forge-ex-card__playbtn" aria-hidden="true">▶</span></div>' +
              '</div>' +
              '<div class="forge-ex-card__body">' +
                '<div class="forge-ex-card__titlerow">' +
                  '<h3 class="forge-ex-card__name">' + esc(ex.name) + '</h3>' +
                  '<span class="forge-ex-card__kind">' + esc(ex.kind) + '</span>' +
                '</div>' +
                '<p class="forge-ex-card__desc">' + esc(ex.description) + '</p>' +
                '<div class="forge-ex-card__tags">' + tags + '</div>' +
                '<div class="forge-ex-card__foot">' +
                  (ex.playgroundUrl
                    ? '<a class="forge-ex-card__pglink" href="' + esc(ex.playgroundUrl) + '">open in playground ↗</a>'
                    : '<a class="forge-ex-card__pglink" href="' + esc(ex.srcUrl) + '" target="_blank" rel="noopener">view source ↗</a>') +
                '</div>' +
              '</div>' +
            '</article>';
    }

    function renderGrid() {
        var list = document.getElementById('ex-list');
        if (!list) return;
        list.innerHTML = DAS_EXAMPLES.map(cardHTML).join('');

        DAS_EXAMPLES.forEach(function (ex) {
            var card = list.querySelector('.forge-ex-card[data-id="' + ex.id + '"]');
            if (!card) return;
            card.addEventListener('click', function (e) {
                // Let the "open in playground" link navigate instead of opening the player.
                if (e.target.closest('.forge-ex-card__pglink')) return;
                openPlayer(ex);
            });
            card.addEventListener('keydown', function (e) {
                if (e.key === 'Enter' || e.key === ' ') { e.preventDefault(); openPlayer(ex); }
            });
        });
    }

    // ─── Overlay player ─────────────────────────────────────────────────
    var activeOverlay = null;

    // Keys whose browser default is to scroll — a game reading them through the
    // wasm canvas should not also scroll the embedding page.
    var SCROLL_KEYS = { ArrowUp: 1, ArrowDown: 1, ArrowLeft: 1, ArrowRight: 1, ' ': 1, Spacebar: 1, PageUp: 1, PageDown: 1, Home: 1, End: 1 };

    function closePlayer() {
        if (!activeOverlay) return;
        // Removing the iframe tears down the wasm instance + its rAF loop.
        activeOverlay.parentNode && activeOverlay.parentNode.removeChild(activeOverlay);
        activeOverlay = null;
        document.removeEventListener('keydown', onKeydown);
        document.removeEventListener('fullscreenchange', onFsChange);
        if (document.fullscreenElement) document.exitFullscreen().catch(function () {});
    }

    function onKeydown(e) {
        if (e.key === 'Escape') closePlayer();
    }

    // The "⤢ fullscreen" button fullscreens the parent viewport element, which leaves
    // keyboard focus on the parent — a game whose input listener lives inside the iframe
    // goes deaf. Refocusing the frame is all this has to do now.
    //
    // It used to also CSS-stretch the frame's canvas to fill the screen, because the
    // backing store stayed at its original size and nothing resized it: a magnified
    // render, not a bigger one. That stretch is what desynced emscripten's cursor
    // mapping (clicks landing away from the UI), which is why it had to skip imgui
    // cards. dasGlfw's glfwPollEvents now reconciles the GLFW window size to the canvas
    // CSS box every frame, so entering fullscreen grows the box and the next frame
    // re-renders at the new size for real, at native resolution, for every card.
    function onFsChange() {
        var frame = document.getElementById('ex-frame');
        var vp = document.getElementById('ex-viewport');
        if (!frame) return;
        var entering = !!document.fullscreenElement && document.fullscreenElement === vp;
        try {
            var idoc = frame.contentDocument;
            var canvas = idoc && idoc.getElementById('canvas');
            if (entering && canvas && canvas.focus) canvas.focus();
        } catch (e) {}
        if (entering && frame.contentWindow) { try { frame.contentWindow.focus(); } catch (e) {} }
    }

    function runnerUrl(ex) {
        return USE_COMPILED ? ex.gameUrl : ('examples/_interp.html?g=' + encodeURIComponent(ex.id));
    }
    function viewportHTML(ex) {
        if (ex.compiledOnly && !USE_COMPILED) {
            return '<div class="forge-ex-player__fallback">' +
                     '<div class="forge-ex-player__fallback-glyph" aria-hidden="true">⚠</div>' +
                     '<div class="forge-ex-player__fallback-head">Compiled only</div>' +
                     '<p class="forge-ex-player__fallback-body">' + esc(ex.name) + ' bundles the Dear ImGui ' +
                       'module compiled to WebAssembly. The universal interpreter can’t bind a native module, so ' +
                       'this one has no interpreted form.</p>' +
                     '<a class="forge-ex-player__fallback-btn" href="' + esc(ex.srcUrl) + '" target="_blank" rel="noopener">view source ↗</a>' +
                   '</div>';
        }
        return '<iframe class="forge-ex-player__frame" id="ex-frame" title="' + esc(ex.name) + '" ' +
               'src="' + esc(runnerUrl(ex)) + '" allow="autoplay; fullscreen"></iframe>';
    }

    function openPlayer(ex) {
        closePlayer();

        var noInterp = ex.compiledOnly && !USE_COMPILED;
        var statusText = noInterp ? 'compiled only' : (USE_COMPILED ? 'loading…' : 'loading · interpreted');
        var statusClass = noInterp ? 'is-fallback' : 'is-loading';

        var overlay = document.createElement('div');
        overlay.className = 'forge-ex-overlay';
        overlay.innerHTML = '' +
          '<div class="forge-ex-player" role="dialog" aria-modal="true" aria-label="' + esc(ex.name) + ' player"' +
            ' style="--ex-aspect:' + (ex.aspect || (4 / 3)).toFixed(4) + '">' +
            '<div class="forge-ex-player__bar">' +
              '<div class="forge-ex-player__lights" aria-hidden="true"><span></span><span></span><span></span></div>' +
              '<span class="forge-ex-player__name">' + esc(ex.name) + '</span>' +
              '<span class="forge-ex-player__status ' + statusClass + '" id="ex-status">' +
                '<span class="forge-ex-player__dot"></span>' + statusText +
              '</span>' +
              '<div class="forge-ex-player__actions">' +
                '<button type="button" class="forge-ex-pill" data-act="restart">↻ restart</button>' +
                '<button type="button" class="forge-ex-pill" data-act="fullscreen">⤢ fullscreen</button>' +
                '<button type="button" class="forge-ex-pill forge-ex-pill--close" data-act="close" aria-label="close">✕</button>' +
              '</div>' +
            '</div>' +
            '<div class="forge-ex-player__viewport" id="ex-viewport">' + viewportHTML(ex) + '</div>' +
            '<div class="forge-ex-player__toolbar">' +
              '<span class="forge-ex-player__controls"><span class="forge-ex-player__controls-label">controls</span>&nbsp;&nbsp;' + esc(ex.controls) + '</span>' +
              '<div class="forge-ex-player__links">' +
                '<a href="' + esc(ex.srcUrl) + '" target="_blank" rel="noopener" class="forge-ex-player__link">view source ↗</a>' +
                (ex.playgroundUrl
                  ? '<a href="' + esc(ex.playgroundUrl) + '" class="forge-ex-player__link forge-ex-player__link--amber">open in playground ↗</a>'
                  : '') +
              '</div>' +
            '</div>' +
          '</div>';

        // Backdrop click closes; clicks inside the dialog do not.
        overlay.addEventListener('click', function (e) {
            if (e.target === overlay) closePlayer();
        });

        document.body.appendChild(overlay);
        activeOverlay = overlay;
        document.addEventListener('keydown', onKeydown);
        document.addEventListener('fullscreenchange', onFsChange);

        var player = overlay.querySelector('.forge-ex-player');
        player.addEventListener('click', function (e) {
            var btn = e.target.closest('[data-act]');
            if (!btn) return;
            var act = btn.getAttribute('data-act');
            if (act === 'close') closePlayer();
            else if (act === 'restart') {
                var f = document.getElementById('ex-frame');
                if (!f) return;           // no iframe (the compiled-only note) — nothing to restart
                f.src = f.src;
                setStatus(USE_COMPILED ? 'loading…' : 'loading · interpreted', 'is-loading');
            } else if (act === 'fullscreen') {
                var vp = document.getElementById('ex-viewport');
                if (vp && vp.requestFullscreen) vp.requestFullscreen().catch(function () {});
            }
        });

        var frame = overlay.querySelector('#ex-frame');
        if (frame) {
            frame.addEventListener('load', function () {
                setStatus(USE_COMPILED ? 'running' : 'running · interpreted', 'is-running');
                // Focus the game canvas inside the iframe so keyboard input flows, and
                // guard scroll-keys at runtime (belt-and-suspenders with the shell's own
                // guard — covers any already-deployed card built before that shell). The
                // game frame is same-origin, so both are reachable.
                try {
                    frame.contentWindow && frame.contentWindow.focus();
                    var idoc = frame.contentDocument;
                    var canvas = idoc && idoc.getElementById('canvas');
                    if (canvas && canvas.focus) canvas.focus();
                    if (frame.contentWindow) {
                        frame.contentWindow.addEventListener('keydown', function (e) {
                            if (SCROLL_KEYS[e.key]) e.preventDefault();
                        }, { passive: false });
                    }
                } catch (e) {}
            });
        }
    }

    function setStatus(text, cls) {
        var el = document.getElementById('ex-status');
        if (!el) return;
        el.className = 'forge-ex-player__status ' + cls;
        el.innerHTML = '<span class="forge-ex-player__dot"></span>' + esc(text);
    }

    // ─── Deep link (?example=arcanoid / ?g=pacman) ──────────────────────
    function autoOpenFromQuery() {
        var q = new URLSearchParams(window.location.search);
        var slug = q.get('example') || q.get('g');
        if (!slug) return;
        var ex = DAS_EXAMPLES.filter(function (e) { return e.id === slug; })[0];
        if (ex) openPlayer(ex);
    }

    document.addEventListener('DOMContentLoaded', function () {
        renderGrid();
        autoOpenFromQuery();
    });
})();
