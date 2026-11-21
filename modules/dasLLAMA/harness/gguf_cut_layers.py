#!/usr/bin/env python3
# Cut a GGUF decoder to its first N blocks: the metadata as the
# source holds it but the block count and the per-block lists
# (shortened to N), every kept tensor's bytes as the source holds
# them. A file whose block count includes NextN draft blocks
# (`<arch>.nextn_predict_layers`, the last blocks) keeps its first N
# trunk blocks and every draft block, renumbered to follow them.
# `--set <key>=<number>` writes one numeric key over the source's: a
# gemma cut lifts `gemma4.final_logit_softcapping`, which would pin
# most of a row whose logits the missing blocks never brought into
# range. A cut of a large model is the small file a
# per-PR test loads in its place (model_specs.das names each cut's
# source and N in its recipe). Needs llama.cpp's gguf-py on the path:
#
#   PYTHONPATH=<llama.cpp>/gguf-py python3 gguf_cut_layers.py <src.gguf> <dst.gguf> <blocks> \
#       [--set <key>=<number> ...]
import re
import sys

import gguf

# The keys, after `<arch>.`, that hold one entry a block when they are
# arrays (a scalar is every block's). Only these arrays are shortened;
# any other array as long as the block count stops the cut, since a
# list that only matches the count by chance would be cut short.
PER_BLOCK_KEYS = ('attention.head_count_kv', 'attention.sliding_window_pattern')


def cut(src: str, dst: str, blocks: int, overrides: dict[str, float]) -> None:
    reader = gguf.GGUFReader(src, 'r')
    arch = reader.get_field(gguf.Keys.General.ARCHITECTURE).contents()
    count_key = f'{arch}.block_count'
    src_blocks = reader.get_field(count_key).contents()
    draft_field = reader.get_field(f'{arch}.nextn_predict_layers')
    drafts = draft_field.contents() if draft_field is not None else 0
    trunk = src_blocks - drafts
    if not 0 < blocks < trunk:
        sys.exit(f'{src} has {trunk} trunk blocks: the cut keeps between 1 and {trunk - 1}')
    per_block = {f'{arch}.{key}' for key in PER_BLOCK_KEYS}
    writer = gguf.GGUFWriter(dst, arch=arch, endianess=reader.endianess)
    alignment = reader.get_field(gguf.Keys.General.ALIGNMENT)
    if alignment is not None:
        writer.data_alignment = alignment.contents()
    unused = set(overrides)
    for field in reader.fields.values():
        if field.name == gguf.Keys.General.ARCHITECTURE or field.name.startswith('GGUF.'):
            continue
        val_type = field.types[0]
        sub_type = field.types[-1] if val_type == gguf.GGUFValueType.ARRAY else None
        value = blocks + drafts if field.name == count_key else field.contents()
        if field.name in per_block and sub_type is not None:
            if len(value) != src_blocks:
                sys.exit(f'{src}: {field.name} holds {len(value)} entries, not one a block')
            value = value[:blocks] + value[trunk:]
        elif sub_type is not None and len(value) == src_blocks:
            sys.exit(f'{src}: {field.name} holds {src_blocks} entries, one a block or not: '
                     'name it in PER_BLOCK_KEYS if it is per-block, else the cut cannot tell')
        if field.name in overrides:
            value = type(value)(overrides[field.name])
            unused.discard(field.name)
        writer.add_key_value(field.name, value, val_type, sub_type=sub_type)
    if unused:
        sys.exit(f'{src} has no key {", ".join(sorted(unused))}')
    kept = []
    for t in reader.tensors:
        m = re.match(r'blk\.(\d+)\.', t.name)
        if not m or int(m.group(1)) < blocks:
            kept.append((t.name, t))
        elif int(m.group(1)) >= trunk:
            kept.append((f'blk.{int(m.group(1)) - trunk + blocks}.{t.name[m.end():]}', t))
    for name, t in kept:
        writer.add_tensor_info(name, t.data.shape, t.data.dtype, t.data.nbytes, t.tensor_type)
    writer.write_header_to_file()
    writer.write_kv_data_to_file()
    writer.write_ti_data_to_file()
    for _, t in kept:
        writer.write_tensor_data(t.data, tensor_endianess=reader.endianess)
    writer.close()
    print(f'{dst}: {blocks} of {trunk} trunk blocks and {drafts} draft, '
          f'{len(kept)} of {len(reader.tensors)} tensors')


if __name__ == '__main__':
    args = sys.argv[1:]
    overrides = {}
    while '--set' in args:
        at = args.index('--set')
        key, _, number = args[at + 1].partition('=')
        overrides[key] = float(number)
        del args[at:at + 2]
    if len(args) != 3:
        sys.exit('usage: gguf_cut_layers.py <src.gguf> <dst.gguf> <blocks> '
                 '[--set <key>=<number> ...]')
    cut(args[0], args[1], int(args[2]), overrides)
