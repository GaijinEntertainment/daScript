# src/ast architecture notes - inference

Companion to `ARCHITECTURE.md` in this folder; section numbers are unique across the family.

## 5. Overloads of `=`, `<-` and `:=` (`ast_infer_type_op.cpp`, `ast_infer_type_function.cpp`) {#assign-operator-lookup}

An `ExprCopy`, `ExprMove` or `ExprClone` whose `no_promotion` flag is clear first looks for a
user function named `=`, `<-` or `:=` whose two parameters take the left type as a mutable
reference and the right type (`inferAssignOperator`); a match replaces the node with a call to
it, and the built-in operation runs only when there is none. The raw forms `!==`, `!<-` and
`!:=` set the flag. The built-in clone of a copyable type lowers to an `ExprCopy`, and that node
inherits the flag, so a raw clone never re-enters `operator =`.

The call an overload becomes keeps the `_::` spelling in its name (`_::=`), the way the clone
lookup does, never a plain `=`. Initialization lowers to the builtin generics `copy_to_move`
and `move_to_move` (`daslib/builtin.das`), whose instance body assigns `copy_dest = copy_src`
and is inferred from the builtin module: a plain name resolved there looks from `builtin` and
never sees the caller's overload, while `_::` names the module being compiled
(`getSearchModule("_")`).

Initialization - a local `let` or `var`, a global, a struct field default, a field in a
make-struct - runs `promoteInitToAssign`. When an overload exists for the declared type and
the initializer's type, the initializer becomes `copy_to_move(init, type<T>)` (`move_to_move`
for `<-`): a fresh `T` assigned through the overload and moved into the variable. The lookup
drops the declared constness, because an initialization writes its destination whatever the
declaration says. The variable is then move-initialized only when `T` cannot be copied: the
call's result is a constant value, and a copyable type never move-initializes from a constant.
`isAssignInitCall` keeps a promoted initializer from being promoted again on the next
inference pass; it compares the generic's origin name, because a resolved instance carries a
mangled one.
