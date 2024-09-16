Indicates that the function is a pinvoke function, and will be called via pinvoke machinery::

    [pinvoke, export]   // we will invoke this via pinvoke(other_context,"setup_command_processor",ch,cch)
    def setup_command_processor(ch, cch : Channel?)
        ...

