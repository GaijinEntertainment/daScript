---
title: LINQ?
date: 2025-10-08 17:27:00
tags: daScript
---

LINQ? But Boris, DASLANG already has high-order functions.

<!-- more -->

Like seriously, just require daslib/functional and you are done. Its only 340 lines.
It reads like a Haskell tutorial. Well. Almost. Its concise. Its lazy. Its also slow.

Lets start with an example

    require daslib/functional

    [export]
    def main {
        let seq = (range(10)
                |>  each()
                |>  map(@(x : int) => x + 1)
                |>  filter(@(x : int) => x % 2 == 0)
                |>  reduce(@(a, b) => a * b))
        print("2*4*6*8*10 = {seq}\n")
    }

What we have here, is a chain of 3 iterators, and a reduction loop. Yea, dog slow.
I think I'm going to deprecate it eventually. I never liked it. I could never remember function names.

-------------------
I always liked LINQ
-------------------

There is a better way

    require daslib/linq_boost

    [export]
    def main {
        let seq = (range(10)
            .each()
            ._select(_+1)
            ._where(_ % 2 == 0)
            .aggregate(1, $(a, b) => a * b)
            ._fold()
        )
        print("2*4*6*8*10 = {seq}\n")
    }

On the first glance its very similar. Minus the fold thing of cause. And the anonymous notation for the 'select' and 'where'.
Also it uses blocks instead of lambda. But the overall feel is the same.

Performance however is not. If the 'fold' was removed it would produce code, very similar to that of functional.
Inside the fold lies the magic

    [export]
    def main {
        var seq = invoke($ ( var source : iterator<int> ) {
            var pass0 <- [for (x in source); x + 1; where ((x+1) % 2 == 0)]
            var pass1 = pass0 |> aggregate(1, $(a, b) => a * b)
            delete pass0
            return pass1
        }, unsafe(each(range(10))) )
        print("2*4*6*8*10 = {seq}\n")
    }

Its obviously missing the select-where-aggregate onto one function fold. That and many more are coming soon.
But its already two orders of magnitude faster, to the point where its already useable.

It's also compatible with DECS, although more support is needed

    for (i in range(3)) {
        create_entity <| @(eid, cmp) {
            cmp.eid := eid
            cmp.index := i
            cmp.text := "number_{i}"
        }
    }
    commit()
    var one_then_two = (from_decs($(index : int; text : string){})
        ._where(_.index < 2)
        .reverse()
        .to_array()
    )

----------------------------------------
Now, why is it possible to make it fast?
----------------------------------------

* only blocks are supported; no functions, no lambdas
* both array and iterator sources are supported
    - only iterators are consumed
* all intermediate calculations are done on top of the arrays
    - temporary data is immediately deleted
* everything folds
    - if it does not already fold, it will

Its all very early. Its all very promising.

---------------------
What is already there
---------------------

* all basic operations from C# LINQ and a few extra
    - the list is on the very top of [daslib/linq](https://github.com/GaijinEntertainment/daScript/blob/master/daslib/linq.das)
* some obvious folds (select/where, order/distinct and such)
* basic DECS integration

----------
Whats next
----------

* more folds
* wide support for DECS (including edit)
* support for SQL and dasSQLITE
* possibly custom reader macro for even more familiar syntax?

Suggestions are welcome, as always.
Also, folds are easy to write. See [daslib/linq_boost](https://github.com/GaijinEntertainment/daScript/blob/master/daslib/linq_boost.das) for details.




