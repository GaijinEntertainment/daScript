# A value made in a loop header or a call argument

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

## An array literal as a loop source (`ast_infer_type.cpp`) {#stack-loop-literal}

`for (x in [a, b, c])` iterates a fixed array on the stack. A gen2 array literal is
`to_array_move` over a make-array node built on the heap; where one is a loop source, inference
(`stackLoopLiteral`) drops the call and leaves the node, which then builds `T[N]` as
`fixed_array(a, b, c)` does. No policy asks for it: the literal is a temporary nothing else names,
so it cannot outlive the loop. It stays a heap array when its elements own heap, which a stack
array has no collect to free, and past 1024 bytes, which would otherwise come out of the
function's stack. A generator takes the same rewrite, and the lowering of a loop that yields
keeps the fixed array in the generator's frame.

## A stack array literal under a call argument (`ast_inscope_pod.cpp`, `ast_allocate_stack.cpp`) {#stack-array-argument}

Under `force_inscope_pod` an array literal that binds to a constant `array<T>` parameter is built
as a fixed array on the stack and not on the heap (`PodVisitor`). The parameter needs an
`array<T>`, so the literal goes through `to_array_temp` (`daslib/builtin.das`, repo root), which
returns an array header that points into the fixed array and allocates nothing.

`PodVisitor` takes a literal only when each of these holds:

- its elements own no heap - a collect would have freed them, and a stack array has no collect;
- the parameter is declared an `array<T>` and is constant, so the callee cannot resize or move
  the array;
- the callee's result cannot hold the array: it is no reference, pointer, iterator, lambda or
  block, and it holds none of them - a result that owns heap other than an array or a table of
  plain values is refused whole;
- the function is not a generator, whose frame does not last across a `yield`, and not a
  generated function other than a lambda or a local function;
- its module, and the module of the generic it was instanced from, allow in-scope POD.

The rewrite adds no collect, so a function with `unsafe` or `try`/`recover` takes it. The check
reads the callee's signature only: a callee that stores the array through `unsafe`, or a C++
builtin that keeps its argument, is outside what it sees.

The header points into the fixed array, which sits in an argument slot of the `to_array_temp`
call, and the outer call reads it after that call has returned. The stack allocator gives a call's
argument slots to whatever is allocated next once the call ends, and here the next allocation is a
sibling argument of the outer call. So the allocator releases no slot allocated under a
`to_array_temp` call (`holdsArgumentStorage`); the slots stay taken until the outer call ends.
