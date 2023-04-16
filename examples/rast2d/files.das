module files private

require daslib/array_boost

require fio
require strings

let public WL6_FILES_ROOT = "{get_das_root()}/examples/rast2d"

let public SOUND_SAMPLE_RATE = 7000

let public TOTAL_LEVELS = 60

var rlewTag : uint16
var mapHead : uint[TOTAL_LEVELS]

let public GamePalette = [[uint
    0xff000000; 0xffa80000; 0xff00a800; 0xffa8a800; 0xff0000a8; 0xffa800a8; 0xff0054a8; 0xffa8a8a8;
    0xff545454; 0xfffc5454; 0xff54fc54; 0xfffcfc54; 0xff5454fc; 0xfffc54fc; 0xff54fcfc; 0xfffcfcfc;
    0xffececec; 0xffdcdcdc; 0xffd0d0d0; 0xffc0c0c0; 0xffb4b4b4; 0xffa8a8a8; 0xff989898; 0xff8c8c8c;
    0xff7c7c7c; 0xff707070; 0xff646464; 0xff545454; 0xff484848; 0xff383838; 0xff2c2c2c; 0xff202020;
    0xff0000fc; 0xff0000ec; 0xff0000e0; 0xff0000d4; 0xff0000c8; 0xff0000bc; 0xff0000b0; 0xff0000a4;
    0xff000098; 0xff000088; 0xff00007c; 0xff000070; 0xff000064; 0xff000058; 0xff00004c; 0xff000040;
    0xffd8d8fc; 0xffb8b8fc; 0xff9c9cfc; 0xff7c7cfc; 0xff5c5cfc; 0xff4040fc; 0xff2020fc; 0xff0000fc;
    0xff5ca8fc; 0xff4098fc; 0xff2088fc; 0xff0078fc; 0xff006ce4; 0xff0060cc; 0xff0054b4; 0xff004c9c;
    0xffd8fcfc; 0xffb8fcfc; 0xff9cfcfc; 0xff7cfcfc; 0xff5cf8fc; 0xff40f4fc; 0xff20f4fc; 0xff00f4fc;
    0xff00d8e4; 0xff00c4cc; 0xff00acb4; 0xff009c9c; 0xff008484; 0xff006c70; 0xff005458; 0xff004040;
    0xff5cfcd0; 0xff40fcc4; 0xff20fcb4; 0xff00fca0; 0xff00e490; 0xff00cc80; 0xff00b474; 0xff009c60;
    0xffd8fcd8; 0xffb8fcbc; 0xff9cfc9c; 0xff7cfc80; 0xff5cfc60; 0xff40fc40; 0xff20fc20; 0xff00fc00;
    0xff00fc00; 0xff00ec00; 0xff00e000; 0xff00d400; 0xff00c804; 0xff00bc04; 0xff00b004; 0xff00a404;
    0xff009804; 0xff008804; 0xff007c04; 0xff007004; 0xff006404; 0xff005804; 0xff004c04; 0xff004004;
    0xfffcfcd8; 0xfffcfcb8; 0xfffcfc9c; 0xfff8fc7c; 0xfffcfc5c; 0xfffcfc40; 0xfffcfc20; 0xfffcfc00;
    0xffe4e400; 0xffcccc00; 0xffb4b400; 0xff9c9c00; 0xff848400; 0xff707000; 0xff585800; 0xff404000;
    0xfffcbc5c; 0xfffcb040; 0xfffca820; 0xfffc9c00; 0xffe48c00; 0xffcc7c00; 0xffb46c00; 0xff9c5c00;
    0xfffcd8d8; 0xfffcbcb8; 0xfffc9c9c; 0xfffc807c; 0xfffc605c; 0xfffc4040; 0xfffc2420; 0xfffc0400;
    0xfffc0000; 0xffec0000; 0xffe00000; 0xffd40000; 0xffc80000; 0xffbc0000; 0xffb00000; 0xffa40000;
    0xff980000; 0xff880000; 0xff7c0000; 0xff700000; 0xff640000; 0xff580000; 0xff4c0000; 0xff400000;
    0xff282828; 0xff34e0fc; 0xff24d4fc; 0xff18ccfc; 0xff08c0fc; 0xff00b4fc; 0xfffc20b4; 0xfffc00a8;
    0xffe40098; 0xffcc0080; 0xffb40074; 0xff9c0060; 0xff840050; 0xff700044; 0xff580034; 0xff400028;
    0xfffcd8fc; 0xfffcb8fc; 0xfffc9cfc; 0xfffc7cfc; 0xfffc5cfc; 0xfffc40fc; 0xfffc20fc; 0xfffc00fc;
    0xffe400e0; 0xffcc00c8; 0xffb400b4; 0xff9c009c; 0xff840084; 0xff70006c; 0xff580058; 0xff400040;
    0xffdce8fc; 0xffd0e0fc; 0xffc4d8fc; 0xffbcd4fc; 0xffb0ccfc; 0xffa4c4fc; 0xff9cbcfc; 0xff90b8fc;
    0xff80b0fc; 0xff70a4fc; 0xff609cfc; 0xff5c94f0; 0xff588ce8; 0xff5488dc; 0xff5080d0; 0xff4c7cc8;
    0xff4878bc; 0xff4470b4; 0xff4068a8; 0xff3c64a0; 0xff38609c; 0xff345c90; 0xff305888; 0xff2c5080;
    0xff284c74; 0xff24486c; 0xff20405c; 0xff1c3c54; 0xff183848; 0xff183040; 0xff142c38; 0xff0c2028;
    0xff640060; 0xff646400; 0xff606000; 0xff1c0000; 0xff2c0000; 0xff102430; 0xff480048; 0xff500050;
    0xff340000; 0xff1c1c1c; 0xff4c4c4c; 0xff5c5c5c; 0xff404040; 0xff303030; 0xff343434; 0xfff4f4d8;
    0xffe8e8b8; 0xffdcdc9c; 0xffc8c874; 0xffc0c048; 0xffb4b420; 0xffb0b020; 0xffa4a400; 0xff989800;
    0xff8c8c00; 0xff848400; 0xff7c7c00; 0xff787800; 0xff747400; 0xff707000; 0xff6c6c00; 0xff880098]]

