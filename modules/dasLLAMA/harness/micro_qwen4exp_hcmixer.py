#!/usr/bin/env python3
# The `-hcmixer` twin of a hyper-connection NextN head shard: the shard copied whole, with the head mixer the loader asks of
# a hyper-connection head (`blk.<N>.nextn.hc_head_{norm,down,up}.weight`) added as copies of the shard's own
# `output_hc_{norm,down,up}.weight` - the same geometry, random either way on micro-qwen4exp. Mints the head
# tests/test_gpu_resident_hc_micro.das loads beside the published trunk; the performance/model_specs.das row names it.
#
#   python micro_qwen4exp_hcmixer.py <models>/micro-qwen4exp/micro-qwen4exp-realqsa-mtp.gguf <models>/micro-qwen4exp/micro-qwen4exp-realqsa-mtp-hcmixer.gguf
#
# Needs llama.cpp's gguf-py (pip install gguf, or GGUF_PY pointing at the checkout's gguf-py).
import os
import sys

if os.environ.get("GGUF_PY"):
    sys.path.insert(0, os.environ["GGUF_PY"])
import gguf  # noqa: E402

MIXER_PARTS = ("norm", "down", "up")


def nextn_block(reader: gguf.GGUFReader) -> int:
    """The NextN block's index: the one `blk.<N>.nextn.eh_proj.weight` the shard carries."""
    for t in reader.tensors:
        if t.name.endswith(".nextn.eh_proj.weight"):
            return int(t.name.split(".")[1])
    raise SystemExit("no blk.<N>.nextn.eh_proj.weight: not a NextN head shard")


def copy_metadata(reader: gguf.GGUFReader, writer: gguf.GGUFWriter) -> None:
    """Every key-value of the shard but the ones GGUFWriter writes itself (the GGUF.* virtual fields, the architecture)."""
    for field in reader.fields.values():
        if field.name == gguf.Keys.General.ARCHITECTURE or field.name.startswith("GGUF."):
            continue
        val_type = field.types[0]
        sub_type = field.types[-1] if val_type == gguf.GGUFValueType.ARRAY else None
        writer.add_key_value(field.name, field.contents(), val_type, sub_type=sub_type)


def main() -> None:
    if len(sys.argv) != 3:
        raise SystemExit(__doc__ or "usage: micro_qwen4exp_hcmixer.py <head.gguf> <head-hcmixer.gguf>")
    src, dst = sys.argv[1], sys.argv[2]
    reader = gguf.GGUFReader(src)
    arch = reader.fields[gguf.Keys.General.ARCHITECTURE].contents()
    block = nextn_block(reader)
    by_name = {t.name: t for t in reader.tensors}
    if any(f"blk.{block}.nextn.hc_head_{p}.weight" in by_name for p in MIXER_PARTS):
        raise SystemExit(f"{src} already carries a head mixer")
    sources = {p: by_name.get(f"output_hc_{p}.weight") for p in MIXER_PARTS}
    missing = [p for p, t in sources.items() if t is None]
    if missing:
        raise SystemExit(f"{src} carries no output_hc_{{{', '.join(missing)}}}.weight: not a hyper-connection head")
    writer = gguf.GGUFWriter(dst, arch, endianess=reader.endianess)
    copy_metadata(reader, writer)
    tensors = list(reader.tensors) + [(f"blk.{block}.nextn.hc_head_{p}.weight", sources[p]) for p in MIXER_PARTS]
    for entry in tensors:
        name, t = entry if isinstance(entry, tuple) else (entry.name, entry)
        writer.add_tensor_info(name, t.data.shape, t.data.dtype, t.data.nbytes, t.tensor_type)
    writer.write_header_to_file()
    writer.write_kv_data_to_file()
    writer.write_ti_data_to_file()
    for entry in tensors:
        t = entry[1] if isinstance(entry, tuple) else entry
        writer.write_tensor_data(t.data, tensor_endianess=reader.endianess)
    writer.close()
    print(f"wrote {dst}: {len(reader.tensors)} tensors + the head mixer blk.{block}.nextn.hc_head_* off output_hc_*")


if __name__ == "__main__":
    main()
