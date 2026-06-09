---
title: It's tables all the way down
date: 2026-06-08 20:35:17
tags:
    - daScript
    - C++
    - Hash
---

The Art Of Computer Programming Volume 3. Kind of. Again. This time with hash.

<!-- more -->

linq_fold_json tells a story of small tables and data marshaling, but mostly small tables. And data marshaling. Because strtod is faster than what we have on some Apple platforms. But not today. Today its hashing time (can't touch this).

Open addressing hash string -> * hash table goes like this. Compute hash function of the string. Get initial index. Compare hashes on that index. Keep moving forward until hash is empty slot (or a tombstone if we insert). When hashes match - compare strings. If string match - its a match found. Been like that since the dawn of day.

It becomes faster with the quality of the implementation. It gets better with better or faster hash function. With trickery.

It gets bold when you say '64 bit hash is good enough for small tables'. Literally don't compare strings until there are more than say 8 entries in the table. Don't pick initial index either - its a linear search for a table that small - using vector instructions too. No hash collisions during first 8 either.

It's a statement. One of u will get unlucky. Maybe. Probably not during my time. It will hurt. Some of u may die. But its a sacrifice I'm willing to make.

The interpreter stops there. JIT just moves most of the lookup and insert to the JIT side - only "grow" remains on C++ side.

Open addressing int -> * is a tad less silly. There is hardly any point in storing hash - all it does is give u an initial index. We store 2 bits - one for empty, one for tombstone. I'll find some use for the rest of the byte at some point. Perhaps i'll interleave data for some types as well.

Once dust settles, everything is way faster (2-4 times on JIT) and uses less memory (everywhere). Not that it was bad before. But linq_fold_json stories of small tables and engineers' shortcomings are hard to ignore.

And now for something completely different.
- Boris, I did not understand shit. Are You writing for us or for yourself?
- Yes.
But. If u scroll down, there is a comment section. Its about as useful as blinkers on the BMW. In the unlikely event of something being there I answer, in detail. There is also https://t.me/daScript and somehow I got way more mileage out of that one. Both are there for u. So as blog. Definitely. Maybe.
