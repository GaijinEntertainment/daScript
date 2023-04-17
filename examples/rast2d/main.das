options persistent_heap

require llvm/llvm_jit

require app
require files
require game
require engine

require math

[export]
def main
    InitFiles()
    setupLevel(0)
    set_palette(GamePalette)
    raster_app_main <| $ ( backbuffer )
        updateGame()
        draw(level, backbuffer)


