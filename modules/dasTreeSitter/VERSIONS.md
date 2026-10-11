# Vendored Tree-sitter components

The module vendors generated parsers and the dependency-free C runtime so
normal builds never need network access or a Tree-sitter CLI installation.

| Component | Release | Upstream |
| --- | --- | --- |
| Tree-sitter runtime | v0.27.1 | https://github.com/tree-sitter/tree-sitter |
| C grammar | v0.24.2 | https://github.com/tree-sitter/tree-sitter-c |
| C++ grammar | v0.23.4 | https://github.com/tree-sitter/tree-sitter-cpp |
| Markdown + Markdown inline grammars | v0.5.3 | https://github.com/tree-sitter-grammars/tree-sitter-markdown |
| daslang grammar | repository-local | `tree-sitter-daslang/` |

The runtime copy holds `lib/include/` and `lib/src/` of the release without
`lib/src/wasm-stdlib/`, which only a build with `TREE_SITTER_FEATURE_WASM`
uses.

Runtime and grammar upgrades should be made together and followed by all
multilingual highlight tests. The generated grammars currently use ABI 14 or
ABI 15; the runtime accepts both.
