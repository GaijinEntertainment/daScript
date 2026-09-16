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
 * TWO ISOLATION SUBTREES, ONE GATE: both /examples (the compiled builds, cross-compiled as
 * wasm64 and lowered to a 32-bit memory at link time) and /playground (the threaded wasm32
 * interpreter, daslang_static -pthread) run on every engine and need SharedArrayBuffer on
 * every engine, so the worker isolates both wherever a service worker can register.
 *
 * COEP VALUE: credentialless lets cross-origin no-cors subresources (Google Fonts, Algolia
 * docsearch) load without per-resource CORP, but Safari/WebKit doesn't support it. So we emit
 * credentialless on Chromium/Firefox and require-corp on Apple WebKit. Under require-corp those
 * cross-origin subresources are blocked (fonts fall back to system, docsearch won't load) — an
 * accepted degradation on Safari.
 *
 * Adapted from github.com/gzuidhof/coi-serviceworker (MIT). */

// Only these subtrees get isolated. Root-scoped worker, narrow effect.
// /files/wasm/ is the interpreter runtime the /examples ?force=interp runner (_interp.html)
// spawns its -pthread workers from: a dedicated worker script must itself carry
// COEP under an isolated owner, or the load dies with ERR_BLOCKED_BY_RESPONSE
// and the runtime waits on "loading-workers" forever. On a non-isolated page
// (the landing hero) the extra COEP on these responses is ignored.
function coiInIsolationScope(url) {
    return url.origin === self.location.origin &&
        (url.pathname === "/examples.html" ||
         url.pathname.indexOf("/examples/") === 0 ||
         url.pathname.indexOf("/playground") === 0 ||
         url.pathname.indexOf("/files/wasm/") === 0);
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
