# tree-sitter-daslang

Tree-sitter grammar for the gen2 syntax of daslang. The daslang compiler is in the daslang repository,
GaijinEntertainment/daScript, whose name keeps the old spelling.

## Reference

- The authority is the gen2 syntax of the daslang compiler: `src/parser/ds2_parser.ypp` (the grammar),
  `src/parser/ds2_lexer.lpp` (the tokens and the lexer states), `src/parser/parser_impl.cpp`, `parser_impl.h`, and
  `parser_state.h` (the parser helpers and the lexer state). Settle each grammar question in that code, not in the docs.
- Gen1 is out of scope. `src/parser/ds_parser.ypp` and `ds_lexer.lpp` are not a source, and a file with
  `options gen2 = false` is not input of the grammar.
- Two releases of the compiler are in use: daslang `master` (0.6.5) and the copy in `prog/1stPartyLibs/daScript/` of
  the Dagor engine (0.6.4). The grammar accepts the input of both. Only 0.6.5 accepts these forms:
  - `-9223372036854775808l`, the minimum of `int64`, as a number constant and an annotation argument.
  - An `int64`, a negative `int64`, a `uint64`, and a negative `int` or `float` constant as an annotation argument.
  - `=>` at the end of a line: the line end after it returns no semicolon.
- An input that the compiler accepts parses without ERROR or MISSING nodes, and `has_error` of the root node is false.
  An input that the compiler rejects for its syntax produces ERROR wherever the grammar can see the defect.
- The syntax verdict of the compiler comes from its parser: the compiler rejects the syntax when the parse reports a
  lexer error (an error code from 10000 to 19999) or an error whose message starts with `syntax error` (codes 20001 and
  30151). The parse actions report other errors from 20000 to 29999, such as an unknown annotation or module; they are
  not syntax errors, and the grammar accepts that input.
- `daslang -compile-only file.das` resolves each `require` before it parses the file, and a module that it cannot find
  stops the compile before the parse. For a file whose modules are not available, call `parseDaScriptNoInfer` of the
  compiler library with `policies.version_2_syntax = true`, which parses without the requires.
- The grammar does not check what the compiler checks after it reads the syntax: type inference, name resolution, and
  the checks in the actions of the parser.
- The compiler reads a file with gen2 syntax unless the first `options gen2` outside comments and strings is followed by
  `= false` (`detectGen2Syntax` in `src/ast/ast_parse.cpp`). A file without the marker takes the default of the host:
  gen2 for `daslang`.
- In Dagor, a file without the marker is gen1 for the game hosts: they set `policies.version_2_syntax` from the
  `DasSyntax` of the loader context (`prog/gameLibs/ecs/scripts/das/das_scripts.cpp`, the enum in
  `prog/gameLibs/publicInclude/ecs/scripts/dasEs.h`), and no game setting selects `V2_0`. Two sets of such files are
  gen2: those in the daslang copy, `prog/1stPartyLibs/daScript/`, which `daslang` runs, and those that the internal
  hosts with gen2 policies load. Outside the copy, `git grep -n -E 'version_2_syntax = true|syntax_version=V2_0'` finds
  these hosts. On Dagor `master`, two of them load files of the checkout: a user-script host loads a samples directory,
  and a server started with `--local_dascript_syntax_version=V2_0` loads the directory that its start script passes in
  `--scripts_path`. The Dagor repository is internal: cite a Dagor path here only when GaijinEntertainment/DagorEngine
  on GitHub has it.
- The compiler rejects this input, and the grammar accepts it. Keep the list current:
  - `..` or `=>` after a block, lambda, or local function whose body follows `=>`, as in `$(x) => a .. b`. The
    compiler's parser tables merge the lookaheads of every `=>` body and reject the token there; the grammar reads an
    interval or a tuple pair of the block.
  - `>>` or `>>>` in an expression inside the `<` `>` of a type, as in `array<typedecl(a >> b)>`. The compiler lexer
    splits it into `>` tokens while a type is open; the scanner splits it only where the parse state takes no shift
    operator.
  - `>>` or `>>>` in an expression anywhere after a type macro with `<`, as in `a : foo<int>` and a later `x >> 1`. The
    type macro rules of `ds2_parser.ypp` raise `das_arrow_depth` and never lower it, so the compiler lexer splits each
    later `>>` into `>` tokens. That is a compiler defect; the grammar does not copy it.
  - A token that the compiler lexer reads by the longest match where the grammar's parse state takes only a shorter
    one, outside the tokens that the scanner reads: `var a &= 1`, `a : int&&`, and `x => a..b` read as `&` `=`, `&`
    `&`, and `.` `.`. The internal lexer of tree-sitter considers only the tokens of the parse state; the scanner
    would have to read every operator to see these.
