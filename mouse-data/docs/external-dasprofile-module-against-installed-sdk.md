---
slug: external-dasprofile-module-against-installed-sdk
title: How do I wire an external dasProfile-style C++ module to an installed daslang SDK so `require testProfile` works in dynamic builds?
created: 2026-05-11
last_verified: 2026-05-11
links: []
---

Use `find_package(DAS REQUIRED)` and resolve SDK root from `${DAS_DIR}/../../..`.

Build the module as a shared module artifact and place it in the SDK modules tree:
- target: `testProfile` built as `MODULE`
- properties: `PREFIX ""`, `SUFFIX ".shared_module"`
- output dir: `${DAS_SDK_ROOT}/modules/dasProfile`
- link against `DAS::libDaScript`
- compile with SDK-compatible flags (`-fno-rtti` on macOS/clang in this setup)
- ensure Release/NDEBUG when SDK is Release, otherwise runtime rejects with build-id mismatch (`host 60208`).

Create `${DAS_SDK_ROOT}/modules/dasProfile/.das_module` containing:
- `register_dynamic_module("{project_path}/testProfile.shared_module", "testProfile")`

Important: the second argument must match the symbol from `REGISTER_DYN_MODULE(..., testProfile)`; using `Module_TestProfile` fails with missing `register_dyn_Module_TestProfile`.

For dasProfile scripts that use `get_das_root()+"/examples/profile/..."`, sync the repo scripts into `${DAS_SDK_ROOT}/examples/profile` during build (copy `main.das`, `config.das`, `bytecode_test.das`, `tests/`, `extra_test/`).

Validation command:
`$DAS_SDK_ROOT/bin/daslang -compile-only $DAS_SDK_ROOT/examples/profile/tests/native.das`
should pass with module resolution working.

## Questions
- How do I wire an external dasProfile-style C++ module to an installed daslang SDK so `require testProfile` works in dynamic builds?
