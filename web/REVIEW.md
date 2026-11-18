# Web Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `README.md`.

**A playground UI file - an `examples/ui/src` script or stylesheet, or an
`examples/ui/samples` bundle - answers to `examples/ui/REVIEW.md`, wherever the diff puts
it.**

**A diff that edits `REVIEW.das` outside its `ALLOWED_HOSTS` and `DEPLOYED_ROOTS` lists so that
any of its checks reads fewer files or reports less than before is a defect.**

**A diff that makes the deploy (`.github/workflows/pages.yml`, repo root),
`daspkg release wasm`, or a build step that feeds either copy a tree under this folder into a
page a visitor loads, by a step `REVIEW.das` does not read, also makes `REVIEW.das` read that
step and adds the step's shape to this rule's list of the shapes it reads, in the same change.**
The shapes `REVIEW.das` reads: a `cp` in the deploy whose destination starts with `_site/`, and a `-E copy` or `-E copy_directory` in `CMakeLists.txt` whose destination
contains `SITE_` or `/site/`.

**A diff that removes an entry from `REVIEW.das`'s `DEPLOYED_ROOTS` states, in the PR body,
which staging step stopped copying that tree.**

**A diff that adds a host to `REVIEW.das`'s `ALLOWED_HOSTS` states, in the PR body, what a
visitor sends that host and whether the host sets cookies.**

**A diff that flips the `DAS_WASM_RELAXED_SIMD` default to ON, or makes `-mrelaxed-simd`
unconditional in this folder's `CMakeLists.txt`, is a defect.** Safari and every iOS browser
refuse a whole module carrying one relaxed opcode, so the page fails to load rather than
running slower.

**A diff that changes a `-m` feature flag in this folder's `CMakeLists.txt`
`add_compile_options` makes the matching change to `modules/dasImgui/CMakeLists.txt`'s
`IMGUI_WASM_FLAGS` in the same change.** Neither build inherits the other's flags.

**A diff that changes a `-m` feature flag, an exception-handling flag (`-fwasm-exceptions`,
`-sWASM_LEGACY_EXCEPTIONS`), `-pthread`, or `-sMEMORY64` in this folder's `CMakeLists.txt` makes
`site/files/wasm-gate.js` (repo root) test for exactly the WebAssembly features and page
isolation the build then requires, in the same change.** A gate that tests less lets the page
fail with the engine's raw error text; one that tests more refuses a browser that could run the
build.
