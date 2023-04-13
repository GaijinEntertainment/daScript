options persistent_heap

require app_opengl_glfw

require fio

[export]
def main
    var t = 0u
    raster_app_main <| $ ( backbuffer )
        t += 123743u
        unsafe
            // memset32(backbuffer.pixels, t, backbuffer.w * backbuffer.h)
            for i in 0 .. backbuffer.w * backbuffer.h
                backbuffer.pixels[i] = t





