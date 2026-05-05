The FIO module implements file input/output and filesystem operations.
It provides functions for reading and writing files (``fopen``, ``fread``, ``fwrite``),
directory management (``mkdir``, ``dir``), path manipulation (``join_path``,
``basename``, ``dirname``), and file metadata queries (``stat``, ``file_time``).

All functions and symbols are in "fio" module, use require to get access to it.

.. code-block:: das

    require daslib/fio

Example:

.. code-block:: das

    require daslib/fio

        [export]
        def main() {
            let fname = "_test_fio_tmp.txt"
            fopen(fname, "wb") $(f) {
                fwrite(f, "hello, daslang!")
            }
            fopen(fname, "rb") $(f) {
                let content = fread(f)
                print("{content}\n")
            }
            remove(fname)
        }
        // output:
        // hello, daslang!

Glob and pattern matching
-------------------------

The fio module also provides path-aware glob matching and expansion. ``match_glob``,
``glob``, ``glob_filtered``, ``is_glob_pattern`` and ``to_generic_path`` are the low-level
primitives; ``expand_glob`` and ``parse_file_list`` are the higher-level helpers used by
CLI tools that take a user-supplied pattern or a comma/newline-separated list of paths.

* ``match_glob(pattern, path) : bool`` — pure path-string match. ``*`` does not cross ``/``;
  ``**`` does. ``[abc]`` / ``[a-z]`` / ``[!abc]`` / ``[^abc]`` character classes are supported.
* ``glob(root, pattern, blk)`` — recursive walk + match in one pass. Callback receives
  ``/``-normalized paths regardless of host OS.
* ``glob_filtered(root, includes, excludes, blk)`` — multi-pattern walk; excludes win on conflict.
* ``is_glob_pattern(pattern) : bool`` — true if the string contains ``*``, ``?``, or ``[``.
* ``to_generic_path(s) : string`` — convert a path to use forward slashes regardless of host.
* ``expand_glob(pattern, var result)`` — expand a single glob pattern (e.g. ``src/**/*.das``)
  into a sorted list of matching file paths. Splits the literal directory prefix from the glob
  remainder, picks recursive vs shallow walk based on whether the remainder contains ``**`` or ``/``,
  filters out directories, sorts locally, and **appends** to ``result``.
* ``parse_file_list(file, var result)`` — parse a comma- or newline-separated list of files,
  directories, and globs into a flat list of paths. Strips whitespace, skips empty entries,
  literal entries pass through, glob entries go through ``expand_glob``. Also **appends** to ``result``.

.. code-block:: das

    require daslib/fio

    [export]
    def main() {
        // expand a single pattern
        var das_files : array<string>
        expand_glob("daslib/**/*.das", das_files)
        for (f in das_files) { print("{f}\n") }

        // parse a user-supplied paths argument: any mix of plain paths and globs
        var inputs : array<string>
        parse_file_list("README.md,daslib/*.das,tests/fio/*.das", inputs)
    }

For runnable examples covering every flavor, see ``tutorials/language/54_glob.das``.