def LoadBytes ( filename : string )
    var bytes : array<uint8>
    fopen("{WL6_FILES_ROOT}/{filename}", "rb") <| $ ( fr )
        fmap(fr) <| $ ( data )
            bytes := data
    if empty(bytes)
        panic("failed to load file: {filename}")
    return <- bytes

def public getUInt8 ( bytes : array<uint8>; offset : int ) : uint8
    return bytes[offset]

def public getUInt8 ( bytes : array<uint8>#; offset : int ) : uint8
    return bytes[offset]

def public getUInt16 ( bytes : array<uint8>; offset : int ) : uint16
    return uint16(uint(bytes[offset]) + (uint(bytes[offset+1]) << 8u))

def public getUInt16 ( bytes : array<uint8>#; offset : int ) : uint16
    return uint16(uint(bytes[offset]) + (uint(bytes[offset+1]) << 8u))

def public getUInt32 ( bytes : array<uint8>; offset : int ) : uint
    return uint(bytes[offset]) + (uint(bytes[offset+1]) << 8u) + (uint(bytes[offset+2]) << 16u) + (uint(bytes[offset+3]) << 24u)

def public getUInt32 ( bytes : array<uint8>#; offset : int ) : uint
    return uint(bytes[offset]) + (uint(bytes[offset+1]) << 8u) + (uint(bytes[offset+2]) << 16u) + (uint(bytes[offset+3]) << 24u)

def public setUInt8 ( var bytes : array<uint8>; offset : int; value : uint8 )
    bytes[offset] = value

def public setUInt16 ( var bytes : array<uint8>; offset : int; value : uint16 )
    bytes[offset] = uint8(uint(value) & 0xFFu)
    bytes[offset+1] = uint8((uint(value) >> 8u) & 0xFFu)

def rlewDecode ( bytes : array<uint8> )
    var buffer : array<uint16>
    let rlewSize = getUInt16(bytes, 0)
    buffer |> resize(int(rlewSize)/2)
    var inOffset = 2
    var outOffset = 0
    let bytesSize = bytes |> length
    while inOffset < bytesSize
        var w = getUInt16(bytes, inOffset)
        inOffset += 2
        if w == rlewTag
            var n = getUInt16(bytes, inOffset)
            var x = getUInt16(bytes, inOffset + 2)
            inOffset += 4
            for i in range(uint(n))
                buffer[outOffset] = x
                outOffset ++
        else
            buffer[outOffset] = w
            outOffset++
    return <- buffer

def carmackDecode ( bytes : array<uint8># )
    let size = getUInt16(bytes, 0)
    var outView : array<uint8>
    outView |> resize(int(size))
    var inOffset = 2
    var outOffset = 0
    let bytesSize = bytes |> length
    while inOffset < bytesSize
        let x = getUInt8(bytes, inOffset + 1)
        if x == 0xA7u8 || x == 0xA8u8
            // possibly a pointer
            var n = getUInt8(bytes, inOffset)
            if n == 0u8
                // exception (not really a pointer)
                setUInt8(outView, outOffset, getUInt8(bytes, inOffset + 2))
                setUInt8(outView, outOffset + 1, x)
                inOffset += 3
                outOffset += 2
            elif x == 0xA7u8
                // near pointer
                let offset = 2 * int(getUInt8(bytes, inOffset + 2))
                for i in range(uint(n))
                    setUInt16(outView, outOffset, getUInt16(outView, outOffset - offset))
                    outOffset += 2
                inOffset += 3
            else
                // far pointer
                let offset = 2 * int(getUInt16(bytes, inOffset + 2))
                for i in range(uint(n))
                    setUInt16(outView, outOffset, getUInt16(outView, offset + 2 * i))
                    outOffset += 2
                inOffset += 4
        else
            // not a pointer
            setUInt16(outView, outOffset, getUInt16(bytes, inOffset))
            inOffset += 2
            outOffset += 2
    return <- outView

def InitMapHead()
    var bytes <- LoadBytes("MAPHEAD.WL6")
    rlewTag = bytes |> getUInt16(0)
    assert(uint(rlewTag) == 0xABCD, "invalid rlew tag")
    for i in range(TOTAL_LEVELS)
        mapHead[i] = bytes |> getUInt32(2 + i*4)
    to_log(LOG_INFO,"Loaded MAPHEAD.WL6\n")

struct public Level
    plane0 : array<uint16>
    plane1 : array<uint16>

var public Levels : Level[TOTAL_LEVELS]

def InitGameMap()
    var bytes <- LoadBytes("GAMEMAPS.WL6")
    for level in range(TOTAL_LEVELS)
        let offset = mapHead[level]
        array_view(bytes, int(offset), 42) <| $ ( header )
            let plane0Offset = header |> getUInt32(0)
            let plane0Size = header |> getUInt16(12)
            let plane1Offset = header |> getUInt32(4)
            let plane1Size = header |> getUInt16(14)
            array_view(bytes, int(plane0Offset), int(plane0Size)) <| $ ( plane0View )
                var cdec <- carmackDecode(plane0View)
                Levels[level].plane0 <- rlewDecode(cdec)
            array_view(bytes, int(plane1Offset), int(plane1Size)) <| $ ( plane1View )
                var cdec <- carmackDecode(plane1View)
                Levels[level].plane1 <- rlewDecode(cdec)
            assert(Levels[level].plane0 |> length == 64*64, "invalid plane0 size")
            assert(Levels[level].plane1 |> length == 64*64, "invalid plane1 size")
    to_log(LOG_INFO,"Loaded GAMEMAPS.WL6\n")

struct public WallTexture
    data : uint8[64*64]

var public WallPics : array<WallTexture>

struct public SpriteTexture
    data : array<uint8>
    firstCol, lastCol : int

var public Sprites : array<SpriteTexture>

struct public Sound
    data : array<uint8>

var public Sounds : array<Sound>

var public Audio : array<Sound>

def prepareSprite ( var sprite : SpriteTexture )
    sprite.firstCol = int(sprite.data |> getUInt16(0))
    sprite.lastCol = int(sprite.data |> getUInt16(2))

def InitResources()
    var bytes <- LoadBytes("VSWAP.WL6")
    let totalChunks = int(getUInt16(bytes, 0))
    let firstSpriteChunk = int(getUInt16(bytes, 2))
    let wallChunks = firstSpriteChunk
    let firstSoundChunk = int(getUInt16(bytes, 4))
    let spriteChunks = firstSoundChunk - firstSpriteChunk
    let soundChunks = totalChunks - firstSoundChunk
    to_log(LOG_INFO,"total {totalChunks} chunks\n")
    to_log(LOG_INFO,"total {wallChunks} wall chunks, at 0\n")
    to_log(LOG_INFO,"total {spriteChunks} sprites, at {firstSpriteChunk}\n")
    to_log(LOG_INFO,"total {soundChunks} sounds, at {firstSoundChunk}\n")
    WallPics |> resize(wallChunks)
    let wallsOffset = getUInt32(bytes, 6)
    for i in range(wallChunks)
        let wallSize = int(bytes |> getUInt16(6 + totalChunks*4 + i*2))
        //to_log(LOG_INFO,"wall {i} offset {int(wallsOffset) + i*4096} of {wallSize} bytes\n")
        array_view(bytes, int(wallsOffset) + i*4096, 4096) <| $ ( view )
            for s,d in view, WallPics[i].data
                d = s
    Sprites |> resize(spriteChunks)
    for i in range(spriteChunks)
        let spriteOffset = getUInt32(bytes, 6 + (firstSpriteChunk + i)*4)
        let spriteSize = int(bytes |> getUInt16(6 + totalChunks*4 + (firstSpriteChunk + i)*2))
        // to_log(LOG_INFO,"sprite {s} offset {spriteOffset} of size {spriteSize}\n")
        array_view(bytes, int(spriteOffset), spriteSize) <| $ ( view )
            Sprites[i].data := view
            prepareSprite(Sprites[i])
    // extra sounds
    Sounds |> resize(soundChunks)
    for i in range(soundChunks)
        let soundOffset = getUInt32(bytes, 6 + (firstSoundChunk + i)*4)
        let soundSize = int(bytes |> getUInt16(6 + totalChunks*4 + (firstSoundChunk + i)*2))
        // to_log(LOG_INFO,"sound {i} offset {soundOffset} of size {soundSize}\n")
        array_view(bytes, int(soundOffset), soundSize) <| $ ( view )
            Sounds[i].data := view
    to_log(LOG_INFO,"Loaded VSWAP.WL6\n")

let NUMSOUNDS = 87
let STARTPCSOUNDS = 0
let STARTADLIBSOUNDS = 87
let STARTDIGISOUNDS = 174
let PC_RATE = 140
let PC_BASE_TIMER = 1193181
let PC_VOLUME = 20

def convertPcSoundToWave ( src:array<uint8>#; hertz:int )
    var dst : array<uint8>
    dst |> resize(src |> length * hertz / PC_RATE)
    var dstOut = 0
    var sign = -1
    var phase_tic = 0
    for tone in src
        let phase_length = (hertz * int(tone) * 60) / (2 * PC_BASE_TIMER)
        for i in range(hertz / PC_RATE)
            if tone != 0u8
                dst[dstOut++] = uint8(128 + sign * PC_VOLUME)
                if phase_tic >= phase_length
                    sign = -sign
                    phase_tic = 0
                phase_tic += 1
            else
                phase_tic = 0
                dst[dstOut++] = 128u8
    return <- dst

def public InitAudio()
    var audioHeader <- LoadBytes("AUDIOHED.WL6")
    var audioData <- LoadBytes("AUDIOT.WL6")
    to_log(LOG_INFO,"total {NUMSOUNDS} sounds\n")
    for i in range(NUMSOUNDS)
        let offset = getUInt32(audioHeader, (i+STARTPCSOUNDS)*4)
        let size = getUInt32(audioHeader, (i+1+STARTPCSOUNDS)*4) - offset
        array_view(audioData, int(offset), int(size)) <| $ ( view )
            var snd <- [[Sound data <- convertPcSoundToWave(view, SOUND_SAMPLE_RATE)]]
            Audio |> emplace(snd)
    to_log(LOG_INFO,"Loaded AUDIOHED.WL6 and AUDIOT.WL6\n")

def public InitFiles
    InitMapHead()
    InitGameMap()
    InitResources()
    InitAudio()

