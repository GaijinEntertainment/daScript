---
title: NO COMMENT
date: 2026-08-21 23:25:24
tags:
    - daScript
    - Claude
---

Been a minute. But. Absolutely no comments.

<!-- more -->

None whatsoever. Comments suck. They rot. U change code, it's no longer the same code. Comments get stale. Misleading, wishful, treacherous little things.

Little? Did I say little? My buddy here disagrees. War. And Peace. And war again. Because details matter. A lot. They are scaffolding. Unless u look at other buddies — they don't care.

I'm not talking about embedded documentation markdown. Nor do I diss lint suppression. No. It's the other ones. Pages upon pages. Sure, u can afford them. I can't afford for u to afford them. U'll pay. Every. Single. Time. He reads today's edition of `daslib/linq_boost`, and boy do u pay. Thousands of lines of how and why, in perfectly good English. Every session. Every cache miss. It has to go.

He can't help himself. We can help. Here's a hook: "U wrote `//` again, don't". But. That is not the way. U r missing out. Hear me out.

By all means, delete them. Have a script that removes them. For good. After the fact. But before it does, send a rescue boat. This is what u need to save:
* rules. "when u add a kernel, register it in the table `g_all_kernels`". goes to your _local_ rules file (REVIEW.md is a story for another day)
* architecture. "this is a sorted list of elements, and we need it sorted to look good on camera". goes to your _local_ architecture overview

But wait, there is even more. List the things u need to rename.

    - struct Node { // A* node for the path-finding graph
    + struct AStarNode {

This is strangely liberating. To the point where, when u have no comments to begin with, u *first* ask to document, and then run a rescue.

Feels like magic. At times scary. At times powerful. But most of the time... no comment.


