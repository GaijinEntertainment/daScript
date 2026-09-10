"""The TTSO oracle container the three TTS oracle scripts write and `tests/_tts_oracle.das`
reads: `TTSO`, u32 version 1, u32 record count, then per record a u32 name length, the utf8
name, u32 dtype (0 f32, 1 i32), u32 ndims, i32 dims, the raw little-endian data. Plus the
fixture pick the scripts share."""
import struct

import numpy as np

MAGIC = b"TTSO"
DTYPES = {np.dtype(np.float32): 0, np.dtype(np.int32): 1}


def write_dump(path, records):
    with open(path, "wb") as f:
        f.write(MAGIC + struct.pack("<II", 1, len(records)))
        for name, arr in records.items():
            arr = np.ascontiguousarray(arr)
            if arr.dtype == np.int64:
                arr = arr.astype(np.int32)
            nb = name.encode("utf8")
            f.write(struct.pack("<I", len(nb)) + nb)
            f.write(struct.pack("<II", DTYPES[arr.dtype], arr.ndim))
            f.write(struct.pack(f"<{arr.ndim}i", *arr.shape))
            f.write(arr.tobytes())


def pick_sentences(rows, count):
    """Deterministic spread over the corpus: rows whose normalized text is 20..110 characters,
    every k-th by corpus order."""
    ok = [r for r in rows if 20 <= len(r["norm"]) <= 110]
    step = max(1, len(ok) // count)
    return ok[::step][:count]
