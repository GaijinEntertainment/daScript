/* Forge nav — "N NEW" blog chip.
 *
 * Data source: files/blog.json (built by site/blog/build_blog.py).
 * Storage:     localStorage["daslang:blog:lastSeen"] = "YYYY-MM-DD".
 *
 * Semantics:
 *   - First-time visitor (no key)  → seeded from blog.json baseline_date,
 *                                    which is the 2nd-newest post's date.
 *                                    So count = 1 (just the newest).
 *   - Visiting /blog/ (the index)  → key set to newest_date; chip clears.
 *   - count > 9                    → render "9+ NEW".
 *   - count === 0                  → chip not rendered.
 */
(function () {
    'use strict';

    var STORAGE_KEY = 'daslang:blog:lastSeen';

    function isBlogIndex() {
        var p = window.location.pathname;
        return p.endsWith('/blog/') || p.endsWith('/blog/index.html');
    }

    function readLastSeen() {
        try { return window.localStorage.getItem(STORAGE_KEY); }
        catch (_) { return null; }
    }

    function writeLastSeen(date) {
        try { window.localStorage.setItem(STORAGE_KEY, date); }
        catch (_) { /* private mode / quota — silent */ }
    }

    function blogJsonUrl() {
        // Locate our own <script> tag and derive blog.json sitting next to it.
        var scripts = document.getElementsByTagName('script');
        for (var i = 0; i < scripts.length; i++) {
            var src = scripts[i].getAttribute('src') || '';
            if (src.indexOf('blog-counter.js') !== -1) {
                return src.replace(/blog-counter\.js(\?.*)?$/, 'blog.json');
            }
        }
        return 'files/blog.json';
    }

    function findBlogLinks() {
        var links = document.querySelectorAll('a[href]');
        var out = [];
        for (var i = 0; i < links.length; i++) {
            // .href resolves the relative href against the page URL.
            var resolved = links[i].href;
            if (resolved.endsWith('/blog/index.html') ||
                resolved.endsWith('/blog/')) {
                // Skip the footer link — only chip the top nav. Heuristic:
                // the nav link sits inside .forge-nav__links.
                if (links[i].closest('.forge-nav__links')) {
                    out.push(links[i]);
                }
            }
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

    function update(data) {
        // On the blog index itself: mark seen, render nothing.
        if (isBlogIndex()) {
            writeLastSeen(data.newest_date);
            return;
        }
        var lastSeen = readLastSeen() || data.baseline_date;
        var count = 0;
        for (var i = 0; i < data.posts.length; i++) {
            if (data.posts[i].date > lastSeen) count++;
        }
        if (count <= 0) return;
        var anchors = findBlogLinks();
        for (var j = 0; j < anchors.length; j++) {
            renderChip(anchors[j], count);
        }
    }

    function start() {
        fetch(blogJsonUrl(), { cache: 'no-cache' })
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
