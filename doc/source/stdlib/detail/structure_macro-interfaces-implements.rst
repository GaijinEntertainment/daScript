Generates interface bindings for a struct. Creates a proxy class that delegates
interface method calls to the struct's own methods, and adds a ``get`InterfaceName``
method that lazily constructs the proxy. Applied via ``[implements(InterfaceName)]``.

If the interface inherits from a parent interface (``class IChild : IParent``),
ancestor getters are generated automatically so that ``is``/``as``/``?as`` work
with all ancestor interfaces.

At ``finish`` time, verifies that all abstract interface methods are implemented.
Methods with default bodies in the interface class are optional — the proxy
inherits them via class hierarchy.
