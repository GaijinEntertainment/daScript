Generates interface bindings for a struct. Creates a proxy class that delegates
interface method calls to the struct's own methods, and adds a ``get`InterfaceName``
method that lazily constructs the proxy. Applied via ``[implements(InterfaceName)]``.
