---
title: running-it-live
date: 2026-03-20 09:55:49
tags:
    - daScript
    - daslang
---

I like live coding. Here, I said it.

<!-- more -->

Daslang is built for it, its in the DNA. Games had it from pretty much the get go. Edenspark has it.
Even raw daslang sort of had it with the live.das. Kind of. But we can do better.

As part of 0.6.1 (which is gonna be first week of April) daslang-live executable will be there, next to the regular dalang executable.
Or you can check it right now - clone master, build, run.
There are accompanying modules, and tones of examples. Even couple of reasonably implemented games like 'arcanoid' and 'sequence'.

More importantly, this is what your app should do - from the get go. Sadly, it's app specific - so I can't just provide you with "make it awesome" function,
which does it for you. But I can provide you with an example of such integration - and thats the 2nd purpose for daslang-live. It's small enough,
and will arrive fully commented and documented.

Here are some things your game needs to handle, to support live coding - apart from the obvious ability to reload and recompile, without restart.
* be able to do full restart automatically. because things will get broken to the point, where it has to restart.
* handle broken state from script not compiling, and be able to reload after it's fixed
* handle broken state from script failing with exception - and clear all the data it could have potentially corrupted
* persist data between reloads - either in the global storage, or by keeping scripts stateless
* handle multiple instances of the app running - or block it

You would also want an MCP server, if AI is doing some of your live codding. To me it kept forgetting how to talk to the app.

There are more considerations - like the rest of the app needs to handle reloads gracefully. Once you start live-coding, you'll know all about it.
Because it will fail, crash, get into states which require full reloads, or worse.

The way I approach it, I treat live-ability as very high priority. If I'm live codding, and something in the process failed - I fix it,
till it survives those errors - and allows them to be addressed live.

I'd like to get to the point, where I start with black screen in the morning, and end up with the game at the evening - without having to restart or crash once.
It's not there yet, but it's getting pretty close.

Obviously, its not just for the games. I've tried it with the REST API development, MCP plugin itself, few bigger projects.
The robustness it adds to the system is an icing on the cake. Its like testing your app 100% of the time, in the real world scenarios.
