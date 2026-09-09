Deserializes the next program from the stream and calls ``block`` with it, the way the
two-argument form does, and gives the restored program ``access`` as its own: a stream carries
no file access, and a ``require_module_now`` issued from the program's macros or ``[init]``
walks through its program's access, so a reader that restores programs whose code requires
modules late passes the access it would have compiled them with.
