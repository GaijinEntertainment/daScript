# tree-sitter-daslang

[![CI][ci]](https://github.com/GaijinEntertainment/tree-sitter-daslang/actions/workflows/ci.yml)
[![crates][crates]](https://crates.io/crates/tree-sitter-daslang)
[![npm][npm]](https://www.npmjs.com/package/tree-sitter-daslang)
[![pypi][pypi]](https://pypi.org/project/tree-sitter-daslang)

Daslang grammar for [tree-sitter](https://github.com/tree-sitter/tree-sitter). [Daslang](https://daslang.io/) is a
statically typed programming language for games and real-time applications. Its compiler is in the daslang repository,
[GaijinEntertainment/daScript](https://github.com/GaijinEntertainment/daScript).

The grammar follows the gen2 syntax of the daslang compiler. It does not read the gen1 syntax, which a file selects with
`options gen2 = false`. The grammar checks the syntax only: it does not do the checks that the compiler does after it
reads a file, such as type inference and name lookup.

## Versioning

The version of the grammar is `X.Y.P`. It is not a semantic version.

```text
0.6.1
|   |
|   +-- P: the number of the grammar release for that daslang version; the first release is 0
+------ X.Y: the newest daslang version that the grammar covers
```

- `X.Y` is the major and minor number of the newest daslang version that the grammar covers. Each `0.6.P` covers
  daslang 0.6, and the first release for daslang 0.7 is `0.7.0`.
- `P` is the grammar's own number. It counts the releases of the grammar for one daslang version, and it does not
  follow the patch number of the compiler. `0.6.1` is the second release of the grammar for daslang 0.6. It is not a
  grammar for the compiler release 0.6.1.

A new `P` is a new release of the grammar for the same daslang version: a correction of the trees or of the queries. A
new `X.Y` adds the syntax of a newer daslang version.

## References

- [The daslang parser](https://github.com/GaijinEntertainment/daScript/tree/master/src/parser)
- [The daslang language reference](https://github.com/GaijinEntertainment/daScript/tree/master/doc/source/reference/language)

[ci]: https://img.shields.io/github/actions/workflow/status/GaijinEntertainment/tree-sitter-daslang/ci.yml?logo=github&label=CI
[crates]: https://img.shields.io/crates/v/tree-sitter-daslang?logo=rust
[npm]: https://img.shields.io/npm/v/tree-sitter-daslang?logo=npm
[pypi]: https://img.shields.io/pypi/v/tree-sitter-daslang?logo=pypi&logoColor=ffd242
