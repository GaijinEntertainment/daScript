Returns the macro context of ``module``, giving it a context mutex so ``invoke_in_context`` can lock it. Throws when the module has no macro context - ask ``has_macro_context`` first.
