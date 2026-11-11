True when ``program``'s own module named a typedef that ``module`` declares. Inference
replaces an alias with the type it stands for, so the typed tree can lose every reference to
the aliasing module; this is the record of that reference. A typedef named inside
``with (module X)`` is looked up from ``X`` and does not count.
