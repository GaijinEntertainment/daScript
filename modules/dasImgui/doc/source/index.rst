.. dasImgui documentation master file.

dasImgui documentation
======================

Part of the daslang ecosystem. See also the `daslang documentation
<https://daslang.io/doc/>`_ and `daslang.io <https://daslang.io>`_.

dasImgui is the daslang binding for `Dear ImGui <https://github.com/ocornut/imgui>`_,
including the boost macro layer, live-reload integration, and a Playwright-style
testing harness.

**Source code**: https://github.com/borisbat/dasImgui

**Issues**: https://github.com/borisbat/dasImgui/issues

Install
=======

Via daspkg:

.. code-block:: bash

   daslang utils/daspkg/main.das -- install github.com/borisbat/dasImgui

Or add to your project's ``.das_package``:

.. code-block:: das

   [export]
   def dependencies(version : string) {
       require_package("github.com/borisbat/dasImgui")
   }

Then run ``daspkg install``.

Start with :doc:`tutorials/application_lifecycle`: one application structure
for standalone, live-reload, and headless execution, including the required
between-frame garbage-collection boundary for long-running processes.

----

This site documents the **v2.0** surface. The legacy ``daslib/imgui_boost`` (v1)
is not documented here; v1 users pin to the older daspkg release.

.. toctree::
   :maxdepth: 2
   :caption: Contents

   stdlib/index
   tutorials/index
   internal_families
