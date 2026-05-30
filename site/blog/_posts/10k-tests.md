---
title: 10k
date: 2026-05-29 19:15:24
tags:
    - daScript
    - Claude
---

It's an anniversary. Why don't I feel like celebrating?

<!-- more -->

Today we've hit 10k tests. Feb 6 I had just under 1k. They were not very targeted. They did something, though. AOT was mostly stable, kind of. I would add a couple more tests every time I wrote something new. It was okay. I could work. I could refactor. I hardly broke things. Because I could not.

I did not write any of those new tests. I prompt "abracadabra" and they come out of the hat. 1k by hand is a chore. 10k by hand is a different job, and not the one I signed up for. Software engineer in test anyone (hi Sofia)? Now that's an old dying breed reborn.

The last couple of months, I broke things a lot more. This time with passion, with intention, with the conviction of a zealot. I had somewhere between 6k and 8k tests during that time, and they gave me a lot of confidence. Confidence to demolish things, that is.

I did a lot of breaking changes. I got rid of smart_ptr. I made those 64-bit arrays. Lambda can be copied. Named tuples are no longer compatible with mismatching names. Classes now require you to call super constructors explicitly. All those things made daslang a much better language. They also broke some existing code (sorry Profelis).

There is one more change like that coming. The dreaded fixed arrays with a Type::tFixedArray. This will break the debugger, the plugin, and some macros. It will also eliminate the need for the [] overloads on some functions (which you did not know you needed to write for your generics).

I'm contemplating changing the constness model to match C#/C++; just because people get confused. daslang has a better model, but it's not the one everyone is used to. I'm on the fence with that one. I'd give it a 15% chance this year, a 5% chance next year, and no chance at all past that. I was not a fan of 'const' to begin with; weak contract.

Those 10k tests - they made it possible for me to break things. Thing is, breaking things does not make me happy. So it's going to happen sooner rather than later. Dobby will be free.

Somewhere before 0.7 (September) I'll be done changing things. It'll settle, it'll be solid. Won't reek of design debt. Then I'll celebrate with the number du jour. 16738 tests? Count me in.
