---
title: Hectic
date: 2023-04-17 10:44:39
tags:
    - OpenAI
    - Telegram
    - daScript
---

Hectic is exactly the word which comes to mind, when I look at the commit history for the month.

<!-- more -->

There are bug-fixes. Always. Little things. Corner cases. Its especially good, when you are trying to change something big.

Things like properties. Properties are now proper functions, and proper operators. We had generic . operator for a while now.
Looked like this

    def operator . ( t:Foo; name:string )

Now, with some generic magic, like [constant_expression] you can even make it return different types.
Feels like a hack, though. Feels like good old C++ programming, which is exactly what I've been trying to avoid.
So now we also have something like this

    def operator . yourFieldNameHere ( t:Foo )

Proper function. Takes what it does, returns what it wants. More so, ManagedStructureAnnotation now implements its properties in exactly that manner.
It so happens, that old implementation was const-insensitive. Now it's proper const correct. So the big codebase had to adjust some sections of code, to also be const correct.
Took couple weeks, give or take. Made code cleaner. Found couple real bugs, like you always do.

Then alone came telegram API. Its huge, so I had to automate bindings yet again. Got HTML page with documentation, converted to XHTML.
Got PugiXML (hi Zeux) and wrote some basic bindings. Manually. Because its there to stay, and it needs to be extra robust.
With PugiXML got the XHTML parsed, generated huge 10k lines file with everything in it, including JSON serialization.

Found out it takes 4-5 seconds to compile. That is awful. It's like being back to C++ land. Again. Tried to optimize the compiler even more.
Thats hard. At this point outside of major expression\type rewrite, there are no more simple gains. Either way got it down to 3.5-4 seconds.
Than put on my thinking hat, and turned those functions into generics. And its down to 0.3 sec. You never use entire API, its too big.
So we are back in the land of instant restarts and reloads, like we should.

Then there was JSON. Some API prefer not having 'default' or 'empty' values in there, to the point where they consider it 'broken'.
Also You can prompt OpenAI to output JSON. It mainly works. Sometimes it does not. Sometimes it outputs mildly broken JSON.
Well hello def try_fixing_broken_json ( var bad:string ). It fixes some of it, otherwise retry the request.

Then there were UTF8 utils and JSON. The parser used to ignore \u. UTF32 decoder was non existent. Hacks propagated. But no more.
Hello def decode_unicode_escape ( str:string ), hello more bugfixes. Its now reasonably robust.
One day I'll have a problem which requires performant base64 encoder, utf8 utils, and other goo. Then I'll optimize it. Until then it works.

Afterwards I needed a database. So I added Sqlite3 and wrote some SQL. It needs more macros. It probably needs something like LINQ.
That will take a few days to setup, once I have a real-world problem which needs it. Mine was too small - so a few wrappers did it.

Then came few high-performance demos, which needed high-performance low-level intrinsics. Yes, in daScript.
memset16, memset32, memset64, memset128 - because filling things with 8 byte values is nice, but i need std::fill only better.
gather_scatter, gather_store, gather_store_mask_not_equ, and bunch of more - because software rasterization in the interpreted language is where its at (you'd be surprised).

Bunch of fixes in JIT to support the above, along with more testing on real world scenarios. JIT rocks. <2ms rasterization on the 1920x1080 image was surprising.
I guess it's not 1992 any more.

Than came the installer. CMake install, all that jazz. Because soon we will be releasing pre-built binaries. At least for windows and mac.
Probably with everything. Possibly with everything but LLVM modules first. So GLFW\SFML\BGFX or command line apps. Images and fonts. Imgui.
Should come with batteries included.

Installer brought up some bugs in the CMake compilation scripts. Turns out default build was not using SSE2, let alone AVX512 under MSVC.
Also CLANG build for windows needs to be revisited. Integrated Microsoft-clang (sic!) builds fine. LLVM ones, not so much. I'll fix that one day soon.
I'll probably switch to LLVM14 or even LLVM15 for the LLVM\JIT bindings, now that I'm at it. I so want proper JIT on Mac M1\M2. Because reasons.

daScript needs ML framework, though. OpenAI is nice, but it needs real ML framework. There is a branch which has libTorch compiled in. Takers?