- The grammar does not run reader macros or read included files. Input whose syntax depends on them can parse
  otherwise than in the compiler: an included file that holds part of a declaration, an inline reader macro whose
  rewrite is not one operand, an inline reader macro whose parenthesized rewrite completes the syntax before it, as
  `f %m! x %%` is a call when the rewrite is `(x)`, and a `%name~` macro that ends its text at a place other than the
  first `%%`, where `reader_text` ends.
- The files of daScript `master` that the compiler rejects for a syntax error: `tests/language/failed_aka.das`,
  `failed_annotation_int64_overflow.das`, `failed_annotation_int64_positive.das`, `failed_cast_requires_parens.das`,
  `failed_comment_eof.das`, `failed_constants.das`, `failed_mismatching_curly_bracers.das`,
  `failed_mismatching_parentheses.das`, and `failed_named_call_order.das` (all in `tests/language/`),
  `utils/detect-dupe/fixture/_fixture_broken_cases.das`, `utils/mcp/tests/_fixture_gen1.das`, and
  `utils/mcp/tests/_fixture_syntax_error.das`.

## Where things live

- `grammar.js` - the syntax.
- `src/scanner.c` - the lexer state of the compiler: the line-end tokens, the depth of `(` and `[`, the open braces, the
  keyword flag of a statement, strings, numbers, character constants, reification tags, the include directive, and the
  tokens that the compiler lexer reads with lookahead.
- `src/parser.c`, `src/grammar.json`, `src/node-types.json`, `src/tree_sitter/` - generated by `tree-sitter generate`
  with tree-sitter CLI 0.27.1. Never edit them by hand. Regenerate with that CLI version, and make a CLI upgrade its own
  change, which also moves the `tree-sitter-ref` of the CI workflow, the `TREE_SITTER_CLI_VERSION` and
  `TREE_SITTER_CLI_SHA256` of the publish workflow, the `tree-sitter-cli` of `package.json`, and the `tree-sitter`
  dev-dependency of `Cargo.toml` with its docs link in `bindings/rust/lib.rs`: CI regenerates the parser and fails on a
  difference.
- `bindings/`, the package manifests (`binding.gyp`, `Cargo.toml`, `CMakeLists.txt`, `go.mod`, `Makefile`,
  `package.json`, `Package.swift`, `pyproject.toml`, `setup.py`), `.editorconfig`, `.gitattributes`, and `.gitignore` -
  generated by `tree-sitter init` from `tree-sitter.json`. Run `tree-sitter init --update` after a change to
  `tree-sitter.json` or a CLI upgrade, and set the version with `tree-sitter version <version>`. The update of CLI
  0.27.1 adds a second `let dir` line to `Package.swift` and reorders `tree-sitter.json`; revert both. The MinGW
  branches of the `install` and `uninstall` rules in `Makefile` differ from the template; keep them.
- `package-lock.json`, `Cargo.lock`, `go.sum`, `Package.resolved` - lockfiles that npm, cargo, go, and swift write when
  they resolve the dependencies of the manifests. Commit them with the manifest change.
- `.github/` - the CI and publish workflows, and the issue template config that sends issues to the daslang
  repository. The publish workflow authenticates to crates.io, PyPI, and npm with trusted publishing and holds no
  registry token.
- `.github/workflows/sync.yml`, `.github/scripts/sync-mirror.sh`, and `.github/workflows/close-pull-requests.yml` - the
  copy of the grammar changes of daslang `master` to `main`, and the close of pull requests. Both workflows run only
  when the repository variable `MIRROR_SYNC` is `on`.
- `eslint.config.mjs` - the lint configuration for `grammar.js` (`npm run lint`).
- `examples/` - daslang files that the CI workflow parses.
- `queries/highlights.scm` - editor highlighting. `test/highlight/` holds its capture assertions.
- `queries/tags.scm` - the definitions and references for code navigation (`tree-sitter tags`). `test/tags/` holds its
  assertions.
- `test/corpus/` - corpus tests, one file per topic. `errors.txt` holds the inputs that must produce ERROR, each marked
  `:error`.

