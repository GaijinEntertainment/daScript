Similar to regular `assert` function, but always happens at compilation time.
It would also display the error message from where the asserted function was called from, not the assert line itself::

    def join(it; separator:string implicit)
        static_if !typeinfo(is_iterable it)
            concept_assert(false,"can't join non-iterable type")
            return ""
        ...

