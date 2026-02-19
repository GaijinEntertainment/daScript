This macro converts each(bitfield) to the following code::
    generator<string>() <|
        yield "field1"
        yield "field2"
        ...
        return false
