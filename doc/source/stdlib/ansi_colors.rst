
.. _stdlib_ansi_colors:

=====================================
ANSI terminal color and style helpers
=====================================

.. das:module:: ansi_colors

The ANSI_COLORS module provides helpers for wrapping strings with ANSI escape
codes for colored and styled terminal output.

Color output is controlled by the ``use_tty_colors`` variable. Call
``init_ansi_colors`` to auto-detect support from command-line flags
(``--color`` / ``--no-color``) and environment variables (``TERM``,
``NO_COLOR``), or set ``use_tty_colors`` directly.

All functions and symbols are in "ansi_colors" module, use require to get access to it.

.. code-block:: das

    require daslib/ansi_colors

Example:

.. code-block:: das

    require daslib/ansi_colors

    [export]
    def main() {
        init_ansi_colors()
        print("{red_str("error")}: something went wrong\n")
        print("{green_str("ok")}: all good\n")
        print("{bold_str("important")}: pay attention\n")
    }



+++++++++++++
Configuration
+++++++++++++

  *  :ref:`init_ansi_colors (args: array\<string\>) <function-ansi_colors_init_ansi_colors_array_ls_string_gr_>`
  *  :ref:`init_ansi_colors () <function-ansi_colors_init_ansi_colors>`


init_ansi_colors
^^^^^^^^^^^^^^^^

.. _function-ansi_colors_init_ansi_colors_array_ls_string_gr_:

.. das:function:: init_ansi_colors(args: array<string>)

Detect color support from `args`, ``TERM``, and ``NO_COLOR`` environment variables.
Sets ``use_tty_colors`` accordingly. Respects ``--color``, ``--no-color``, and ``NO_COLOR``.


:Arguments: * **args** : array<string>

.. _function-ansi_colors_init_ansi_colors:

.. das:function:: init_ansi_colors()


++++++
Colors
++++++

  *  :ref:`blue_str (str: string) : string <function-ansi_colors_blue_str_string>`
  *  :ref:`cyan_str (str: string) : string <function-ansi_colors_cyan_str_string>`
  *  :ref:`green_str (str: string) : string <function-ansi_colors_green_str_string>`
  *  :ref:`magenta_str (str: string) : string <function-ansi_colors_magenta_str_string>`
  *  :ref:`red_str (str: string) : string <function-ansi_colors_red_str_string>`
  *  :ref:`yellow_str (str: string) : string <function-ansi_colors_yellow_str_string>`

.. _function-ansi_colors_blue_str_string:

.. das:function:: blue_str(str: string) : string

Wraps `str` with ANSI blue (``\\x1B[34m``) if colors are enabled.


:Arguments: * **str** : string

.. _function-ansi_colors_cyan_str_string:

.. das:function:: cyan_str(str: string) : string

Wraps `str` with ANSI cyan (``\\x1B[36m``) if colors are enabled.


:Arguments: * **str** : string

.. _function-ansi_colors_green_str_string:

.. das:function:: green_str(str: string) : string

Wraps `str` with ANSI green (``\\x1B[32m``) if colors are enabled.


:Arguments: * **str** : string

.. _function-ansi_colors_magenta_str_string:

.. das:function:: magenta_str(str: string) : string

Wraps `str` with ANSI magenta (``\\x1B[35m``) if colors are enabled.


:Arguments: * **str** : string

.. _function-ansi_colors_red_str_string:

.. das:function:: red_str(str: string) : string

Wraps `str` with ANSI red (``\\x1B[31m``) if colors are enabled.


:Arguments: * **str** : string

.. _function-ansi_colors_yellow_str_string:

.. das:function:: yellow_str(str: string) : string

Wraps `str` with ANSI yellow (``\\x1B[33m``) if colors are enabled.


:Arguments: * **str** : string


++++++
Styles
++++++

  *  :ref:`bold_str (str: string) : string <function-ansi_colors_bold_str_string>`
  *  :ref:`dim_str (str: string) : string <function-ansi_colors_dim_str_string>`
  *  :ref:`reset_str () : string <function-ansi_colors_reset_str>`
  *  :ref:`underline_str (str: string) : string <function-ansi_colors_underline_str_string>`

.. _function-ansi_colors_bold_str_string:

.. das:function:: bold_str(str: string) : string

Wraps `str` with ANSI bold (``\\x1B[1m``) if colors are enabled.


:Arguments: * **str** : string

.. _function-ansi_colors_dim_str_string:

.. das:function:: dim_str(str: string) : string

Wraps `str` with ANSI dim (``\\x1B[2m``) if colors are enabled.


:Arguments: * **str** : string

.. _function-ansi_colors_reset_str:

.. das:function:: reset_str() : string

Returns the ANSI reset sequence if colors are enabled, empty string otherwise.


.. _function-ansi_colors_underline_str_string:

.. das:function:: underline_str(str: string) : string

Wraps `str` with ANSI underline (``\\x1B[4m``) if colors are enabled.


:Arguments: * **str** : string


