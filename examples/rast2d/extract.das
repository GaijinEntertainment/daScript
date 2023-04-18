require files

require stbimage
require fio
require strings

def save_wall_pic ( index:int )
    let folder = "{WL6_FILES_ROOT}/WALLS"
    mkdir(folder)
    let filename = "{folder}/wall_{index}.png"
    var data : array<uint>
    data |> resize(64*64)
    for s,d in WallPics[index].data,data
        d = GamePalette[int(s)]
    stbi_write_png(filename, 64, 64, 4, unsafe(addr(data[0])), 64*4)

def spriteDecode ( bytes : array<uint8> )
    let firstCol = int(bytes |> getUInt16(0))
    let lastCol = int(bytes |> getUInt16(2))
    let nbCol = lastCol - firstCol + 1
    var pixelPoolOffset = 4 + 2 * nbCol
    var outView : array<uint>
    outView |> resize(64*64)
    for col in range(firstCol, lastCol + 1)
        var colOffset = int(bytes |> getUInt16(4 + 2 * (col - firstCol)))
        while true
            let endRow = int(bytes |> getUInt16(colOffset)) / 2
            if endRow == 0
                break
            let startRow = int(bytes |> getUInt16(colOffset + 4)) / 2
            colOffset += 6
            var outOffset = col * 64 + startRow
            for row in range(startRow, endRow)
                outView[outOffset] = GamePalette[int(bytes[pixelPoolOffset])]
                outOffset ++
                pixelPoolOffset ++
    return <- outView

def save_sprite_pic ( index:int )
    let folder = "{WL6_FILES_ROOT}/SPRITES"
    mkdir(folder)
    let filename = "{folder}/sprite_{index}.png"
    var pixels <- spriteDecode(Sprites[index].data)
    stbi_write_png(filename, 64, 64, 4, unsafe(addr(pixels[0])), 64*4)

def save_level ( level:int )
    let folder = "{WL6_FILES_ROOT}/LEVELS"
    mkdir(folder)
    let filename = "{folder}/LEVEL_{level}.txt"
    fopen(filename, "w") <| $ ( file )
        if file == null
            return
        let txt = build_string <| $ ( writer )
            for y in 0..64
                for x in 0..64
                    writer |> format("%02x", int(Levels[level].plane0[y*64+x]))
                writer |> write("\n")
            writer |> write("\n")
            for y in 0..64
                for x in 0..64
                    writer |> format("%02x", int(Levels[level].plane1[y*64+x]))
                writer |> write("\n")
            writer |> write("\n")
        file |> fwrite(txt)

struct WavHeader
    riff : uint8[4]             //  4(0)
    totalLength : uint          //  4(4)
    wave : uint8[8]             //  8(8)
    lengthFormat : uint         //  4(16)
    dataType : uint16           //  2(20)
    channelNumbers : uint16     //  2(22)
    sampleRate : uint           //  4(24)
    bytesPerSecond : uint       //  4(28)
    bytesPerSample : uint16     //  2(32)
    bitsPerSample : uint16      //  2(34)
    data : uint8[4]             //  4(36)
    lengthData : uint           //  4(40)

def save_sound ( snd:Sound; prefix:string; index:int )
    static_assert(typeinfo(sizeof type<WavHeader>) == 44)
    let folder = "{WL6_FILES_ROOT}/SOUNDS"
    mkdir(folder)
    let filename = "{folder}/{prefix}_{index}.wav"
    fopen(filename, "wb") <| $ ( file )
        if file == null
            return
        let header = [[WavHeader
            riff = [[uint8 'R'u8; 'I'u8; 'F'u8; 'F'u8]],
            totalLength = uint(32 + length(snd.data)),
            wave = [[uint8 'W'u8; 'A'u8; 'V'u8; 'E'u8; 'f'u8; 'm'u8; 't'u8; ' 'u8]],
            lengthFormat = 16u,
            dataType = uint16(1),
            channelNumbers = uint16(1),
            sampleRate = uint(SOUND_SAMPLE_RATE),
            bytesPerSecond = uint(SOUND_SAMPLE_RATE),
            bytesPerSample = uint16(1),
            bitsPerSample = uint16(8),
            data = [[uint8 'd'u8; 'a'u8; 't'u8; 'a'u8]],
            lengthData = uint(length(snd.data))
        ]]
        file |> fwrite(header)
        file |> fwrite(snd.data)

def public save_pic ( pic:Picture; prefix:string; index:int )
    let folder = "{WL6_FILES_ROOT}/PICTURES"
    mkdir(folder)
    let filename = "{folder}/{prefix}_{index}.png"
    var data : array<uint>
    data |> resize(pic.width * pic.height)
    for d,s in data,pic.texels
        d = GamePalette[int(s)]
    stbi_write_png(filename, pic.width, pic.height, 4, unsafe(addr(data[0])), pic.width*4)

def public DumpFiles
    for i in range(WallPics |> length)
        save_wall_pic(i)
    for i in range(Sprites |> length)
        save_sprite_pic(i)
    for l in range(TOTAL_LEVELS)
        save_level(l)
    for i in range(Sounds |> length)
        save_sound(Sounds[i],"sound",i)
    for i in range(Audio |> length)
        save_sound(Audio[i],"audio",i)
    for i in range(Pictures |> length)
        save_pic(Pictures[i],"pic",i)

[export]
def main
    InitFiles()
    DumpFiles()
