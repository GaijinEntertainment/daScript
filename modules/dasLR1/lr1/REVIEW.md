# dasLR1 Engine Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`../ARCHITECTURE.md`.

**A diff that adds code to `json_schema.das` that is not part of its port of llama.cpp's
`json-schema-to-grammar` converter is a defect** - put it in a file with no raised limit
(`../ARCHITECTURE.md#ported-converter`).
