Accumulating form: returns `acc + idot4(a, b)` per lane in one step — the exact shape of the hardware accumulate instructions (`sdot` accumulates in place, `vpdpbssd` adds to the destination).
