# dasLLAMA Architecture - the Metal tower attention routes and encode chains

Companion to `ARCHITECTURE_GPU.md`; section numbers are `ARCHITECTURE.md`'s. This document
carries sections 2.2w-2.2x: the three routes that serve tower attention, and the one-command-buffer
encode chain each family the tower driver serves gets. The GPU backend role table these sections
build on - the tower driver's role row included - stays in `ARCHITECTURE_GPU.md` sec.1.5.

### 2.2w The tower attention routes {#tower-attn-routes}

A tower head width is padded to `hs_pad = max(64, ceil32(hs))` - 72 and 80 both land on 96 -
because the QK stamps walk multiples of 32 while the guarded AV forms carry no multiple-of-64
requirement. A route that uses the padded width pays a restride to and from the towers' compact
rows. Three routes serve tower attention:

- **The flash route** takes a head size of exactly 72 on a canvas whose row count divides 64.
  It runs one pass - no score slab, no rowstat, no head restrides - reading K and V as
  per-head-contiguous f16 panels (the panel and tail-slack contract sits on the kernel class)
  while Q and the output ride the compact f32 rows. `DASLLAMA_METAL_TOWER_FLASH=0` pins the
  slab trio.
- **The per-window route** serves the qwen2.5 ViT's block-diagonal layers: one threadgroup per
  (window, head), one thread per query row, windows capped at 64 rows. It reads q, k and v
  compact and straight from device in f32 - a window's K and V sit in L1, and f16 staging noise
  would compound over the tower's 28 layers. Layers that attend in full restride onto the
  padded slab and take the shared trio.
- **The slab trio** - the prefill driver's QK, rowstat and AV builders - serves everything else
  and sizes its score slab at `heads x mp x nk64` halfs. Off the flash route that slab is the
  encode's only attention allocation; on it the driver takes a stub.

### 2.2x The tower driver's encode chains {#tower-encode-chains}

Each family the tower driver serves gets one chain that encodes the whole encode into ONE
command buffer and reads back once: the block loop, and - for gemma4a and qwen3a - the mel and
conv front ahead of it. A chain is dispatch for dispatch the same graph as its family's CPU
encoder loop, in the same order and at the same operand shapes, so the CPU loop is the chain's
specification and the CPU-vs-GPU transcript cells are its parity instrument. Every chain is
best-effort: it answers false (or -1) on any shape, knob, quant-mode or device decline, and the
CPU chain serves that encode. Engage is read from counter deltas (`metal_tower_stats`,
`metal_tower_f16_encodes`), never from "the model ran".
