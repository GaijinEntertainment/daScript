---
date: 2026-09-03
tag: engine
title: dasLLAMA speaks - KittenTTS nano and mini, Kokoro-82M, and a text front end that is nothing but data.
---

Three text-to-speech models serve from the same engine and the same tuned kernels as the
language models, on the CPU, through `/v1/audio/speech` on dasllama-server and a speech
studio on its control page. On an M1 Max the served 8-bit lane reads a real-time factor of
0.026 for kitten-nano (onnxruntime on the same input, 0.035) and 0.072 for kokoro-82m (torch
CPU, 0.097), measured 2026-09-02. Why a game wants this, how the front end became two data
packs, and the afternoon an "s" turned out to be a comma:
[the story](stories/2026-09-03-text-to-speech.html).
