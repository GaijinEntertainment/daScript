---
date: 2026-09-11
tag: engine
title: The Qwen family runs on Vulkan.
---

Every Qwen text model in the zoo, 0.5B to 48 GB, dense and mixture-of-experts, runs on the
Vulkan tier. Qwen3.8-27B UD-IQ4_XS runs whole on an RTX 5060 Ti 16 GB under Windows at
865.4 / 23.5 tok/s (pp512 / tg128, measured 2026-09-07), Qwen3.6-35B-A3B UD-IQ2_XXS at
2995.8 / 107.7 there (2026-09-09) and at 5156 / 146.7 (pp512 / tg32, 2026-09-11) on an RTX
5080 under Linux. We have a story to tell about how we
got there: [Vulkan and the Qwen family fortune](stories/2026-09-11-vulkan-and-the-qwen-family-fortune.html).
