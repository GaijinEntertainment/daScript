# Modules Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`src/ast/ARCHITECTURE.md` (repo root; sec. 2, the module scan and the deferred shared modules).

**A diff under `dasMetal/` or `dasSpirv/`, or to any kernel body or fixture either emitter
compiles, applies `REVIEW_SHADER_EMITTERS.md` (beside this file) together with its own folder's
checklist.**

**A C++ module whose shared module is linked against another module's target - so its
`.shared_module` imports the other's - requires that module in its `initDependencies`
(`Module::require("<name>")`, then the required module's own `initDependencies`) in the same
change; a link without the requirement is a defect.** The descriptor manifest records a module
as deferred when nothing required it in the recording start, and the loader resolves the import
before any of the importing module's code runs - so an imported sibling left deferred beside an
eager importer fails the importer's load on the next start with "the specified module could not
be found", by whichever manifests the last run left.
