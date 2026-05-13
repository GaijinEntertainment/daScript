/* Vertical splitter between code and output inside `.main_workspace`. Drag
 * sets `flex: 0 0 <pct>%` on the left column; the right column keeps
 * `flex: 1 1 0` so it absorbs the remainder. Position is persisted to
 * localStorage and CodeMirror is refresh()'d so gutter + scroll caches
 * reflect the new width. */

(function () {
    const STORAGE_KEY = 'daslang.playground.splitLeftPct';
    const MIN_PCT = 15;
    const MAX_PCT = 85;

    function applyPct(left, right, pct) {
        left.style.flex = '0 0 ' + pct + '%';
        right.style.flex = '1 1 0';
    }

    function init() {
        const ws = document.querySelector('.main_workspace');
        const cols = ws ? ws.querySelectorAll(':scope > .main_col') : null;
        if (!ws || !cols || cols.length !== 2) return;
        const [left, right] = cols;

        const handle = document.createElement('div');
        handle.className = 'pg-splitter';
        handle.setAttribute('role', 'separator');
        handle.setAttribute('aria-orientation', 'vertical');
        right.before(handle);

        const saved = parseFloat(localStorage.getItem(STORAGE_KEY));
        if (saved >= MIN_PCT && saved <= MAX_PCT) applyPct(left, right, saved);

        let dragging = false;
        let refreshQueued = false;
        let activePct = null;

        function refreshCM() {
            if (refreshQueued || !window.code) return;
            refreshQueued = true;
            requestAnimationFrame(() => {
                refreshQueued = false;
                try { window.code.refresh(); } catch (_) {}
            });
        }

        function onMove(e) {
            if (!dragging) return;
            const rect = ws.getBoundingClientRect();
            let pct = ((e.clientX - rect.left) / rect.width) * 100;
            pct = Math.max(MIN_PCT, Math.min(MAX_PCT, pct));
            activePct = pct;
            applyPct(left, right, pct);
            refreshCM();
            e.preventDefault();
        }

        function endDrag() {
            if (!dragging) return;
            dragging = false;
            handle.classList.remove('is-dragging');
            document.body.style.userSelect = '';
            document.body.style.cursor = '';
            document.removeEventListener('pointermove', onMove);
            document.removeEventListener('pointerup', endDrag);
            document.removeEventListener('pointercancel', endDrag);
            if (activePct != null) {
                localStorage.setItem(STORAGE_KEY, String(activePct.toFixed(2)));
            }
            refreshCM();
        }

        handle.addEventListener('pointerdown', (e) => {
            if (e.button !== undefined && e.button !== 0) return;
            dragging = true;
            handle.classList.add('is-dragging');
            document.body.style.userSelect = 'none';
            document.body.style.cursor = 'col-resize';
            document.addEventListener('pointermove', onMove);
            document.addEventListener('pointerup', endDrag);
            document.addEventListener('pointercancel', endDrag);
            e.preventDefault();
        });

        // Double-click resets to 50/50.
        handle.addEventListener('dblclick', () => {
            applyPct(left, right, 50);
            localStorage.removeItem(STORAGE_KEY);
            refreshCM();
        });
    }

    if (document.readyState === 'loading') {
        document.addEventListener('DOMContentLoaded', init);
    } else {
        init();
    }
})();
