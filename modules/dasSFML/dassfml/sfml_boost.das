options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module sfml_boost shared

require sfml public

enum WindowStyle
    None       = 0      //!< No border / title bar (this flag and all others are mutually exclusive)
    Titlebar   = 1 << 0 //!< Title bar + fixed border
    Resize     = 1 << 1 //!< Title bar + resizable border + maximize button
    Close      = 1 << 2 //!< Title bar + close button
    Fullscreen = 1 << 3 //!< Fullscreen mode (this flag and all others are mutually exclusive)
    Default = (1<<0) | (1<<1) | (1<<2) //!< Default window style

def close ( var win:RenderWindow implicit )
    close(cast<Window> win)