# dasCUDA Ledger

**Living document.** Planned work on the CUDA home's emitter and host boost: what each entry
costs today and what the change would do. An entry leaves when it lands or is refuted.

## Entries

- **The three text emitters carry one kernel-class scan each.** The CUDA, MSL and SPIR-V emitters
  each discover user functions, devirtualize methods and mark write targets with the same walk
  (`cuda_emit.das`'s `is_user_fn` / `mark_write_target` / `resolve_method` / `discover_user_fn` /
  `discover_method` and their twins in `modules/dasMetal/metal/msl_emit.das` and
  `modules/dasSpirv/spirv/spirv_emit.das`), differing on the context type, the excluded builtin
  modules and the error wording. Done = one discovery rail in `daslib/shader_emit_common.das`,
  generic over the context, each emitter keeping its spelling table.
- **The statement emitters shared with MSL.** `emit_if`, `emit_for`, `emit_loop_hints`,
  `emit_user_fns` and `hoist_const_table` differ from their MSL twins on the context type, the
  pragma spelling, the constant-table keyword and the language word in errors. Done = the five in
  the shared rail over a per-backend spelling hook.
- **The coopmat marker names are parsed twice.** `coop_info`, `coop_type_of`, `wg_info_of`,
  `is_tensor_layout` / `is_tensor_view` and the SPIR-V emitter's `coopmat_info`,
  `coopmat_wg_info`, `tensor_object_type_of` parse the same use, component, width, rows, cols,
  scope and layout out of a marker name. Done = one classifier beside the markers in
  `spirv_builtins`, returning the tuple; each emitter maps it to its own form.
- **A compile-time integer from an expression, three ways.** `cuda_emit`'s `const_int_of` folds a
  global `let` by hand, the SPIR-V emitter's `const_uint_value` repeats the peel, and
  `emit_value`'s global-constant arm already goes through `get_const_expr`. Done = one
  `const_int_of(e)` over `get_const_expr` for all three.
- **The `cuda_kernel` / `metal_kernel` / `spirv_kernel` annotations declare their companion
  globals by hand.** Each `apply` chains `add_typed_global` calls and each `fixup` writes the
  constant inits one by one. Done = a table of (suffix, type, value) walked by one loop in both,
  with the SPIR-V annotation on the shared scaffolding too.
- **Local folds inside `cuda_emit.das`.** The `ldmatrix` PTX text appears six times; the
  four-wide decode run and the predicted-step register set are spelled in both the plain and the
  pipelined decode; `coop_load_helper` holds byte-identical f16/s8 bodies; the linear thread index
  and the `ExprRef2Value` peel are spelled at several sites. Done = one helper each.
- **The oracle fixtures' decode trio.** `test_cuda_oracle.das`'s `test_cm2deck_gpu`,
  `test_cm2raw_gpu` and `test_cm2dec_gpu` and the `Cm2DecK` / `Cm2Raw` fixtures differ on the
  block struct, the blocks a row, the column count and the pad clamp. Done = one fixture
  template over the block type and one `decode_case` the three tests call.
- **`cuda_intrinsics` under AOT.** The module is `options no_aot`: with its stand-ins AOT'd through
  the `tests/cuda/_cuda_common.das` translation unit, `test_aot` links every function the `Intrin`
  fixture's CPU replay reaches (`ldmatrix` on the s8 fragments, `mma_scale_acc`, `byte_perm`,
  `clock32`, `cuda_asm_value`) with a stub hash that differs from the runtime's, while
  `cp_async_16` beside them links; a fresh `_aot_generated` and a fresh module cache leave it.
  Done = the two SimNode dumps side by side (`skills/internal/aot_hash_desync_debugging.md`),
  the cause fixed, the option dropped.
- **The driver binding under AOT.** A dasbind late extern AOTs as its proxy's mangled name, which
  no C++ translation unit can spell, so `cuda_driver.das` and `das_cuda_boost.das` are
  `options no_aot` and the module's AOT file list is empty; a program that AOTs its CUDA launch
  plumbing runs it interpreted. Done = an AOT lowering for late externs in `daslib/aot_cpp.das`
  that resolves the symbol through the dasbind table at first call, the two options dropped, the
  module's AOT file list restored, `test_aot` green over `tests/cuda`.
