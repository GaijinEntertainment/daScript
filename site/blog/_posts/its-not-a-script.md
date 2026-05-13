---
title: It's not a script
date: 2022-12-31 12:03:22
tags: daScript
---

Whats in a name? The sheer fact that I'm writing this post implies a problem.
"It's not a script" is something we had to say on numerous occasions.

- `Boris ELI5 script?`
- Script is a simple programming langue you write your game logic in. When it gets slow you rewrite it to C++.

daScript is none of the above.

<!-- more -->

Its not a simple programming language. Its statically typed multi-paradigm programming language with macros.

This looks like any other script:

    def fib(n)
        if n < 2
            return n
        return fib(n - 1) + fib(n - 2)

It's a lie. If you peek behind the smoke and mirrors via `options log` you'll see the next level of reality:

    [fastcall]
    def private `fib ( n:int const explicit ) : int const
        return (n < 2) ? n : (__::`fib(n - 1) + __::`fib(n - 2))

Which comes as a result of instancing of a generic function in the current module.
Types are inferred automatically, upon instancing. `fastcall` annotation is assigned by an optimizer.

At its lowest level daScript is C. It has pointers and pointer arithmetics. Static arrays. Structures.
daScript strings are very much C strings. MemCopy. In fact `=` operator is a memcpy.
There is no hidden overhead, or abstraction penalties.

Sure its missing some very specific syntax sugar, like `union` and `bitfields`, but it nothing you can't rectify with macros.
There is some light ABI overhead when interacting with daScript Context, but none inside of the Context.
At the end of the day low level daScript code can be converted to C, and vice versa.

The next level of abstraction comes with blocks and lambdas, dynamic arrays and tables, classes, generators and iterators.
It still looks like any other script (python anyone? ruby?):

    def each_even_in_range(start, stop)
        return <- generator<int>() <|
            for t in start..stop
                if t % 2 == 0
                    yield t
            return false

    [export]
    def main
        for t in each_even_in_range(0, 10)
            print("t = {t}\n")

The reality, however, gets [more and more peculiar](https://tinyurl.com/mpjh4fuz).

After that come macros. Think LISP, or HAXE. daScript is the ultimate EDSL, expanding it is so easy.
There are even reader macros and comment parsing macros.
[Something like adding interfaces to an OOP model](https://tinyurl.com/27c93mkf) takes only [couple pages of code to implement](https://tinyurl.com/49ym77ey).

A friend recently compared daScript to an iceberg, with the `script like` portion sticking on top.
There is of cause a question of `how do you learn all this machinery?`. But the alternative questions are a lot less appealing.

In daScript conception the idea was 'never have to rewrite to C or C++`. So far never had to.
In fact naively written daScript typically outperforms naively written C++, and similar optimizations are available in both.

The upcoming JIT often outperforms C++ on similar tests. On the 3D math heavy code the results can be dramatic.
`pathtracer` computes 16.3 mil rays a second with JIT, and 5.1 mil rays with AOT (and one day i'll write a detail post as of why).

And thats exactly why daScript is not a script.

* its not simple
* you don't write your game logic in it, you write your everything in it. yes your physics, your renderer, your shaders, and, one day, perhaps your kernel drivers.
* no, it does not get slow, and you never have to rewrite it to C++. or C. ever

Its obscure enough to be renamed. But those *.das files aren't. daSlang? dasLang?
