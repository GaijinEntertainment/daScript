// Algolia DocSearch init for daslang.io.
//
// The apiKey below is the PUBLIC search-only key — DocSearch always ships it to
// the client; the admin/write key is never exposed here.
//
// Runs DOM-ready-safe so the same file works whether it's loaded with `defer`
// (the hand-authored + blog pages) or injected into <head> non-deferred (the
// Sphinx docs, via conf.py html_js_files). The @docsearch/js UMD bundle is
// always loaded before this file, so the global `docsearch` is defined.
//
// NOTE: a byte-identical copy lives at doc/source/_static/docsearch.js for the
// Sphinx docs (separate static tree). Keep the two in sync.
(function () {
  function init() {
    if (typeof docsearch !== 'function') return;
    if (!document.querySelector('#docsearch')) return;
    docsearch({
      container: '#docsearch',
      appId: 'Z88JBBPHHH',
      indexName: 'daslang.io crawler',
      apiKey: '09d715f38753c82d2f87a9824ac81c3f',
    });
  }
  if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', init);
  } else {
    init();
  }
})();
