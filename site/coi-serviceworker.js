/*! coi-serviceworker — cross-origin isolation for the threaded-wasm examples + playground.
 *
 * The /examples cards and the /playground run daslang compiled to WebAssembly. The
 * released builds are threaded (-pthread) so new_thread / JobQue / parallel_for map to
 * real Web Workers — which needs SharedArrayBuffer, which is gated behind
 * crossOriginIsolated (COOP same-origin + COEP). GitHub Pages can't set response
 * headers, so this service worker injects them.
 *
 * SCOPED ON PURPOSE: the worker registers at site root (it must, to control the
 * root-level /examples.html navigation) but only rewrites responses under /examples*
 * and /playground*. The landing page, docs and blog pass through untouched — they don't
 * need isolation and keep their ordinary cross-origin fonts / docsearch behavior.
 *
 * TWO ISOLATION SUBTREES, DIFFERENT REGISTRATION GATES:
 *  - /examples ships only wasm64 (memory64) compiled builds, so isolation is pointless on
 *    an engine without memory64 (the games can't instantiate there anyway). Gated on memory64.
 *  - /playground runs the THREADED wasm32 interpreter (daslang_static, -pthread), which needs
 *    SharedArrayBuffer on EVERY engine — including Safari, which has no memory64 but does have
 *    threads. So /playground isolates regardless of memory64.
 *
 * COEP VALUE: credentialless lets cross-origin no-cors subresources (Google Fonts, Algolia
 * docsearch) load without per-resource CORP, but Safari/WebKit doesn't support it. So we emit
 * credentialless on Chromium/Firefox and require-corp on Apple WebKit. Under require-corp those
 * cross-origin subresources are blocked (fonts fall back to system, docsearch won't load) — an
 * accepted degradation on Safari/playground until the fonts are self-hosted (follow-up). The
 * /examples gate means Safari never reaches the examples subtree, so its require-corp only ever
 * applies to /playground.
 *
 * Adapted from github.com/gzuidhof/coi-serviceworker (MIT). */

// Only these subtrees get isolated. Root-scoped worker, narrow effect.
function coiInIsolationScope(url) {
    return url.origin === self.location.origin &&
        (url.pathname === "/examples.html" ||
         url.pathname.indexOf("/examples/") === 0 ||
         url.pathname.indexOf("/playground") === 0);
}

// /playground is the threaded wasm32 interpreter — it isolates on every engine (gate dropped).
function coiIsPlaygroundScope(pathname) {
    return pathname.indexOf("/playground") === 0;
}

// Apple WebKit (Safari, iOS) ships SharedArrayBuffer/threads but NOT COEP credentialless, so it
// must isolate via require-corp. Everything else (Chromium, Firefox) supports credentialless,
// which keeps the cross-origin fonts/docsearch working. (No JS feature-detect for credentialless
// exists; this UA split is what upstream coi-serviceworker does.)
function coiUsesCredentialless(ua) {
    ua = ua || "";
    var appleWebKit = /AppleWebKit\//.test(ua) && !/Chrome\/|Chromium\/|Android/.test(ua);
    return !appleWebKit;
}

if (typeof window === "undefined") {
    // ─── Service-worker context ─────────────────────────────────────────
    var COEP = coiUsesCredentialless(self.navigator && self.navigator.userAgent)
        ? "credentialless" : "require-corp";

    self.addEventListener("install", function () { self.skipWaiting(); });
    self.addEventListener("activate", function (e) { e.waitUntil(self.clients.claim()); });

    self.addEventListener("fetch", function (event) {
        var r = event.request;
        // Range/cache probes for cross-origin resources must not be touched.
        if (r.cache === "only-if-cached" && r.mode !== "same-origin") return;

        var url;
        try { url = new URL(r.url); } catch (e) { return; }
        if (!coiInIsolationScope(url)) return;   // landing page / docs / blog: pass through

        event.respondWith(
            fetch(r).then(function (response) {
                if (response.status === 0) return response;   // opaque — can't add headers
                var headers = new Headers(response.headers);
                headers.set("Cross-Origin-Embedder-Policy", COEP);
                if (r.mode === "navigate") {
                    headers.set("Cross-Origin-Opener-Policy", "same-origin");
                }
                headers.set("Cross-Origin-Resource-Policy", "cross-origin");
                return new Response(response.body, {
                    status: response.status,
                    statusText: response.statusText,
                    headers: headers,
                });
            }).catch(function (e) { console.error(e); return Response.error(); })
        );
    });
} else {
    // ─── Window context: register self ──────────────────────────────────
    (function () {
        // memory64 probe: \0asm | version | memory section { count=1, flags=0x04 (memory64), min=1 }.
        var hasWasm64 = false;
        try {
            hasWasm64 = WebAssembly.validate(new Uint8Array([0, 0x61, 0x73, 0x6d, 1, 0, 0, 0, 5, 3, 1, 4, 1]));
        } catch (e) { hasWasm64 = false; }
        // /playground (threaded wasm32 interpreter) isolates on every engine; /examples
        // (wasm64 compiled builds) only where memory64 exists — nothing to isolate otherwise.
        if (!coiIsPlaygroundScope(window.location.pathname) && !hasWasm64) return;
        if (!window.isSecureContext) return;     // service workers need https (localhost counts)
        if (window.crossOriginIsolated) return;  // already isolated — nothing to do
        if (!("serviceWorker" in navigator)) return;
        if (navigator.serviceWorker.controller) return;  // controlled but not isolated → don't reload-loop

        var reloaded = window.sessionStorage.getItem("coiReloaded");
        var scriptUrl = (document.currentScript && document.currentScript.src) || "/coi-serviceworker.js";
        navigator.serviceWorker.register(scriptUrl).then(function (registration) {
            registration.addEventListener("updatefound", function () {
                if (reloaded) return;
                window.sessionStorage.setItem("coiReloaded", "1");
                window.location.reload();
            });
            // SW is active but not yet controlling this page: reload once so this
            // navigation goes through the worker and comes back isolated.
            if (registration.active && !navigator.serviceWorker.controller && !reloaded) {
                window.sessionStorage.setItem("coiReloaded", "1");
                window.location.reload();
            }
        }).catch(function (e) { console.error("coi-serviceworker registration failed:", e); });
    })();
}
