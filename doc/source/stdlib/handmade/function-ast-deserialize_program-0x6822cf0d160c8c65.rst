Deserializes the next program from the stream and calls ``block`` with it, the way the
three-argument form does, and gives the restored program ``access`` as its own: a stream carries
no file access, and a ``require_module_now`` issued from the program's macros or ``[init]``
walks through its program's access, so a reader that restores programs whose code requires
modules late passes the access it would have compiled them with. ``libGroup`` is the module
group the restored program finalizes its annotations against, the way ``compile_file`` takes
one: a function or block annotation's ``finish`` is handed this group, so it must be the
caller's own rather than an empty stand-in.
