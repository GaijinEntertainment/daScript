---
title: Its like dejavu all over again
date: 2026-05-19 19:48:17
tags:
    - daScript
    - C++
---

I feel like I've done this before. It brings back memories.

<!-- more -->

Back in 1812 we went from Borland C++ 3.1 to 4.0. It had 32 bit support via DPMI. I had 32 bit PC. We later switched to Watcom C++ with its DOS/4GW. Fun times (ask your AI of choice to explain).

Part of the fun was porting codebase to 32 bit. It mostly just worked, somehow. Not that it was that much code. Not that it had any test coverage worth mentioning. There were few containers, some pointer math here and there, but overall smooth sailing. Took like half a day.

Its 2026 and I'm switching das to use 64 bit arrays and tables. I could have made that choice 7 years ago. I seriously thought about it. It was an embedded game language at the time. Since then we've hit this limitation once - due to a bug in a serializer. Its just not that important, as long as overall memory pool is not limited to 64 bits - limiting tables and even arrays is not an issue.

Until u want to play with inference that is. Big models, big matrices, fast idiomatic code. I'm pretty sure I can get there without intrinsics - quantum macros advantage with LLVM fast path is something else. You just wait and see. CPU first. Then maybe Vulkan, and sadly CUDA.

But to even attempt something like this, we need 64 bit arrays. So here I am with my trusty (but verifyish) companion looking at 8.5k+ tests, couple dozen systems, and 7 years of bad habits. It'll hit 9k before we're done.

It could not have been size_t - it had to be uint32_t - otherwise bindings diverge. 32-bit version will suffer. It'll likely choke. 32-bit MSVC hardly works. 32-bit CLANG has its moments. It had one thing going for it - FP exceptions, but today I would not even bother.

Do you also measure new features in tests? Do you measure your technical debt in tests? Or do you scream AI slope?? Its turtles all the way down that rabbit hole.

As a result I have time to write a blog post, while mechanical search-and-rewrite is searching for int32_t/uint32_t/int where the data is, adding tests every time one is removed, replaced, or 'preserved'.

There will be long_length next to length, long_count next to count, long_find_index, and some other goofy stuff which u can find in legacy systems. I am writing a legacy system after all - lisp+fortran in disguise of a modern language syntax.

Imagine breaking 8.5k tests, several released titles, and few more 'in production' just to play with inference and loop optimizations... tempting. Thats how u end up with long_reserve and 2 sets of array index nodes with independent fusion.

It is AI friendly though; when its das. When its C++ part - its blog time.
