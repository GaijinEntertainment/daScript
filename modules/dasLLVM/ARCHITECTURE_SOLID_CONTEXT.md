# dasLLVM architecture - solid-context globals

Companion to `ARCHITECTURE.md`: what the JIT does with a global's address under
`options solid_context` between the pass pipeline and code generation.

## 1. A solid array's base reaches code generation hidden {#solid-array-base}

Under `options solid_context` a global's address is the globals block plus a constant, and the
optimizer uses that: two globals at different offsets do not overlap. The backend's loop strength
reduction uses it too, rebasing a loop's induction variables on the block, which costs registers
and spills. So after the pass pipeline and before the emit, `hide_solid_array_bases` wraps each
such address that an indexed access is based on in `llvm.launder.invariant.group`, which code
generation strips after the reduction ran. The pass works from the values that hold a block, not
from one instruction shape: a load of the context's globals or shared slot, and an argument of a
local function whose every caller passes such a value - what argument promotion leaves of the
load in a function that only reads the block. A global's address is one of those values plus a
constant; the first global of a block is the value itself, and there the launder covers the
indexed accesses alone, so the other globals keep the plain block. The one-module build does it in
`optimize_llvm_module`; the split build runs its pool twice for a partition set that addresses a
solid global - optimize, hide, emit - and writes the IR dump between the two. An LTO partition is
left alone: its link optimizes again, and a hidden base would cost that pass the overlap facts.
