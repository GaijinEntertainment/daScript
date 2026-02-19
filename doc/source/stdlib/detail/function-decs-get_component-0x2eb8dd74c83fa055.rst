Returns a copy of the named component for the given entity.
If the entity is dead or the component is not found, returns ``defval``.
The type of the component is inferred from the type of ``defval``.
Panics if the component exists but its type does not match.
