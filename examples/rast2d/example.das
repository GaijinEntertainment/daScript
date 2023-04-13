options persistent_heap

require llvm/llvm_jit

require daslib/random

require app_opengl_glfw

require fio

var seed = random_seed(12345678)

def draw ( var backbuffer : Bitmap )
    backbuffer |> clear(0xFF000000)
    // stress
    let limit = int4(backbuffer.w, backbuffer.h, 64, 64)
    for s in 0..10000
        var box = random_int4(seed)
        box = box - limit*(box/limit)
        box.zw += box.xy
        let color = random_uint(seed)
        backbuffer |> fill_rect( box.x, box.y, box.z, box.w, color)
    let xc = backbuffer.w / 2
    let yc = backbuffer.h / 2
    for y in -4..4
        for x in -4..4
            let color = ((x^y)&1)!=0 ? 0xFF3D300D : 0xFFE6C5A4
            backbuffer |> fill_rect( xc + x * 64, yc + y * 64, xc + (x+1)*64, yc + (y+1)*64, color)
    var w = 1
    var x = 0
    for t in range(20)
        backbuffer |> fill_rect ( x, 0, x+w, backbuffer.h, (t&1)!=0 ? 0xFF000000 : 0xFFFFFFFF)
        x += w
        w ++

[export]
def main
    raster_app_main <| $ ( backbuffer )
        draw(backbuffer)



