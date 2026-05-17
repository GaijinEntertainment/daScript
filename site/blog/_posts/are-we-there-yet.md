---
title: Are we there yet?
date: 2026-05-16 17:01:24
tags:
    - daScript
    - Claude
---

Yes and No.

<!-- more -->

Like a good little shaman I've been praying the technical debt away. Prompts are new incantations. Prompts are new prayers. Mine been about quality gates, tests, tools, cleanup, refactor, tools, documentation, tutorials, integration, better syntax, and tools.

I managed to squeeze some features along the way, but far too few to my liking. Strudel is awesome. LINQ is amazing. It will get better. But not yet.

We prayed the smart_ptr away from the ast - and now everyone can write macros care-free. There are more demons left to slay. The big ones are

* 32-bit arrays. yes, in 2026. tables too. but not for long. i'll try not to break your code too much. u'll get length32, count32, 64-bit index; u get to fix some warnings here and there.
* proper generic resolution for fixed array dimensions. u probably are not going to notice (hi Profelis). one day it goes from it just works to it just works with less generics, and thats it.
* lambda should be copyable. delete becomes unsafe, but thats ok - we have GC. we might even squeeze clone for it - we already do it for the job-que.

There will be 0.6.3 and likely 0.6.4 before there will be 0.7.

But did u know

* daslang -exe outputs standalone binary. and daspkg can package it
* did u even know there is daspkg, its like npm but for das
* there is an MCP server, which helps with das. and cpp. like if u work with cpp - it can lookup symbols and do the works. fast.
* there is also 'blind mouse' MCP server, which is very experimental - but Claude says it helps already
* there is lint. lint + Claude is synergetic. lint + Claude + Boris is something I'm figuring out
* there is detect-dup and find-dup. detect-dup is very local. when Claude writes a lot of Code, it says 'and these 3 functions are too similar' (btw, Boris wrote two of them - one in 2022). find-dup is how u do it on a large scale.
* dastest and benchctl. one is tests. and benchmarks. the other is to remind u that performance regression is a regression
* llvm support is getting better. way better. loop annotations are now first class thing.

But did u try

* daslang-live and the dasLiveHost. Claude loves it. MCP talks to it. My dasIMGUI tutorial recorder uses it. Its getting stable. Don't restart.
* LINQ. because its awesome
* dasSQLITE. because its excellent. there will be more on how it plays with LINQ and DECS.
* Strudel and the rest of dasAudio. It has SF2, which happens to be excellent. It has MIDI. Strudel can do SF2 and HRTF like it belongs (because it does).
* dasIMGUI. well almost, u could not have tried that - and maybe for another week u should only peek. but its shaping. Claude can talk to it out of the box - don't write an MCP to your editor, just be.

So what about that 0.7

Easy. Technical debt free Boris sits there, fixes bugs, and works on performance - while addressing everyone's troubles.

* Compiler will get faster. Somewhat. Its getting hard
* Interpreter will get faster - I got more tricks (I know, hard to imagine)
* JIT will get faster
* GC will get faster
* LINQ will get even faster
* DECS will get ... u know the drill

I gonna get few more things hooked up along the way, but Vulkan is for sure. We have DAS->GLSL macro, and we have LLVM. I just don't know when. There are basic bindings already.

So much to pray for. Like a good shaman me.
