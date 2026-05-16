---
title: Internal tools
date: 2026-05-15 19:56:22
tags:
    - daScript
---

Why internal tools?

<!-- more -->

Because intern wrote them. Another intern maintained them. And that good one talked master Yoda into doing touchups once; that did not last though. But I digress.

Tools are hard - internal ones are harder yet. Think undesigned, undertested quick fix from 7 years ago; yea, that one. I feel lucky its not that other one, the shader compiler written in perl in 2004 (hi Shodan).

Luckily its 2026. With the right direction that same intern+AI can clean them up, make them good, learn in the process. Focus on quality, tests, documentation; refactor like crazy - grown into a senior engineer, if that's still a thing.

For me tools are an excellent case of dogfooding. Here is daslang MCP. Written in daslang. With test framework in daslang. Currently slightly monolithic, but I'm breaking it apart - JSON RPC section goes to daslib as we speak.

I don't sweat the quality of code on that one though. As long as it's good. Just like with other internal tools. When it's not good - I'll add more lint.

Tools tell stories. Mine tell me where daslang is lacking. I slap myself on the wrist every time 'its that small python script'. I stop and implement what's missing. Lately I get more ambitious - maybe because I miss getting slapped.

This is where the productivity gains hide. Intern wrote them. AI cleans them up. Just keep them INTERNal.
