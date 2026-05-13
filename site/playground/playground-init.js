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

// `#code=<percent-encoded-source>` — the hero's "↗ playground" button sets this
// when opening the playground. We poll for the CM instance (created later by
// main.js) and inject the source once it shows up.
function applySharedCodeFromHash() {
    if (!window.location.hash || !window.location.hash.startsWith('#code=')) return;
    let src;
    try { src = decodeURIComponent(window.location.hash.slice(6)); }
    catch (e) { return; }
    const deadline = Date.now() + 5000;
    (function tryApply() {
        if (window.code && typeof window.code.setValue === 'function') {
            window.code.setValue(src);
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
