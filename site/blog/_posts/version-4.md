---
title: 0.4 and THE PLAN
date: 2023-05-01 17:11:37
tags: daScript
---

0.4 is out. Its a big deal. Its not a big deal.

<!-- more -->

It took a year and a half. But what exactly 'it' entailed? I was trying to get JIT in the 0.4, but its not to be. It needs more love.
Otherwise its a good stopping point; nothing more, nothing less.

The language is a lot more mature. Its shaping nicely, its a joy to work with, it makes me happy more often than not.
Despite my best efforts OOP support got a lot more mature. Constant and sealed methods, sealed classes, interfaces, member operators, and all that jazz.
I guess being multi paradigm language entails supporting programming models you do not necessarily adore. Its getting good though, and makes sense.
Macros got a lot stronger, especially with reification. They are now ready for the next step to become safe in the user code.
Lots of small things add up to make big difference. Accessors and methods. Extensive operator overloading. Variant types. More flexible generics with contracts.
Error reporting is more on point. Parser even allows dangling commas and semicolons. Surprisingly we compile faster, and when we don't - we know how to speed things up.
Macros now live in their own contexts, and for the most part don't pollute result contexts. This got safer with lock checks and validations - I wish it was free though.
We've made more things unsafe - some which we thought were safe; they weren't. C++ minefield leaves a stamp on a character, hard to get around - but I'm trying; I promise I'll be more paranoid in future.

Having clear goals for the releases seem like a good idea. Master is safe. Grab the latest master, build it, work with it.
Once releases come online - grab the latest master, work with. There is no point in lots of small releases, since there is no plan to support older versions just yet.
However we are the the point where clear direction will facilitate the process. Plus it provides framework for the feedback.
"Boris, you are missing XXX" or "Boris, we need YYY faster" is a types of suggestions which I always consider.

To call it 0.5, it will take

* JIT. "all tests compile and run with jit; all demos compile and run with jit; showcase examples are there - including partial and full jit; jit happy with threading, jobs, etc"
* serialization. save compilation results in binary form, add macro support for 'after the serialization' so that the state can be restored. main idea is to get the startup time down by a lot.
* validation. argument aliasing, 'shader like' code annotation. project callbacks for validation. user `scripts` should be safe enough.
* 'safe' macros. signing unsafe flags. every builtin macro to sign unsafe flags. goes under that same `safe enough` umbrella for the user `scripts`.
* binary releases. "go to webpage, follow the download link, download the latest master binaries for Win64 or Mac OSX".
* 64-bit happy. len to replace deprecated length. 64 bit pointer arithmetics. 64 bit everything in the memory model. array size\capacity and table size\capacity to be 64 bit. 64 bit array indices. counter to replace deprecated count. non of this is necessary just yet, not even close. but it will be.
* the works - getting documentation up to date, updating website, updating readme, updating build, replacing 0.4 with 0.5 everywhere, perhaps documenting the release process, and, of cause, making the branch.

It will have more things than that. I like tangents. I like things which increase productivity, make quality of life better, or otherwise cool. I often just add them. But the list above will make it to 0.5, or the 0.5 'not to be, left for 0.6' list.

I like 'by the end of the year'. Its a bit optimistic. JIT and serialization are the big ones. JIT is almost there. Serialization by itself is easy - its the implications to the environment which would take time and effort.

1.0 is ways away. I don't have any specific date. I don't even consider specific date. Tall call it 1.0 it will take the following

* versioning. daslib/version/... and the works. that way we can run old and new versions of daslib alongside, and maintain old releases if need be.
* standalone das contexts. AOT -> C++ custom class, which has all the functions explicitly as member functions. Does not require compilation or source code. 'Just works'. Can be linked with the daScript runtime, minus the compiler. Can function as part of runtime.
* many portions of the compiler rewritten to daScript - AOT, print, and debug print - to become standalone contexts first; optimizations and later infer to follow.
* smart_ptr validation. potentially changing memory model, possibly changing to ref-types - it `works` when its impossible to write macro which leaks smart_ptr. possibly moving entire ast to daScript native types, and getting read of smart_ptr altogether.
* compiler portion of the runtime explicitly separated. should be able to link without a compiler.
* safe default C++ bindings. some postprocessing on the module, to force string null to empty, among other binding issues, to be addressed automatically. potentially mark everything C++ unsafe, and always require daScript boost package - in the paranoid mode.

There are lesser topics which will find there way in alone the way. I'm fine for some of them to be released past 1.0 if they have to.

Major language features:

* inlining.
* unsafe. this needs major cleanup.

Parser and lexer:

* bind PRCE or RE2. current regex engine is cute. a better multi-regex rewrite is overdue for the purposes of LEXER, but a robust PCRE\RE2 implementation with possibly identical front-end would make things a bit more robust.
* parsing framework. I'd like me some LR1 and PEG. Ideally with the same front end. Needs good macro support to make rules look organic. once there, daScript parser needs to be rewritten in daScript.
* ast_print should produce compilable results.

Homogenous environment:

* bind VULKAN. expand GLSL. possibly support SPIR-V explicitly. make homogeneous compute model - current options are based on OpenGL compute, and BGFX compute; they are not spelled out explicitly. potentially they can be.
* expand GLSL to support HLSL. support permutations in form of pins.
* bind some sort of ML framework. There is LIBTORCH branch, but someone with experience should actually do it.
* consider CUDA?

Optimizations and linter:

* data flow analysis. its a bit cumbersome to do on a tree, but just as doable and just as robust. if need be can be converted in different form.
* better boolean optimizations. we do very little at the moment.
* even better constant folding. we do a lot, but there is much more - we hardly do any partial folding, i.e. *0, *1, +0, etc. yes, that simple.
* CSE - which ties into data flow analysis. something very basic will keep interpreter happy. anything above is a waste - JIT and AOT are more than capable.
* AOT optimizations. We can be less naive with ABI.

Modules:

* upgrade ImGUI to the latest version. should be as simple as get-latest, re-run binder, re-compile. Its possible there are still issues with the binder - clean up is more important than 'just do what needs to be done with bound code'. It needs to be 100% autogenerated.
* upgrade BGFX to the latest version. ImGUI stuff applies. recompiling tools should be part of the build, not manually built and copied.
* upgrade SFML (or let it go). it was used in the DASBOX, but never exposed. I'm not sure where this goes any more.
* expand PugiXML bindings; once there is use case.
* expand LibHV binging; servers and web development are already very possible - streamlining the process is a good idea.
* expand Sqlite3 bindings; set of macros is necessary, to make it similar to LINQ.
* expand raster. should be able to draw a tad more in a safe fashion in both 8 and 32 bit.
* UI framework. I have ideas. I don't like any existing solution. This is bad, right? :)

Tools:

* refactoring in the plugin. 'extract method', 'extract expression' to name a few.

Tests:

* AOT should be part of testing.
* Old tests need to be ported to new tests.
* Multithreaded version of test compiler with bunch of sanitizers as part of the release.
* Fuzzy tests need more work. Some sort of result driven randomization.
* Can we generate an ast and try to crash the infer? Can we do the same for the parser?

I call the list above THE PLAN. Its nice to have it spelled out. I'll continue revisiting it to keep things more transparent.


