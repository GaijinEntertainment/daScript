---
title: Daslang it is
date: 2023-12-28 20:51:24
tags:
    - daScript
    - C++
    - C#
---

The plug has been pulled. Daslang it is. Take a look at dasCHash too ( C#->DAS ).

<!-- more -->

{% post_link its-not-a-script %} so renaming it was a matter of time; and the time is now. Too bad, I liked the old name. Now lets make a new one proud.

We are well on our way to 0.5.
* the works - oh how I love the works; bugs fixed, documentation updated, small features, large features, this and that.
* serialization is there and is kicking ass. Can save. Can load. Can check if it needs to be regenerated. Blazing fast.
* JIT is well on the way. Lots of new tests. A lot more things can now go all the way JIT. Some crash though and LLVM has not been on its best behaviour but more of that later.

We took the detour though. Welcome C# -> DAS. Yes, really. [I like `hash` instead of `sharp`](https://github.com/borisbat/dasCHash). Why, you ask? Did someone mention Unity?
There is some sidekick code on the Unity side and we'll publish it at some point too. There will be more announcements.

On the plus side Daslang now supports a lot better property system. Classes got face lift. Static properties. Abstract properties. C#-style overrides, all that jazz::

    class Foo
        dir : float3
        def Foo ( x,y,z:float )
            dir = float3(x,y,z)
        def Foo ( d:float3 )
            dir = d
        def const operator . length
            return length(dir)
        def operator . length := ( value:float )
            dir = normalize(dir) * value

JIT is whole different story. It feels like it should have been done by now; after all its getting close. There is whole LLVM issue.
LLVM-C interface feels like an afterthought. My main complaint is that it crashes on wrong types of input, instead of reporting an error.
I guess I should have manually wrapped it by now but that seems like an overkill. Pain to debug, demotivating.

LLVM crashes on some optimizations; my hope is to upgrade the version. I'm currently stuck to 15.7 because, you guessed it, opaque pointers::

    LLVMContextSetOpaquePointers(LLVMGetGlobalContext(), 0)

Yes, that thing. Once addressed, for sure some of the optimization related crashes will go away. Right? Right???
The more I play with it, the more it feels like a C++ compiler backend, and not a 'general purpose multi-platform backend'.
The results are spectacular though 18.7 mrays/sec vs 7.2 mrays/sec for raytracing demo on single core on JIT vs AOT.
So hopefully soon it will get stable enough.

We did some work on standalone contexts. Hopefully soon. Once there - everything possible goes native. We'll see how it plays out.
Looking at the new year with hope. This one was a tad slow at times. See you in the next one.

P.S. What wishes do I have for new year? I'm glad you asked. Mercy. Just kidding.

