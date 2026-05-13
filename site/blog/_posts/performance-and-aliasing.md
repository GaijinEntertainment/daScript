---
title: Performance and CMRES aliasing
date: 2023-01-21 14:31:03
tags:
    - daScript
---

From dascript.org "... is high-level, statically strong typed scripting language, designed to be fast ..."

With an emphasis on fast. Performance first, last, and foremost.

<!-- more -->

There are many ways, how we make it fast. But today we are going to focus on CMRES (Copy-or-Move Result) aliasing.

Here is basic idea. Say we have a function 'set', which returns result of type `Foo` like this

    struct Foo
        a : int

    def set ( a : Foo )
        var b = [[Foo a = 13]]
        b.a += a.a
        return b

From the language perspective is semantic equivalent of

    def set ( a : Foo; var cmres : Foo ) : Foo&
        var b = [[Foo a = 13]]
        b.a += a.a
        cmres = b
        return cmres    // address only

After optimizations we get the following

    def set ( a : Foo; var cmres : Foo ) : Foo&
        cmres = [[Foo a = 13]]
        cmres.a += a.a
        return cmres    // address only

Now, consider this code

    var a = [[Foo a = 1]]
    var b = set(a)

Interpreter ABI requires caller to allocate function result. That means that function knows nothing about where CMRES came from.
More interestingly daScript compiler assumes that CMRES does not alias anything, including input parameters or global variables.

As the result the example above will be equivalent of

    var a = [[Foo a = 1]]
    var b : Foo
    set(a, b)       // b passes as CMRES directly

This is neat optimization which saves copying from result of `set` to the destination (which is b).
Now consider the following code

    var a = [[Foo a = 1]]
    a = set(a)

The same optimization can't be safely performed, because `cmres = [[Foo a=13]]` will clobber input argument a,
which will result in a.a = 26.

So, how do we make it fast? There is `no_aliasing` setting in the CodeOfPolicies, as well as `options no_aliasing`.
When enabled, it produces the following error message

    hello_world.das:14:8:
        a = set(a)
            ^^^
    14:8 - 14:10
    40211: function set result aliases argument a
    some form of ... a ... = set( ... a ... )

This gives programmer opportunity to find out, if otherwise common optimization has been disabled.

There is also an option of telling the compiler to ignore CMRES aliasing for the specific function altogether.

    [never_alias_cmres]
    def set ( a : Foo )
        var b = [[Foo a = 13]]
        b.a += a.a
        return b

this will force the code above to produce a.a = 26. Surprisingly there are good uses for it.
As well as for the `[alias_cmres]` annotation, which will always disable the optimization.

The more interesting aspect of it is how daScript programming can be a dialog with the compiler,
as oppose to more traditional monolog-style code writing. Don't just write it, and hope for the best.
Check out what it actually produces.

I often program with `options log` to see, how my code ends up working.
Some type related questions are best answered with `options log_infer_passes` - you can see your macros expanding pass-by-pass.
There are `options log_stack`, `options log_nodes`, and many more. One day I'll even write an entire post about them.

Now there is even `options log_aliasing`. For the example above it will tell you this

    ALIASING:
    function set CS<::Foo> returns by reference
        argument 0 aliasing result with type Foo const
    hello_world.das:12:8: set aliases with CMRES, stack optimization disabled

So don't just talk to the compiler. Listen to what it has to say and you'll never have to walk alone.

P.S. And now of something completely different. Did you know we have integrated (into vscode) debugger? Did you know it can now debug macros?
