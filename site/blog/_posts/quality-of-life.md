---
title: quality-of-life
date: 2026-01-22 20:21:45
tags:
    - daScript
    - daslang
---

Sometimes little things make big difference.

<!-- more -->

Type less. Like somewhat less. When its obvious.

In no particular order. It's been a minute - so I probably forgot something good from 2025.

-------------
String format
-------------

It's just like fmt. Because it is fmt.

    [export]
    def main {
        var a = 123.456789
        print("{a:.8g}\n")  // that syntax. its basically print("{fmt(":.8g",a)}\n")
                            // and it does not need 'require strings'
    }

-------------------
We can expand tuple
-------------------

And not just in the let.

    [export]
    def main {
        var a = [(foo=1,bar=2.), (foo=3,bar=4.)]
        var (F,B) = a[0]                    // yes, in the let
        print("F={F}, B={B}\n")
        for ( (f,b) in a ) {                // but also in the loop iterators
            print("f={f}, b={b}\n")
        }
    }


----------
It's a for
----------

'for' is almost 'zip'. Almost. := into clone makes a lot of sense. Like define clone, and then := it.
'each' is the other white meat. Like define 'each' and then iterate like its nothing.

    [expect_any_bitfield(b)]
    def each(b) : iterator<bool> {
        let bits = uint64(b)
        return <- generator<bool>() <| $() {
            var mask = 1ul
            for (i in range(typeinfo sizeof(b) << 3)) {
                yield ((bits & mask) != 0ul)
                mask <<= 1ul
            }
            return false
        }
    }

    ...
    for ( x in bitfield(0x1317) ) // no need to type each(bitfield(0x1317))

----------------
It's an operator
----------------

It's fun with properties. Because EDSL matters. Sometimes fancy assignments carry meaning.
Sometimes it's performance based, but with properties it's one off - it sort of matters. Kinda.

    struct Foo {
        a : int
        def operator . A { return "{a}"; }
        def operator . A := ( v : string ) { a = to_int(v); }
        def operator . A &= ( v : string ) { a &= to_int(v); }
    }

    var f : Foo
    f.A = "123"
    f.A &= "1"

It's even more fun, when it's indices. Because performance matters. Sometimes its EDSL.
But when its a bit array - overwriting [] &= speeds this up 3x.
Trust me. Or better yet, don't. Tests are in the code-base, under examples/profile/bool_array_tests/test04.das
Yes, all 3, interpreter, AOT, JIT. No, LLVM does not fold it.

    struct Foo {
        a : uint
        def operator [] ( index : int ) : bool {
            return (a & (1u << uint(index))) != 0u
        }
        def operator [] =( index : int, v : bool) {
            let idx = uint(index)
            if (v) {
                a = a | (1u << idx)
            } else {
                a = a & ~(1u << idx)
            }
        }
        def operator [] &=( index : int, v : bool) {
            let idx = uint(index)
            if (!v) {
                a = a & ~(1u << idx)
            }
        }
    }

----------------
Consume argument
----------------

Sometimes it eats inputs. But noone reads documentation.

    [consume(a)]
    def foo ( var a ) {
        do(a)
        delete a
    }

    foo(<-b)                    // just like that, or else its an error
    for(consume_argument(b))    // or like that

And what if it is not?

        foo(a)
        ^^^
    argument a is not passed as moved value, a
    use '<-' operator to move the argument, i.e. <-a

------------------------
Boris would eat anything
------------------------

Maybe not. But parser will.

    bitfield<one,two;three> // sure, who cares if its , or ;

Half the time I can't remember myself

    def foo ( a,b : int, c : float; d ) // it's all the same

Sometimes it can't figure it out. Most times it can. Type something. Like it matters.... not.

I can trail commas.

    [1,2,3,4,] or like Foo(a=1,b=2,) // because why not

Some things are like somewhat related, right. Just type something.

    t.foo(a,b,c) // what could this possibly mean?

Thats a hard one. We try class method (or structure method). We try static class method.
We also try foo(t,a,b,c). Something would compile. Probably.

    a.b.foo(c) // that one even harder

It's a method of a.b. Or a property. It's all good. Few years ago you could tell one from the other.
Now there is 'options log', vscode plugin, or just go with the flow. Feel the vibe.
Copilot likes it. It does not have to thing. You might like it.

Or don't. Good old a->foo(b) still works. CodeOfPolicies is still there - turn it off if you will.


-----------------------
Last block pipes itself
-----------------------

And if it does not have argument, it does not need block sign

    def repeat(a : int; b : block) {
        for (i in 0 .. a) {
            b()
        }
    }

    def repeati(a : int; b : block<(v : int)>) {
        for (i in 0 .. a) {
            b(i)
        }
    }

    [export]
    def main {
        repeat(5) {                         // note, no $, no $(), and no <| $()
            print("Hello, World!\n")
        }
        repeati(5) $(i) {                   // note, no <|
            print("Hello, World! {i}\n")
        }
    }

-------------
Typedef what?
-------------

Local typedef is very local

    def foo ( var a ) {
        typedef TT = typedecl(a)    // like why not. it goes out of scope - its gone
        print("a is {typeinfo typename(type<TT>)}")
    }

Class typedefs are very classy

    struct Foo {
        typedef TT = ...

Same thing. Intuitive visibility applies. Good for templates. I have entire blog post about templates.
It's also good for less typing.

--------------------------
Bitfield magic is so magic
--------------------------

Sure

    bitfield : uint8 < one, two >   // cause why not. uint16, uint64 works as well

    bitfield Foo {
        a
        b
        ab = a | b  // oh, wow, bitfields can now have constants too
    }

Also

    daslib/bitfield_boost   // its an array. behold 'each', '[]' and all that jazz


------------
Annotations?
------------

Sure. Write one for enum via 'enumeration_macro'. I'm sure someone will ask for bitfield annotation one day.

----------
Typemacro?
----------

    typedef Counter = $counter(type<int>)   // it's obvious
    typedef Counter = counter<int>          // it's just as obvious. sort of

--------
Modules?
--------

    module Foo public !inscope  // its awesome.

It's so awesome, its visible everywhere, even if you don't require it implicitly or explicitly.
EDLS? Perhaps daslib/match should be that.

---------------------------
It doubles as documentation
---------------------------

Like who would want a complex class as a hash key (hi JavaScript)? As if cuckoo hash is ever good.

    daslib/flat_hash_table
    daslib/cuckoo_hash_table

Bit arrays? Really? But then again, memory is getting more expensive.

    daslib/bool_array

---------
Releases?
---------

Sort of. Pre-releases. They are on github alright.

JIT works well. It may just replace AOT, unless you are on a platform where it can't. Like 'cross-compile' can't.

Next big thing - compile your module as 'external' module, without making it a part of the submodules.
Next big thing - integration examples.
Next big thing - headers and libraries as part of the release. (Libraries sort of are already. Well, almost.)

Once there, we call it a release. Until then - download and play with big one. Or small one. Or wait a tad more.
Or sign up for edenspark.io

-------------------
The need for speed?
-------------------

Still there. Compilation time went down 2x to 3x last year. Seriously. Build times went down to.
We are getting more robust. It will get faster - there are more tricks up that sleeve.



