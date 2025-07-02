# dasLLVM 
LLVM-C Bindings for daScript

This project provides LLVM-C bindings for daScript, generated for LLVM 16.0.6. These bindings allow you to interact with LLVM's compiler infrastructure directly from daScript.

## Installation

### Windows
The official LLVM Windows releases don't include developer headers. We provide prebuilt packages:
Download our custom LLVM 16.0.6 build from: https://github.com/aleksisch/llvm-windows-prebuilt/releases/tag/v1.0.0

### Linux (Debian/Ubuntu)
```bash
sudo apt-get install -y libclang-16-dev clang-16
```

### Darwin
```
brew install llvm@16
```