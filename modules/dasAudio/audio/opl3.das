options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module opl3 shared private

require audio public

def public OPL3_Reset ( var chip:Opl3Chip; rate:int )
    OPL3_Reset(unsafe(addr(chip)),uint(rate))

def public  OPL3_WriteReg ( var chip:Opl3Chip; reg:uint; val:uint8 )
    OPL3_WriteReg(unsafe(addr(chip)), uint16(reg), val)

def public OPL3_GenerateStreamMono ( var chip:Opl3Chip; var data:array<uint16> )
    var tdata : array<uint16>
    tdata |> resize(length(data)*2)
    let pdata : void? = unsafe(addr(tdata[0]))
    OPL3_GenerateStream(unsafe(addr(chip)), pdata, uint(data |> length))
    for i in range(data |> length)
        data[i] = tdata[i*2]    // only left channel
    delete tdata

def public OPL3_GenerateStream ( var chip:Opl3Chip; var data:array<uint16> )
    let pdata : void? = unsafe(addr(data[0]))
    OPL3_GenerateStream(unsafe(addr(chip)), pdata, uint(length(data)/2))
