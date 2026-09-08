# AST Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`.

- **Weakening `REVIEW.das` (beside this file) is a defect:** dropping its scan of the prints in
  `trySerializeProgramModule` (`ast_parse.cpp`, `ARCHITECTURE.md` sec.1) or of the cache-read
  lines elsewhere in that file, or a finding text that no longer names what failed. What the gate
  enforces is read from the gate itself.

- **A diff that adds a module-cache read diagnostic outside `trySerializeProgramModule`, or moves
  one out of it - into a helper it calls, or another function - extends `REVIEW.das`'s scan to the
  new home in the same change, and the line prints only when the user asked: the serializer's
  `quietCache` off, or `log_module_compile_time` set.** A print the gate does not scan is a print
  nobody checks, and an ungated line there is output every user of the default cache sees.

- **A diff that changes what a manifest written by an earlier binary replays to - a field added,
  removed, reordered or re-typed in `read_manifest` or `write_manifest` (`dyn_modules.cpp`), a
  line added beside the key lines `stamp`, `dll`, `root`, `dasroot`, `target` and `dep`, or a
  change to what an existing row registers - bumps the version in `MANIFEST_HEADER` in the same
  change.** A reader accepts a manifest whose first line equals `MANIFEST_HEADER`, so without the
  bump an older manifest decodes the changed bytes as a wrong registration with no diagnostic.

- **A diff that gives `read_manifest` a new kind of row - one it pushes into `rows` - gives the
  replay loop in `init_dyn_modules` (`dyn_modules.cpp`) a branch for it in the same change.** The
  loop dispatches on `row.dynamic` and then on whether the row carries a das-visible name, so a
  kind it does not know replays as a native path or waits under a name nothing requires.

- **Removing the `setDeferredModuleLoader` call from `require_dynamic_modules`
  (`dyn_modules.cpp`) is a defect.** A descriptor compiled during the scan can require a module
  an earlier replay deferred, and with no loader installed that require fails.

- **A diff that moves the `setDeferredModuleLoader` call in `require_dynamic_modules` keeps it
  above the first `init_modules_for_folder` call, in the same change.** That call compiles the
  descriptors, and a descriptor can require a module an earlier replay deferred.

- **A diff that changes how `Function::getMangledName` (`ast.cpp`) forms a name, or which module
  `Module::addFunction` (`ast_module.cpp`) files a builtin function under, bumps
  `LLVM_JIT_CODEGEN_VERSION` in `modules/dasLLVM/daslib/llvm_jit_run.das` (repo root), in the
  same change.** The JIT's DLL cache key folds the codegen version and each function's AST hash,
  never the name an extern binds under, so a cached DLL binds the old name and crashes on the hit.
