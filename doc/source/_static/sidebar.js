// Hideable desktop/tablet navigation for sphinx_rtd_theme.
//
// The theme already provides an off-canvas hamburger at <=768px. Above that,
// default to a reading-first layout on narrower screens while remembering an
// explicit user choice across documentation pages.
(function () {
  'use strict';

  var STORAGE_KEY = 'forge-docs-sidebar';
  var COLLAPSED_CLASS = 'forge-sidebar-collapsed';
  var desktopMedia = window.matchMedia('(min-width: 769px)');
  var narrowDesktopMedia = window.matchMedia('(max-width: 1099px)');
  var root = document.documentElement;

  function readPreference() {
    try {
      var value = window.localStorage.getItem(STORAGE_KEY);
      return value === 'collapsed' || value === 'expanded' ? value : null;
    } catch (_) {
      return null;
    }
  }

  function writePreference(value) {
    try {
      window.localStorage.setItem(STORAGE_KEY, value);
    } catch (_) {
      // Storage can be disabled; the toggle should still work for this page.
    }
  }

  function shouldCollapse() {
    if (!desktopMedia.matches) return false;
    var preference = readPreference();
    if (preference) return preference === 'collapsed';
    return narrowDesktopMedia.matches;
  }

  // Apply the initial class before deferred DOM setup to avoid a visible
  // sidebar jump on reading-first tablet/narrow-desktop layouts.
  root.classList.toggle(COLLAPSED_CLASS, shouldCollapse());

  function init() {
    var navigation = document.querySelector('.wy-nav-side');
    var toggle = document.querySelector('.forge-sidebar-toggle');
    if (!navigation || !toggle) return;

    navigation.id = 'forge-site-navigation';

    var icon = toggle.querySelector('.fa');
    var label = toggle.querySelector('.forge-sidebar-toggle-label');

    function updateToggle() {
      var collapsed = root.classList.contains(COLLAPSED_CLASS);
      toggle.setAttribute('aria-expanded', collapsed ? 'false' : 'true');
      toggle.setAttribute(
        'title',
        collapsed ? 'Show navigation' : 'Hide navigation'
      );
      if (label) {
        label.textContent = collapsed ? 'Show navigation' : 'Hide navigation';
      }
      if (icon) {
        icon.classList.toggle('fa-chevron-left', !collapsed);
        icon.classList.toggle('fa-chevron-right', collapsed);
      }
    }

    function syncLayout() {
      root.classList.toggle(COLLAPSED_CLASS, shouldCollapse());
      if (!desktopMedia.matches) {
        // Always enter mobile with the theme's off-canvas menu closed.
        document
          .querySelectorAll("[data-toggle='wy-nav-shift']")
          .forEach(function (element) {
            element.classList.remove('shift');
          });
      }
      updateToggle();
    }

    toggle.addEventListener('click', function () {
      if (!desktopMedia.matches) return;
      var collapsed = !root.classList.contains(COLLAPSED_CLASS);
      root.classList.toggle(COLLAPSED_CLASS, collapsed);
      writePreference(collapsed ? 'collapsed' : 'expanded');
      updateToggle();
    });

    desktopMedia.addEventListener('change', syncLayout);
    narrowDesktopMedia.addEventListener('change', syncLayout);
    syncLayout();
  }

  if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', init);
  } else {
    init();
  }
})();
