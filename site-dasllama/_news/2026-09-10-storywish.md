---
date: 2026-09-10
tag: examples
title: Storywish - type the words, a story model trained to take requests writes the tale, in the browser.
---

A second dasLLAMA example on the examples page. The published TinyStories-Instruct models are
GPT-Neo, which no GGUF engine runs, so we trained our own: `tinystories-instruct-27M`, a
27M-parameter llama on the TinyStoriesInstruct corpus with a 4K vocabulary, 15 minutes on one
H100. Asked for three words over five word triples, 24 sampled stories each, it puts all three in
61 of the 120 stories; the official 33M puts them in 60. It
ships as a 32 MB `.dlim` beside KittenTTS nano; you type the words, Enter tells the story, Tab
asks for dialogue. The model, the training recipe and the hit-rate numbers are on
[Hugging Face](https://huggingface.co/borisbat/dasllama-stories). The model sets of both
examples are now minted by the deploy for the build it ships, so a format bump can no longer
leave a page silently declining its images. [Try it](examples.html).
