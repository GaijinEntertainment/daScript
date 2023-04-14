module rast2d private

require math

struct public Texture
    //! texture surface
    size : int2
    [[do_not_delete]] pixels : uint?

struct public Bitmap
    //! single surface
    size : int2
    [[do_not_delete]] data : uint?

def public clear ( var bmp:Bitmap; color:uint )
    //! clear surface with solid color
    unsafe
        memset32(bmp.data, color, bmp.size.x * bmp.size.y)

[jit,hint(unsafe_range_check,noalias=bmp,hot)]
def public fill_rect ( var bmp:Bitmap; var xmin,ymin,dx,dy:int; color:uint )
    //! rectangle by x,y,width,height
    fill_rect_p2(bmp,xmin,ymin,xmin+dx,ymin+dy,color)

[jit,hint(unsafe_range_check,noalias=bmp,hot)]
def public fill_rect_p2 ( var bmp:Bitmap; var xmin,ymin,xmax,ymax:int; color:uint )
    //! fill rectangle with solid color, clip to surface
    unsafe
        xmin = max(xmin, 0)
        xmax = min(xmax, bmp.size.x)
        ymin = max(ymin, 0)
        ymax = min(ymax, bmp.size.y)
        if xmin >= xmax || ymin >= ymax
            return
        var pitch = bmp.size.x
        var span = xmax - xmin
        var pixels = bmp.data + (pitch * ymin + xmin)
        if false // span == pitch
            memset32(pixels, color, span * (ymax - ymin))
        else
            for y in ymin..ymax
                memset32(pixels, color, span)
                pixels += pitch

[jit,hint(unsafe_range_check,noalias=bmp,noalias=texture,hot,alwaysinline),unsafe_deref]
def public draw_vspan (var bmp : Bitmap; texture : Texture; x, y : int; uv : float4; h : int)
    //! draw vertical span of texture
    if texture.size == int2()
        return
    if x < 0 || x >= bmp.size.x
        return
    let yMin = max(y, 0)
    let yMax = min(y + h, bmp.size.y)
    if yMin >= yMax
        return
    let iTexSizeX = int(texture.size.x)
    let i4TexSizeX = int4(iTexSizeX)
    let bmpSizeX = bmp.size.x
    let bmpSizeX4 = bmpSizeX * 4
    let bmpIdxs = int4(0,1,2,3)*bmpSizeX
    var bmpData = unsafe(addr(unsafe(bmp.data[yMin * bmp.size.x + x])))
    var pixelData = unsafe(addr(unsafe(texture.pixels[int(uv.x)])))
    let dh = rcp(float(h))
    var uvY = uv.y + uv.w * (float(yMin - y) * dh)
    let dUVY = uv.w * dh
    let count = yMax - yMin
    var count4 = count >> 2
    let count16 = count4 >> 2
    count4 &= 3
    let tail4 = count & 3
    var uv4 = float4(uvY) + float4(dUVY)*float4(1.,2.,3.,4.)
    let duv4 = float4(dUVY) * 4.
    unsafe
        for _ in range(count16)
            gather_store_stride(bmpData, bmpSizeX, pixelData, int4(uv4) * i4TexSizeX)
            bmpData += bmpSizeX4
            uv4 += duv4
            gather_store_stride(bmpData, bmpSizeX, pixelData, int4(uv4) * i4TexSizeX)
            bmpData += bmpSizeX4
            uv4 += duv4
            gather_store_stride(bmpData, bmpSizeX, pixelData, int4(uv4) * i4TexSizeX)
            bmpData += bmpSizeX4
            uv4 += duv4
            gather_store_stride(bmpData, bmpSizeX, pixelData, int4(uv4) * i4TexSizeX)
            bmpData += bmpSizeX4
            uv4 += duv4
        for _ in range(count4)
            gather_store_stride(bmpData, bmpSizeX, pixelData, int4(uv4) * i4TexSizeX)
            bmpData += bmpSizeX4
            uv4 += duv4
        uvY = uv4.x
        for _ in range(tail4)
            *bmpData = pixelData[int(uvY)*iTexSizeX]
            bmpData += bmpSizeX
            uvY += dUVY

[jit, hint(unsafe_range_check,noalias=bmp,noalias=texture,hot,alwaysinline),unsafe_deref]
def public draw_image_region_masked(var bmp : Bitmap; texture : Texture; x, y : int; uv : float4; size : int2; mask_color:uint)
    //! draw image region with mask color
    if texture.size == int2()
        return
    let w = size.x
    let h = size.y
    let yMin = max(y, 0)
    let yMax = min(y + h, bmp.size.y)
    let xMin = max(x, 0)
    let xMax = min(x + w, bmp.size.x)
    if xMin >= xMax || yMin >= yMax
        return
    var bmpData = unsafe(addr(unsafe(bmp.data[yMin * bmp.size.x + xMin])))
    var pixelData = unsafe(addr(unsafe(texture.pixels[0])))
    let dwh = rcp(float2(w,h))
    let uvUV = uv.xy + uv.zw * float2(xMin - x, yMin - y) * dwh
    let duvUV = uv.zw * dwh
    let count = xMax - xMin
    var count4 = count >> 2
    let tail4 = count & 3
    let count16 = count4 >> 2
    count4 &= 3
    let u4 = float4(uvUV.x) + duvUV.x*float4(1.,2.,3.,4.)
    let du4 = float4(duvUV.x * 4.)
    let bmpSizeX = bmp.size.x
    let mask_color_4 = uint4(mask_color)
    let texSizeX = texture.size.x
    unsafe
        var v = uvUV.y
        for _ in range(yMin, yMax)
            let pixelPart = pixelData + (int(v) * texSizeX)
            var bmpSpan = reinterpret<uint4?> bmpData
            var u = u4
            for _1 in range(count16)
                gather_store_neq_mask(bmpSpan, pixelPart, int4(u), mask_color_4)
                bmpSpan ++
                u += du4
                gather_store_neq_mask(bmpSpan, pixelPart, int4(u), mask_color_4)
                bmpSpan ++
                u += du4
                gather_store_neq_mask(bmpSpan, pixelPart, int4(u), mask_color_4)
                bmpSpan ++
                u += du4
                gather_store_neq_mask(bmpSpan, pixelPart, int4(u), mask_color_4)
                bmpSpan ++
                u += du4
            for _2 in range(count4)
                gather_store_neq_mask(bmpSpan, pixelPart, int4(u), mask_color_4)
                bmpSpan ++
                u += du4
            var u1 = u.x
            var tailSpan = reinterpret<uint?> bmpSpan
            for xx in range(tail4)
                let pixel = pixelPart[int(u1)]
                if pixel != mask_color
                    tailSpan[xx] = pixel
                u1 += duvUV.x
            bmpData += bmpSizeX
            v += duvUV.y