## Names

- A node kind takes the snake_case name of the AST class that the compiler parser builds for the construct, without the
  `Expr` prefix and with each abbreviation spelled out: `ExprOp2` is `binary_operation`, `ExprPtr2Ref` is
  `pointer_to_reference`, `ExprConstUInt8` is `constant_unsigned_integer8`.
- An expression name that equals a keyword takes the suffix `_expression`: `for_expression`, `while_expression`,
  `with_expression`, `unsafe_expression`, `return_expression`, `break_expression`, `continue_expression`,
  `yield_expression`, `let_expression`, `delete_expression`, `label_expression`, `goto_expression`,
  `assume_expression`, `cast_expression`, `new_expression`, `is_expression`, and `block_expression`.
- A declaration takes the name of its class in `include/daScript/ast/ast.h` followed by `_declaration`:
  `function_declaration`, `variable_declaration` (a `Variable`, also each function argument), `field_declaration`,
  `enumeration_declaration`, `annotation_declaration`. A directive takes the name of its rule in `ds2_parser.ypp`:
  `module_declaration`, `require_declaration`, `options_declaration`, `expect_declaration`.
- A form that shares a class with other forms, or has no class, takes the name that the language reference
  (`doc/source/reference/language/`) gives it: `struct_declaration` and `class_declaration` (both `Structure`),
  `typedef_declaration`, `tuple_alias_declaration`, `variant_alias_declaration`, `bitfield_alias_declaration`,
  `local_type_alias` (an `ExprAssume` with a type), `interval` (`..`), `pipe` (`<|`, `|>`, and a piped block),
  `finally_block`, `interpolation`, `format_specifier`, `escape_sequence`. A type form takes the reference name with the
  suffix `_type`: `pointer_type`, `smart_pointer_type`, `fixed_array_type`, `array_type`, `table_type`, `iterator_type`,
  `block_type`, `function_type`, `lambda_type`, `tuple_type`, `variant_type`, `bitfield_type`, `option_type`,
  `auto_type`, `basic_type`, `typedecl_type`, `type_type` (`type<T>` in a type), and `type_macro`.
- A form with neither a class nor a reference name takes the name of its rule in `ds2_parser.ypp`: `global_let`,
  `structure_type`, `qualified_name` (`name_in_namespace` with `::`), `annotation_list`, `expected_error`.
- A field takes the name of the AST member that holds the part, spelled out: the `condition`, `if_true`, and `if_false`
  of `if_then_else`, the `subexpression`, `left`, and `right` of `ternary_operation`, the `first_type` and
  `second_type` of a type, the `init` of a variable. A pipe has the `function_call` and `argument` fields of its parser
  rule.
- Exceptions to these rules:
  - `source_file` is the root, as in the other tree-sitter grammars.
  - `make_block` is one kind for `$`, `@`, and `@@`, although the reference names a block, a lambda, and a local
    function: `block_expression` is the statement block, and the leading token tells the three apart.
  - `make_array` covers `[]`, which builds an `ExprMakeStruct`, and the `array struct<T>(...)`, `array tuple<...>(...)`,
    and `array variant<...>(...)` forms. `make_table` covers `{...}` and `table(...)`, which build an `ExprMakeArray`
    inside a call. `make_tuple` covers `tuple<...>(...)`, which builds an `ExprMakeStruct`.
  - `parenthesized_expression` has no class and no reference name.
  - `move_argument` is the `<- x` argument of a call, which builds a call of `consume_argument`.
  - `unsafe_call` is `unsafe(x)`; the reference calls it an unsafe expression, a name that `unsafe_expression` (the
    `unsafe { }` block, `ExprUnsafe`) takes.
  - `modified_type` holds a type and one of the modifiers `const`, `&`, `#`, `implicit`, `explicit`, `-const`, `-&`,
    `-[]`, `-#`, `==const`, `==&`, which the reference does not name.
  - `module_path` is the module name of `require` and `with (module ...)`.
  - `qualified_name` has a `module` field, which no AST member holds.
  - A `field`, `safe_field`, `is_variant`, or `as_variant` whose name is a `$f(...)` tag keeps its kind, and the tag
    is its `name`; the compiler builds an `ExprTag` around the field.
  - `operator_name`, `enumeration_entry`, `bitfield_entry`, `line_directive`, and `include_directive` have no class and
    no reference name.
  - `inline_reader` is the inline form `%name! ... %%` of a reader macro, for which the compiler builds no node.
  - `negative_integer64_minimum` appears in the tree as `constant_integer64`.

