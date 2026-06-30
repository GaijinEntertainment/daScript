/* examples.js — daslang apps compiled to WebAssembly, played in-browser.
 *
 * Each game ships as a standalone wasm64 build (host == target pointer width,
 * so the cross-compile is layout-correct). The compiled artifacts live at
 *   examples/<id>/<id>.{html,js,wasm}
 * and are loaded into an <iframe> player on demand.
 *
 * wasm64 (memory64) is Chrome/Edge/Firefox 133+. On engines without it
 * (Safari, iOS) we fall back to running the same example interpreted in the
 * playground — slower, but universal. */
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
            controls: '← →  paddle · space launch',
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
            id: 'furier', name: 'Fourier Series', kind: 'imgui showcase',
            description: 'Epicycle vectors tracing a Fourier curve — a live OpenGL ' +
                'visualization driven by Dear ImGui controls. The whole ImGui surface ' +
                '(daslang theme, font, widgets) is compiled to WebAssembly and runs here.',
            tags: ['imgui', 'opengl', 'wasm'],
            controls: 'mouse · imgui widgets',
            poster: 'files/examples/furier-poster.png',
            aspect: 1024 / 1024,
            // The in-page player is the dasImgui-bundling wasm64 build, so it is
            // memory64-only (no interpreted fallback in the iframe). The playground,
            // though, runs the threaded daslang_static interpreter — which now binds
            // dasImgui — so it can host this sample there: playgroundSlug names it.
            wasm64Only: true,
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
            // In-page player: threaded (-pthread) wasm64 bundling dasImgui, running
            // new_thread / JobQue on real Web Workers — memory64-only, and the page must
            // be crossOriginIsolated (coi-serviceworker.js). The playground hosts the
            // same sample on the threaded daslang_static interpreter (playgroundSlug),
            // which is likewise isolated (the worker drops the memory64 gate for /playground).
            wasm64Only: true,
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
            // Same wasm64-only / external-dasImgui story as the path tracer: the in-page player is
            // the threaded (-pthread) wasm64 build bundling dasImgui + dasAudio, on real Web Workers
            // and crossOriginIsolated (coi-serviceworker.js). The playground hosts the same sample on
            // the threaded daslang_static interpreter (playgroundSlug), which binds dasImgui and the
            // in-tree dasAudio/strudel.
            wasm64Only: true,
            playgroundSlug: 'physarum_lab',
            src: 'examples/graphics/physarum_lab_opengl_imgui_example.das',
        },
    ];

    DAS_EXAMPLES.forEach(function (ex) {
        ex.gameUrl = 'examples/' + ex.id + '/' + ex.id + '.html';
        ex.srcUrl = ex.src ? (REPO_ROOT + ex.src) : (REPO_BLOB + ex.id + '/main.das');
        // Examples registered as playground samples get an "open in playground" link.
        // playgroundSlug names the data.json sample to deep-link (?example=<slug>). The
        // ImGui showcases set it explicitly: they bind the external dasImgui module, so
        // their in-page player is wasm64-only, but the threaded daslang_static
        // interpreter now hosts them in the playground. A plain (non-wasm64-only) sample
        // defaults to its id; a wasm64-only card with no slug has no playground link.
        var pgSlug = ex.playgroundSlug || (ex.wasm64Only ? null : ex.id);
        ex.playgroundUrl = pgSlug ? ('playground/index.html?example=' + pgSlug) : null;
    });

    // ─── wasm64 (memory64) feature detection ────────────────────────────
    var HAS_WASM64 = (function () {
        try {
            // Validate a minimal module declaring a 64-bit (memory64) memory:
            // \0asm | version | memory section {count=1, flags=0x04 (memory64), min=1}.
            // More robust than `new WebAssembly.Memory({index:'i64'})`, which can
            // false-positive on engines that silently ignore the unknown descriptor
            // field and hand back a wasm32 memory. validate() parses the memory64
            // flag, so it is true only where the engine truly supports it. (Chrome/Edge/FF 133+.)
            return WebAssembly.validate(new Uint8Array([0, 0x61, 0x73, 0x6d, 1, 0, 0, 0, 5, 3, 1, 4, 1]));
        } catch (e) {
            return false;
        }
    })();

    // ?force=interp / ?force=wasm64 forces a path (for QA on a memory64 browser).
    var FORCE = new URLSearchParams(window.location.search).get('force');
    var USE_WASM64 = FORCE === 'wasm64' ? true : (FORCE === 'interp' ? false : HAS_WASM64);

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

    function closePlayer() {
        if (!activeOverlay) return;
        // Removing the iframe tears down the wasm instance + its rAF loop.
        activeOverlay.parentNode && activeOverlay.parentNode.removeChild(activeOverlay);
        activeOverlay = null;
        document.removeEventListener('keydown', onKeydown);
        if (document.fullscreenElement) document.exitFullscreen().catch(function () {});
    }

    function onKeydown(e) {
        if (e.key === 'Escape') closePlayer();
    }

    // wasm64 present → the compiled standalone build; else → the interpreted
    // runner (daslang_static.wasm, universal wasm32), same game source.
    function runnerUrl(ex) {
        return USE_WASM64 ? ex.gameUrl : ('examples/_interp.html?g=' + encodeURIComponent(ex.id));
    }
    function viewportHTML(ex) {
        // wasm64-only example on an engine without memory64: there is no interpreted
        // fallback (the external native module can't run in the interpreter), so show
        // a clear note instead of an iframe that would never load.
        if (ex.wasm64Only && !USE_WASM64) {
            return '<div class="forge-ex-player__fallback">' +
                     '<div class="forge-ex-player__fallback-glyph" aria-hidden="true">⚠</div>' +
                     '<div class="forge-ex-player__fallback-head">Needs a memory64 browser</div>' +
                     '<p class="forge-ex-player__fallback-body">' + esc(ex.name) + ' bundles the Dear ImGui ' +
                       'module compiled to wasm64 — it runs on Chrome, Edge or Firefox 133+ (engines with ' +
                       'WebAssembly memory64). The universal interpreter can’t bind a native module, so ' +
                       'this one has no interpreted fallback.</p>' +
                     '<a class="forge-ex-player__fallback-btn" href="' + esc(ex.srcUrl) + '" target="_blank" rel="noopener">view source ↗</a>' +
                   '</div>';
        }
        return '<iframe class="forge-ex-player__frame" id="ex-frame" title="' + esc(ex.name) + '" ' +
               'src="' + esc(runnerUrl(ex)) + '" allow="autoplay; fullscreen"></iframe>';
    }

    function openPlayer(ex) {
        closePlayer();

        var needs64 = ex.wasm64Only && !USE_WASM64;
        var statusText = needs64 ? 'needs memory64' : (USE_WASM64 ? 'loading…' : 'loading · interpreted');
        var statusClass = needs64 ? 'is-fallback' : 'is-loading';

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

        var player = overlay.querySelector('.forge-ex-player');
        player.addEventListener('click', function (e) {
            var btn = e.target.closest('[data-act]');
            if (!btn) return;
            var act = btn.getAttribute('data-act');
            if (act === 'close') closePlayer();
            else if (act === 'restart') {
                var f = document.getElementById('ex-frame');
                if (!f) return;           // no iframe (e.g. memory64-only note) — nothing to restart
                f.src = f.src;
                setStatus(USE_WASM64 ? 'loading…' : 'loading · interpreted', 'is-loading');
            } else if (act === 'fullscreen') {
                var vp = document.getElementById('ex-viewport');
                if (vp && vp.requestFullscreen) vp.requestFullscreen().catch(function () {});
            }
        });

        var frame = overlay.querySelector('#ex-frame');
        if (frame) {
            frame.addEventListener('load', function () {
                setStatus(USE_WASM64 ? 'running' : 'running · interpreted', 'is-running');
                // Focus the game canvas inside the iframe so keyboard input flows.
                try { frame.contentWindow && frame.contentWindow.focus(); } catch (e) {}
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
