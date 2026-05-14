// Overlay for web/ui's CodeMirror init: switch to the daslang mode + forge theme,
// and accept code shared from the landing-page hero via `#code=<encoded>`.
// Loaded BEFORE main.js, which calls CodeMirror() and exposes `window.code`.

window.FORGE_PLAYGROUND_OPTS = {
    lineNumbers: true,
    matchBrackets: true,
    indentWithTabs: false,
    styleActiveLine: true,
    theme: 'forge',
    mode: 'daslang',
    tabSize: 4,
    indentUnit: 4,
    highlightSelectionMatches: { showToken: /\w/ },
};

// `#code=<percent-encoded-source>` — single-file share from the landing hero.
// `#z=<lz-base64>` — multi-file share from the playground's ↗ share button.
// Both formats hand off to pgLoadFiles once the tab strip is mounted.
function applySharedCodeFromHash() {
    const hash = window.location.hash || '';
    let payload = null;
    if (hash.startsWith('#code=')) {
        try {
            const src = decodeURIComponent(hash.slice(6));
            payload = { files: { 'main.das': src }, active: 'main.das' };
        } catch (e) { return; }
    } else if (hash.startsWith('#z=') && window.LZString) {
        try {
            const json = window.LZString.decompressFromEncodedURIComponent(hash.slice(3));
            const obj = JSON.parse(json);
            if (obj && obj.files) payload = obj;
        } catch (e) { return; }
    } else {
        return;
    }

    // Stash bundle immediately so pgInit picks it up even if it polls in
    // before we do. Also flag the page as "restored from URL state" so
    // main.js skips its default `selectSample("examples", 0)` — otherwise
    // the async data.json fetch occasionally beats pgLoadFiles and the
    // default hello.das overwrites the hash payload.
    //
    // Stash `active` alongside the bundle: when playground-tabs.js's tryInit
    // consumes the pending bundle before this tryApply loop resolves, it
    // needs the active filename or it falls back to main.das and the shared
    // URL's selected tab is silently lost.
    window.__pendingSampleBundle = payload.files;
    window.__pendingSampleActive = payload.active;
    window.pgRestoredFromState = true;
    const deadline = Date.now() + 5000;
    (function tryApply() {
        if (typeof window.pgLoadFiles === 'function') {
            window.__pendingSampleBundle = null;
            window.__pendingSampleActive = null;
            window.pgLoadFiles(payload.files, payload.active);
            return;
        }
        if (Date.now() < deadline) setTimeout(tryApply, 50);
    })();
}

if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', applySharedCodeFromHash);
} else {
    applySharedCodeFromHash();
}

// Run (in main.js) pushes a new history entry on each invocation; popstate
// fires when the user navigates Back/Forward and replays the recorded state.
window.addEventListener('popstate', applySharedCodeFromHash);
