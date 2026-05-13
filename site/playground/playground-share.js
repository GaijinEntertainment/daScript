// ↗ share — generate a URL containing every file in pgState, lz-compressed
// into the hash. Click opens a small popover with the URL, a Copy button,
// and an optional "Shorten to is.gd" action.

(function () {
    const SHORTENER = 'https://is.gd/create.php?format=simple&url=';

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
                '<button class="pg-share__shorten" type="button">↘ Shorten to is.gd</button>' +
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
                const resp = await fetch(SHORTENER + encodeURIComponent(input.value));
                if (!resp.ok) throw new Error('is.gd error ' + resp.status);
                const short = (await resp.text()).trim();
                if (!/^https?:\/\//.test(short)) throw new Error('non-URL response: ' + short.slice(0, 60));
                input.value = short;
                shortenBtn.textContent = '✓ shortened';
            } catch (e) {
                shortenBtn.textContent = 'shorten failed';
                console.warn('share-shorten:', e);
            } finally {
                setTimeout(() => {
                    shortenBtn.textContent = '↘ Shorten to is.gd';
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
