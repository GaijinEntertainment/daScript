# Browser input regression command

Use a matching native SDK and threaded wasm64 runtime/module build, with emsdk active
in the shell. From `modules/dasImgui/tests/`:

```sh
npm ci
npx playwright install chrome webkit
DASLANG_BIN=/absolute/sdk/bin/daslang \
DASLANG_WASM_LIB_DIR=/absolute/sdk/web/output64/lib npm test
```

The command builds both browser-input fixtures in an isolated temporary directory,
runs stock-input checks in Chrome and the Unicode example in Chrome and WebKit,
and removes the builds afterward. Missing paths, compiler/linker/browser failures
and assertions fail the command; it never substitutes a native or synthetic-only
check. Chrome exercises composition through CDP, including the password accessibility
tree. WebKit exercises committed Unicode input, rendering and focus changes; it does
not claim to exercise a real OS IME.

This is the required manual browser gate when changing the bridge or this example.
The ordinary native ImGui suite also runs the example and tests relocated native
packaging; the packaging test reports a skip on SDK builds without dasLLVM.
The current native nightly has no emsdk and disables LLVM. Automated web CI
provisioning remains separate work; a passing native nightly is not browser evidence.
Real input-method checks remain in the example's adjacent `todo.md`.
