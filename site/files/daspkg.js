/* daspkg.js — fetch the live package index, render filterable card grid.
 * Source of truth: github.com/borisbat/daspkg-index/packages.json. */
(function () {
    'use strict';

    var INDEX_URL = 'https://raw.githubusercontent.com/borisbat/daspkg-index/main/packages.json';

    var els = {
        list:         document.getElementById('daspkg-list'),
        status:       document.getElementById('daspkg-status'),
        tags:         document.getElementById('daspkg-tags'),
        search:       document.getElementById('daspkg-search'),
        searchClear:  document.getElementById('daspkg-search-clear'),
        countN:       document.getElementById('daspkg-count-n'),
        countTotal:   document.getElementById('daspkg-count-total'),
        publishCount: document.getElementById('daspkg-publish-count'),
    };

    var state = {
        all: [],
        allTags: [],
        query: '',
        activeTags: [],
    };

    function esc(s) {
        return String(s == null ? '' : s)
            .replace(/&/g, '&amp;')
            .replace(/</g, '&lt;')
            .replace(/>/g, '&gt;')
            .replace(/"/g, '&quot;')
            .replace(/'/g, '&#39;');
    }

    function applyFilter() {
        var q = state.query.trim().toLowerCase();
        return state.all.filter(function (p) {
            if (state.activeTags.length) {
                var tags = p.tags || [];
                for (var i = 0; i < state.activeTags.length; i++) {
                    if (tags.indexOf(state.activeTags[i]) === -1) return false;
                }
            }
            if (!q) return true;
            if (p.name && p.name.toLowerCase().indexOf(q) !== -1) return true;
            if (p.description && p.description.toLowerCase().indexOf(q) !== -1) return true;
            if (p.author && p.author.toLowerCase().indexOf(q) !== -1) return true;
            var ts = p.tags || [];
            for (var j = 0; j < ts.length; j++) if (ts[j].toLowerCase().indexOf(q) !== -1) return true;
            return false;
        });
    }

    function renderTags() {
        els.tags.innerHTML = state.allTags.map(function (t) {
            var active = state.activeTags.indexOf(t) !== -1;
            return '<button type="button" class="forge-daspkg-chip' + (active ? ' is-active' : '') +
                   '" data-tag="' + esc(t) + '">' + esc(t) + '</button>';
        }).join('');
    }

    function renderCards(pkgs) {
        if (!pkgs.length) {
            els.list.innerHTML = '';
            els.status.textContent = 'no packages match the filter';
            els.status.hidden = false;
            return;
        }
        els.status.hidden = true;
        els.list.innerHTML = pkgs.map(function (p) {
            var native = p.has_native
                ? '<span class="forge-daspkg-card__native">native</span>' : '';
            var sdk = p.min_sdk
                ? '<span class="forge-daspkg-card__sdk">sdk ≥ ' + esc(p.min_sdk) + '</span>' : '';
            var license = p.license
                ? '<span class="forge-daspkg-card__license">' + esc(p.license) + '</span>' : '';
            var tags = (p.tags || []).map(function (t) {
                return '<span class="forge-daspkg-card__tag">' + esc(t) + '</span>';
            }).join('');
            var deps = (p.dependencies && p.dependencies.length)
                ? '<div class="forge-daspkg-card__deps">↳ depends on ' +
                  p.dependencies.map(esc).join(', ') + '</div>' : '';
            var url = p.url ? 'https://' + p.url : '#';
            var installStr = 'daspkg install ' + p.name;
            return '' +
                '<a class="forge-daspkg-card" href="' + esc(url) + '" target="_blank" rel="noopener noreferrer">' +
                    '<div class="forge-daspkg-card__head">' +
                        '<div class="forge-daspkg-card__title-row">' +
                            '<h3>' + esc(p.name) + '</h3>' + native + sdk +
                        '</div>' + license +
                    '</div>' +
                    '<p class="forge-daspkg-card__desc">' + esc(p.description || '') + '</p>' +
                    '<div class="forge-daspkg-card__tags">' + tags + '</div>' +
                    deps +
                    '<div class="forge-daspkg-card__foot">' +
                        '<div class="forge-daspkg-card__install">' +
                            '<span class="prompt">$</span>' +
                            '<span>' + esc(installStr) + '</span>' +
                            '<span class="copy" data-copy="' + esc(installStr) + '" title="copy">⎘</span>' +
                        '</div>' +
                        '<span class="forge-daspkg-card__repo">repo ↗</span>' +
                    '</div>' +
                '</a>';
        }).join('');
    }

    function updateCounts(visible) {
        els.countN.textContent = visible;
        els.countTotal.textContent = state.all.length;
        els.publishCount.textContent = visible + ' of ' + state.all.length + ' packages';
    }

    function rerender() {
        var pkgs = applyFilter();
        renderTags();
        renderCards(pkgs);
        updateCounts(pkgs.length);
    }

    function copyToClipboard(text, srcEl) {
        var done = function (ok) {
            if (!srcEl) return;
            var prev = srcEl.textContent;
            srcEl.textContent = ok ? '✓' : '⎘';
            setTimeout(function () { srcEl.textContent = prev; }, 900);
        };
        if (navigator.clipboard && navigator.clipboard.writeText) {
            navigator.clipboard.writeText(text).then(function () { done(true); }, function () { done(false); });
        } else {
            try {
                var ta = document.createElement('textarea');
                ta.value = text;
                ta.style.position = 'fixed';
                ta.style.opacity = '0';
                document.body.appendChild(ta);
                ta.select();
                var ok = document.execCommand('copy');
                document.body.removeChild(ta);
                done(ok);
            } catch (e) { done(false); }
        }
    }

    function wireEvents() {
        els.tags.addEventListener('click', function (e) {
            var btn = e.target.closest('[data-tag]');
            if (!btn) return;
            var tag = btn.getAttribute('data-tag');
            var i = state.activeTags.indexOf(tag);
            if (i === -1) state.activeTags.push(tag);
            else state.activeTags.splice(i, 1);
            rerender();
        });
        els.search.addEventListener('input', function () {
            state.query = els.search.value;
            els.searchClear.hidden = !state.query;
            rerender();
        });
        els.searchClear.addEventListener('click', function () {
            els.search.value = '';
            state.query = '';
            els.searchClear.hidden = true;
            rerender();
            els.search.focus();
        });
        document.addEventListener('click', function (e) {
            var copy = e.target.closest('[data-copy]');
            if (!copy) return;
            e.preventDefault();
            e.stopPropagation();
            copyToClipboard(copy.getAttribute('data-copy'), copy);
        });
    }

    function init(packages) {
        state.all = packages.slice();
        var tagSet = {};
        state.all.forEach(function (p) {
            (p.tags || []).forEach(function (t) { tagSet[t] = true; });
        });
        state.allTags = Object.keys(tagSet).sort();
        wireEvents();
        rerender();
    }

    function fail(err) {
        els.status.textContent = 'failed to load package index — ' + (err && err.message ? err.message : 'network error');
        els.publishCount.textContent = '— packages';
        els.countN.textContent = '0';
        els.countTotal.textContent = '0';
    }

    fetch(INDEX_URL, { cache: 'no-cache' })
        .then(function (r) {
            if (!r.ok) throw new Error('HTTP ' + r.status);
            return r.json();
        })
        .then(init)
        .catch(fail);
}());
