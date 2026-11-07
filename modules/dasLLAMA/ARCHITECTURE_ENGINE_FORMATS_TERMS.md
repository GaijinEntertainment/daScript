# dasLLAMA Architecture - the weight-format terms

Companion to `ARCHITECTURE_ENGINE_FORMATS.md`; a section is cited by its anchor. This document
defines the terms the weight-format rules use: a site, a lever and a converted copy
(`#kq-format-terms`), and the served form, a second member over the same disk type, with its case
`iq3s4` (`#served-form`).

### The weight-format terms {#kq-format-terms}

**A site** is a weight plane that a backend's serving call reads. The term covers the CPU and
every GPU backend: one plane is one site, and each backend whose serving call reaches it reads it
in some form.

**A lever** is an env knob or a setter that changes which form a site reads. A lever at its
default leaves the site on its default form; `DASLLAMA_METAL_DEVW` and `set_metal_devw_panels`,
which make the Metal dense prefill sites read stored f16 panels, are levers.

**A converted copy** is another form of a site's weights whose bytes outlive the kernel call or
the window that made them, or reach the `.dlim` image. Two cases are not converted copies:

- **Working memory.** A form made from the site's own bytes inside one kernel call or one window,
  and dropped or overwritten by the next one, is the kernel's working memory - the Metal prefill's
  per-window scratch panel is one. A STORED panel, baked into an image or kept across windows, is a
  converted copy.
- **A served form.** A value-exact re-encoding of the disk values with no wider element type is a
  served form, not a converted copy: a second `KqFmt` member over the same disk type
  (`ARCHITECTURE_ENGINE_FORMATS_TERMS.md#served-form`), or a carrier's float planes served as
  their own disk type (GigaAM's F16 halfwords).

So a lasting form that widens the element type or changes the values is a converted copy even
when its values come close: the q8 transcode of an F16 classifier (`cls_q8_copy`), a Q8_0
requantization, an f32 widening.

### A served form is a second member over the same disk type {#served-form}

`iq3s4` is IQ3_S served as a 16-entry codebook plane: every IQ3_S weight is a signed odd
magnitude 1..15 times its block scale, so the grid entry, the `qh` ninth bit and the sign fold
at transcode into one nibble index (`iq3s4_lut`, `-15, -13, .., 15`) laid in the k4 tiling, and
the scale row is iq3s's own. The dequant is value-exact against the grid form; the planes cost
17% more bytes (128 against 104 a superblock) and decode through the iq4xs codebook kernels -
one table lookup a nibble in place of the grid gather, which is where the IQ3_S CPU decode spent
its time. The member's descriptor row names the source type (`GGML_TYPE_IQ3_S`, 110 disk bytes)
with its own schema and stream ids, so the image and the stream rail carry it like any lattice
member. The choice is `kq_fmt_of`'s at the load (`iq3s_serve_lut`, the `DASLLAMA_IQ3S_SERVE`
knob or a process pin, `set_iq3s_serve_pin`: `grid`, `lut`, `auto` = lut on the CPU rails and the
Vulkan tier, the grid under the Metal tier), and it is an image identity bit
(`DlimCpuConfig.iq3s_lut`), because it changes plane bytes. Both GPU tiers decode it on every
ladder, the codebook arithmetic (`2q - 15`, no table bound) and the stamps the iq4xs kernels with
the odd codebook switched in: on Vulkan the device row is iq4xs's quant plane under iq3s's scale
word; on Metal the blob's split scale plane carries the eight `1 + 2s` strip bytes and the f16 d
exactly as iq4xs's carries its signed sub-scales (`ARCHITECTURE_GPU_QUANT_PLANES.md#metal-kq-split-scale-plane`),
so every iq4 family stamp under `IQODD` reads it - the GEMV, the small-batch forms, the base and
tensor tiles, the routed GEMV and tiles, the token gather. So a host that sums routed experts
beside a Vulkan device reads the fast planes, and the device reads the same bytes; under Metal
`auto` keeps the grid, which streams 17% fewer bytes a superblock.

The nv4 member - its planes' bytes and the per-tensor scale beside them - is `ARCHITECTURE_ENGINE_FORMATS_NV4.md`; the snapshot source and its affine members are `ARCHITECTURE_ENGINE_FORMATS_MLX.md`.
