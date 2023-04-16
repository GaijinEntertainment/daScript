module rast2d private

require math

struct public Bitmap8
    //! single surface
    size : int2
    [[do_not_delete]] data : uint8?

def public clear ( var bmp:Bitmap8; color:uint8 )
    //! clear surface with solid color
    unsafe
        memset8(bmp.data, color, bmp.size.x * bmp.size.y)

[jit,hint(unsafe_range_check,noalias=bmp,hot)]
def public fill_rect ( var bmp:Bitmap8; var xmin,ymin,dx,dy:int; color:uint8 )
    //! rectangle by x,y,width,height
    fill_rect_p2(bmp,xmin,ymin,xmin+dx,ymin+dy,color)

[jit,hint(unsafe_range_check,noalias=bmp,hot)]
def public fill_rect_p2 ( var bmp:Bitmap8; var xmin,ymin,xmax,ymax:int; color:uint8 )
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
            memset8(pixels, color, span * (ymax - ymin))
        else
            for y in ymin..ymax
                memset8(pixels, color, span)
                pixels += pitch
