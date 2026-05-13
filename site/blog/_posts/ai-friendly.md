---
title: ai-friendly
date: 2026-03-06 20:19:47
tags:
    - daScript
    - Claude
---

You say You are AI friendly. But how friendly are You?

<!-- more -->

All this talk, adjusting syntax, working on iteration speed - surely this is going to be beneficial one day. Right?

Well, its time we found out. Claude Code with Opus 4.6 is actually capable of doing serious tasks on Daslang codebase, both in C++ and DAS.
Good news - it knows DAS out of the box. Bad news - it knows gen1 syntax well. Good news - it was easy to have it adjust.

And adjusted it is. With the right amount of instructions and skills, it started to write DAS code pretty fast.
With even more instructions it became capable of writing some fairly decent macros. And it started to look like something beautiful.
Iteration speeds were decent, code was being written. Things were looking up.

Then it got better. At some point one can't help but ask - "so, how do I make your job easier". "make me an MCP server, duh".
Ok. And I'm like - "I know nada about it, why don't u make me the simpliest one?"

So we got protocol down, JSON parsing, HTTP layer, all this boiler plate. Made a tool. Made it pick it up. Cool.
Asked for which other tools it wanted. Got a wish list. Started going though it - mainly by making it make it's own tools.

It was fascinating to see more and more tool calls, and less and less 'greep' and 'bash' in the output.
The more tools we made, the faster things got. A hammer took a minute. A saw was there quickly.
A lathe arrived in a jiffy. By the time I came to my senses, we were building industrial grade 5-axis CNC machine.

Whats even more interesting, a lot of functionality is very similar to what our Vscode plugin is doing for people.
Other stuff was very close to different log modes which we already had. Some were shortcuts for the tools - just to speed things up.

There was hardly anything new, apart from the MCP bindings.
I am finally approaching stuff, which we don't have exposed via plugin - so I wonder how that would go.

Another big change is the focus on testing. To iterate fast you can't afford to regress.
So every new feature now gets decent set of tests from the get go. We went from ~1k to about 4k, and it is growing fast.

I'd like to mention documentation and tutorials, but in this new reality its a given. Someone will read them. Or something...

At the end of the day MCP server is going to be part of releases.
0.6.1 will go out with CLAUDE.md, skills, MCP server, and many more goodies.

Stay tuned.


