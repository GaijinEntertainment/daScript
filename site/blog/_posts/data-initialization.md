---
title: Data initialization in DAS
date: 2024-07-23 21:35:17
tags:
    - daScript
    - C++
---

Updated on 2024-07-29 16:31:00

Sometimes shorthand notation arrives first.

<!-- more -->

Some of the DAS syntax has been AI-inspired. Tools like github Copilot like to write DAS code in a certain manner.
Who am I to disagree? After all they represent collective wisdom of the internet. What could possibly go wrong?

So, first things first, here are some new DAS syntax features (no 'Expressions' has been harmed in the process)::

This is structure, variant, or tuple initializer::

    var val = Foo(a=1,b=2.0) // same as [[Foo() a=1,b=2.0]]

This is array initializer::

    var arr <- [1, 2, 3, 4] // same as [{auto 1;2;3;4}]

This is table (set) initializer::

    var set <- {1, 2, 3, 4} // same as {{1; 2; 3; 4}}

This is table initializer::

    var tab <- {1=>"1", 2=>"2", 3=>"3"} // same as {{1=>"1", 2=>"2", 3=>"3"}}

This is tuple initializer::

    var tup <- (1,2.,"3") // same as [[auto 1,2.,"3"]]

This is array comprehension::

    var acomp <- [for x in 0..10; x*x; where x%2==0] // same as [{for x in 0..10; x*x; where x%2==0}]

This is table (set) comprehension::

    var scomp <- {for x in 0..10; x*x; where x%2==0} // this is new

This is table comprehension::

    var scomp <- {for x in 0..10; x*x => "{x*x}"; where x%2==0} // this is also new

Now, with that out of the way.

I'm not opposed to symbolic syntax also known as concise syntax. Smaller programs are typically easier to read.
The downside is you actually have to learn what unfamiliar symbols mean.
Languages like C++ gave us decent amount of familiar symbols. Imagine::

    a ? b->c() : *d.e()

C# adds more symbols::

    a?.b?[d] ?? e

DAS adds even more::

    a <- [[b() c]] <| $ ( var e )
        e.f = @@g

Some of it is a shorthand notation for data initialization.

DAS parser is fast. Part of the reason is LR(1) (or LALR(1)). That limits syntax somewhat and also limits some of the symbol reuse.
At this point there is whole bunch of code which limits massive changes to certain extent as well.

There is certain inherit logic in how data initialization shorthand is organized. Time to voice it explicitly.
Also for there is a shorthand, there is a full notation. Time to introduce this one as well.

Everything in [[ here ]] is local data initialization. It could be array, structure, class, tuple, or variant.

Lets start with fixed arrays::

    var a1 = [[int 1;2;3;4]]
    var a2 = [[auto 1;2;3;4]]   // here it will infer type, so a2 is exactly the same

The full notation is::

    var a1 = fixed_array<int>(1,2,3,4)
    var a2 = fixed_array(1,2,3,4)           // when type is not specified, auto is assumed

Dynamic arrays are similar, only [{ }] are used, and not [[ ]]::

    var a1 <- [{int 1;2;3;4}]
    var a2 <- [{auto 1;2;3;4}]
    var a2 <- [1,2,3,4]          // same thing, shorter syntax

The full notation is::

    var a1 <- array<int>(1,2,3,4)
    var a2 <- array(1,2,3,4)

Now, lets look at the structures::

    struct Foo
        a : int = 5
        b : float = 3.0

    var a1 = [[Foo() b=2.0]]
    var a1 = Foo(b=2.0)                 // same thing, shorter syntax
    var a2 = unsafe([[Foo a=1, b=2.0]])
    var a3 = [[Foo() a=1,b=2.0; a=2,b=3.0]]
    var a3 = Foo(a=1,b=2.0; a=2,b=3.0)  // same thing, shorter syntax
    var a4 <- [{Foo() a=1, b=2.0}]

This brings a few questions.
* Foo() means Foo will be initialized. Imagine calling Foo(), and then applying fields.
* Foo in a2 is unsafe, because by default it's unsafe to have structures uninitialized. (there is [safe_when_uninitialized] annotation to mitigate that)
* a3 is a fixed array of 2 different Foo
* a4 is an array with 1 element Foo ( note [{ }] syntax for dynamic arrays )

