module rast2d private

require math

struct public Bitmap
    //! single surface
    w, h : int
    [[do_not_delete]] pixels : uint?

def public clear ( var bmp:Bitmap; color:uint )
    //! clear surface with solid color
    unsafe
        memset32(bmp.pixels, color, bmp.w * bmp.h)

[jit]
def public fill_rect ( var bmp:Bitmap; var xmin,ymin,xmax,ymax:int; color:uint )
    //! fill rectangle with solid color, clip to surface
    xmin = max(xmin, 0)
    xmax = min(xmax, bmp.w)
    ymin = max(ymin, 0)
    ymax = min(ymax, bmp.h)
    if xmin >= xmax || ymin >= ymax
        return
    var pitch = bmp.w
    var span = xmax - xmin
    unsafe
        var pixels = bmp.pixels + (pitch * ymin + xmin)
        for y in ymin..ymax
            memset32(pixels, color, span)
            pixels += pitch
