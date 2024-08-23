Marks function arguments, which are unused.
That way when code policies make unused arguments an error, a workaround can be provided::

    [unused_argument(ctx)]
    def private start_debug_agent(var ctx: Context)
        pass    // notice, how `ctx` is not used in the function body


