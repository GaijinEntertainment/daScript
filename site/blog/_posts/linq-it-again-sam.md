---
title: LINQ? yes, really
date: 2026-06-05 22:07:00
tags:
    - daScript
    - LINQ
---

LINQ? I think I've seen that one — but that was a year ago.

<!-- more -->

Reader macros are dope. We use one for regex. There's a JSON one. There's a string one. I even made a spoof one, just to see if I could — and that one needs to go. But we've yet to tap their full potential.

There are two ways to handle a reader macro. One is to parse and return an AST. The other is to parse and return a text buffer — and let the compiler deal with it. Most of the time the first approach is way cleaner. But sometimes — sometimes u just wanna wrap some code in a bit of quick text processing, Perl style (remember that one?).

Enter C#'s integrated LINQ syntax. Did u know it's a mechanical rewrite? Like, `select` becomes `|> select` or something. It's so ugly it's beautiful.

```daslang
var a <- %linq! from c in cars where c.price > 100 select c.name %%
```

What is `c`? I dunno. Could be an array. Could be a DECS query. Could be a SQLite table. Could be that JSON of mine. Or an XML. Can't be a daslang `table` though — not yet. Not for the next 4-5 days, until I wire the proper adapter. Could be something of yours — that adapter is not that hard to write.

Comprehension? I know, I know. Not a big deal. But how deep is the rabbit hole?

```daslang
let perHq <- %linq! from h in hqs2
                    join c in cars on h.brand equals c.brand into g
                    select (brand = h.brand,
                            country = h.country,
                            n     = g |> length,
                            total = g |> select($(u : Car) => u.price) |> sum) %%
```

It's fast, too. Most of the time it's already about as fast as a hand-rolled loop; and where it isn't — I'll work on it until it is. This is the fast path.

Of course u can always write the same thing without the reader macro. But where's the fun in that?

```daslang
let perHq <- ( _fold( each(hqs2)
    |> _group_join(each(cars),
                $(h, c) => h.brand == c.brand,
                $(h, g) => (brand   = h.brand,
                            country = h.country,
                            n       = g |> length,
                            total   = g |> select($(u : Car) => u.price) |> sum))
    |> to_array() ) )
```

`_fold` does most of the heavy lifting. Forget the `_fold`, and it all falls back to good old higher-order functions — the dog-slow kind. `_fold` also figures out your inputs and routes them to the right adapters. They, in turn, optimize the hell out of the sequence.

I like homogeneous environments, with a single unified source of truth. But every now and then I want a slightly more familiar syntax, without leaving said environment. That's when a reader macro has your back. Happy reading.
