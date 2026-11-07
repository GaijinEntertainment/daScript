# dasLLAMA Architecture - the encoder towers' served GEMM widths

Companion to `ARCHITECTURE_MEDIA.md`; a section is cited by its anchor. This document carries the
widths a tower's GEMMs are served at (`ARCHITECTURE_MEDIA_TOWER.md#tower-padded-widths`).

### Towers serve padded GEMM widths {#tower-padded-widths}

A tower's served GEMM widths round UP to 64 - the Metal mm tile - while the tensor-shape verify
keeps the file's own widths. The gated FFN pair pads with zero weight ROWS and the down GEMM with
zero tail COLUMNS; a patch conv pads its K through the im2col row stride, whose tail lanes are the
resize's zeros. Every pad lane contributes a zero product, so a padded GEMM's output is
bit-identical to the unpadded one on both the CPU and the Metal route, and one served layout feeds
both. `q25v_patch_pad` / `gemma3v_patch_pad` and the `ff_pad` fields are the served widths;
`im2col_rgb_patches` takes the padded stride as `row_stride`.

The qwen25v tower stages every GEMM f32-in-blob at those widths whatever the file's element type -
blocks and merger alike - because the Metal tower reads f32 planes or the baked halfword twin.
