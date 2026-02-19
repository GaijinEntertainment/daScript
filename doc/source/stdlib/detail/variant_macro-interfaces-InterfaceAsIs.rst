Variant macro that enables ``is``, ``as``, and ``?as`` operators for
interface types declared with ``[interface]`` / ``[implements]``.

* ``ptr is IFoo``  — compile-time check (``true`` when the struct implements ``IFoo``)
* ``ptr as IFoo``  — obtains the interface proxy via the generated getter
* ``ptr ?as IFoo`` — null-safe version: returns ``null`` when the pointer is null
