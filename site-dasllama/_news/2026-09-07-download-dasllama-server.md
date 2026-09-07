---
date: 2026-09-07
tag: site
title: dasllama-server is a download now - one archive per platform, no daslang install.
---

The OpenAI-compatible server ships as a standalone bundle for macOS (Apple silicon), Windows
x64, Linux x86_64 and Linux arm64, from the rolling
[dasllama-server release](https://github.com/GaijinEntertainment/daScript/releases/tag/dasllama-server)
on GitHub - the links are in *get it* on the home page. Unpack it, start the watchdog (on a Mac,
the app), and open the control page at `127.0.0.1:8080`: it leads with the model catalog - pick
a model, it downloads, the server restarts into it. The exe is a fat build - plain code for the platform's baseline CPU
class, with one clone of every tuned kernel per class the engine ships a profile for, chosen from
cpuid when it starts; on a Mac the Metal kernel choices are raced once, at the first start. The
download is the good default. The SDK's `bin/daslang -jit` runs the same server and tunes every
kernel on your own box instead of picking from the shipped profiles.
