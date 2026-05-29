// ↗ share — generate a URL containing every file in pgState, lz-compressed
// into the hash. Click opens a small popover with the URL, a Copy button,
// and an optional "Shorten URL" action.

(function () {
    // Free, no-auth shorteners tried in order until one returns a URL. Each
    // `run` performs its own fetch + parse and returns the short URL string;
    // the chain validates the `https?://` shape. is.gd/v.gd are intentionally
    // absent — they send no CORS headers, so a browser fetch from daslang.io is
    // always blocked. da.gd/tinyurl (plain-text GET) and spoo.me (form POST +
    // JSON) do send them. A longer chain survives any one service rate-limiting
    // or dropping CORS later.
    async function getText(url, name) {
        const resp = await fetch(url);
        if (!resp.ok) throw new Error(name + ' error ' + resp.status);
        return (await resp.text()).trim();
    }

    const SHORTENERS = [
        { name: 'da.gd', run: (u, name) => getText('https://da.gd/s?url=' + encodeURIComponent(u), name) },
        { name: 'tinyurl', run: (u, name) => getText('https://tinyurl.com/api-create.php?url=' + encodeURIComponent(u), name) },
        { name: 'spoo.me', run: async (u, name) => {
            const resp = await fetch('https://spoo.me/', {
                method: 'POST',
                headers: { 'Content-Type': 'application/x-www-form-urlencoded', 'Accept': 'application/json' },
                body: 'url=' + encodeURIComponent(u),
            });
            if (!resp.ok) throw new Error(name + ' error ' + resp.status);
            // spoo.me returns { short_url: "http://spoo.me/xxxx" } — upgrade to https.
            return ((await resp.json()).short_url || '').replace(/^http:/, 'https:');
        } },
    ];

    async function shortenWithFallback(longUrl) {
        let lastErr = null;
        for (const svc of SHORTENERS) {
            try {
                const short = (await svc.run(longUrl, svc.name)).trim();
                if (!/^https?:\/\//.test(short)) throw new Error(svc.name + ' non-URL: ' + short.slice(0, 60));
                return { short, name: svc.name };
            } catch (e) {
                lastErr = e;
                console.warn('share-shorten:', e);
            }
        }
        throw lastErr || new Error('no shortener available');
    }

    function buildShareUrl() {
        if (!window.pgState || !window.LZString) return null;
        const payload = JSON.stringify({
            files: Object.fromEntries(
                Object.entries(window.pgState.files).map(([k, doc]) => [k, doc.getValue()])
            ),
            active: window.pgState.active,
        });
        const z = window.LZString.compressToEncodedURIComponent(payload);
        return location.origin + location.pathname + '#z=' + z;
    }

    function fileCount() {
        return window.pgState ? Object.keys(window.pgState.files).length : 0;
    }

    function makePopover(url) {
        const wrap = document.createElement('div');
        wrap.className = 'pg-share';
        wrap.innerHTML = (
            '<div class="pg-share__header">' +
                '<span>Share this playground</span>' +
                '<button class="pg-share__close" type="button" title="close">×</button>' +
            '</div>' +
            '<div class="pg-share__row">' +
                '<input class="pg-share__url" readonly spellcheck="false" />' +
                '<button class="pg-share__copy" type="button">Copy</button>' +
            '</div>' +
            '<div class="pg-share__footer">' +
                '<button class="pg-share__shorten" type="button">↘ Shorten URL</button>' +
                '<span class="pg-share__meta">' + esc(fileCount() + ' file' + (fileCount() === 1 ? '' : 's')) + '</span>' +
            '</div>'
        );
        wrap.querySelector('.pg-share__url').value = url;
        return wrap;
    }

    function esc(s) { return String(s).replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;'); }

    function openPopover(anchor) {
        closePopover();
        const url = buildShareUrl();
        if (!url) return;
        const pop = makePopover(url);
        document.body.appendChild(pop);

        // Position under the button.
        const r = anchor.getBoundingClientRect();
        pop.style.position = 'absolute';
        pop.style.top = (window.scrollY + r.bottom + 8) + 'px';
        pop.style.right = Math.max(8, window.innerWidth - r.right) + 'px';

        const input = pop.querySelector('.pg-share__url');
        const copyBtn = pop.querySelector('.pg-share__copy');
        const shortenBtn = pop.querySelector('.pg-share__shorten');
        const closeBtn = pop.querySelector('.pg-share__close');

        input.addEventListener('focus', () => input.select());
        copyBtn.addEventListener('click', async () => {
            try {
                await navigator.clipboard.writeText(input.value);
                copyBtn.textContent = '✓ copied';
                setTimeout(() => { copyBtn.textContent = 'Copy'; }, 1500);
            } catch (e) {
                // Fallback: select text so user can ⌘C manually.
                input.select();
            }
        });
        shortenBtn.addEventListener('click', async () => {
            shortenBtn.disabled = true;
            shortenBtn.textContent = '…';
            try {
                const { short, name } = await shortenWithFallback(input.value);
                input.value = short;
                shortenBtn.textContent = '✓ via ' + name;
            } catch (e) {
                shortenBtn.textContent = 'shorten failed';
                console.warn('share-shorten:', e);
            } finally {
                setTimeout(() => {
                    shortenBtn.textContent = '↘ Shorten URL';
                    shortenBtn.disabled = false;
                }, 2000);
            }
        });
        closeBtn.addEventListener('click', closePopover);

        // Click-outside to close. Capture phase so we see the click before
        // the popover's own handlers (which stopPropagation).
        setTimeout(() => {
            document.addEventListener('mousedown', outsideHandler, true);
        }, 0);
        window.__pgSharePopover = pop;
    }

    function outsideHandler(e) {
        const pop = window.__pgSharePopover;
        if (!pop) return;
        const btn = document.getElementById('share');
        if (pop.contains(e.target) || (btn && btn.contains(e.target))) return;
        closePopover();
    }

    function closePopover() {
        document.removeEventListener('mousedown', outsideHandler, true);
        if (window.__pgSharePopover) {
            window.__pgSharePopover.remove();
            window.__pgSharePopover = null;
        }
    }

    function wireShareButton() {
        const btn = document.getElementById('share');
        if (!btn) return;
        btn.addEventListener('click', (e) => {
            e.stopPropagation();
            if (window.__pgSharePopover) closePopover();
            else openPopover(btn);
        });
    }

    if (document.readyState === 'loading') {
        document.addEventListener('DOMContentLoaded', wireShareButton);
    } else {
        wireShareButton();
    }

    // Expose for tests.
    window.pgBuildShareUrl = buildShareUrl;
})();
