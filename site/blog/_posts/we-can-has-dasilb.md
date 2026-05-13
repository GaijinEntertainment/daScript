---
title: we-can-has-dasilb
date: 2026-02-20 21:20:35
tags:
    - daScript
    - daslang
---

Sometimes technical debt gets paid off.

<!-- more -->

[Documentation. So much documentation.](https://daslang.io/doc/index.html)
And tutorials. [C-integration tutorials](https://daslang.io/doc/reference/tutorials.html#c-integration-tutorials),
[C++ integration tutorials](https://daslang.io/doc/reference/tutorials.html#tutorials-integration-cpp),
[Language tutorials](https://daslang.io/doc/reference/tutorials.html#language-tutorials),
and even [Macro tutorials](https://daslang.io/doc/reference/tutorials.html#macro-tutorials).

Its in the GIT already. It's on the [website](https://daslang.io).
Its so awesome, it almost makes me cry. Almost.

First 90% were easy. Second 90% we kind of hard. The remaining 90% took even longer.
I can't wait to finish another 90%. It takes years to realize, how amazing MSDN documentation is.

But wait, it gets better. Daslib is maturing. Little missing bits are adding up.
A lot a bugfixes - but most are features.
They are all documented. Many are in the tutorials too.

Let me go through some of them.

----------
interfaces
----------

This one got a lot more useful.

```
Interface inheritance: [interface] class IChild : IParent
— ancestor getters auto-generated for is/as/?as
Default method implementations — non-abstract methods inherited by proxy
Completeness checking — compile error on missing abstract methods
```

-------------------
regex & regex_boost
-------------------

It's still no PRCE. It's still very naive. But it just got a lot closer.
Still no lookbehind though.

```
Counted repetitions: {n}, {n,}, {n,m}
Anchors/boundaries: ^ (BOS), \b, \B
Non-capturing groups: (?:...), named groups: (?P<name>...)
Lazy quantifiers: *?, +?, ??, {n,m}?
Lookahead: (?=...) / (?!...)
Case-insensitive + dot-all mode flags

regex_search, regex_split, regex_match_all, regex_group_by_name
regex_replace with template strings ($0, $1, ${name})
operator [] for group access by index or name

Reader macro flag suffix: %regex~pattern~flags%% (i = case-insensitive, s = dotAll)
```

----
json
----

```
Proper \uXXXX Unicode escape → UTF-8 encoding
```

-------------
strings_boost
-------------

```
contains(str, sub), count(str, sub), last_index_of(str, sub)
pad_right / pad_left (with optional fill char)
trim_prefix / trim_suffix
capitalize, is_null_or_whitespace
```

----
decs
----

This one gonna get a lot more love soon. Like batch object creation.

```
is_alive(eid) — check entity is alive (generation matches)
entity_count() — total alive entity count across archetypes
get_component(eid, name, defval) — named component by value with fallback
```

---------
algorithm
---------

```
upper_bound, equal_range — binary search family (with/without comparator)
fill, is_sorted, rotate, min_element, max_element
```

----------
functional
----------

```
reduce_or_default, fold, scan — reduction/accumulation family
enumerate, for_each, find, find_index, partition
tap, iterate, chain, pairwise, flat_map
repeat now infinite by default; echo is non-destructive
```

----
linq
----

```
order_descending / order_descending_inplace / order_descending_to_array
count(iter, predicate) — count matching elements
skip_last / take_last (4 variants each)
zip / zip_to_array with 3 sources
```

------------
jobque_boost
------------

```
try_pop, try_pop_clone — non-blocking channel pop
pop_with_timeout, pop_with_timeout_clone — timed channel pop
with_wait_group — auto-join wait group pattern
done(status) — alias for notify_and_release
parallel_for, parallel_for_each, parallel_map — automatic job partitioning macros
```

-------------------------------
bitfield_trait / bitfield_boost
-------------------------------

```
each / each_bit_name now support bitfield8, bitfield16, bitfield64 (not just bitfield)
each renamed to each_bit in bitfield_boost, returns iterator<bool>
```

---

Soon it will all be unleashed on the unsuspected world in form of 0.6 release.
Like with downloads, binaries built, modules built against the release, the whole 9 yards.
