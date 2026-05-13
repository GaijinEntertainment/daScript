---
title: Zero point five
date: 2024-10-10 17:36:36
tags: daslang
---

Construction can't be finished, only stopped. 0.5 is out.

<!-- more -->

Whats in there? JIT is good. Less bugs are less buggy. Simpler syntax is what it is. Who would not like [1,2,3] vs [[auto 1,2,3]].
Things are slightly faster. There are type macros, and type functions - and there will be a post about that at some point, once the helper functions and macros are stable.
Serialization is a big deal - helps with the load times.

With that out of the way, lets talk about future. 0.6 biggest feature is going to be syntax revamp.

Sure, a few things will tag alone. I'll likely add proper regex bindings, and few other libraries.
Maybe standalone contexts will finally make it. This and that, but syntax change is where its at.

We are going to say good bye to the significant whitespace - after long transition period with double parsers, and with the script to convert existing files.

To understand why first thing is to understand why its there in the first place beside being modern and in vouge, my love for less typing notwithstanding.
The main reason is C++ integration of the language which is very much not like C++.

The idea was that people will switch the mindset together with syntax, and not write C++ code is Daslang. Naive, I know.
Lets look at couple of examples from our short but intense history.

Originally there was no plan to support classes, or structure methods for that matter; the idea was not to have significant OOP support at all.
First thing people did is an OOP implementation on top of structures.
If you can't beat them, join them—and then lead them - now we have classes, visibility, and other OOP bells and whistles.

Then there were properties. First via :=, and now we support += operators and such.
More stuff followed - think match, initialization shortcuts, dot after enum, and many a thing.

Thats because people like to write code a certain way.
AI completion (like copilot) reflects it even more. Who am I to tell how collective unconscious (?) wants to write their binary search.

At this point I suspect I'd have to add more syntax sugar around some basic generic data types,
though tuples do the job nicely minus the visibility. There will be another post about that as well.

That being said the single biggest complaint was significant white space. Turns out there are enough people who don't like python syntax,
and some won't touch something like that with a 6 ft pole. Worse a flex\bison pair which we use for parsing does not like it either,
it takes constant effort and limits features significantly.

There is also an issue of 'cryptography'. Which is people really don't like to read something like

    $ <| [ann1,ann2(name=3)] [[&foo,:=bar]] ( ... )

For it has to many cryptic characters. Who knew.

There is also a learning curve. C# was easy to pick up, because I already knew C++. Python was harder, and took getting used to.
JS was ok, because many things were familiar, and I only had to learn new concepts (even more so with C#).

This is where we are going with 0.6. I'll probably look a lot more like C#. It will get more verbose and less cryptic.
It will also get the dreaded 'transitioning from C-like language' tutorial, with all the things which are the same.

I'm not looking forward to that stupid das -> das_c_like script, but I am looking forward to simpler parser.
I'd like to use my tricks to make syntax better, as oppose to make semicolon work 'every time there is new line but only when necessary'.