## Rules

- The compiler lexer returns a semicolon token at a line end, at the end of the text, and before a `}`, unless a `(` or
  `[` is open on the current level or the keyword flag is set. The scanner returns `_newline_semicolon` there, also
  where no rule accepts it, so that a line break inside an expression is an error as it is for the compiler.
- `_newline_semicolon` and `_newline_comma` are zero-width and end before the line break, so that no node takes the
  line break; the next scan returns the line break as the extra `_line_end`.
- The compiler parser opens a level of lexer state at each statement block and enumeration body, and the scanner keeps
  a stack of the open braces. The parse state chooses the kind of a `{`: `_block_open` opens a statement level,
  `_list_open` an enumeration or bitfield level where a line end returns `_newline_comma`, `_table_open` counts as an
  open `(`, `_brace_open` changes no level, and `_interpolation_open` starts the expression of a string.
- Inside an interpolation the compiler returns no semicolon before a `}`, so a block with a one-line body needs an
  explicit `;` there; the scanner follows that.
- The compiler parser sets a keyword flag before the keyword of an `if`, `static_if`, `for`, `while`, or `with`
  statement and clears it when the statement ends or a one-line body starts. The zero-width `_keyword_start` sets the
  flag and `_keyword_end` clears it. The scanner returns `_keyword_end` after the line breaks that the flag skipped, and
  not before `else`, `elif`, `static_elif`, `finally`, or a `{` where the parse state takes a block.
- An `if` statement keeps the flag until its first one-line body. `braced_if_then_else` is the statement whose bodies
  all have braces, and `_keyword_end` follows it; `short_if_then_else` takes `_keyword_end` before its first one-line
  body, and `plain_elif` and `_plain_else` follow after that body. All of them appear as `if_then_else`.
- The compiler parser opens a statement level right after `finally`, so a line break between `finally` and its `{` is
  an error even after an `if` or inside `(`. The scanner returns `_finally` and keeps that in `after_finally`.
- `_operand` is an expression that can be the operand of an operator; `_expression_no_bracket` adds the interval and
  the tuple pair, which the compiler's precedence makes non-associative; `_expression` adds the table constructor and
  the comprehension in braces, which cannot start a statement. Keep each rule at the level where the compiler parser
  takes it.
- The precedence of a step governs the parse position right after it, and the last step of a `prec` region takes the
  outer precedence when more steps follow. The compiler parser shifts a token at the precedence of the token, so a rule
  whose token has a precedence other than the rule gives that precedence to the steps before the token: the `?` of
  `?as` shifts at the precedence of `? :`, and a postfix `++` or `--` at the precedence of the unary operators.
- The body after `=>` is an `_operand` at the lowest precedence, because the compiler parser reads every `=>` body up to
  the first token below its precedence.
- The compiler parser recovers from a `.` that no name follows through an error rule that reports nothing, and builds a
  field with an empty name. `field` takes that form with `_dot_without_name`, whose low precedence keeps a name after
  the `.` in the field.
- The compiler lexer reads `include`, skips blanks and line breaks, and replaces the directive and the next run of
  non-blank characters with the text of that file; at the end of the text, the directive has no file name. The
  scanner returns that span as the extra `include_directive`, and the grammar reads the text around it as if the
  included text were empty. A word that only starts with `include` is a name.
- An internal token that matches the same text as `identifier`, such as a bare `include`, stops keyword extraction in
  every state where both are valid: the lexer then reads a keyword as the start of a longer name, so `deffoo` reads
  as `def foo`. The scanner reads such a token. The `:error` corpus tests with a keyword joined to a name fail when this
  happens.
- The compiler lexer reads `%name!`, the text, and the first `%%` after it as one token, and parses the rewrite that the
  macro returns in its place. The language reference requires one parenthesized expression as the rewrite, so
  `inline_reader` is one token where an operand can stand.
- The compiler takes one annotation list before a declaration.
- A node kind has the same fields wherever it appears. `structure_type` in `make_struct` is an alias of
  `_name_in_namespace`, so that the parser does not choose between a call and a constructor at the name, and the
  `structure_type` rule has no field either.
- The generator copies the fields of an aliased hidden rule into the parent node. A rule that has fields and appears
  under an alias gets a visible name, as `untyped_argument`, `single_let`, and `braced_if_then_else` do.
