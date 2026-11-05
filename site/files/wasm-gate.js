// The browser gate every daslang wasm page runs before it loads its program. `daspkg release
// wasm` inlines this file where a shell carries its wasm-gate marker comment; a page outside a release
// loads it as a script.
(function () {
  var HEADER = [0, 97, 115, 109, 1, 0, 0, 0];
  var TYPE_SECTION_RETURNING_EXNREF = [1, 5, 1, 0x60, 0, 1, 0x69];
  var TYPE_SECTION_TAKING_V128 = [1, 5, 1, 0x60, 1, 0x7b, 0];
  function accepts(typeSection) {
    try { return WebAssembly.validate(new Uint8Array(HEADER.concat(typeSection))); } catch (e) { return false; }
  }
  var ua = navigator.userAgent;
  var MINIMUM_BROWSERS = 'Safari 18.4, Firefox 131 and Chrome 137 or newer run it';
  var gate = {
    hasExnref: accepts(TYPE_SECTION_RETURNING_EXNREF),
    hasSimd: accepts(TYPE_SECTION_TAKING_V128),
    is32bit: (/Windows NT/.test(ua) && !/Win64|x64/.test(ua)) || /Linux i686/.test(ua),
    isIsolated: !!(self.crossOriginIsolated && typeof SharedArrayBuffer !== 'undefined'),
    force: new URLSearchParams(location.search).get('force'),
    /** '' when this engine can run the build, else 'wasm' or '32bit'. */
    engineReason: function () {
      if (!gate.hasExnref || !gate.hasSimd) return 'wasm';
      if (gate.is32bit) return '32bit';
      return '';
    },
    /** '' when the build can run on this page, else 'isolation', 'wasm' or '32bit'. */
    reason: function () {
      if (gate.force === 'run') return '';
      if (gate.force === 'unsupported' || !gate.isIsolated) return 'isolation';
      return gate.engineReason();
    },
    /** Plain text in every field, as explain() returns: the page escapes it. */
    refusal: function (reason) {
      if (reason === 'isolation') return {
        status: 'needs SharedArrayBuffer',
        title: 'SharedArrayBuffer is not available on this page.',
        text: 'The build runs on Web Workers over it, and a browser grants it only to a cross-origin-isolated page - one the server sends the Cross-Origin-Opener-Policy and Cross-Origin-Embedder-Policy headers for.'
      };
      if (reason === 'wasm') return {
        status: 'needs a newer browser',
        title: "This browser's WebAssembly lacks " + (gate.hasExnref ? 'SIMD' : 'exception references') + ', which the build uses.',
        text: MINIMUM_BROWSERS + '; on an iPhone or iPad every browser is the system WebKit, so an iOS update brings it.'
      };
      if (reason === '32bit') return {
        status: 'needs a 64-bit browser',
        title: 'This browser is a 32-bit build.',
        text: 'The program is tens of megabytes of WebAssembly, several times that once compiled, and a 32-bit browser has no room for it: the load ends in "out of memory". A 64-bit Firefox, Chrome or Edge on the same machine runs it.'
      };
      return null;
    },
    explain: function (what) {
      what = String(what);
      if (/out of memory/i.test(what)) return 'The browser ran out of memory compiling the program: a 32-bit browser has no room for it, and a 64-bit one may need other heavy tabs closed first.';
      if (/CompileError/i.test(what)) return "This browser's WebAssembly does not accept the build; " + MINIMUM_BROWSERS + '.';
      return '';
    },
    /** Appends a live copy of the script tag emcc put in the inert `loader` template (a cloned template script stays inert). */
    loadProgram: function (onError) {
      var inert = document.getElementById('loader').content.querySelector('script');
      var live = document.createElement('script');
      live.async = true;
      if (onError) live.onerror = onError;
      if (inert.getAttribute('src')) live.src = inert.getAttribute('src');
      else live.textContent = inert.textContent;
      document.body.appendChild(live);
      return live;
    }
  };
  window.dasWasmGate = gate;
})();
