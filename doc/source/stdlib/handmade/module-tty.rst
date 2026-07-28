The TTY module answers whether a stream is attached to a real terminal, and how
wide that terminal is.

This is the gate for anything that redraws itself with a carriage return: a
progress bar written to a pipe, a redirect, or a supervising process is garbage
in the resulting log. Checking ``TERM`` is not a substitute — it stays set when
standard output is redirected. ``daslib/ansi_colors`` handles styling; this
module answers the capability question underneath it.

All functions and symbols are in "tty" module, use require to get access to it.

.. code-block:: das

    require daslib/tty

Example:

.. code-block:: das

    require daslib/tty

    [export]
    def main() {
        if (is_stdout_terminal()) {
            print("drawing a {terminal_columns()}-column progress bar\n")
        } else {
            print("output is captured — emitting plain lines instead\n")
        }
    }
