This macro converts await(<coroutine>) expression into::

    for t in THAT
        yield t

The idea is that coroutine or generator can continuously yield from another sub-coroutine or generator.