- Keep the `externals` array in `grammar.js` and `enum TokenType` in `src/scanner.c` in the same order.
- The scanner returns each external token where the compiler lexer would return it, whether or not the parse state
  accepts it. The internal lexer of tree-sitter considers only the tokens of the parse state, so a token that the
  compiler lexer reads with lookahead or by the longest match goes into the scanner.
- `_error_sentinel` is the last external token and no rule uses it. It is valid only in error recovery, where the
  scanner returns no zero-width token.
- The runtime restores the scanner state from the last external token, so a scan that changes the state returns a token.
- `KEYWORDS` in `grammar.js` holds every word of a `<normal>"word"` rule in `ds2_lexer.lpp` except `finally`, which the
  scanner reads, and `include`, which `include_directive` takes. The `reserved` set makes each of them a non-name. A
  reserved word must be a token of a rule.
- `module_path` takes each `.` and `/` after a name, as the compiler does: a require guard ends only before a word, so
  `require ?guard ./mod` is an error.
- In `queries/highlights.scm`, a later pattern overrides an earlier one in both tree-sitter-highlight and Neovim. Put a
  specific pattern after the general pattern, and give each pattern one capture: tree-sitter-highlight drops every
  capture of an earlier match that shares a node with a later match.
- A `#match?` regex must mean the same in Rust regex syntax and in Vim very-magic syntax (Neovim). Write a literal `@`
  as `[@]` and a literal `~` as `[~]`.
- `queries/highlights.scm` uses only the `#match?`, `#not-match?`, `#eq?`, and `#not-eq?` predicates. The daslang
  editor runs the file through `modules/dasTreeSitter`, which drops each pattern that uses another predicate or a
  directive.
- The daslang editor ranks overlapping captures by pattern order, not by nesting: inside the range of an earlier
  capture, a later pattern's capture wins. Put a pattern that captures a node holding other captured nodes, such as a
  string with interpolations, before the patterns of what it holds.
- Each capture name in `queries/highlights.scm` starts with `comment`, `string`, `character`, `number`, `boolean`,
  `keyword`, `type`, `function`, `variable`, `property`, `constant`, `label`, `attribute`, `module`, `operator`,
  `punctuation`, or `none`. The daslang editor colors a capture by the start of its name and gives no color to
  another name.
- Every `.md` file other than `README.md` is ASCII only, and no link names an anchor of another `.md` file: the
  daslang repository checks the `.md` files in its tree.
- The daslang build writes `daslang.dylib`, `daslang.so`, or `daslang.dll` into this directory. Keep the `*.dylib`,
  `*.so`, and `*.dll` entries of `.gitignore`.
- `.lint_config` sets `format_enabled = false` for the daslang formatter, which checks every `.das` file in the
  daslang repository. The assertions in `test/highlight/` and `test/tags/` point at exact columns, so the formatter
  must not touch these files.
- In `queries/tags.scm`, tree-sitter-tags keeps one tag per name node, from the earliest pattern that matches it. Put
  a specific pattern before the general pattern for the same node.
- A workflow pins each action to the commit SHA of a release and names the release in a comment
  (`actions/checkout@<sha> # v7.0.1`). A checkout sets `persist-credentials: false`.
- A change reaches `main` through a pull request, merged by squash or rebase after the `ci-ok` job of
  `.github/workflows/ci.yml` passes; the `protect-main` ruleset rejects a direct push. Add each new CI job to the
  `needs` list of `ci-ok`.

## Releasing

- The version is `X.Y.P`. `X.Y` is always the major and minor number of the newest daslang version that the grammar
  covers. `P` is the grammar's own number: it counts the grammar releases for that daslang version and does not follow
  the patch number of the compiler. The `Versioning` section of `README.md` gives the users the meaning of the three
  numbers; change it with this rule.
- To release, set the version with `tree-sitter version X.Y.P`, then run `tree-sitter generate`, because
  `src/parser.c` holds the version too, and update the lockfiles with `cargo update --workspace --offline` and
  `npm install --package-lock-only --ignore-scripts`. Commit, and push the tag `vX.Y.P`.
- The Go module path has no `/vX` suffix while `X` is 0 or 1. From major version 2, the path ends in `/vX`, as Go
  requires; change it in `go.mod` and in the import of `bindings/go/binding_test.go`.
