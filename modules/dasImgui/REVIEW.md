# dasImgui Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`.

**Never add or change a dasImgui test file - a `.das` file that declares a `[test]` function or
is named `test_*.das`, `failed_imgui_*.das`, or `record_*.das` - outside `modules/dasImgui/tests`
- put it under that folder instead.** A dasImgui test file, wherever the diff puts it, also
answers to the `tests/` subfolder's checklist (`modules/dasImgui/tests/REVIEW.md`).

**A diff that changes anything but comments in a `.das` file under this folder, or any file
under `modules/dasImgui/bind/` or `modules/dasImgui/src/`, runs the test suite on the author's
host OS before the PR: `preflight --only imgui`** (the per-OS exclude split: module `CLAUDE.md`
sec. Tests).

**A diff that changes the wasm SIMD compile flags in this folder's `CMakeLists.txt` -
`-msimd128`, `-mnontrapping-fptoint`, `-mrelaxed-simd`, or the `DAS_IMGUI_WASM_RELAXED_SIMD`
default - makes the matching change to `web/CMakeLists.txt` (repo root) in the same change.**
The archives link into the binary that file builds, and a relaxed-SIMD mismatch fuses `v_madd`
on one side of the archive boundary and splits it on the other.
