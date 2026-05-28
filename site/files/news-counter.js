/* Forge nav — "N NEW" news chip.
 *
 * Data source: files/news-meta.json (built by site/blog/build_blog.py).
 * Storage:     localStorage["daslang:news:lastSeen"] = "YYYY-MM-DD".
 *
 * Semantics (mirrors blog-counter.js):
 *   - First-time visitor (no key)   → seeded from news-meta.json baseline_date,
 *                                     which is the 2nd-newest entry's date.
 *                                     So count = 1 (just the newest).
 *   - Landing with hash "#news"     → key set to newest_date; chip clears.
 *   - Click on the news nav link    → key set to newest_date.
 *   - count > 9                     → render "9+ NEW".
 *   - count === 0                   → chip not rendered.
 *
 * Nav anchors are matched by link.hash === "#news" so the same script works
 * from any page that links to "#news" / "index.html#news" / "../index.html#news".
 */
(function () {
    'use strict';

    var STORAGE_KEY = 'daslang:news:lastSeen';

    function readLastSeen() {
        try { return window.localStorage.getItem(STORAGE_KEY); }
        catch (_) { return null; }
    }

    function writeLastSeen(date) {
        try { window.localStorage.setItem(STORAGE_KEY, date); }
        catch (_) { /* private mode / quota — silent */ }
    }

    function newsJsonUrl() {
        // Locate our own <script> tag and derive news-meta.json next to it.
        var scripts = document.getElementsByTagName('script');
        for (var i = 0; i < scripts.length; i++) {
            var src = scripts[i].getAttribute('src') || '';
            if (src.indexOf('news-counter.js') !== -1) {
                return src.replace(/news-counter\.js(\?.*)?$/, 'news-meta.json');
            }
        }
        return 'files/news-meta.json';
    }

    function findNewsLinks() {
        // Match any nav link whose URL hash is "#news" — works for
        // "#news", "index.html#news", "../index.html#news".
        var links = document.querySelectorAll('.forge-nav__links a[href]');
        var out = [];
        for (var i = 0; i < links.length; i++) {
            if (links[i].hash === '#news') out.push(links[i]);
        }
        return out;
    }

    function renderChip(anchor, count) {
        if (anchor.querySelector('.forge-blog-chip')) return;
        var chip = document.createElement('span');
        chip.className = 'forge-blog-chip';
        chip.textContent = (count > 9 ? '9+' : count) + ' NEW';
        anchor.appendChild(chip);
        anchor.classList.add('forge-blog-link');
    }

    function clearChips() {
        var chips = document.querySelectorAll('.forge-nav__links a .forge-blog-chip');
        for (var i = 0; i < chips.length; i++) {
            var parent = chips[i].parentNode;
            if (parent && parent.hash === '#news') {
                chips[i].remove();
            }
        }
    }

    function update(data) {
        // Landing-with-#news: mark seen and render nothing. Early-return so
        // that a silently-failed writeLastSeen (private mode / quota) cannot
        // still cause the chip to render — we are already on the destination.
        if (window.location.hash === '#news') {
            writeLastSeen(data.newest_date);
            return;
        }
        var anchors = findNewsLinks();
        for (var k = 0; k < anchors.length; k++) {
            (function (a) {
                a.addEventListener('click', function () {
                    writeLastSeen(data.newest_date);
                    clearChips();
                });
            })(anchors[k]);
        }
        var lastSeen = readLastSeen() || data.baseline_date;
        var count = 0;
        for (var i = 0; i < data.news.length; i++) {
            if (data.news[i].date > lastSeen) count++;
        }
        if (count <= 0) return;
        for (var j = 0; j < anchors.length; j++) {
            renderChip(anchors[j], count);
        }
    }

    function start() {
        fetch(newsJsonUrl(), { cache: 'no-cache' })
            .then(function (r) { return r.ok ? r.json() : null; })
            .then(function (data) { if (data) update(data); })
            .catch(function () { /* offline / missing — silent */ });
    }

    if (document.readyState === 'loading') {
        document.addEventListener('DOMContentLoaded', start);
    } else {
        start();
    }
})();
