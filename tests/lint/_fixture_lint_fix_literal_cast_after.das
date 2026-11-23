//fmt:ignore-file
options gen2
options auto_inline_functions = false
options infer_time_folding = false

def scale(x : float) : float => x * 2.0 + -3.0

def widen(x : int64; y : uint64) : int64 => x + 5l + int64(y & 6ul)

def mask(x : uint) : uint => x & 3u

def precise(x : double) : double => x * 7.0lf

def byte_pair() : array<uint8> {
    var bytes : array<uint8>
    bytes |> push(0u8)
    bytes |> push(255u8)
    return <- bytes
}

def kept(n : int) : float => float(n) + float(16777217) + float(-0)

[export]
def main {
    print("{scale(1.0)} {widen(1l, 1ul)} {mask(1u)} {precise(1.0lf)} {byte_pair()} {kept(1)}\n")
}
