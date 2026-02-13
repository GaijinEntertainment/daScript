
.. _stdlib_apply_in_context:

================================
Cross-context evaluation helpers
================================

The APPLY_IN_CONTEXT module extends apply operations to work across
different execution contexts, enabling cross-context function invocation
with packed arguments.

All functions and symbols are in "apply_in_context" module, use require to get access to it. ::

    require daslib/apply_in_context

++++++++++++++++++++
Function annotations
++++++++++++++++++++

.. _handle-apply_in_context-apply_in_context:

.. das:attribute:: apply_in_context

[apply_in_context] function annotation.
Function is modified, so that it is called in the debug agent context, specified in the annotation.
If specified context is not installed, panic is called.

For example::
 [apply_in_context(opengl_cache)]
 def public cache_font(name:string implicit) : Font?
     ...
 ...
 let font = cache_font("Arial") // call invoked in the "opengl_cache" debug agent context


