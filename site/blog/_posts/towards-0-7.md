---
title: Best-laid plans
date: 2026-10-30 16:46:24
tags:
    - daslang
    - dasllama
---

Not so fast. I mean it's just not that fast anymore.

<!-- more -->

The worst part is - it's in the DNA. Back in 2019 the vec4f ABI was a good idea. cast, dispatch, eval - it all made perfect sense. It was fast. It was faster than everything else. It still is.

And yet it's not. I blame dasllama. How do u express a 16-wide float, when all u have is 4-wide.
Sure, we can shoehorn us some 16-wide int8, but it's not quite the same thing.
That custom straight-to-LLVM generator needs to go. That dispatch class really needs one function.
Those arguments - they need to be a struct.

But wait, there is even more. Those SimNodes - they really should be declared somewhere, generated from something,
folded on simulate, fused as they go - not the custom fuse on the side for the most common cases.
Not to mention that the simulate macro should finally get some substance. Like 'these are my nodes, go bonkers'.

I want me some bonkers. It's been 7 years. It's that time. Leap of faith. New interpreter.
Old API on the side, of course. I'm not breaking everyone's stride. Your C++ will keep compiling. But there will be a reason to switch. In spades.

Cool story bro. If that was all there was. It's not. Time to mature indeed. The package manager should be able to have it all pre-built. We already `pip install daslang`, time to `daspkg install foobar` without MSVC. Not hard. Tedious. So along comes a CI template, some annoyance, some burden on good people. They can handle it, though. Up until I get my shit together. Maybe some build setup somewhere if this ever gets past that landing strip.

And then there is dasllama. Custom inference needs something custom to infer. Perhaps an LLM which knows das by heart (hi Astra). Perhaps another one which can help with the lint. A public training set. Weights. Miracles. "Scientists are saying the future is going to be far more futuristic than they originally predicted." That one used to be a joke. I kid u not.

Lastly, WASM. It's been good. It's gonna get first-class treatment. An emitter without the LLVM. Interpreter out of the box. Probably JIT. `%require~ foo.wasm%%`, and then just call those functions. Maybe u can see where this is going. Feels like bootstrap to me (Zig?).

Lint. Infer. Parallel compilation. HLSL emitter. Maybe dasDX12. Maybe dasWebGPU. U know, the works. They tend to just happen these days, like that awesome [dasSDL](https://github.com/spiiin/dasSDL3), [c2das](https://github.com/lookibed/c2das), or running on a microcontroller with daslang-nano, or, of all things, in [Unreal Engine](https://www.youtube.com/watch?v=7lqbYKtIZ3c). Who's going bonkers now.

These days it feels like the sky is the limit. Just don't fall. 'Cause it's not that fast. Yet.
