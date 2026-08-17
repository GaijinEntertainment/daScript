# Chunk 0: local Git through the terminal

This probe measures the cost of using the existing Windows terminal stack for
Git before dasHerd chooses a fast path or moves terminal ownership into a
detachable host.

It compares the same Git operation through three lanes:

1. direct process capture;
2. direct capture plus a raw log;
3. ConPTY plus terminal emulation plus a raw log.

Run the headless benchmark from the repository root:

```text
bin\Release\daslang.exe utils\dasHerd\chunk0\benchmark_git_terminal.das -- --warmup=5 --rounds=30
```

Run the visible terminal probe with the local dasImgui module:

```text
bin\Release\daslang-live.exe -load_module modules\dasLiveHost -load_module modules\dasImgui utils\dasHerd\chunk0\git_terminal_window.das
```

The visible probe starts maximized at 150% zoom, displays the invocation in a
dim `$ command` visual header followed by colored Git output, and writes the
process's raw byte stream under `logs/dasHerd/`. The header is terminal view
state, separate from emulator state and the raw log. Ctrl+mouse-wheel and the
bottom buttons change zoom in 5% steps. `imgui_snapshot` exposes the terminal's
`visual_header`, screen text, cursor, selection, scrolling, and rendering
metrics to live commands, so an agent can inspect the same terminal without
image recognition.

## First measurement

Measured 2026-07-20 on Windows 11 Pro 10.0.26200, an AMD Ryzen Threadripper
3990X, Git 2.17.1.windows.1, and Release daScript binaries. Each result has five
unrecorded warmups and 30 recorded samples.

The table reports launch-to-root-process-exit latency. This is the fair measure
of execution overhead; terminal `total` additionally includes the conservative
250 ms quiet period used to retain late ConHost output.

| command | direct p50 / p95 | direct + log p50 / p95 | terminal + log p50 / p95 | terminal p50 overhead |
| --- | ---: | ---: | ---: | ---: |
| `git --version` | 40.2 / 51.0 ms | 39.3 / 49.9 ms | 44.9 / 60.4 ms | +4.7 ms vs 40.2 ms, 1.12x |
| colored `git status --short --branch` | 71.5 / 79.0 ms | 71.3 / 85.5 ms | 87.2 / 92.0 ms | +15.7 ms, 1.22x |
| intentional missing ref | 42.6 / 46.4 ms | 42.9 / 50.5 ms | 58.5 / 59.9 ms | +15.9 ms, 1.37x |

Raw-log-only median differences were -0.9 ms, -0.2 ms, and +0.3 ms. At this
sample size that is scheduler noise, so logging does not justify a separate
fast path. ConPTY and terminal emulation have a real but modest cost for these
short commands. We should repeat this probe after detach/IPC exists before
deciding whether that additional boundary needs optimization.

Both successful commands returned zero through every lane. The missing-ref
command returned 128, and every lane contained the expected diagnostic. The
terminal logs also retained the forced ANSI color stream.
