---
title: Better safe
date: 2022-11-14 18:29:45
tags:
    - C++
    - daScript
---

[Herb Sutter is right](https://youtu.be/ELeZAKCN4tY?t=2391). Its the same thing year after year after year. `Lifetime`, `Initialization`, `Type safety`, and `Bounds safety` are the things which your language should do for you.

<!-- more -->

### LIFETIME

daScript does not have any safe way to access objects outside of their lifetime. Seriously, if you can find one - lets us know; we will fix it.

daScript goes out of its way, sometimes even at cost of performance, to make sure there are no lifetime related bugs. Array and table locking happens in runtime. The bulk of effort happens at the language design stage. Here are some of the ideas on how we are doing it:
* pointer delete is unsafe; `GC` or `GC0` are the safe way
* local references are unsafe; `assume` to the rescue
* lambda capture is explicit, lambda capture by reference is unsafe
* arrays and tables are locked when iterated, or explicitly
* data is not shared between contexts; data can be shared between jobs or threads via safe channels
* inter-context invocations are locking
* temporary types

#### `unsafe` keyword

There is an `unsafe` keyword; it helps with performance and practicality - it also helps to narrow down the bugs.
When facing a lifetime bug, there is a 99.9% chance it has occurred under the `unsafe` umbrella.

#### Temporary types

Concept of temporary types is unique to daScript. Temporary (or local) types insure that data can't be accessed outside of its scope.

Consider the following example:

```python
[export]
def main
    var tab <- {{ "1" => 1; "2" => 2 }}
    lock(tab) <| $ ( temp_tab )
        let one = temp_tab |> find("1")
        if one != null
            print("found {*one}\n")
```

Pointer `one` is temporary (as well as temp_tab). `lock` insures that table can't be modified. Temporary pointer can not exit the block - it can't be copied, or returned. As a result, having pointer to the table element is safe.

#### Pointer dereferencing and pointer arithmetic

daScript has both. Pointer dereference is safe, however null pointer dereference always causes panic. Pointer arithmetics are unsafe.
There are no safe ways to create dangling pointers. Taking the address of the value is unsafe.

It is possible to make temporary pointers to some values, via `safe_addr` - but those are covered under temporary type protection.

### INITIALIZATION

daScript does not have uninitialized data structures of any kind.

```python
[export]
def main
    let a : int
    print("{a}\n")
```

Always prints 0. If not initialized - everything is a semantic zero of some kind. Strings are empty.
Pointers are null. Arrays and tables are empty. Iterators are closed. There is no safe way to get to uninitialized memory.

Case closed? Well, almost. daScript is designed to talk to C++. It is always possible to bind the `safe` C++ function, which does unsafe things.

### TYPE SAFETY

daScript types are safe by default - there is no need for `type safety guidelines'.
* `reinterpret` casts are unsafe
* casting `const` away is unsafe
* LSP casts are safe, but can be disabled via `explicit` keyword
* there is no automatic type promotion
** with the exception of any pointer to void pointer (but not from void pointer)
* types can be inspected with a combination of `static_if` and `typeinfo` at compilation time
* everything is always initialized
* there are no unions, only variants - and those are safe; or they panic
* there are no va_args - macros are the future; and the future is now

All good? Well, almost. Macros can do just about anything, and 'alwaysSafe' flag on expression is there for a good reason.
But then you can always disable user code macros from the `CodeOfPolicies`, or `Lint`, or with another macro.

### BOUNDS SAFETY

In daScript every indexing operation causes bounds checks - on static or dynamic arrays, as well as tables. Accessing pointer by index is unsafe.

For practical purposes there is always `[unsafe_deref]`, however optimizer does decent job in JIT or AOT. At some point interpreter will follow, and its rarely an interpreter bottleneck anyway.

As always - you can always bind custom C++ type which throws it out of the window, but you don't have to - indexing is explicitly bound; its always good to check.

### Anything else?

`Lint`. daScript has lint macros and they are awesome. We've found that many things traditionally left to code reviews are easily automateable. Some of them are already in the `CodeOfPolicies`.

### Where do we go from here?

daScript is a fine ballance between safe and robust. Try to keep your code safe, but occasional isolated `unsafe` here and there for practical purposes is perfectly fine.

