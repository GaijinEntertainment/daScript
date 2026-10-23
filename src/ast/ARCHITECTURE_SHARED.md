# src/ast architecture notes - shared modules

Companion to `ARCHITECTURE.md` in this folder.

## Promotion of a shared dependency (`compileDaScript`, `ast_parse.cpp`) {#shared-promotion}

A module whose file declares `module X shared` is promoted to the process-global module list
the first time a compile requires it (`parseRequiredModules` with `promoteShared`,
`Module::promoteToBuiltin`), and every later compile in the process reuses that one body
through `Module::requireEx`, whatever the later compile's `CodeOfPolicies` say. The body is the
first requirer's: it was parsed, inferred and optimized under that compile's policies.

`compileDaScript` promotes a shared dependency only when the compile optimizes. Under
`policies.no_optimizations` the dependencies it compiles stay in its `ModuleGroup` and die with
it, because an unoptimized body is a different body - its SimNodes keep the `Ref2Value` the
optimizer's ref fold removes, so it hashes to no AOT stub (`error[50101]` on every function that
reaches it) and runs slower - and the first requirer of a process would otherwise decide that
body for every program after it. dastest's walker visits `tests/` in readdir order, so which
program requires a daslib module first is the filesystem's choice: a lint test that compiles a
fixture with optimizations off runs before `tests/linq` on APFS and after it on ext4 and NTFS.
A compile with optimizations off still reuses a module promoted earlier, and `requireModuleNow`
(`ARCHITECTURE.md#require-after-walk`) promotes its target whatever the policy, since a late
require asks for a process module by name. The MCP server, the LSP and the lint runner outside its
shared-modules mode set `ignore_shared_modules` beside `no_optimizations`, so they neither promote
nor reuse. `promote_shared_unoptimized` is the override: a compile under `no_optimizations` that sets it
promotes its shared dependencies as an optimizing compile would, for a process that compiles many
programs under one policy set and never serializes them - the lint's shared-modules mode, where
the unoptimized body serves every later file and the AOT-stub mismatch has nothing to reach. A
program the process compiles AFTER such a promotion reuses that unoptimized body, so the override
belongs to a process that runs nothing.
