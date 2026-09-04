# src/ast - the front end's AST

Contract: `ARCHITECTURE_COMMON.md` (repo root). This document carries the mechanisms of this
folder that a rule cites. The folder's other subsystems - parsing, inference, simulation - are
described in `skills/internal/cpp_codebase_notes.md` (repo-only).

## 1. The module-cache read in `trySerializeProgramModule` (`ast_parse.cpp`)

The front end parses modules in require order. When a module cache is installed
(`ModuleFileCache`), each module first tries `trySerializeProgramModule`. The reader stands at
the next record and checks that the record names this file, that the file's mtime and size
match, and that every compile-time input the record's macros pinned
(`add_module_cache_dependency`) still has the same content. A match deserializes the module and
counts it as served. The first mismatch is the cutoff: the reader marks the stream failed, the
module and everything after it parse from source, and the writer rewrites the whole file - the
served records re-serialized from the modules the reader restored, then the freshly parsed
ones. A record whose header matched but whose payload fails to deserialize reparses in place,
without cutting the stream, whenever the record carries a usable payload length.

Every diagnostic the read prints - a record for another file, a changed file, a changed
dependency, a truncated or version-mismatched stream, a failed payload, a reparse in place - is
gated on the serializer's `quietCache`. The host sets that flag for the default cache, which is
on unasked for an ordinary run; ungated, each of those lines would be output every user sees on
an ordinary edit. An explicit `-module-cache` leaves the flag off, so those runs get the lines
together with the host's verdict.
