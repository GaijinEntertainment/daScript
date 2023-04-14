options persistent_heap

require llvm/llvm_jit

require daslib/random

require app_opengl_glfw

require fio

var seed = random_seed(12345678)

[jit]
def draw ( var backbuffer : Bitmap )
    backbuffer |> clear(0xFF000000)
    // stress
    let limit = int4(backbuffer.size.x, backbuffer.size.y, 64, 64)
    for s in 0..10000
        var box = random_int4(seed)
        box = box - limit*(box/limit)
        box.zw += box.xy
        let color = random_uint(seed)
        backbuffer |> fill_rect_p2( box.x, box.y, box.z, box.w, color)
    let xc = backbuffer.size.x / 2
    let yc = backbuffer.size.y / 2
    for y in -4..4
        for x in -4..4
            let color = ((x^y)&1)!=0 ? 0xFF3D300D : 0xFFE6C5A4
            backbuffer |> fill_rect_p2( xc + x * 64, yc + y * 64, xc + (x+1)*64, yc + (y+1)*64, color)
    var w = 1
    var x = 0
    for t in range(20)
        backbuffer |> fill_rect_p2 ( x, 0, x+w, backbuffer.size.y, (t&1)!=0 ? 0xFF000000 : 0xFFFFFFFF)
        x += w
        w ++

[export]
def main
    raster_app_main <| $ ( backbuffer )
        draw(backbuffer)



