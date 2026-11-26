# src/simulate Architecture

`src/simulate/` holds the interpreter's runtime sources. The node declarations sit in
`include/daScript/simulate/`, whose own architecture document covers them. This document
covers the fusion engine, the `simulate_fusion*` files.

A fused node is one interpreter node that replaces a node and the simple children it reads
through: a local, an argument, a global, a constant. `fusionContext` (`simulate_fusion.cpp`)
walks every function a round at a time, children before their parent, until a round fuses
nothing. A node fused in one round is seen by its parent in the next round under its fused
name: the op name followed by the source name of each operand, `PtrAtR2VGlobAny` for one.

The semantic hash of a function is taken from its fused tree. A new fused node therefore
changes the hash of every function that gets it, and AOT output is tied to the fusion level
that generated it.

## Small-int reads answer widened {#fused-small-int-reads}

A small int is `int8`, `uint8`, `int16` or `uint16`. A read of one from memory hands it to
`eval` widened to 32 bits: zero-extended when unsigned, sign-extended when signed. A small-int
argument slot is not such a read: a fused call fills it zero-extended whatever its sign.

A non-fused read of a small int answers `eval` only. Its `evalInt` reads four bytes from a
slot one or two bytes wide, so nothing calls it. A fused read of a small int answers `eval`,
`evalInt` and `evalUInt` with the same widened value (`DAS_FUSION_WIDE_NODE`,
`simulate_fusion.h`). The fused reads are the small-int stamps of the array, fixed-array and
pointer index families, and the widened reads (`simulate_fusion_widen.cpp`).

A cast from a small int to `int` or `uint` changes no bit of that widened value. So the widen
fusion point replaces the cast over a read of a local, of a local reference or of an argument
reference with one widened read, and removes the cast over a fused small-int index read: the
read below it answers `evalInt` and `evalUInt` directly. It tells a fused index read from a
non-fused one by its name, an op name from `g_fusedSmallIntReads` followed by two source
names. The same cast over any other node stays: a by-value small-int argument, a field
reached through a pointer, a read with a 64-bit index.

The small-int stamps exist for a 32-bit index only. An array, fixed-array or pointer read of
a small int with a 64-bit index stays on the non-fused node.

## The pointer index family {#pointer-index-family}

`simulate_fusion_ptr_at.cpp` fuses `p[i]` on a pointer: the read by value, the read by
reference, and the store. One class template computes the address from two policy types, where
the pointer comes from and where the index comes from. A table of instantiations stamps the
element types. The family has no bounds check and no null check, like the nodes it replaces;
the index is `int` or `uint`, and a 64-bit index stays on the non-fused node.

A store is `p[i] = value`, for the same pointer and index sources as the read. `*p = value`
is the store without an index; it fuses for a pointer held in a local or an argument, and
keeps the null check of the `Ptr2Ref` node it replaces. A store evaluates the value first and
the address second, the order of the `Set` node it replaces. A small-int store takes its value
through `eval` and keeps the low bits, so a narrowing cast from `int` or `uint` right under it
is removed.

The family is compiled at `DAS_FUSION` 2 and above. The small-int stamps, the widened reads
and the cast removal are compiled at `DAS_FUSION` 1 and above.

## Globals read by name hash {#global-by-hash-source}

A context that is not solid reads a global through its mangled name hash: the node holds the
hash and looks the offset up on every access. `SimSourceType::sGlobalMnh` is that source, named
`GlobMnh` in a fused node's name. The array and fixed-array index families take it as a base:
their two files define `FUSION_OP2_SET_GLOBAL_MNH` before they include the set permutation
list, which adds the lines for it. The pointer index family takes it as a base too. The `Set`
family and the table index family include the same list without the define, and the binary
operation and call families have a list of their own; none of them takes a global by hash.