- The tag starts `.github/workflows/publish.yml`. It checks that the tag matches `tree-sitter.json` and the Go module
  path, creates the GitHub release with attested artifacts, and publishes to crates.io, PyPI, and npm. A registry that
  already has the version is skipped.
- A manual run of `publish.yml` rehearses a release: it runs the checks and builds every artifact, and publishes
  nothing. Run it after a change to the workflow.
- The `protect-release-tags` ruleset forbids moving or deleting a `v*` tag. When a publish job fails for a reason
  outside the repository, run the failed jobs again; when the fix is a commit, release the next version.
- The three registry environments `crates`, `pypi`, and `npm` accept deployments only from `v*` tags.

## Verification

- After a change to `grammar.js` or `src/scanner.c`, run `tree-sitter generate` and `tree-sitter test` in this
  directory, and commit the regenerated `src/` files with the change. After a change to `grammar.js`, also run
  `npm run lint`. After a change to `src/scanner.c`, also run `tree-sitter fuzz`, which compares incremental parses
  with fresh parses.
- After a change to `queries/highlights.scm` or `queries/tags.scm`, run `tree-sitter test`, which runs the assertions
  in `test/highlight/` and `test/tags/`.
- After a change to `grammar.js` or `src/scanner.c`, also parse every gen2 file of a daScript checkout. Run these
  commands from the root of the checkout, with `GRAMMAR` set to the directory of this grammar:

  ```sh
  bom=$(printf '\357\273\277')
  git ls-files '*.das' | while read -r f; do
    grep -m1 -E "^($bom)?[[:space:]]*options[[:space:]]+gen2([^_[:alnum:]]|\$)" "$f" |
      grep -qE 'gen2[[:space:]=]*false' || echo "$f"
  done > /tmp/das-files.txt
  tree-sitter parse --grammar-path "$GRAMMAR" --paths /tmp/das-files.txt --quiet --stat
  ```

  Only the files that the Reference section lists may fail.
- With a Dagor checkout, also parse its gen2 files. Run these commands with `sh` from the root of the checkout, with
  `GRAMMAR` set as above and `GEN2_DIRS` set to the directories, separated by spaces, that the internal gen2 hosts of
  the Reference section load:

  ```sh
  bom=$(printf '\357\273\277')
  git ls-files '*.das' | while read -r f; do
    m=$(grep -m1 -E "^($bom)?[[:space:]]*options[[:space:]]+gen2([^_[:alnum:]]|\$)" "$f")
    if [ -n "$m" ]; then
      printf '%s\n' "$m" | grep -qE 'gen2[[:space:]=]*false' || echo "$f"
    else
      for d in prog/1stPartyLibs/daScript $GEN2_DIRS; do
        case "$f" in "$d"/*) echo "$f"; break ;; esac
      done
    fi
  done > /tmp/dagor-das-files.txt
  tree-sitter parse --grammar-path "$GRAMMAR" --paths /tmp/dagor-das-files.txt --quiet --stat
  ```

  Only the files that both compiler releases reject for a syntax error may fail. To get the verdict of the Dagor
  release, build the compiler of a daScript checkout with the generated parser files of the copy
  (`prog/1stPartyLibs/daScript/src/parser/ds2_parser.cpp`, `ds2_parser.hpp`, `ds2_lexer.cpp`) in place of its own;
  the Dagor copy has no CMake build.
- Check each new corpus case with the compiler before you add it: a case without `:error` parses without a syntax
  error, and a `:error` case has one. Build the compiler from a daScript checkout with
  `cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DDAS_FLEX_BISON_DISABLED=ON` and
  `cmake --build build --target daslang`, which writes `bin/daslang`, and run `bin/daslang -compile-only file.das`.
  `DAS_FLEX_BISON_DISABLED` keeps the generated parser files of the checkout; without it, the build rewrites tracked
  files.
- Write the expected tree of a new corpus test with its field names. `tree-sitter test --update` writes a new tree
  without them, and the test then ignores the fields.
- The CLI caches one compiled parser per grammar name. After you parse with another grammar named `daslang`, pass
  `--rebuild` to `tree-sitter test`.
- After a change to `tree-sitter.json`, `bindings/`, or a package manifest, build and test each binding in a copy of
  this directory, so that no build output lands in the tree: `cargo test`, `make && make test`, `go test ./...`,
  `npm install && npm test`, `swift test`, and `python -m unittest discover -s bindings/python/tests` after
  `pip install -e ".[core]"`.
