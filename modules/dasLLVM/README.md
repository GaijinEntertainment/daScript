# dasLLVM 
LLVM-C Bindings for daScript

This project provides LLVM-C bindings for daScript, generated for `LLVM 16.0.6`.
These bindings allow you to interact with LLVM's compiler infrastructure
directly from daScript.

# Installation
## Automatic Installation (Recommended)
During the CMake configuration process, the build system will:
- First attempt to locate a system-installed `LLVM 16`
- If not found, automatically download prebuilt LLVM packages from:
https://github.com/aleksisch/llvm-16-prebuilt/releases/tag/v2.0.0
- Place the library into `das_root/lib/LLVM.dll`.

Note that the filename is fixed and cannot be changed, regardless of your platform.
The reason is [bindings](bindings/llvm_func.das)

### Manual installation
#### Linux (Debian/Ubuntu)
```bash
sudo apt-get install -y libclang-16-dev clang-16
```
#### Darwin
```sh
brew install llvm@16
```
#### Windows
Note: The `LLVM 16.0.6` release on the `LLVM` website does not ship developer headers.
Consider building it from scratch.

## JIT Usage
Take your project, for example:
```
[jit] // Not necessary, default behaviour
def foo() {
    print("Hello from JIT!")
}

[no_jit]
def bar() {
    print("Hello from interpreter!")
}

[export]
def main() {
    print("Hello from JIT!\n")
}
```
And run it:
```
./bin/daslang -jit main.das
```
This module will create binaries on-the-fly for the `foo` and `main`
functions, and use them instead of the interpreter at runtime.

## Caching System
When using dynamic daslang (`./bin/daslang_dyn`) and `jit_use_dll_mode=true`
(default behaviour):
- **First Run:** Compiles the code to `LLVM IR`, optimizes it, generates native
code, and saves it as a dynamic (shared) library.
- **Subsequent Runs:** Checks if the source code has changed. If unchanged, loads the
cached DLL for instant execution.
### DLL location
- By default, the `dll` is stored in `get_das_root()/jitted_scripts`.
- This can be changed using `jit_output_folder` (not exposed to `CLI` yet).
