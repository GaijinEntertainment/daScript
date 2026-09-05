---
date: 2026-09-05
tag: examples
title: dasLLAMA runs in the browser - the storyteller, compiled to wasm64, opens the new examples page.
---

The engine behind the ladder now also ships as a WebAssembly build. `daspkg release wasm`
compiles the storyteller - llama2.c's stories15M writing a children's tale while KittenTTS nano
reads it aloud - into one 26 MB wasm64 module, and the two models arrive as prepared `.dlim`
images, 76 MB with the two English phoneme packs, the same format the native engine maps.
Decode, phonemizer and vocoder all run on Web Workers in the page; nothing is interpreted.
Chrome, Edge or Firefox 133+ (memory64). [Try it](examples.html).
