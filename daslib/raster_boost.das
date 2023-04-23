module raster_boost private

require math
require raster public

struct public Bitmap8
    //! single surface
    size : int2
    data : array<uint8>

def public lock_u8 ( dim:auto[]; blk:block<(var arg:array<uint8>#):void> )
    let num_elem = typeinfo(sizeof dim)
    unsafe
        map_to_array(addr(dim[0]), num_elem, blk)

[sideeffects,jit,hint(unsafe_range_check,noalias=bmp)]
def public rast_fillRect ( var bmp:Bitmap8#; x,y,w,h:int; color:uint8 )
    let xmin = max(0,x)
    let xmax = min(bmp.size.x,x+w)
    if xmax <= xmin
        return
    let ymin = max(0,y)
    let ymax = min(bmp.size.y,y+h)
    if ymax <= ymin
        return
    let H = ymax-ymin
    for X in range(xmin,xmax)
        unsafe
            memset8(addr(bmp.data[X*bmp.size.y+y]),color,H)
