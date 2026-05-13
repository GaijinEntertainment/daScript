---
title: Garbage collection in the wild
date: 2024-04-16 21:07:26
tags:
    - daScript
    - GC
---


What do a mafia boss and a garbage-collector have in common?
Both make sure you'll never see unwanted things again!

<!-- more -->

Until you do that is.

------------------
First things first
------------------

How to turn it on::

    options persistent_heap

Or persistent_heap=true in the CodeOfPolicies.

What happens when its on?

* LinearHeapAllocator is replaced with PersistentHeapAllocator
* LinearStringAllocator is replaced with PersistentStringAllocator
* allocations become slightly slower
* `delete` now works well, as oppose to sometimes maybe
* `heap_collect` can be called from anywhere interpreted (yes, the whole stack)

What happens when `heap_collect` is called?

* mark phase is called on heaps
* data walker walks every global variable, and marks ranges of used memmory
* data walker walks stack and marks ranges of used memory on local variables, as well as arguments
* sweep phase is called on heaps, where unmarked areas are marked as free

`finalizer` machinery DOES NOT GET CALLED on any objects of any kind.

What happens when `delete` is called?

* `finalizer` machinery is called for the specified argument.
* Eventually context level delete is called, which in turn calls heap->free.

Do not forget to mark [[do_not_delete]] on the non-owning fields of the structure or class.

There is a way to call delete for the local variable automatically::

    let inscope foo = .... // will add delete foo in the finally section of the scope

What happens with strings?

* there is a way to delete them explicitly. I highly recommend to not do it.
* there is a way to turn on string interning via `options intern_strings` or through the CodeOfPolicies.
* functions which do not have `captureString` side effect delete string builder after their execution

--------------------
Second things second
--------------------

So, whats the problem?

1. Mark phase is 'slow' i.e. it grows somewhat linearly with the amount of data it needs to walk.
2. Mark phase is blocking, i.e. nothing can be happening with the context during the mark phase.

This is pretty fundamental. But fear not, we have plans, backup plans, and even more backup plans.

For the embedded scenario, when possible, do not use DAS side as storage, and reset context after every use or after every frame.
Do not use GC, use linear heap allocator. In fact, by default, daScript is using linear heap allocator. It hardly deletes.
Reset is very very fast. There is a locking mechanism which prevents reset for a few frames, if need be.
Basically if it's a game script on top of ECS - linear allocator, reset, store data in ECS.

When you have to use persistent_heap, the strategy is to call 'heap_collect' every so often, and see if it 'just does it', which is often the case.
There is always a way to call it less often - sprinkle some 'inscope' and 'delete' around the code with obvious lifetime. It might get you ways.
Keeping data in the ECS helps a lot as well, be that external solution or DECS.

The idea is to call `heap_collect`, but not often at all. Have some automatic thresholds (mainly on strings).
daScript plugin does that with a good success.

Move things to jobs or threads. `daslib/jobque_boost` is good for more things, than just parallelizing evaluation. Jobs and threads are separate contexts, they get collected separately and independently.

------------------------
Now for the nitty-gritty
------------------------

In no particular order.

DAS is more like GO. Most of the local variables are on the stack. So the whole 'generation 0 first' thing is not an optimization.
Also, just like GO, a lot of future work will be about compiler determining lifetime of the objects and automatically disposing of them.

let inscope on raw pod is pretty much always a go. In fact I'm planning for the compiler to add let 'secret_inscope' which clears the array data, without calling finalize. It`s safe like 99% of the time, and I can detect the rest.

'captureString' sideeffect is important. Compiler is half way there - I'm currently working on the ways to infer it for the DAS functions, but it's already correct for the builtin ones.
When not present, string builder result is destroyed after the call. ie print("foo={i}") which creates temporary string will delete it soon after the print is called.

-------------------
When all else fails
-------------------

I've yet to see this - but when it does - the plan is to add custom project-specific macros to address project-specific lifetime.
At the end of the day C++ programs run without GC, and DAS can behave like on - but with a lot less work, and the benefit of calling GC when other methods fail.
Basically at this point You'll have to flip the script - it becomes a manual memory management with GC backup, not an automatic one with some manual hints.

