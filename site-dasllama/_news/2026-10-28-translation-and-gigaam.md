---
date: 2026-10-28
tag: engine
title: Translation and Russian speech-to-text join the engine - Bergamot and GigaAM v3, in master, in the next release.
---

Two families landed in master this week, both ahead of the next release. **Translation**: the
Bergamot students (Firefox Translations' Marian int8 models) run on the CPU, on Metal and on
Vulkan, and a translation LLM - MiLMMT-46, or any instruct GGUF through its own chat template -
loads through the same `load_translator` / `translate` facade; `dasllama-cli translate` takes
typed, scripted or spoken input, and the server gains LibreTranslate-shaped `/translate` and
`/languages`. **GigaAM v3**, the Russian speech-to-text model, with its CTC and RNN-T heads,
loads from one GGUF like the other speech models. Every family the engine loads is now listed on
one page: [features](features.html). Translation has a tutorial:
[dasLLAMA 17 - translate](https://daslang.io/doc/reference/tutorials/dasLLAMA_17_translate.html).
