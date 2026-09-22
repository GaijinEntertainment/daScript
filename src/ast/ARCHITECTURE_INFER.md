# src/ast architecture notes - inference

Companion to `ARCHITECTURE.md` in this folder; section numbers are unique across the family.

## 5. Overloads of `=`, `<-` and `:=` (`ast_infer_type_op.cpp`, `ast_infer_type_function.cpp`) {#assign-operator-lookup}

An `ExprCopy` or `ExprMove` whose `no_promotion` flag is clear first looks for a user function
named `=` or `<-` whose two parameters take the left type as a mutable reference and the right
type, by the ordinary overload rules (`inferAssignOperator`); a match replaces the node with a
call to it, and the built-in operation runs only when there is none. An `ExprClone` does the
same through its own lookup of `clone`, the name `operator :=` declares. The raw forms `!==`,
`!<-` and `!:=` set the flag. A lookup starts with `hasFunctionNamed`, a per-module name-table
probe, so a program that declares no such operator pays no temporary call per copy or move.
A copy or move the compiler manufactures - the inliner's return store, a `let` the stack
allocator relocates into an assignment, a generator's yield store - carries the flag, and a
generated variable's initializer is never promoted: the user's operator runs once, at the site
the user wrote, in every build configuration. The built-in clone of a copyable type lowers to an `ExprCopy`, and that node
inherits the flag, so a raw clone never re-enters `operator =`. The built-in clone of a
non-copyable type is a call to a generated `clone` function, and a user `operator :=` on the
exact pair is a `clone` function with the same signature that takes its place: a raw `!:=` on
such a type has nothing to reach, so it is reported as an error instead of re-entering the
overload (the `generated` flag on the matched function tells the two apart).

The call an overload becomes keeps the `_::` spelling in its name (`_::=`), the way the clone
lookup does, never a plain `=`. Initialization lowers to the builtin generics `copy_to_move`
and `move_to_move` (`daslib/builtin.das`), whose instance body assigns `copy_dest = copy_src`
and is inferred from the builtin module: a plain name resolved there looks from `builtin` and
never sees the caller's overload, while `_::` names the module being compiled
(`getSearchModule("_")`).

Initialization - a local `let` or `var`, a global, a struct field default, a field in a
make-struct - runs `promoteInitToAssign`. When an overload exists for the declared type and
the initializer's type - a plain function or a generic, the same set `inferFunctionCall`
consults for an assignment; two plain candidates select none, and the built-in operation runs,
as an assignment's `tryOperator` lookup does - the initializer becomes `copy_to_move(init, type<T>)` (`move_to_move`
for `<-`): a fresh `T` assigned through the overload and moved into the variable. The lookup
drops the declared constness, because an initialization writes its destination whatever the
declaration says. The variable is then move-initialized only when `T` cannot be copied: the
call's result is a constant value, and a copyable type never move-initializes from a constant.
`isAssignInitCall` keeps a promoted initializer from being promoted again on the next
inference pass; it compares the generic's origin name, because a resolved instance carries a
mangled one.
