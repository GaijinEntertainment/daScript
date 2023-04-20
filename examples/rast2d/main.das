options persistent_heap

// require llvm/llvm_jit

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
    raster_app_main <| $ ( dt, backbuffer )
        updateGame(dt)
        draw(level, backbuffer)
        set_app_sound_position(float2(player.x, player.y), float2(player.dx, player.dy))



