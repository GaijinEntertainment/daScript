---
title: There is something about JIT
date: 2022-11-20 09:46:37
tags:
    - C++
    - daScript
    - gamedev
---

In the early days of daScript development having robust JIT was never a priority.
In fact during the initial planning meetings JIT was postponed until the cows came home.

<!-- more -->

It may seem controversial until you realize the role JIT plays in the development pipeline.
Final code is converted to C++ via AOT and that includes development builds.
As the result, only a small percentage of the code is interpreted at any given time.
Guaranteed it may include some hot spots but between the ECS and the blasting fast daScript interpreter its very rarely an issue.
On top of that JIT is prohibited on most gaming platforms, so it does not solve all that many game development problems.

When [Dasbox](https://github.com/imp5imp5/dasbox) came online things changed. Some of the games developed for it were quickly CPU bound.

My [first attempt](https://github.com/borisbat/dasXbyak/tree/main/jit) at JIT was soon to follow.
I initially considered LLVM, but decided against it due to size of dependencies and the amount of C++ bindings I have to write.
I settled on XBYAK and limiting the exercise to x64 platform only. After all, this is where the bulk of JIT happens.

This project quickly showed some promising results in both performance and compilation time but ultimately failed.
The endeavour turned out to be bit more than I could chew in a reasonable amount of time.
x64 assembly is cumbersome - there are too many options, and they affect performance. Register coloring is not uniform.
It was looking like a full time job for a medium size team, not a 2-3 month project.

On the plus side daScript now has well integrated x86/x64 inline assembly generator.
The WEB version demonstrated another issue. Running the interpreter on top of already managed code is silly.
It was back to the freezer for the JIT.

In the meantime more fun things were happening. I've integrated LLVM CLANG compiler infrastructure for the purposes of automating C++ bindings.
Then I've made it self hosting. [LLVM CLANG binding](https://github.com/GaijinEntertainment/daScript/tree/master/modules/dasClangBind) was generated via LLVM CLANG binding.
The minimalists version which bound only functions needed for the binding was no more.
All the sudden it became fairly easy to bind large modules like SFML, BGFX, Imgui, and ultimately LLVM-C.

With LLVM-C in place it was time to take that JIT out of the closet - and it went reasonably well.
There is of course the initial learning curve; this is my first LLVM based project after all.
On top of that the majority of the examples are written using C++ interfaces to LLVM and at times LLVM-C seems like a bastard love child.
But once the ball started rolling it became clear that this is the way.

During the initial phase of daScript development [a set of performance tests](https://github.com/GaijinEntertainment/daScript/tree/master/examples/profile/tests) emerged.
I was constantly comparing performance with the other scripting languages, as well as C++ baseline. Its just as important for the JIT.
Plus I was hoping that given the nature of LLVM I would be able to give it more information than I give to the C++ compiler in AOT.
In theory it should result in better performance.
If thats indeed the case, we can generate AOT that way as well - LLVM is perfectly capable of outputting a dynamic or static library.

There is this warm nostalgic feeling of looking at the same code few years down the road. I started with the fibbonacci and went down the list.
Here is performance snapshot from this morning. All tests compiled with MSVC 2019, Release. JIT is using an LLVM version 13.

| Test                    | C++        | daScript   |
|-------------------------|------------|------------|
| dictionary              | 0.045285   |__0.019225__|
| exp loop                | 0.003801   | 0.003735   |
| fibbonacci loop         |__0.001612__| 0.001936   |
| fibbonacci recursive    | 0.00624    | 0.004375   |
| n-bodies                |__0.699158__| 0.787601   |
| particles kinematics    | 0.004502   |__0.002981__|
| primes loop             | 0.038882   | 0.038846   |
| tree                    | 0.187152   | 0.173577   |


We are not really comparing apples to apples here.
* In the dictionary test C++ uses std::unordered_map. daScript tables are more robust.
* C++ n-bodies implementation is manually vectorized and performs better than naive daScript version

There are two tests which actually stand out.
* In `fibbonacci loop` MSVC produces better block structure.
* particles kinematics is faster with daScript due to better vectorization, as well as some fancy float3 optimizations in the jit itself

Even this early on overall JIT performance looks on par or slightly faster than C++.
It would only get better from here - aliasing optimizations are not implemented yet, as well as many other LLVM features.

My next big goal is to make JIT fully functional. Its already robust enough to try on individual hot spots here and there, but it will take some time to implement the remainder of the daScript language features and runtime.
Next big performance test is going to be the path-tracer. I expect great things due to the vectorized nature of the code. Stay tuned.

