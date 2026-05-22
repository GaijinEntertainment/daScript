---
title: Verbose much?
date: 2026-05-21 22:10:20
tags:
    - daScript
    - AI
---

Much code. Very explain. Wow.

<!-- more -->

It's AI friendly Boris with his very AI friendly daslang getting revelations. For once I'm not trying to catch the last train, and maybe, just maybe, am close to the locomotive. Naive, I know.

I was always for less typing. You can see it in daslang design. "Don't declare types, unless u declare data". Lots of shorthand notation (my colleagues call it cryptography). Generics to not type the same things twice. Macros to not type at all. Code reuse and automation.

Less code to write means less code to read. Less room for errors. Easier to understand. Overall felt like win. Intuitively. These days we actually have definitive proof. Less code fits in the LLM context way better than "I'm all for typing"; not that I've seen that attitude in a while.

How much less? With style lint, and perf lint, it's been shrinking. Question is how far it can be pushed, especially when macros are involved.

Today I had my first reality check. We've been working on linq_fold.das - it takes linq expressions, and turns them into very solid loops; the works. I like nice academic iterator chains, but they are just not robust. I like writing them. I like reading them. Running them is a problem. linq_fold solves that problem for containers and for DECS (das has its own ECS).

Don't get me wrong. linq_fold has to do a lot of work. It's somewhat similar to EFCore/Query + EFCore.Relational/Query (~66k raw / ~43k code lines). I think EFCore is awesome. I also think I'm nowhere near done.

It reached just over 5k of heavy macro code and I've decided that its refactor time. Oh, the confidence. How hard can it be. I'll add some lint rules, helpers, look for patterns. And... we are close to 4.8k of slightly cleaner code. 200 lines. Thats it. qmacro, qmatch, all that machinery - already there. Code dedup every PR - already there. Patterns - well I found one, and now its a helper and a paragraph in a skill.

But then I thought I can optimize 5k lines of macros. Maybe not. Maybe thats what it's boiling down to.

Dunno. Been a lot of talk about languages for AI lately. How to save a few tokens here and there. Cute?