options persistent_heap

require llvm/llvm_jit

require app_opengl_glfw

require files
require game
require engine

require math

[export]
def main
    InitFiles()
    setupLevel(0)
    raster_app_main <| $ ( backbuffer )
        updateGame()
        draw(level, backbuffer)


