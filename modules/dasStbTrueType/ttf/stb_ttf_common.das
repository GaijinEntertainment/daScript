options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module stb_ttf_common shared private

require daslib/safe_addr
require daslib/math_boost

require stbtruetype

require math
require fio
require strings

class public TTFApiAdapter
    def abstract create_font_texture ( dim:int2; data:array<uint8> ) : uint64
    def abstract destroy_font_texture ( tex:uint64 ) : void

var private g_apiAdapter : TTFApiAdapter?

def public has_api_adapter
    return g_apiAdapter != null

def public set_api_adapter ( var aa:TTFApiAdapter? )
    if g_apiAdapter != null
        panic("TTF api is already initialized")
    g_apiAdapter = aa

def public finalize_api_adapter
    unsafe
        delete g_apiAdapter

def private check_api_adapter
    if g_apiAdapter==null
        panic("TTF api adapter is missing")

struct public FontVertex
    xy : float2
    uv : float2

struct public Font
    ttf_buffer : array<uint8>
    cdata : array<stbtt_bakedchar>
    dim : int2
    pixel_height : float
    pwh : int2
    tex : uint64

def public finalize ( var self:Font )
    check_api_adapter()
    delete self.ttf_buffer
    delete self.cdata
    g_apiAdapter->destroy_font_texture(self.tex)

def public load_ttf(fname:string; dim:int2 = int2(512,512); pixel_height:float=32.0; pwh:int2 = int2(32,96); canfail:bool=false)
    check_api_adapter()
    var font : Font
    fopen(fname,"rb") <| $ ( fr )
        if fr!=null
            fmap(fr) <| $(data)
                font.ttf_buffer := data
        elif canfail
            panic("can't open {fname}")
    if !empty(font.ttf_buffer)
        font.dim = dim
        font.pixel_height = pixel_height
        font.pwh = pwh
        font.cdata |> resize(pwh.y-pwh.x+1)
        var temp : array<uint8>
        resize(temp, dim.x*dim.y)
        unsafe
            stbtt_BakeFontBitmap(
                addr(font.ttf_buffer[0]), 0,
                pixel_height,
                addr(temp[0]),
                dim.x, dim.y,
                pwh.x, pwh.y,
                addr(font.cdata[0]))
            font.tex = g_apiAdapter->create_font_texture(dim, temp)
    return <- font

def public create_quads ( font:Font; text:string; at:float2 = float2(0.) )
    var quads : array<FontVertex>
    quads |> reserve(length(text)*4)
    var q : stbtt_aligned_quad
    var x = at.x
    var y = at.y
    for Ch in text
        unsafe
            stbtt_GetBakedQuad(
                addr(font.cdata[0]),
                font.dim.x,font.dim.y,
                Ch-font.pwh.x,
                safe_addr(x),safe_addr(y),
                safe_addr(q),1)
        quads |> push <| [[FontVertex xy=float2(q.x0,q.y0), uv=float2(q.s0, q.t0)]]
        quads |> push <| [[FontVertex xy=float2(q.x1,q.y0), uv=float2(q.s1, q.t0)]]
        quads |> push <| [[FontVertex xy=float2(q.x1,q.y1), uv=float2(q.s1, q.t1)]]
        quads |> push <| [[FontVertex xy=float2(q.x0,q.y1), uv=float2(q.s0, q.t1)]]
    return <- quads

def public quads_dim ( quads : array<FontVertex> ) : tuple<vmin:float2;vmax:float2>
    if empty(quads)
        return [[auto float2(0.), float2(0.)]]
    var vmin = float2(FLT_MAX,FLT_MAX)
    var vmax = float2(-FLT_MAX,-FLT_MAX)
    for q in quads
        vmin = min(vmin, q.xy)
        vmax = max(vmax, q.xy)
    return [[auto vmin, vmax]]

enum public HJust
    left
    center
    right

enum public VJust
    top
    center
    bottom

def public quads_view ( quads : array<FontVertex>; scale:float2; h:HJust = HJust center; v:VJust = VJust center )
    if empty(quads)
        return [[float4x4]]
    let d = quads_dim(quads)
    var o : float2
    if h==HJust left
        o.x = -(d.vmax.x-d.vmin.x)*scale.x
    elif h==HJust center
        o.x = -(d.vmax.x+d.vmin.x)*0.5*scale.x
    else
        o.x = 0.
    if v==VJust bottom
        o.y = -(d.vmax.y-d.vmin.y)*scale.y
    elif v==VJust center
        o.y = -(d.vmax.y+d.vmin.y)*0.5*scale.y
    else
        o.y = 0.
    return compose(
        float4(o,0.,0.),
        float4(1,0,0,0),
        float4(scale,0.,1.))



