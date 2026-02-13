The FUNCTIONAL module implements higher-order functions and itertools-style
patterns including ``filter``, ``map``, ``reduce``, ``zip``, ``flatten``,
``islice``, ``cycle``, ``repeat``, and ``sorted``.

All functions and symbols are in "functional" module, use require to get access to it. ::

    require daslib/functional

Example: ::

    require daslib/functional

        [export]
        def main() {
            var src <- [iterator for (x in range(6)); x]
            var evens <- filter(src, @(x : int) : bool { return x % 2 == 0; })
            for (v in evens) {
                print("{v} ")
            }
            print("\n")
        }
        // output:
        // 0 2 4
