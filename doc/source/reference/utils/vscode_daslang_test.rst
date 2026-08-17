.. _utils_vscode_daslang_test:

.. index::
   single: Utils; vscode-daslang-test
   single: Utils; VSCode

================================================
 vscode-daslang-test --- VSCode Test Explorer
================================================

A VSCode extension that integrates the ``dastest`` framework with the
native Test Explorer UI: it discovers ``[test]`` and ``[benchmark]``
functions in ``.das`` files, runs individual tests, files, or everything
(three run profiles: all / tests only / benchmarks only), shows
benchmark timings in the sidebar, and navigates to failure locations.
The file watcher keeps the test tree in sync as you edit.

Requirements: VSCode 1.85+, a built daslang compiler, and
``dastest/dastest.das`` in the workspace.

Build from source
=================

The extension ships as source::

   cd utils/vscode-daslang-test
   npm install
   npm run compile
   npx @vscode/vsce package

Install the resulting ``.vsix`` via VSCode's "Install from VSIX".

.. seealso::

   :ref:`utils_dastest` -- the test framework the extension drives
