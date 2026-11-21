# In-place builds and aliasing

This document follows `ARCHITECTURE_COMMON.md`.

## What the alias pass counts as a read of the target (`ast_derive_alias.cpp`) {#in-place-build-aliases}

A struct, tuple, variant or array literal assigned to an existing target is built in the
target's own storage, one field after another. A call or an invoke that returns by value writes
its result straight into the target. Each saves one copy, and each is wrong when the right side
reads the target after the first write. So `AliasMarker` compares what the two sides read. When
they share a variable, or when either side dereferences a pointer with `*`, the value is built
in a temporary and copied: `alwaysAlias` on the literal, `cmresAlias` on the call or invoke.
Under `options no_aliasing` the same assignment is a compile error.

A call is compared only when the called function carries neither annotation: `[alias_cmres]`
always builds in a temporary, `[never_alias_cmres]` always builds in place. Of its arguments,
only a reference or a pointer that shares a component type with the result is compared - the
type itself, an array or table element, a struct field, a tuple or variant member, or a
pointee, at any depth, scalars included. A reference or pointer global that shares a component
type with the result is compared too, when the called function, or any function it calls, uses
it. When the shared type is reached through a pointer on either side, the value is always built
in a temporary.

An invoke of a function or a lambda compares every reference or pointer argument, and the
globals used by every function or lambda whose signature matches. In a module, such an invoke
assigned to a global always builds in a temporary. An invoke of a block is never compared: it
builds in place.

`SourceCollector` lists the variables an expression reads. It answers two questions:

- **Which storage an expression names** - asked of the target, of a call's argument, and of
  what a second name resolves to. An index, the condition of `?:` and the arguments of a call
  that returns a value are evaluated before the write, so they are left out; so are an invoke
  of a block and the body of a block. A call that returns an iterator passes its argument on,
  because the iterator points into it.
- **Every read** - asked of the right side of a literal, where each field is evaluated after
  the fields before it are written. An index, a condition, the arguments of a value call and
  the body of a block all count. When the target is a global, the globals a called function
  uses, directly or through the functions it calls, count too.

Neither question walks a `typeinfo` operand, a quote, or the initializer block of a struct
literal.

A second name is another variable that reaches the target's storage, and it is resolved to
what it names. A loop variable that is a reference resolves to its loop source. A reference
local resolves to its initializer. The loop source comes from the `for` the pass is walking,
not from `Variable::loop_source`: an inline splice replaces the source's root node and leaves
that weak pointer on the detached one. A second name is resolved once in an expression, so a
chain of reference locals costs one walk a link.

The pass traces the aliases safe code can make. A route that needs `unsafe` is outside it. A
field or an index of a pointer names only the pointer variable, so a read through it, or a
pointer handed to a call, is not followed to a target whose address was taken. A reference or
an iterator that a function hands out into a global is not traced back to that global. Once
`unsafe` is written, what a pointer or a handed-out reference reaches is the author's to know,
and following it flags every pointer read in code that cannot alias.

Inside that scope two things stay unseen. A reference argument of the enclosing function
resolves to nothing: the caller can bind it to storage the right side reaches by another
route - another reference argument, a global, or a block argument. A block that is invoked
reads what it likes: its result is built in place even when the block reads the target.
