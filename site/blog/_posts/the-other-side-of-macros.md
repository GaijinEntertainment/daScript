---
title: From macro hell
date: 2026-05-24 23:10:20
tags:
    - daScript
    - Hell
---

I have macros and I'm not afraid to use them.

<!-- more -->

[IMGUI](https://github.com/ocornut/imgui) needs wrappers. But not just any begin/end wrappers. It needs playwright. Inspect. Invoke. Serialize. Other scary words. Also APNG recording. Narration. Mouse trails.

I also think there was no documentation to speak of. [Now there is](https://borisbat.github.io/dasImgui/). But I digress.

So I have my macros. I also have lambdas. I like `json_boost.das`. But what I really like is to have the right EDSL for the job. So I'm going to generate a lot of canonically clean code, and it will be amazing.

If only. Lambda is 1 structure + 2 functions. `JsonValue` instantiates reader and writer every time u look at it funny. It is glorious. It's very clean.

It took 30 seconds to compile.

In the land of daslang thats unheard of. I've never seen anything which takes that long. Entire games shipped with shorter compilation time. So I started looking.

C++ side of the compiler is something I'd love to profile more. Problem is that synthetic tests reflect nothing, and regular tests are too noisy on short intervals.

So here I am with PerfView, trying to see if I've missed anything during previous 99 optimization sessions. Turns out I did.

I am at 20 seconds, and your das compilation time just went down ~20%

Then we started looking at macro emissions. Did you know that each lambda is two functions plus a structure? If it can be a local function `@@` - it should be. The best ones, of cause, are the ones you don't emit at all.

So at 13.5 sec the light finally shines at the end of that tunnel. Its a train.

Next was `JsonValue?` with `from_JV` and `JV` pairs. They are excellent when u want to customize your serialization and do some other fancy shit on random data. But if your data is simple, and widgets are simple and well defined in compile time, there are `json_sprint` and `json_sscan` - they take any das data and marshal it to json and back. There are also `json_sprint_at` and `json_sscan_at` - those take `TypeInfo` pointer.

There are also `binary_save` and `binary_load` in `builtin.das`. Its the same story, only without json. Good story, but for another day.

I'm sitting here at 7.5 sec, with the promise there is more. There is always more. At the end of the day plain functions on top of plain structures are it, and always will be.



