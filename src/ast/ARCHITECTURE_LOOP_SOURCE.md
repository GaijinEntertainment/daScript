# A value made in a loop header

This document follows `ARCHITECTURE_COMMON.md`.

## A temporary under a loop source (`ast_allocate_stack.cpp`) {#for-source-temporary}

`keys(tab)`, `values(tab)` and every other function marked `[unsafe_outside_of_for]` return an
iterator that points into their argument. When the argument is a value made in place - a
call that returns by value, a literal, an `invoke` - its stack slot belongs to the source
call's own scope, and a scope that ends gives its slots to whatever is allocated next: here the
loop body's locals, while the iterator still reads the slot. Only the interpreter's stack layout
does this: the AOT emitter declares such a value at function scope.

So the stack allocator releases no slot allocated under a loop-source call of such a function
(`holdsLoopSourceTemporaries`); the slots stay taken until the scope around the loop ends. The
source stays in the loop header as written, a const value and a value under a nested call
included.

A generator loop that yields is outside this: `replaceGeneratorFor` rewrites it into plain
statements, and a stack slot does not survive a `yield` in any tier. The next section covers it.

## A temporary under the source of a generator loop that yields (`ast_generate.cpp`) {#generator-for-source-temporary}

A generator's locals are fields of the generator, and its stack frame lasts only from one `yield`
to the next. `replaceGeneratorFor` rewrites a loop whose body yields into plain statements: the
source is evaluated once into a generator variable, and the iterator it holds is read again after
every `yield`. A value made in the loop header - a call that returns by value, a literal, a `?:`
or a `??` that gives a value - lives in the frame, so the iterator would read a frame that is gone.

`GeneratorSourceTemporaries` holds each such value in a generator variable of its own, by move, in
front of the source. It reaches the value through what can pass a pointer on: a field, a variant
`as`, an index, a dereference, the left side of a `??` that gives a reference, a cast, and a call
that returns a reference, a pointer or an iterator. It stops at the first value it holds, and it
enters neither an arm of a `?:`, nor the default of a `??`, nor the body of a block: those are not
evaluated every time, or not in the header's own frame. An iterator is never held itself: the walk
goes through the call that made it.

The held values are evaluated first, in their own order, ahead of whatever else the header
evaluates. So in a call with several arguments, a held argument runs before an earlier argument
that stays in place.

A call that returns a reference is not a value made in place, so it is never moved: the loop reads
the owner's storage, and the owner keeps its contents.

A const value cannot be moved. A loop over one is a compile error here (30941), where a loop that
does not yield accepts it.

The rewritten source is no longer a loop source, and the loop-source flag does not survive the
clones the generator lowering makes. So a source that calls an `[unsafe_outside_of_for]` function,
such as `keys(tab)`, is marked `alwaysSafe` (`isSafeOnlyAsLoopSource`), like the generated `each`
call beside it: it needs no `unsafe`, as in the loop header it came from. The mark covers that one
call and not its arguments.
