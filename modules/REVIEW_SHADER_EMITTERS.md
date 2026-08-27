# Shared emitter rules - dasSpirv and dasMetal

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `modules/dasMetal/MASTERPLAN.md` and `modules/dasSpirv/MASTERPLAN.md`.

**These rules bind both emitters.** Each module's `REVIEW.md` routes here, and a change under
that module applies this file together with the module's own list.

- **Never put anything that cannot run on the CPU into a kernel body or into a function a
  kernel calls - keep both in ordinary das.** The CPU run of the same body is the test oracle.

- **A diff that adds or changes an emitter builtin ships a CPU body that returns what the
  emitted form returns, argument for argument.**

- **Never let a construct the emitter cannot lower produce a kernel or a crash - the emitter
  reports a compile error that names the construct.**

- **Never pass a shape constant to a kernel as a runtime argument - pass it as a call-site
  constant.** A shape constant is any value that fixes the kernel's tiling: a tile row count, a
  tile column count, a cooperating-simdgroup count, a staged chunk depth.

- **Never pass a matmul reduction width known only at run time into a kernel any way but
  through the emitter's runtime-extent descriptor.** The reduction width is the K dimension -
  the length of the loop the kernel accumulates over. That descriptor is `dynamic_extent` on
  Metal, and on SPIR-V a `tensorLayout2D` or a `tensorLayout2DPad` whose dimension
  `tensorLayoutSetDimension` sets. K does not fix tiling, so it is not a shape constant.

- **A diff that adds a kernel capability needing a runtime shape value ships a specialization
  path.** A specialization path is one compiled variant per constant shape. When the capability
  cannot have one, the same diff records that in the module's `MASTERPLAN.md`.

- **Never check a claim about emitted shape against the das source - check it in the emitted
  words or text.** Emitted shape is the structure of the emitted kernel - its signature, its
  parameter attributes, its statement forms - and its stamped shape values (tile, grid,
  threadgroup sizes).

- **A diff that adds a kernel-model capability to one emitter adds it to the other, or records
  the asymmetry in the shared ledger (`modules/dasMetal/MASTERPLAN.md`).** A kernel-model
  capability is a way of authoring or lowering kernels: class members, multiple kernels per
  class, method flattening, tensor-GEMM builtins.

- **An emitter diff that uses a `daslib/shader_lingua_franca` declaration this emitter does not
  handle ships that emitter's lowering of the declaration, in the same change.** The diff may
  instead ship a test showing this emitter rejects the declaration by name. A declaration in
  that module is available to both emitters.
