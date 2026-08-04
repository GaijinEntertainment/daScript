// ↗ share — store the editor state on the daslang.io sample service and hand
// out a stable `daslang.io/s/<hash>` link. Content-addressed: the same code
// always mints the same URL. Falls back to the legacy lz-compressed `#z=` URL
// when the service is unreachable (e.g. the GH Pages mirror), so sharing
// never hard-fails.

(function () {
    function sharePayloadBody() {
        if (!window.pgState) return null;
        const entries = Object.entries(window.pgState.files).map(([k, doc]) => [k, doc.getValue()]);
        if (entries.length === 1) return entries[0][1];
        return JSON.stringify({
            files: Object.fromEntries(entries),
            active: window.pgState.active,
        });
    }

    async function mintShareUrl() {
        const body = sharePayloadBody();
        if (body == null || body === '') throw new Error('nothing to share');
        const resp = await fetch('/api/samples', {
            method: 'POST',
            headers: { 'Content-Type': 'text/plain; charset=utf-8' },
            body,
        });
        if (!resp.ok) throw new Error('share service ' + resp.status);
        const j = await resp.json();
        if (!j.url) throw new Error('share service returned no url');
        return j.url;
    }

    // Legacy fallback URL — every file lz-compressed into the hash.
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

    function esc(s) { return String(s).replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;'); }

    function makePopover() {
        const wrap = document.createElement('div');
        wrap.className = 'pg-share';
        wrap.innerHTML = (
            '<div class="pg-share__header">' +
                '<span>Share this playground</span>' +
                '<button class="pg-share__close" type="button" title="close">×</button>' +
            '</div>' +
            '<div class="pg-share__row">' +
                '<input class="pg-share__url" readonly spellcheck="false" placeholder="minting link…" />' +
                '<button class="pg-share__copy" type="button">Copy</button>' +
            '</div>' +
            '<div class="pg-share__footer">' +
                '<span class="pg-share__meta">' + esc(fileCount() + ' file' + (fileCount() === 1 ? '' : 's')) + '</span>' +
            '</div>'
        );
        return wrap;
    }

    function openPopover(anchor) {
        closePopover();
        const pop = makePopover();
        document.body.appendChild(pop);

        // Position under the button.
        const r = anchor.getBoundingClientRect();
        pop.style.position = 'absolute';
        pop.style.top = (window.scrollY + r.bottom + 8) + 'px';
        pop.style.right = Math.max(8, window.innerWidth - r.right) + 'px';

        const input = pop.querySelector('.pg-share__url');
        const copyBtn = pop.querySelector('.pg-share__copy');
        const closeBtn = pop.querySelector('.pg-share__close');
        const meta = pop.querySelector('.pg-share__meta');

        input.addEventListener('focus', () => input.select());
        copyBtn.addEventListener('click', async () => {
            if (!input.value) return;
            try {
                await navigator.clipboard.writeText(input.value);
                copyBtn.textContent = '✓ copied';
                setTimeout(() => { copyBtn.textContent = 'Copy'; }, 1500);
            } catch (e) {
                // Fallback: select text so user can ⌘C manually.
                input.select();
            }
        });
        closeBtn.addEventListener('click', closePopover);

        mintShareUrl()
            .then((url) => { input.value = url; })
            .catch((e) => {
                console.warn('share-mint:', e);
                const legacy = buildShareUrl();
                if (legacy) {
                    input.value = legacy;
                    meta.textContent = 'share service unavailable — long link';
                } else {
                    meta.textContent = 'share failed: ' + e.message;
                }
            });

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
    window.pgMintShareUrl = mintShareUrl;
})();
