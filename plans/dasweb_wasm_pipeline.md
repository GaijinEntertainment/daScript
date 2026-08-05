# dasweb phase 3 — the wasm build pipeline

Detailed plan for phase 3 of `plans/dasweb_backend.md` (which carries the arc contour and the
architecture invariants all phases obey). Written 2026-08-04, after phases 1+2 shipped and the
security review round (#3625) landed.

Split, per Boris: **3a machinery first, 3b UX second.**

## State when written

- `dasweb-playground` is live on daslang.io: `/s/<hash>` share links, curated dropdown fed from
  the store, the "jit" radio permanently off since phase 2. Web box `dasweb-1`.
- Compute box `zen4` (65.108.238.44, Debian 12, Ryzen 8700GE 8c/16t, 61 GB RAM) is released to
  this arc. `~/SETUP.md` on the box is the contract: read first, extend, never stomp — it also
  documents the dasLLAMA profiling rig that still lives there.
- zen4 starting state, probed 2026-08-04: **no emsdk, no emcc, no podman/nsjail/bwrap**, and
  **52 GB free of 437 GB (88% used)** — the model catalog owns ~300 GB. Worktree
  `~/daScript-dasweb` exists and tracks master at `ebb422779`.
- CI (`pages.yml`) builds every wasm artifact today. The steps this phase touches are named in
  the diet section below.

**Disk fits — measured, not estimated.** A daslang build tree on that box is **537 MB**
(`~/daScript/build`) to **854 MB** (`~/daScript-dasweb/build`), not the many-GB I first guessed.
So the toolchain footprint is roughly emsdk 1.5–4 GB + a second (libc++/exceptions) host build
~1 GB + `web/build64` and the wasm64 module archives a few GB — call it **5–8 GB against 52 GB
free**. Nothing needs pruning to start.

The only unbounded part is the **artifact cache**, and it is bounded by policy, not by hope: a
hard size cap with LRU eviction, curated entries pinned. Set the cap when the cache lands.

If space is ever wanted, the obvious lever is already on the box: `~/models` is 333 GB, of which
**171 GB is 18 `.dlim` images** — derived artifacts, regenerable from their GGUF sources by the
conversion tool. That is Boris's call and it is not needed now; it is recorded here so the option
is known rather than rediscovered under pressure.

---

## Trust model (unchanged from the arc plan, restated because everything here depends on it)

Compiling user `.das` is remote code execution by design — macros run at compile time. So:

- The **compute box pulls**; the web box never connects to it. Box death degrades to a backed-up
  queue, never to a site outage.
- Every build runs in a throwaway sandbox: no network, CPU/memory/wall-clock caps, tmpfs scratch.
- Artifacts are content-addressed by (source hash × toolchain_id) and served as static files.
  Sources stay precious (SQLite, litestream); artifacts stay a regenerable cache.

## 3a — machinery

### Queue

A new table in `samples.db` (migration v4 — the migration stream is append-only):

```
BuildJob: Hash, ToolchainId, State (queued|building|done|failed), Attempts,
          QueuedAt, StartedAt, FinishedAt, Error, ArtifactPath
```

Primary key `(Hash, ToolchainId)` — a job is a (source, toolchain) pair, so a toolchain bump
re-enqueues lazily rather than invalidating anything eagerly.

### Endpoints — and how they differ from the operator surface

The builder is remote, so these **cannot** use the loopback gate that #3625 put on `/shutdown`
and `/admin/*`. They are public routes, authenticated by a bearer token from the toml:

```
GET  /api/build/next     → claim one queued job (bearer)   → {hash, source, toolchain_id}
POST /api/build/result   → upload artifact or error (bearer)
```

Rules that fall out of the review round and must hold here:

- The token is compared in **constant time**, and never appears in a log line, a response body,
  or an error message — the config banner logs its key name and provenance, never its value.
- A claim mutates state, so it is a `POST`-shaped operation semantically; keep `GET` only if the
  claim is idempotent per-worker, otherwise make it `POST`.
- `caddy.snippet` gains these routes explicitly, with a `request_body max_size` sized for an
  artifact upload (MB-scale, unlike the 1 MB source cap) — the service still cannot bound a body
  it has already buffered.
- Artifact uploads are written to a temp path and renamed into place only after the hash checks
  out; a failed upload must never leave a half-written artifact being served.

### Builder service (`utils/dasweb-buildd`, on zen4)

Same service anatomy as `dasweb-playground` — clargs + toml, `logger_init_tee`, watchdog
contract (`/healthz`, `POST /shutdown`, exit 0/4), systemd wrapping the watchdog, module-global
lifecycle state. Its own `CODEREVIEW.md` from the get-go, same as the playground service got.

It polls the queue, and for each claimed job runs one build in a sandbox. **Sandbox choice is
open**: `bwrap` (bubblewrap) is the lightest and is a single apt package; rootless `podman` is
heavier but gives cgroup limits and image pinning for free; `nsjail` needs building. Decide when
implementing, on the box — the requirement is no network, a tmpfs scratch dir, a memory cap, and
a wall-clock kill.

### Toolchain — `build_wasm_host.sh` (checked in, extracted from `pages.yml`)

The cross-compile host is the load-bearing part, and its recipe is **not** free-form. Verbatim
from `pages.yml`'s "Build: Daslang host" step, which is the authority:

```
cmake --no-warn-unused-cli -B./build \
  -DCMAKE_BUILD_TYPE:STRING=Release \
  -DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang++ \
  -DCMAKE_C_FLAGS="-DDAS_ENABLE_EXCEPTIONS=1" \
  -DCMAKE_CXX_FLAGS="-stdlib=libc++ -DDAS_ENABLE_EXCEPTIONS=1 -D_LIBCPP_DISABLE_DEPRECATION_WARNINGS" \
  -DCMAKE_EXE_LINKER_FLAGS="-stdlib=libc++" -DCMAKE_SHARED_LINKER_FLAGS="-stdlib=libc++" \
  -DDAS_HV_DISABLED=OFF -DDAS_PUGIXML_DISABLED=OFF -DDAS_LLVM_DISABLED=OFF -DDAS_GLFW_DISABLED=OFF \
  -G Ninja
```

Why each piece matters: the host JIT-codegens the sample and **bakes C++ handled-type layouts
from its own ABI**. The emscripten target is libc++ with C++ exceptions, so a default
libstdc++/setjmp host bakes wrong offsets (std::string 32 vs 24 bytes, mutex 80 vs 40) and the
wasm heap corrupts. Host stdlib/exception config must equal target, and host pointer width must
equal target width (wasm64 ⇒ 64-bit host).

Consequences for zen4 specifically:

- zen4's **profiling daslang (`~/zen4_build.sh`, native clang-19) can never serve as the wasm
  host** — different stdlib/exception config. The wasm worktree gets its own host build. Two
  daslang builds coexist on the box; `SETUP.md` must say which is which.
- `web/CMakeLists.txt` **warns rather than fails** when the host compiler is not clang.
  `build_wasm_host.sh` hard-requires clang so that degradation cannot happen quietly.
- emsdk pins to the same version `pages.yml` pins (**5.0.7** at time of writing). The pin history
  in that file documents why: emsdk LLVM snapshots repeatedly broke `ds_parser.cpp`, intermittently
  enough to survive a first green run.

**Validated on zen4, 2026-08-04** — `web/build_wasm_host.sh` is checked in and was run there for
real, not just written: 381/381 targets, `bin/daslang` links `libc++.so.1` + `libc++abi.so.1`
(the load-bearing property), runs, and the whole tree cost ~1 GB (52 GB free, unchanged). The
non-clang gate was exercised too: `HOST_CXX=g++` is refused, so `web/CMakeLists.txt`'s
warn-only degradation cannot happen quietly. zen4 needed `libc++-19-dev libc++abi-19-dev`
installed, and `/usr/bin/clang` there is clang-14 (cannot build this tree) — pass
`HOST_CC=clang-19 HOST_CXX=clang++-19`.

**dasLLVM is present and reachable** from that host: `require llvm/daslib/llvm_jit` and
`require llvm/daslib/llvm_exe` (where `--jit-check-abi` lives) both resolve and run. Note the
mount path — it is `llvm/daslib/...`, not `daslib/...`; the latter fails with
`error[20605] missing prerequisite`, which looks exactly like a missing module and is not one.

**The wasm build MUST have its own worktree. This is a hard requirement, not hygiene** — it was
demonstrated the expensive way while deploying #3625:

- **Every build of this tree writes to the same tree-level `bin/` and `lib/`**, whatever build
  directory it was configured in. So the emscripten build and the native build fight over one
  output tree.
- The wasm host build leaves `bin/daslang` linked against **libc++**. The web box has no libc++,
  so a release bundle baked with that host would not run there — and nothing about the bake says
  so. (`build_wasm_host.sh` now warns when the binary it produced links libstdc++, i.e. the
  reverse mistake.)
- Worse, an emscripten build **replaces static archives in `lib/` with WebAssembly ones**. After
  one interrupted `daspkg build --wasm`, 8 of 23 archives in `lib/` held wasm objects — including
  `liblibUriParser.a`, which links into daslang itself. The native rebuild then died on
  `error adding symbols: file format not recognized`, and `file(1)` on the archive says
  "current ar archive" (it reports the container, not the members), so the usual sanity check
  looks clean. Only `ar x` + `file` on a member reveals it.
- Recovery is `rm -f lib/*.a && rm -rf bin build web/build64` plus a full native rebuild. Budget
  for it if the two ever share a tree again — which they should not.

Still unproven end-to-end: an actual `daspkg build --wasm` run, which needs emsdk 5.0.7 (not yet
installed). The host half is validated; the emscripten half is not.

`web/build64` is configured as CI does it:
`DAS_WASM_MEMORY64=ON`, `DAS_WASM_PTHREADS=ON`, `-sMEMORY64=1` in C and CXX flags, and
`DAS_HOST_DASLANG_OVERRIDE` pointing at the host build.

`toolchain_id` = the wasm worktree's git sha. A bump changes the id, which lazily invalidates the
artifact cache by construction. Version skew mid-bump is fine: each `release wasm` artifact
bundles its own runtime and runs standalone.

### ABI canary — gate on every toolchain bump

`--jit-check-abi` (`modules/dasLLVM/daslib/llvm_exe.das`) bakes host size and field offsets for
every handled type into the minted binary, resolves the target's at runtime, records divergences,
dumps them all and aborts. Protocol: **a new `toolchain_id` goes live only after a canary sample
built with `--jit-check-abi` runs clean**, and the result — including the full divergence dump on
failure — goes in the log. Parity gate before first serve: build a curated sample on zen4 and
diff behavior against the CI-built artifact of the same (source, toolchain) pair.

### Artifacts

`blobs/<toolchain_id>/<hash>.wasm[.br]` on the web box (the `blobs/` dir phase 1 left empty),
served static. LRU eviction with curated entries pinned; a warmup job re-enqueues the curated set
after a toolchain bump.

## 3b — UX

- "building… (queue position N)" state in the playground; a failed build shows the compiler error
  text (it is the user's own code — showing it is the point).
- Rename **jit → wasm** everywhere user-visible. The radio has been off since phase 2; phase 3 is
  when it comes back meaning what it says.
- Restore the engine-transition playwright tests that phase 2 rewrote to pin the radio off.

## pages.yml diet (folded into this phase per Boris — CI makes every PR cost an hour)

Mapped onto the actual steps in `.github/workflows/pages.yml`:

| Step | Fate |
|---|---|
| "Build: Daslang host" | **Stays.** das2rst needs it, and it is the recipe `build_wasm_host.sh` mirrors. |
| 1. wasm32 threaded `daslang_static` + playground staging | **Stays, untouched.** This is the toolchain, not a sample — the interpreter runtime the playground runs on. (Boris: "amazing it actually works".) |
| 2. `web/build64` configure + `daspkg build --wasm` archives | **Moves** to the builder box; it exists in CI only to serve steps 3–7. |
| 3. `cmake --build web/build64 --target all_wasm` (per-sample compute wasm64) | **DELETE.** Orphaned since phase 2 turned the radio off; the queue replaces it. |
| 4–7. arcanoid, pacman, furier, path_tracer_lab, physarum_lab | **Fold** into the content-addressed cache rail. They change rarely, so the cache near-always hits and CI stops rebuilding them. |
| "Verify wasm example artifacts" | Follows steps 4–7 wherever they land — it exists because those builds are non-fatal, and that property must survive the move. |

**Design point the game pages force — and it is NOT about multi-file sources.** Two axes that
look like one and are not:

- **Input multiplicity is already solved, and is not what shapes the artifact.** A multi-file
  sample (curated or user-authored) is stored as one `{files, active}` JSON document under one
  hash — phase 1 does this already — and compiles to **one** wasm module. `arcanoid` is the
  proof: its source dir holds `main.das` + `live_stub.das`, and `daspkg release wasm --root`
  emits a single `arcanoid.wasm`. So N sources in never means N artifacts out.
- **Output multiplicity is about delivery mode.** A playground sample is a bare `.wasm` that the
  already-loaded `daslang_static` runtime instantiates — one file. A game is a **standalone
  page**: its own `html` + `js` + `wasm`, staged as a triple, because the browser fetches the js
  and the js fetches the wasm by name. That, and only that, is what forces the cache to hold
  file sets.

⇒ The cache **key** needs nothing new (the source hash already covers a bundle). The cache
**value** needs to hold a file set only if the games fold in — which is exactly open question 1.
If the answer is "games stay in CI", a single-`.wasm` value is sufficient and simpler.

**Separate gap this exposes, worth fixing regardless:** the old per-sample wasm path skipped
multi-file samples entirely — `deriveJitName` in `web/examples/ui/src/main.js` returns `null`
when `files.length !== 1`, and the engine radio was disabled for those samples. Since users can
author multi-file examples and share them, phase 3 should build them too. Nothing about the
compiler prevents it (the games prove a multi-file root compiles); it was a UI-side shortcut
tied to naming the artifact after a single source file. Content-addressing removes that
constraint — the artifact is named by hash, not by basename.

Keep a CI compile-gate job proving the curated set still builds (a test that publishes nothing) —
a broken sample should red CI, not the production queue.

## Checkpoints

- **3a:** a curated sample builds end-to-end through the real queue from a cold cache, on the
  real boxes, with the canary clean.
- **3b:** the user-visible flow with the building indicator, on the live server.

## Open questions for Boris

1. **Do the example games actually move off CI**, or is the fold a nice-to-have? Moving them
   makes the queue load-bearing for the `/examples` page; leaving them in CI keeps the page
   independent of box health at the cost of the CI time the diet was meant to reclaim. This is
   the one question that changes the shape of 3a — the artifact cache needs multi-file bundle
   support from day one if the answer is yes.
2. **Sandbox**: bwrap (lightest, one apt package) vs rootless podman (best limits) — no strong
   prior; whichever the box takes cleanly. Decidable while implementing.

Disk is **not** a blocker (measured above), and no model pruning is needed.

## Non-goals

- Nothing touches the playground sha256 **benchmark** — it stays non-conforming on purpose
  (dasProfile cross-language parity).
- No new limiter on build requests: the queue plus baking time is the natural throttle. The
  insert ceiling exists only because inserts are the cheap unqueued path.
