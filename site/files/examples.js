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

    var DAS_EXAMPLES = [
        {
            id: 'arcanoid', name: 'Arkanoid', kind: 'brick-breaker',
            description: 'The classic block-breaker — paddle, ball, breakable rows, ' +
                'rendered in 3D straight through the WebGL2 backend.',
            tags: ['game', 'opengl', 'wasm'],
            controls: '← →  paddle · space launch',
            poster: 'files/examples/arcanoid-poster.png',
        },
        {
            id: 'pacman', name: 'Pac-Man', kind: 'maze chase',
            description: 'Maze navigation with classic ghost targeting, pellets and ' +
                'power-ups — entities driven by the daslang ECS, compiled to wasm.',
            tags: ['game', 'decs', 'wasm'],
            controls: 'arrow keys  ·  move',
            poster: 'files/examples/pacman-poster.png',
        },
    ];

    DAS_EXAMPLES.forEach(function (ex) {
        ex.gameUrl = 'examples/' + ex.id + '/' + ex.id + '.html';
        ex.srcUrl = REPO_BLOB + ex.id + '/main.das';
        ex.playgroundUrl = 'playground/index.html?example=' + ex.id;
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
                  '<a class="forge-ex-card__pglink" href="' + esc(ex.playgroundUrl) + '">open in playground ↗</a>' +
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
        return '<iframe class="forge-ex-player__frame" id="ex-frame" title="' + esc(ex.name) + '" ' +
               'src="' + esc(runnerUrl(ex)) + '" allow="autoplay; fullscreen"></iframe>';
    }

    function openPlayer(ex) {
        closePlayer();

        var statusText = USE_WASM64 ? 'loading…' : 'loading · interpreted';
        var statusClass = 'is-loading';

        var overlay = document.createElement('div');
        overlay.className = 'forge-ex-overlay';
        overlay.innerHTML = '' +
          '<div class="forge-ex-player" role="dialog" aria-modal="true" aria-label="' + esc(ex.name) + ' player">' +
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
                '<a href="' + esc(ex.playgroundUrl) + '" class="forge-ex-player__link forge-ex-player__link--amber">open in playground ↗</a>' +
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
                if (f) f.src = f.src;
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
