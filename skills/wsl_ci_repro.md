# Replicating CI failures in WSL

**Principle: replicate CI verbatim.** A failure that reproduces in CI but not on your dev box is almost always environment skew. Don't approximate it — match the CI runner exactly: same OS, same compiler, a **fresh checkout at the same ref**, the same build flags, the same install steps, the same test command. Open the failing job's `.github/workflows/*.yml` and follow it line for line. Anything you change from CI is a variable you'll have to rule out later.

## The WSL distro

`Ubuntu2404-CI` (WSL2) mirrors the Linux CI runner — clang 18.1.3, cmake/ninja, and the apt deps the workflow installs. Drive it from the (Git Bash) tool shell via `wsl -d Ubuntu2404-CI -- bash -lc '...'`. Use it for Linux-only failures: sanitizers (tsan/asan), POSIX-only test divergence, headless-GUI timing.

## Recipe (verbatim CI)

1. **Fresh `git clone` of every repo at the CI ref** — never rsync or copy your Windows working tree. Clone `GaijinEntertainment/daScript` (and any module repos) at the exact branch CI checks out (`ref: master`, the PR branch, etc.). Clone into the Linux filesystem (`~`), not `/mnt/...` (the 9p mount is slow and tempts you into copying the dirty tree).
2. **Build with CI's exact `cmake` invocation** — copy the flags, generator, and target list straight from the workflow's build step.
3. **Install / prepare exactly as CI does** — e.g. `daslang utils/daspkg/main.das -- install <pkg> --global` in the same dependency order.
4. **Run the exact CI test command**, including flags like `--headless`.

## Why "verbatim" — the traps a working-tree copy hits

Copying your Windows checkout instead of cloning drags in non-source state that breaks or silently skews the repro (all observed for real):

- **Windows build artifacts** — `build_msvc/`, `*.shared_module` dirs holding Windows `.dll`s, stale `CMakeCache.txt` — contaminate the Linux build. daspkg reports "Installed" but produces **no Linux `.so`**, so native modules fail to load with a misleading `error[20605]: missing prerequisite '<module>'`.
- **Local-dev junction symlinks** — `modules/<pkg> → /mnt/d/...` (the external-module junction trick) are self-referential. A recursive copy follows them into an infinite loop: a 470 MB tree ballooned a copy to 27 GB before it was killed.
- **A stale / behind daScript** — wrong branch, or N commits behind master — runs different interpreter / live-host / frame-timing code than CI. Fatal for timing-sensitive bugs (e.g. synth-input vs render-poll phase). A fresh clone at the CI ref is the only way to be sure; "it's probably close enough" is how you chase a ghost.

## WSL-from-Windows shell gotcha

The tool shell is Git Bash; WSL is reached as `wsl.exe -- bash -lc "..."`. A `$var` — **especially a loop variable** — can evaporate across the Git Bash → `wsl.exe` → `bash -lc` quoting layers and collapse a path. A real incident: `for repo in a b; do rsync --delete /src/$repo/ ~/$repo/; done` saw `$repo` come through empty, became `rsync --delete /src/ ~/`, and wiped a home directory. Use **explicit literal paths**, or write a script to a file and run `wsl bash script.sh`. Never run `--delete` / `rm -rf` with an interpolated path across that boundary.

## Don't run Windows and WSL suites concurrently if they bind a fixed port

When the thing under test is a **fixed-port live server** — e.g. `daslang-live`'s HTTP live-API on `DEFAULT_LIVE_PORT = 9090` (dasImgui/node-editor headless tests, any `live_*` harness) — you **cannot** run the same suite on Windows and in WSL at the same time. Windows and WSL2 do **not** have independent loopback: `localhostForwarding=true` (the `.wslconfig` default) mirrors a WSL listener onto Windows `localhost`, and Win11 mirrored-networking shares the namespace outright, so `127.0.0.1:9090` is **bridged across the two**. The second binder loses the socket, or a client's POST lands on the *other* OS's server.

The symptom is deceptive: **mass ~120s `timed out` failures landing on *different* tests each run** (whichever was mid-flight when the cross-talk hit) — it reads like a regression but is pure port contention. A timed-out run also leaves the server child holding the port. So:

- Run dual-platform verification **sequentially** — one platform fully finished before the other starts (this defeats the whole point of "run both at once to compare", but there's no way around the shared port).
- **Sweep stale servers between runs**: `Get-Process -Name daslang,daslang-live | Stop-Process -Force` on Windows; `pkill -9 -f daslang` in WSL.
- This is the same hazard the general "never run two shell commands in parallel" rule guards against — here the collision is cross-OS via shared loopback, which is easy to miss because the two feel like separate machines.
