options indenting = 4
options remove_unused_symbols = false

module tga shared

require fio
require math

[private]
def make_header ( width,height : uint ) : uint8[18]
    return [[uint8[18]
        uint8(0);                               // ID length
        uint8(0);                               // no color map
        uint8(2);                               // uncompressed, true color
        uint8(0); uint8(0); uint8(0); uint8(0);
        uint8(0);
        uint8(0); uint8(0); uint8(0); uint8(0); // x and y origin
        uint8(width & 0x00FF);
        uint8(((width & 0xFF00) >> 8u));
        uint8(height & 0x00FF);
        uint8((height & 0xFF00) >> 8u);
        uint8(32);                              // 32 bit
        uint8(0)
    ]]

def save_TGA ( fileName : string; width,height : int; blk : block<(var b:array<uint>):void> )
    var saved = false
    fopen(fileName, "wb") <| $ ( ftga )
        if ftga != null
            var header = make_header(uint(width), uint(height))
            fwrite(ftga, header)
            var b : array<uint>
            invoke ( blk, b )
            assert ( length(b)==width*height )
            fwrite(ftga, b)
            saved = true
    return saved

def linear_to_SRGB ( color : float3 )
    var x = float4(color.x, color.y, color.z, 1.0)
    x = max(x, float4(0));
    x = max(1.055 * pow(x, float4(0.416666667)) - float4(0.055), float4(0));
    return pack_float_to_byte(x.zyxw * float4(255.0))

def save_TGA ( fileName : string; width,height:int; arr:array<float3> )
    if width * height != length(arr)
        return false
    return save_TGA(fileName, width, height) <| $ ( bytes )
        bytes |> resize(width*height)
        for ubc,f3c in bytes,arr
            ubc = linear_to_SRGB(f3c)

