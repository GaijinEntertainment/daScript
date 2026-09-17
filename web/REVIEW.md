# web (the WASM build and its shells) Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `README.md`. A file under this folder is served when the deploy
(`.github/workflows/pages.yml`, repo root), `daspkg release wasm`, or a build step that feeds
either copies it into a page a visitor loads.

**A playground UI file - an `examples/ui/src` script or stylesheet, or an
`examples/ui/samples` bundle - answers to `examples/ui/REVIEW.md`, wherever the diff puts
it.**

**A diff that makes either `REVIEW.das` check read fewer files or report less than it did
before the change is a defect - a change to `ALLOWED_HOSTS` or `DEPLOYED_ROOTS` does not
count as the difference.** The cookie-and-host check bans `document.cookie` and reports any
resource the files under `DEPLOYED_ROOTS` load from a host `ALLOWED_HOSTS` does not name;
the staged-tree check reports a tree a staging step copies into a served page that
`DEPLOYED_ROOTS` does not cover.

**A diff that stages a tree under this folder into a served page by a step `REVIEW.das` does
not read also makes `REVIEW.das` read that step, in the same change.** The gate's own
contract names the three step shapes it reads today.

**A diff that removes an entry from `REVIEW.das`'s `DEPLOYED_ROOTS` states, in the PR body,
which staging step stopped copying that tree.**

**A diff that adds a host to `REVIEW.das`'s `ALLOWED_HOSTS` states, in the PR body, what a
visitor sends that host and whether the host sets cookies.**

**A diff that flips the `DAS_WASM_RELAXED_SIMD` default to ON, or makes `-mrelaxed-simd`
unconditional in this folder's `CMakeLists.txt`, is a defect.** Safari and every iOS browser
refuse a whole module carrying one relaxed opcode, so the page fails to load rather than
running slower.

**A diff that changes a `-m` feature flag in this folder's `CMakeLists.txt`
`add_compile_options`, or the `DAS_WASM_RELAXED_SIMD` default, makes the matching change to
`modules/dasImgui/CMakeLists.txt`'s `IMGUI_WASM_FLAGS` and `DAS_IMGUI_WASM_RELAXED_SIMD` in the
same change.** Neither build inherits the other's flags.
