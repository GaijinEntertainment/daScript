---
date: 2026-09-16
tag: examples
title: The storyteller, storywish and parrot now run in every browser - Safari and iOS included.
---

The three examples were compiled to wasm64, which WebKit does not implement, so Safari and every
iPhone and iPad browser got a note instead of a story. The engine still cross-compiles as wasm64
inside - that is what keeps its layouts honest - but the link now lowers the memory to 32-bit,
and the one SIMD extension WebKit lacks left the build. [Try it](examples.html).
