//fmt:ignore-file
options gen2
options auto_inline_functions = false
options infer_time_folding = false

def scale(x : float) : float => x * float(2) + float(-3)

def widen(x : int64; y : uint64) : int64 => x + int64(5) + int64(y & uint64(6))

def mask(x : uint) : uint => x & uint(3)

def precise(x : double) : double => x * double(7)

def byte_pair() : array<uint8> {
    var bytes : array<uint8>
    bytes |> push(uint8(0))
    bytes |> push(uint8(255))
    return <- bytes
}

def kept(n : int) : float => float(n) + float(16777217) + float(-0)

[export]
def main {
    print("{scale(1.0)} {widen(1l, 1ul)} {mask(1u)} {precise(1.0lf)} {byte_pair()} {kept(1)}\n")
}
