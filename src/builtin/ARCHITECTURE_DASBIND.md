# src/builtin - the interpreter's `[extern]` call

Companion to `ARCHITECTURE.md`; each section is cited by its anchor.

### The interpreter's `[extern]` call {#interpreter-extern-call}

An `[extern]` function (`module_builtin_dasbind.cpp`) is called from the interpreter through
a wrapper `vec4f (*)(void * fn, vec4f * args)`: the node evaluates every argument into a
`vec4f` lane and the wrapper calls `fn` through a C prototype. The wrappers are generated -
`generate_x86_64_calls.das` writes `win_x86_64_wrapper.inc`, `systemV_64_wrapper.inc` and
`systemV_64_extra_wrapper.inc` - one per argument count, result class and float mask of the
first register-class arguments, indexed from 0: four positions on Windows, six on SystemV,
with every integer-class argument spelled `int64_t`, every float-class one `double`, and the
result `int64_t` or `vec4f`. Float class is a `float` or `double` by value; a ref, whatever it
points to, is a pointer and so integer class, on the table and the layout alike. A SystemV call with more than six arguments and a float at index
6 or later needs a prototype from the extra table, keyed by argument count, result class (1 for
a float or double result, as the main table indexes it) and the full mask; its entries are the
hand-listed `systemV_extra` rows in the generator - a new shape is added there and the
generator re-run - and a shape the list lacks is a compile error naming the mask.

The C prototype passes a stack argument as an 8-byte slot. That fits Windows x64, where the
mask covers four positions and everything past the fourth argument is an 8-byte stack slot,
float included. It fits SystemV x86-64 too: its float registers past index 5 are exactly the
extra list's cases. It does not fit arm64: eight float registers, so a float at index 6 or 7
belongs in d6 or d7, and on Apple the stack is packed at natural size and alignment - a 32-bit
`int` in 4 bytes, a `bool` in 1 - where Linux and Android keep 8-byte slots. So on arm64
(Apple, Linux, Android; a Windows arm64 build keeps the table, since `DAS_BIND_ARM64_LAYOUT`
turns on for `__APPLE__` and `__linux__` only while `DAS_BIND_EXTERNAL` is on for every 64-bit
Windows) a call with more than six arguments leaves the table: at bind time the
function's das types produce an `Arm64Layout` - which argument rides in x0-x7, which in d0-d7,
and at which byte offset of a stack image the rest sit, packed on Apple and slotted elsewhere -
and the node calls `das_arm64_call`, an assembly trampoline that copies the image below its
own frame, loads the sixteen argument registers, calls, and stores x0 and d0 for the node to
return. A call of six or fewer arguments has no stack argument and no float past the mask
under any of these ABIs, so it keeps the table there too. A packed width comes from the das
type: a ref, pointer or string is 8 bytes, a scalar its `getBaseSizeOf`, and anything else (an
aggregate by value, which the binder does not support) rides as 8 bytes, as the table would
pass it. A layout lives for the process - `computeArm64Layout` keeps every layout in a static
vector it never frees - because a re-applied `[extern]` replaces the module's function object
while an earlier context's call node, which carries the layout pointer, may still run.

The JIT uses neither: `modules/dasLLVM/daslib/llvm_jit.das` (repo root) emits an extern call
with the function's own LLVM types, so a compiled call is right on every platform. The wrapper
table and the layout are the interpreter's alone, which is why an ABI defect there shows only
in interpreted code - a macro context, a `[no_jit]` function, a plain run. `tests/dasbind`
probes both paths against `tests/dasbind/probe`, a library whose functions sum their arguments
under distinct multipliers, so one argument in the wrong register or slot changes the total.
`tests/dasbind/CMakeLists.txt` builds the probe on a 64-bit host that builds and dlopens a
shared library at test time - not wasm, Android or iOS, none of which walks `tests/` - and
`tests/.das_test` skips the suite only on a 32-bit host, where the probe is not built, and under
dastest's `--ser`/`--deser` sweep. A deserialized program never applies `[extern]`, while a
deserialized call already names its `__dasbind__` function in the `dasbind` module - and only
`apply` or a retarget manufactures that function. A 64-bit desktop tree without the library
fails the suite instead of skipping it.

`bindDynamicLibrary` reads an `[extern]`'s `library` as one name or a `;`-separated candidate
list, and the first candidate that loads wins, so a library named per version
(`nvrtc64_130_0;nvrtc64_120_0`) binds once on every box that has one of them. Each candidate is
tried as written, then with the platform's shared-library extension when it lacks one, and the
handle - a failed load included - is kept per `library` string for the process.

The `__dasbind__` proxy - the `DasBindFunction` a call to an `[extern]` is retargeted to - is
registered by the `[extern]` annotation's `apply` callback when the registrar - the module
carrying the `[extern]` declarations - compiles, and on demand by `transformCall` when the
`dasbind` module lacks it: the module cache serves a registrar in a process that already
compiled it (a live reload - the same process, so the `dasbind` module's hash shows no drift and
nothing reparses the registrar), or a fresh process reads a dependent's record before any
compile applied the registrar. `transformCall` takes the bind name from the call target's own
`[extern]` declaration every time (`bindNameOf`) - the one input every process has - rather than
from a table keyed on the function object, which every deserialization mints anew and whose
freed addresses a later compile reuses. The proxy's signature types are clones with no source
location (`proxyType`): the declaration's types and file record belong to the registrar's
compile, which a served registrar's program takes with it, while the proxy lives for the
process. A failed bind on the on-demand path is the call's transform error, the diagnostic
`apply` would have given. `transformCall` declines a call while any of its arguments is still
untyped - a `?:` over `null` gets its type on a later inference pass - and the call comes back
on that pass.

The proxy carries `SideEffects::modifyArgument` when the `[extern]` takes a non-const reference:
that argument is an out-parameter the C side writes through, and the placeholder body never
writes it, so without the flag the optimizer keeps the caller's old value. The JIT reads the
same flag (`build_interop_call` in `modules/dasLLVM/daslib/llvm_jit.das`, repo root), which drops
`nocapture` from such an argument.
