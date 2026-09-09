The slot ``function`` holds in ``program``'s context: ``-1`` when the program never
allocated it, ``-2`` when the allocation found it unused, its function-table index
otherwise. The index is the program's, not the function's - a shared module's function
has one per program that requires it.
