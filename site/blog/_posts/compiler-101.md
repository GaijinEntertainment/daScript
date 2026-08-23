---
title: Walking the inline.
date: 2026-07-12 5:06:47
tags:
    - daScript
---

Ask not what your compiler can do for you - ask what you can do for your compiler.

*- Y.T., Compiler 101*

<!-- more -->

Really it's all about the interpreter. When we ship with AOT, the C++ compiler picks up the slack, despite the clutter. When we run JIT, which is that same LLVM only better, it picks up even more slack - for there is no clutter, plus better incantations, bigger inline limits, and memory hints. CSE, DCE, the rest of compiler 101 - it just happens.

I've landed a lot of that machinery recently and hardly found anything in daslib, or in most of the shipped titles. Lately lint warns on dead code anyways. That seems to be the theme - if lint can catch it, why bother optimizing it out. Just change the code. But I digress.

With [Edenspark](https://edenspark.io/) we are facing a different audience. "Different challenges," they like to say. Really it's down to the slack which no one can pick up. Do some escape analysis and emit 'delete' on local data. Better yet, make heap allocations stack allocations. Do some bounds checking, "since u r at it" - but only because u r at it. Make those arrays allocate on the heap straight away.

After all is set and done, do inline. That's where the money is. LLVM can't optimize around 'invoke'. It can inline function calls, but blocks are entirely different. Turns out it's not that hard to do. Turns out it speeds up the interpreter 4-5x. Automatic function inlining - that's another "since u r at it". Add a contract though. [inline] does not compile if it can't be inlined. But it will inline across the module boundary. The automatic one won't (think about it).

    def map ( a : array<auto(TT)>; predicate : block<(arg:TT):auto(QQ) > ) {
        var b : array<QQ-const-#>
        for ( element in a ) {
            push ( b, invoke(predicate,element) )
        }
        return <- b
    }

    [export]
    def main {
        var foo <- [10, 20, 30]
        let index <- map(foo) $(x) => "{x}"
        print("index = {index}\n")
    }

If only it did not invoke per element, things would be so much faster... Oh wait. Here is what `options log` prints:

    def main {
        var foo : array<int> <- to_array_move(fixed_array<int>(10, 20, 30))
        let a : array<int> const& = foo
        var b : array<string>
        for ( element in a ) {
            push(b, string_builder(element))
        }
        let index : array<string> const <- b
        print(string_builder_temp("index = ", index, "\n"))
    }

Does not help LINQ. That one already folds those predicates in. Because LINQ is awesome. But now, now all this silly functional nonsense becomes practical. As long as you stay with arrays, that is.

There are restrictions, of course. Currently a return from inside a loop won't inline (but not for long). It's that compiler 101 class that likes to strike back at odd times. But I'm trying, Johnny, I'm trying.

Inlining is a hard line to walk.
