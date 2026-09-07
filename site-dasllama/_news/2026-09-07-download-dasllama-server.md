---
date: 2026-09-07
tag: site
title: dasllama-server is a download now - one archive per platform, no daslang install.
---

The OpenAI-compatible server ships as a standalone bundle for macOS (Apple silicon), Windows
x64, Linux x86_64 and Linux arm64, from the rolling
[dasllama-server release](https://github.com/GaijinEntertainment/daScript/releases/tag/dasllama-server)
on GitHub - the links are in *get it* above. Unpack it, start the watchdog (on a Mac, the app),
and the control page at `127.0.0.1:8080` opens on the model catalog: pick a model, it downloads,
the server restarts into it. The exe is a fat build - plain code for the platform's baseline CPU
class, with one clone of every tuned kernel per class the engine ships a profile for, chosen from
cpuid when it starts; on a Mac the Metal kernel choices are raced once, at the first start. The
fastest a box can serve is still `daslang -jit` from the SDK, tuned on that box - the download is
the good default, the SDK is the ceiling.
