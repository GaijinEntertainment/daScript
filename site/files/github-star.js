/* Forge nav — GitHub star-count chip.
 *
 * Fills every [data-github-star] element with the repo's current star count,
 * compact-formatted (5.5k). Markup ships as a plain "GitHub" link; when the
 * API fetch fails (offline, ad-block, rate limit) the count never appears and
 * the link still works.
 * Storage: localStorage["daslang:gh:stars"] = {"n":<count>,"t":<ms>}, 24h TTL.
 * Shared by daslang.io and dasllama.io — staged into both sites' files/.
 */
(function () {
    'use strict';

    var REPO = 'GaijinEntertainment/daScript';
    var STORAGE_KEY = 'daslang:gh:stars';
    var TTL_MS = 24 * 60 * 60 * 1000;

    function compact(n) {
        if (n >= 1000) {
            var k = (n / 1000).toFixed(n >= 10000 ? 0 : 1);
            return k.replace(/\.0$/, '') + 'k';
        }
        return String(n);
    }

    function render(n) {
        var els = document.querySelectorAll('[data-github-star]');
        for (var i = 0; i < els.length; i++) {
            els[i].textContent = '★ ' + compact(n);
        }
    }

    function readCache() {
        try {
            var raw = window.localStorage.getItem(STORAGE_KEY);
            var v = raw && JSON.parse(raw);
            return (v && typeof v.n === 'number' && typeof v.t === 'number') ? v : null;
        } catch (_) { return null; }
    }

    function writeCache(n) {
        try {
            window.localStorage.setItem(STORAGE_KEY, JSON.stringify({ n: n, t: Date.now() }));
        } catch (_) { /* private mode / quota — silent */ }
    }

    function start() {
        var cached = readCache();
        if (cached) render(cached.n);
        if (cached && Date.now() - cached.t < TTL_MS) return;
        fetch('https://api.github.com/repos/' + REPO)
            .then(function (r) { return r.ok ? r.json() : null; })
            .then(function (data) {
                if (!data || typeof data.stargazers_count !== 'number') return;
                writeCache(data.stargazers_count);
                render(data.stargazers_count);
            })
            .catch(function () { /* offline / blocked — silent */ });
    }

    if (document.readyState === 'loading') {
        document.addEventListener('DOMContentLoaded', start);
    } else {
        start();
    }
})();
