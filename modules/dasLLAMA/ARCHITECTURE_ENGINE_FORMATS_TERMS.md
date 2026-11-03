# dasLLAMA Architecture - the weight-format terms

Companion to `ARCHITECTURE_ENGINE_FORMATS.md`; a section is cited by its anchor. This document
defines the terms the weight-format rules use: a site, a lever and a converted copy.

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
  served form, a second `KqFmt` member over the same disk type
  (`ARCHITECTURE_ENGINE_FORMATS.md#served-form`), not a converted copy.

So a lasting form that widens the element type or changes the values is a converted copy even
when its values come close: the q8 transcode of an F16 classifier (`cls_q8_copy`), a Q8_0
requantization, an f32 widening.