Full notation is::

    var a1 = struct<Foo>(b=2.0)
    var a3 = struct<Foo>(a=1,b=2.0; a=2,b=3.0)
    var a4 <- to_array_move(struct<Foo>(a=1,b=2.0))

There is no version of uninitialized structure full notation (a2).

Classes are very similar to structures::

    class Foo
        a : int
        b : float
        def Foo
            a = 5
            b = 3.0

    var a = new [[Foo() b=2.0]]
    var a = new Foo(b=2.0)  // same thing, shorter syntax

The full notation is::

    var a = new class<Foo>(b=2.0)

Note, that Foo() initializer will be called (as oppose to structures, where default field values are used for initialization of unspecified values).

Class can technically be constructed on the stack, but its unsafe to do so. In that case its exactly the same shorthand notation as structures (and with keyword class for the full notation).

Variants follow the same pattern::

    variant Foo
        a : int
        b : float

    var a1 = [[Foo a=1]]
    var a1 = Foo(a=1)               // same thing, shorter syntax
    var a2 = [[Foo a=1; b=2.1]]     // fixed array of 2 different variants
    var a2 = Foo(a=1;b=2.1)         // same thing, shorter syntax
    var a3 <- [{Foo a=1; b=2.2}]    // array of 2 different variants

The full notation is::

    var a1 = variant<Foo>(a=1)
    var a2 = variant<Foo>(a=1; b=2.1)
    var a3 <- to_array_move(variant<Foo>(a=1; b=2.2))

Tuples follow suit::

    tuple Foo
        a : int
        b : float

    var a1 = [[Foo a=1,b=2.0]]
    var a1 = Foo(a=1,b=2.0)         // same thing, shorter syntax
    var a2 = [[Foo a=1; b=2.1]]
    var a2 = Foo(a=1; b=2.1)        // same thing, shorter syntax
    var a3 <- [{Foo a=1; b=2.2}]

    var b1 = [[auto 1,2.0]]
    var b1 = (1,2.0)        // same thing, shorter syntax
    var b2 = [[auto 1,1.0; 2,2.1]]
    var b3 <- [{auto 1,2.2}]

    return 1,2.2    // same as return [[auto 1,2.2]]

Note, that tuple type declaration is a weak type, i.e. type alias (typedef). a1 is the same type as b1, and so on.
The only difference is tuple fields can be accessed via field name. I.e. a1.a or b1._0, though a1._0 is also available.

The full notation is::

    var a1 = tuple<Foo>(a=1,b=2.0)
    var a2 = tuple<Foo>(a=1;b=2.0)
    var a3 <-> to_array_move(tuple<Foo>(a=1,b=2.2))

    var b1 = tuple(1,2.0)
    var b2 = fixed_array(tuple(1,1.0),tuple(2,2.1))
    var b3 <- array(tuple(1,2.2))

    return tuple(1,2.2)

There are two types of tables. A key-value pair and a table which only has keys. Everything between double curly bracers is a table::

    var t1 <- {{ 1; 2; 3; 4 }}
    var t1 <- { 1, 2, 3, 4 }                        // same thing, shorter syntax
    var t2 <- {{ 1=>"a"; 2=>"b"; 3=>"c"; 4=>"d" }}
    var t2 <- { 1=>"a", 2=>"b", 3=>"c", 4=>"d" }    // same thing - shorter syntax

In reality 1=>"a" is yet another shorthand for [[auto 1,"a"]] which is in turn a tuple initialization.

The full notation is::

    var t1 <- table(1,2,3,4)
    var t2 <- table(1=>"a", 2=>"b", 3=>"c", 4=>"d")

Types can be specified (and verified) explicitly::

    var t1 <- table<int>(1,2,3,4)
    var t2 <- table<int;string>(1=>"a", 2=>"b", 3=>"c", 4=>"d")

If we go deeper under the hood, we will find::

    var t2 : table<int;string> <- to_table_move ( fixed_array<tuple<int;string>> ( tuple<int;string>(1,"a"), tuple<int;string>(2,"b"), ... ) )

But thats a topic for another blog post.



