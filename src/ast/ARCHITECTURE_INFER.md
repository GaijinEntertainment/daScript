# src/ast architecture notes - inference

Companion to `ARCHITECTURE.md` in this folder; section numbers are unique across the family.

## A literal takes the type of its destination (`ast_infer_type_function.cpp`) {#literal-takes-target-type}

An empty array or table literal parses to an `ExprMakeStruct` whose `makeType` is
`array<auto>` or `table<auto;auto>`, and a block, lambda or function literal whose arguments
carry no types infers to a block type with `auto` in it. Nothing in the literal resolves those
`auto`s; the expression the literal is handed to does. `seedAutoLiteralFromTarget` is the one
place a container or block literal takes that type (a `cast<auto>` argument or initializer
takes its destination's type at its own sites): given the literal and the type of its
destination, it runs `inferGenericType` from the literal's type toward the destination,
applies the destination's auto contracts, and writes the result into the literal - `makeType`
for a container, the block's return and argument types for a block - then reports the AST
changed so the next pass infers the literal as a concrete type. A reference destination seeds
its value type. A tuple literal handed a tuple type seeds each element against the matching
element type, which is how `([], "ok")` reaches `tuple<array<int>; string>` at a return, an
assignment or a field; a call or invoke argument never gets that far, because a tuple literal
with an unresolved element has no type and the call waits for one. The helper answers seeded,
untouched or mismatch: a literal that is not auto, a destination still unresolved, an auto
array or table expression that is not an empty literal, and a tuple literal whose element
count differs from the destination's are untouched, and the ordinary type check that follows
reports any incompatibility; an auto literal the destination cannot take is a mismatch, which
the call site reports as a default argument that does not match the signature and every other
site leaves to the same ordinary check.

The destinations that seed are: a call's argument, once a single candidate function is chosen
(`inferFunctionCall`); an `invoke` argument, against the block, lambda or function type's
parameter - class and struct method calls lower to `invoke`, so this is the method-call path;
a `return` value, against the function's or the enclosing block's declared result; the right
side of `=` and `<-`, against the left type; a field in a make-struct, against the field's
type; an element of a tuple literal whose record type is known. A `let` with an empty
container initializer takes the declared type through `isEmptyInit` instead. The matcher
accepts an auto-typed container or block pass type while it is still unseeded
(`isMatchingArgument` with `inferBlock`), so the call and invoke sites type-check in the same
pass the seed lands.

## Overloads of `=`, `<-` and `:=` (`ast_infer_type_op.cpp`, `ast_infer_type_function.cpp`) {#assign-operator-lookup}

An `ExprCopy` or `ExprMove` whose `no_promotion` flag is clear first looks for a user function
named `=` or `<-` whose two parameters take the left type as a mutable reference and the right
type, by the ordinary overload rules (`inferAssignOperator`); a match replaces the node with a
call to it, and the built-in operation runs only when there is none. An `ExprClone` runs the
same lookup for `clone`, the name `operator :=` declares. Each lookup starts with the module
library's cached name probe (`hasFunctionOrGenericNamed`), so a program that declares no such
operator pays no temporary call per copy or move, and it does not run while either operand's
type is still unresolved, void, or an expression type.

The raw forms `!==`, `!<-` and `!:=` set the flag, and a rewrite of a flagged node - the
relaxed-assign move, the in-scope POD move, the `ExprCopy` the built-in clone of a copyable
type lowers to - carries it over. A raw `!:=` still honors a clone the language itself
provides: a C++-bound `clone`, a generic of the builtin module, a generated field-wise clone.
It skips only a user-written `clone` on the pair; when such a function exists for a
non-copyable type it has replaced the generated clone, nothing else can clone the type, and
the raw form is reported as an error instead of re-entering the overload
(`userCloneReplacesBuiltin`).

A copy or move the compiler manufactures carries the flag: the inliner's return store and its
split-ternary arm stores, a `let` the stack allocator relocates into an assignment, a
generator's yield and loop-finally return stores, loop counters and control flags, a
generator-local store whose initializer the compiler already promoted, and the copy into a
lambda or generator capture (`visitMakeStructureField` skips a generated value). A generated
variable's initializer is never promoted, and neither is an initializer that reads a generated
variable - the inliner's result temp, which stands where a promoted call already ran the
operator; the helpers are `[inline]`, so in a function body the init reads that temp after
inlining, while a global or field-default initializer, which no inliner visits, keeps the call:
the lint that refuses an `[inline]` call there exempts a call marked `generated`. The
generated field-wise clone of a struct, tuple or
variant clones each field with `:=`, which reaches the field type's own operators. Code a macro
emits is user code: nothing marks a `qmacro`-built store as compiler-made.

Initialization - a local `let` or `var`, typed or `auto`, a global, a struct or class field
default, a field in a make-struct or `new` - runs `promoteInitToAssign`; an argument default, a
`return` value, and an element of an array, tuple or variant literal are not init sites and copy
with the built-in operation. When an overload exists for the declared type and the
initializer's type, a plain function or a generic, the initializer becomes a call to a builtin
generic (`daslib/builtin.das`): `copy_to_move(init, type<T>)` for `=` and `move_to_move` for
`<-`, the `_ref` variants when the initializer is a variable so a scalar, pointer or string
source reaches the operator by reference, and the const or `var` overload of each by the
source's constness. Each helper is a fresh `T` assigned through the overload and moved out.
Two plain candidates select none, and the built-in operation runs, as an assignment's
`tryOperator` lookup does; among generics the most specialized one wins by the same
`copmareFunctionSpecialization` ordering an assignment uses, and a tie selects none. An
operator marked `[unsafe_operation]` is reported at the initialization site outside an
`unsafe` block, as the call would be at an assignment, because the helper's own `unsafe`
block would otherwise hide it. The lookup drops the declared constness, because an
initialization writes its destination whatever the declaration says. The variable is then
move-initialized only when `T` cannot be copied: a workhorse call result is a constant value,
and a copyable type never move-initializes from a constant. `isPromotedInitCall`
(`ast_generate.cpp`) names the helpers - `clone_to_move` and `clone_string` included - so an
initializer the compiler already promoted is not promoted again on the next pass, and a
generator-local store of such an initializer is raw; it matches names, not the `generated`
mark, because a table read with field defaults is a generated call too and its initializer
still reaches the operator.

## The size of a generic instance's argument types (`ast_infer_type_function.cpp`) {#instance-type-size}

A generic that calls itself with a type built from its own argument type makes a new instance on
every call, and the pass limit stops that chain only while each step adds a constant amount: the
fiftieth instance of `foo([a])` holds an array nested fifty deep. A step that uses the argument
type twice - `foo((a, a))`, a block `$(x : typedecl(a)) { return x }`, two arguments feeding
each other - doubles the type instead, so the thirtieth instance holds, without a limit, a billion type
nodes, and every whole-type walk inference runs (`isAuto`, `canCopy`, the copy itself) is that
large long before the pass limit counts fifty. `inferFunctionCall` therefore counts the type
nodes of the argument types where it is about to create an instance
(`argumentTypesFitInstance`) and reports `exceeds_instance_type` past 4096, which is one walk
per new instance and none per expression. The count is of nodes, never of depth: these types
are a few dozen levels deep when they stop fitting in memory. A structure counts as one node
whatever its fields, so the limit bounds only what type constructors nest.

That error is the only one of its compile. A call that gets no instance leaves its caller
without a type, so the same pass reports errors that only restate it, and `inferTypes` drops
every other error once one `exceeds_instance_type` is present.

An inference error names the function it was compiling and every function that instance came
from, each with its argument types. `Function::getLocationExtra` prints a type of more than 64
nodes as its constructor and `<...>`, so an error on an oversized instance stays short.

A type as written has its own limit, in the parser (`parser_impl.cpp`): both grammars stop at
64 levels of `<...>` nesting (`das_type_arrow_enter`), and a pointer wraps a type only while
the nesting under it fits the same limit (`das_check_pointer_nesting`), so no path through a
parsed type is longer than twice that. A postfix `?` is left-recursive and costs no parser
stack, so without the limit a pointer chain of any length parses and the first recursive walk
over it overflows the C++ stack. The limit is sized for a host that compiles on a 1 MB thread
stack in a build that inlines little: a type the parser accepts has to survive every recursive
walk inference runs over it. Past the limit the parse ends with one `exceeds_type_nesting`
error.

The stack one nesting level costs is the frame of the walk that recurses per level, and in a
build that inlines little every string temporary of every error message a function can raise
has its own slot in that frame. `InferTypes::verifyType` recurses once per level and can raise
about thirty errors, so it builds none of their text itself: `typeError`,
`malformedTypeError` and `templateStructureError` do, in frames that exist only while an
error is reported. With the text inline, one level costs 4176 bytes in a Debug build and a type
at the limit needs more than 512 KB; with it in the helpers the same type compiles in 256 KB.

## An `unsafe` block in a generator (`ast_infer_type.cpp`) {#generator-unsafe-block}

A generator body is lowered in place during inference: a local moves into the generator's
struct only once it is a statement of the body's own block, and a `yield` leaves a resume
label that the entry `goto` reaches only at that level. Nested blocks get there because
inference collapses them into their parent, pass by pass.

`unsafe { }` is a wrapper around a block, not a block, and collapse does not see through it.
Left in place, a local declared inside stays a stack local across a `yield`, and a `yield`
inside leaves its label where the entry `goto` cannot reach. So inference replaces a
statement-position `unsafe { }` in a generator with its block and sets
`Function::unsafeGenerator`; every later pass visits that function with unsafe allowed. The
price is the scope: once a generator has one `unsafe` block, its whole body is unsafe-permitted.
An expression-position `unsafe(...)` stays as written.
