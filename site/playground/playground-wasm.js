// wasm engine — compile the editor's code to WebAssembly on the build service
// and hand back the artifact URLs.
//
// The build is queued, not immediate: the compute box pulls jobs, so a first
// build takes as long as a real compile and a repeat build is a cache hit. The
// flow mirrors sharing exactly — the code is stored first, and its content hash
// is the build key, so sharing a link and building it name the same bytes.
//
// Reports progress through a callback rather than touching the DOM, so the
// caller decides where "building…" appears.

(function () {
    const POLL_INTERVAL_MS = 1200;
    const POLL_TIMEOUT_MS = 10 * 60 * 1000;   // matches the builder's own wall-clock kill

    // Whether the service can build at all, asked once per page load. Null
    // until the first probe resolves.
    let infoPromise = null;

    function buildInfo() {
        if (!infoPromise) {
            infoPromise = fetch('/api/build/info')
                .then(r => r.ok ? r.json() : { enabled: false })
                .catch(() => ({ enabled: false }));
        }
        return infoPromise;
    }

    // The exact bytes the store will hash — shared with the share button so a
    // build and a share link of the same editor state agree on one hash.
    function payloadBody() {
        if (window.pgSharePayloadBody) return window.pgSharePayloadBody();
        return null;
    }

    async function storeSample() {
        const body = payloadBody();
        if (body == null || body === '') throw new Error('nothing to build');
        const resp = await fetch('/api/samples', {
            method: 'POST',
            headers: { 'Content-Type': 'text/plain; charset=utf-8' },
            body,
        });
        if (!resp.ok) throw new Error('sample store returned ' + resp.status);
        const j = await resp.json();
        if (!j.hash) throw new Error('sample store returned no hash');
        return j.hash;
    }

    function sleep(ms) { return new Promise(r => setTimeout(r, ms)); }

    function describe(status) {
        if (status.state === 'queued') {
            return status.position > 1
                ? 'queued for build (position ' + status.position + ')…'
                : 'queued for build…';
        }
        if (status.state === 'building') return 'building…';
        return status.state;
    }

    // Resolve to {ok:true, files:[url]} or {ok:false, error:"..."}. `onProgress`
    // is called with a human-readable line each time the state changes, so a
    // cache hit prints nothing and a real build narrates itself.
    async function build(onProgress) {
        const info = await buildInfo();
        if (!info.enabled) {
            return { ok: false, error: 'wasm builds are not available right now' };
        }
        const hash = await storeSample();

        const requested = await fetch('/api/build/request/' + hash, { method: 'POST' });
        if (!requested.ok) {
            const j = await requested.json().catch(() => ({}));
            return { ok: false, error: j.error || ('build request failed: ' + requested.status) };
        }
        let status = await requested.json();

        let said = '';
        const deadline = Date.now() + POLL_TIMEOUT_MS;
        while (status.state === 'queued' || status.state === 'building') {
            const line = describe(status);
            if (line !== said) { said = line; if (onProgress) onProgress(line); }
            if (Date.now() > deadline) {
                return { ok: false, error: 'build timed out; the queue may be backed up' };
            }
            await sleep(POLL_INTERVAL_MS);
            const r = await fetch('/api/build/status/' + hash);
            if (!r.ok && r.status !== 404) {
                return { ok: false, error: 'build status failed: ' + r.status };
            }
            status = await r.json().catch(() => ({ state: 'unknown' }));
        }

        if (status.state === 'done') {
            if (!status.files || !status.files.length) {
                return { ok: false, error: 'build reported done with no artifact' };
            }
            // kind: 'module' (bare wasi .wasm) or 'page' (standalone html+js+wasm)
            return { ok: true, kind: status.kind || 'module', files: status.files, hash };
        }
        if (status.state === 'failed') {
            return { ok: false, error: status.error || 'build failed', compileError: true };
        }
        return { ok: false, error: 'build ended in state "' + status.state + '"' };
    }

    window.pgWasmBuild = { build, buildInfo };
})();
