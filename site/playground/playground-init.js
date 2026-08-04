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

// Hand a {files, active} payload to pgLoadFiles once the tab strip is mounted.
// Stash the bundle immediately so pgInit picks it up even if it polls in
// before we do, and flag the page as "restored from URL state" so main.js
// skips its default `selectSample("examples", 0)` — otherwise the async
// data.json fetch occasionally beats pgLoadFiles and the default hello.das
// overwrites the shared payload. `active` rides along for playground-tabs.js's
// tryInit, which otherwise falls back to main.das and loses the selected tab.
function applySharedPayload(payload) {
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

// `?s=<sha256>` — a stored sample on the daslang.io sample service; the body
// is either one raw .das source or a {files, active} JSON bundle (multi-file
// shares store the same payload the legacy #z= format compressed).
function applySharedCodeFromServer(hash) {
    fetch('/api/samples/' + hash)
        .then((resp) => {
            if (!resp.ok) throw new Error('sample fetch ' + resp.status);
            return resp.text();
        })
        .then((src) => {
            let payload = null;
            try {
                const obj = JSON.parse(src);
                if (obj && obj.files) payload = obj;
            } catch (e) { /* plain single-file source */ }
            if (!payload) payload = { files: { 'main.das': src }, active: 'main.das' };
            applySharedPayload(payload);
        })
        .catch((e) => { console.warn('shared-sample load failed:', e); });
}

// `#code=<percent-encoded-source>` — single-file share from the landing hero.
// `#z=<lz-base64>` — legacy multi-file share URLs (kept working forever).
function applySharedCodeFromHash() {
    const sParam = new URLSearchParams(window.location.search).get('s');
    if (sParam && /^[0-9a-f]{64}$/.test(sParam)) {
        applySharedCodeFromServer(sParam);
        return;
    }
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
    applySharedPayload(payload);
}

if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', applySharedCodeFromHash);
} else {
    applySharedCodeFromHash();
}

// Run (in main.js) pushes a new history entry on each invocation; popstate
// fires when the user navigates Back/Forward and replays the recorded state.
window.addEventListener('popstate', applySharedCodeFromHash);
