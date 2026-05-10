---
slug: daslib-json-missing-key-returns-jv-null-not-null-pointer
title: In daslib/json, what does `parsed?["MISSING_KEY"]` return when the key is absent — null pointer or `JV(null)`?
created: 2026-05-10
last_verified: 2026-05-10
links: []
---

**Returns a non-null `JsonValue?` wrapping the JV-null variant** (i.e. the literal JSON `null`), NOT a literal-null pointer. So:

```das
let v = parsed?["missing_key"]
if (v == null) {
    // NEVER taken when key is absent — `v` is a real pointer to JV(null)
}
write_json(v)   // prints "null"
```

This is intentional — it lets you chain `?[]` deeply without each link having to guard for null:

```das
let leaf = parsed?["a"]?["b"]?["c"]?["d"]   // safe even if any link is missing
```

But it bites when you want to assert "key X is absent." Three correct patterns:

1. **Check a known sub-field that only exists on real entries:**
   ```das
   let kind = from_JV(snap?["globals"]?[ident]?["kind"], type<string>, "")
   return !empty(kind)
   ```

2. **Test the variant tag:**
   ```das
   let v = parsed?[key]
   return v != null && !(v.value is _null)
   ```
   (Note `v.value` requires the variant access to compile; `v` is `JsonValue?`.)

3. **Use `from_JV` with a sentinel default:**
   ```das
   let n = from_JV(parsed?[key], type<int>, INT_MIN)
   if (n == INT_MIN) { /* absent or wrong type */ }
   ```

**Found 2026-05-09**, dasImgui playwright harness — `find_widget(snap, "SPEED.PUBLIC") == null` failed even though no such key existed; `widget_exists` helper now checks for `kind` field presence instead. See `modules/dasImgui/tests/integration/live_driver.das`.

## Questions
- In daslib/json, what does `parsed?["MISSING_KEY"]` return when the key is absent — null pointer or `JV(null)`?
