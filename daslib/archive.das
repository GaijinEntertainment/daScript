options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module archive shared private

require daslib/contracts
require daslib/apply
require daslib/safe_addr
require daslib/templates
require math
require strings
require rtti

class public Serializer
    def abstract write ( bytes:void? implicit; size:int ) : bool
    def abstract read ( bytes:void? implicit; size:int ) : bool
    def abstract error ( code:string ) : void
    def abstract OK : bool

class public MemSerializer : Serializer
    def MemSerializer
        pass    // writing
    def MemSerializer ( from:array<uint8> )
        data := from
    def extractData : array<uint8>
        return <- data
    def getCopyOfData : array<uint8>
        var cdata := data
        return <- cdata
    def getLastError
        return lastError
    def override OK
        return lastError==""
    def override write ( bytes:void? implicit; size:int ) : bool
        let pos = length(data)
        data |> resize(pos + size)
        unsafe
            memcpy ( addr(data[pos]), bytes, size)
        return true
    def override read ( bytes:void? implicit; size:int ) : bool
        let newOffset = readOffset + size
        let maxOffset = length(data)
        if newOffset > maxOffset
            error("reading past the end of stream")
            readOffset = maxOffset
            return false
        unsafe
            memcpy ( bytes, addr(data[readOffset]), size)
        readOffset = newOffset
        return true
    def override error ( code:string ) : void
        lastError = code
    private data:array<uint8>
    private readOffset:int
    private lastError:string

struct public Archive
    version : uint
    reading : bool
    stream  : Serializer?

def public serialize_raw ( var arch:Archive; var value : auto(TT)& )
    if arch.reading
        arch.stream->read(safe_addr(value), typeinfo(sizeof type<TT-&>))
    else
        arch.stream->write(safe_addr(value), typeinfo(sizeof type<TT-&>))

def public read_raw ( var arch:Archive; var value : auto(TT)& )
    assert(arch.reading)
    arch.stream->read(safe_addr(value), typeinfo(sizeof type<TT-&>))

def public write_raw ( var arch:Archive; var value : auto(TT)& )
    assert(!arch.reading)
    arch.stream->write(safe_addr(value), typeinfo(sizeof type<TT-&>))

[expect_any_enum(value), template(dummy=false)]
def public serialize ( var arch:Archive; var value : auto(TT)&; dummy:TT-&?????? = null )
    arch |> serialize_raw(value)

[expect_any_function(value), template(dummy=false)]
def public serialize ( var arch:Archive; var value:auto(TT)&; dummy:TT-&????? = null )
    if arch.reading
        var mnh : uint
        arch |> read_raw(mnh)
        if mnh != 0u
            unsafe
                value = reinterpret<TT-#>(get_function_by_mangled_name_hash(mnh))
    else
        var mnh = get_function_mangled_name_hash(value)
        arch |> write_raw(mnh)

[expect_any_workhorse_raw(value), template(dummy=false)]
def public serialize ( var arch:Archive; var value : auto(TT)&; dummy:TT-&???? = null )
    arch |> serialize_raw(value)

def public serialize ( var arch:Archive; var value : float3x3 )
    arch |> serialize_raw(value)

def public serialize ( var arch:Archive; var value : float3x4 )
    arch |> serialize_raw(value)

def public serialize ( var arch:Archive; var value : float4x4 )
    arch |> serialize_raw(value)

[expect_any_struct(value), template(dummy=false)]
def public serialize ( var arch:Archive; var value : auto(TT)&; dummy:TT-&? = null )
    if arch.reading
        delete value
    apply(value) <| $ ( name:string; field )
        arch |> _::serialize(field)

[expect_any_tuple(value), template(dummy=false)]
def public serialize ( var arch:Archive; var value : auto(TT)&; dummy:TT-&?? = null )
    if arch.reading
        delete value
    apply(value) <| $ ( name:string; field )
        arch |> _::serialize(field)

[expect_any_variant(value), template(dummy=false)]
def public serialize ( var arch:Archive; var value : auto(TT)&; dummy:TT-&??? = null )
    if arch.reading
        delete value
        var index : int
        arch |> read_raw(index)
        unsafe
            value |> set_variant_index(index)
    else
        var index = variant_index(value)
        arch |> write_raw(index)
    apply(value) <| $ ( name:string; field )
        arch |> _::serialize(field)

def public serialize ( var arch:Archive; var value : auto(TT)[] )
    for element in value
        arch |> _::serialize(element)

def public serialize ( var arch:Archive; var value : array<auto(TT)> )
    if arch.reading
        var len : int
        delete value
        arch |> read_raw(len)
        value |> resize(len)
        for element in value
            arch |> _::serialize(element)
    else
        var len = length(value)
        arch |> write_raw(len)
        for element in value
            arch |> _::serialize(element)

def public serialize ( var arch:Archive; var value : table<auto(KT);auto(VT)> )
    if arch.reading
        var len : int
        arch |> read_raw(len)
        delete value
        for i in range(len)
            var k : KT-const-&-#
            arch |> _::serialize(k)
            arch |> _::serialize(value[k])
    else
        var len = length(value)
        arch |> write_raw(len)
        for k,v in keys(value),values(value)
            var K = k
            arch |> _::serialize(K)
            arch |> _::serialize(v)

def public serialize ( var arch:Archive; var value : string& )
    unsafe
        if arch.reading
            var len: int
            var tbuf: array<uint8>
            arch |> read_raw(len)
            tbuf |> resize(len+1)                           // todo: implement preallocat string with raii
            arch.stream->read(addr(tbuf[0]), len+1)
            let tstr = reinterpret<string#> addr(tbuf[0])
            value := tstr
            delete tbuf
        else
            var len = length(value)
            arch |> write_raw(len)
            arch.stream->write(reinterpret<void?> value, len+1)

def public serialize ( var arch:Archive; var value:auto(TT)? )
    if arch.reading
        value = null

def public mem_archive_save ( var t:auto& )
    var mems = new MemSerializer()
    var sarch = [[Archive reading=false, stream=mems]]
    sarch |> serialize(t)
    return <- mems->extractData()

def public mem_archive_load ( var data:array<uint8>; var t:auto&; canfail:bool=false ) : bool
    var memr = new MemSerializer(data)
    var rarch = [[Archive reading=true, stream=memr]]
    rarch |> serialize(t)
    if !memr->OK()
        if !canfail
            panic(memr->getLastError())
        return false
    return true
