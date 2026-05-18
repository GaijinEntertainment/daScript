---
slug: das-namespace-abstraction-std-vs-eastl-swap
title: how does daslang das:: namespace abstraction work — what does das::vector resolve to and how does the eastl swap happen
created: 2026-05-18
last_verified: 2026-05-18
links: []
---

`das::vector`, `das::pair`, `das::move`, `das::hash`, `das::equal_to`, etc. are NOT defined as `using std::X` declarations — they're injected wholesale by a single `using namespace std;` or `using namespace eastl;` directive inside `namespace das { ... }`.

**Default config** (`include/daScript/das_config.h:34`):
```cpp
namespace das {using namespace std;}
```

**EASTL config** (`cmake/das_config_eastl/das_config.h:45-56`):
```cpp
namespace das {
using namespace eastl;
using std::atomic;
using std::condition_variable;
using std::lock_guard;
using std::mutex;
using std::nullptr_t;
using std::recursive_mutex;
using std::stringstream;
using std::thread;
using std::unique_lock;
} // namespace das
```

EASTL config does `using namespace eastl;` THEN explicit `using std::X` for the handful of things EASTL doesn't have (thread primitives, atomic, stringstream).

**Activated by** `-DDAS_CONFIG_INCLUDE_DIR=cmake/das_config_eastl` at CMake time. CI workflow: `.github/workflows/build_eastl.yml`.

**Implications when writing code that ends up in `namespace das`:**

1. Always write `das::vector` not `std::vector`. The EASTL build will route to `eastl::vector` automatically. Writing `std::vector` actively bypasses the abstraction.

2. Don't add `using std::vector;` blocks inside `namespace das { ... }` — they'll shadow `eastl::vector` in the EASTL build, defeating the abstraction. The das_hash_map.h cleanup in PR #2716 was exactly this pattern.

3. Things EASTL doesn't have (`out_of_range`, `exception`, `runtime_error`, `<stdexcept>` in general) must use `std::` directly — there's no `eastl::out_of_range`. EASTL's own headers throw `std::out_of_range` (see `eastl/include/EASTL/bitset.h`, `map.h`, `array.h`).

4. **Type traits, iterators, containers, allocators**: all dual-namespace, use `das::`.
   **Exception types, fmt library, threading from std**: use `std::` directly.

To check what's available where: `grep -rn "das::X\|eastl::X" /Users/borisbatkin/Work/daScript/include/ 2>/dev/null | head` shows the convention in practice.

## Questions
- how does daslang das:: namespace abstraction work — what does das::vector resolve to and how does the eastl swap happen
