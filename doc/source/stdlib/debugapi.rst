
.. _stdlib_debugapi:

===============
Debug agent API
===============

.. das:module:: debugapi

The DEBUGAPI module provides the debug agent infrastructure — creating, installing,
and communicating with persistent debug agents that live in their own forked contexts.
It supports cross-context function invocation, agent method calls, log interception,
data and stack walking, instrumentation, and breakpoint management.

See :ref:`tutorial_debug_agents` and :ref:`tutorial_data_walker` for hands-on tutorials.

All functions and symbols are in "debugapi" module, use require to get access to it.

.. code-block:: das

    require debugapi



++++++++++
Structures
++++++++++

.. _struct-debugapi-DapiArray:

.. das:attribute:: DapiArray

Lightweight descriptor for an array encountered during data walking.

:Fields: * **data** : void? - Pointer to the array data.

         * **size** : uint - Number of elements currently in the array.

         * **capacity** : uint - Allocated capacity of the array.

         * **lock** : uint - Reference count lock for the array.

         * **flags** : bitfield<_shared;_hopeless> - Bitfield flags (`_shared`, `_hopeless`).



.. _struct-debugapi-DapiTable:

.. das:attribute:: DapiTable : DapiArray

Lightweight descriptor for a table encountered during data walking.  Extends `DapiArray`.

:Fields: * **keys** : void? - Pointer to the table keys data.

         * **hashes** : uint? - Pointer to the hash values array.



.. _struct-debugapi-DapiBlock:

.. das:attribute:: DapiBlock

Lightweight descriptor for a block encountered during data walking.

:Fields: * **stackOffset** : uint - Stack offset of the block.

         * **argumentsOffset** : uint - Arguments offset on the stack.

         * **body** : void? - Pointer to the block's simulation node (`SimNode`).

         * **aotFunction** : void? - Pointer to the AOT-compiled function (if any).

         * **functionArguments** : float4? - Pointer to the block's function arguments.

         * **info** :  :ref:`FuncInfo <handle-rtti-FuncInfo>`? - Pointer to the block's `FuncInfo` metadata.



.. _struct-debugapi-DapiFunc:

.. das:attribute:: DapiFunc

Lightweight descriptor for a function pointer encountered during data walking.

:Fields: * **index** : int - Index of the function in the context's function table.



.. _struct-debugapi-DapiLambda:

.. das:attribute:: DapiLambda

Lightweight descriptor for a lambda encountered during data walking.

:Fields: * **captured** : void? - Pointer to the lambda's captured variable block.



.. _struct-debugapi-DapiSequence:

.. das:attribute:: DapiSequence

Lightweight descriptor for an iterator/sequence encountered during data walking.

:Fields: * **iter** : void? - Pointer to the underlying `Iterator` object.




++++++++++++++++++
Handled structures
++++++++++++++++++

.. _handle-debugapi-Prologue:

.. das:attribute:: Prologue

Annotation for inspecting function call prologues.  Provides access to call metadata inside the annotated block.

:Fields: * **_block** : block<void>? - The block to execute with prologue context.

         * **info** :  :ref:`FuncInfo <handle-rtti-FuncInfo>`? - Pointer to the `FuncInfo` of the called function.

         * **arguments** : any? - Pointer to the function argument values.

         * **fileName** : string - Source file name of the call site.

         * **cmres** : void? - Pointer to the copy-on-return result slot.

         * **line** :  :ref:`LineInfo <handle-rtti-LineInfo>`? - Line number of the call site.

         * **stackSize** : int - Stack size used by the function.



.. _handle-debugapi-DebugAgent:

.. das:attribute:: DebugAgent

:Fields: * **isThreadLocal** : bool - Annotation for declaring a debug agent class. Generates the boilerplate to subclass `DebugAgent` from C++. The optional `isThreadLocal` field installs as a thread-local agent.



.. _handle-debugapi-DataWalker:

.. das:attribute:: DataWalker

Annotation for declaring a data walker class. Generates the boilerplate to subclass `DataWalker` from C++.



.. _handle-debugapi-StackWalker:

.. das:attribute:: StackWalker

Annotation for declaring a stack walker class. Generates the boilerplate to subclass `StackWalker` from C++.




+++++++
Classes
+++++++

.. _struct-debugapi-DapiDebugAgent:

.. das:attribute:: DapiDebugAgent

:Fields: * **thisAgent** :  :ref:`DebugAgent <handle-debugapi-DebugAgent>`? - Base class for debug agents.  Subclass and override methods (`onLog`, `onCollect`, `onBreakpoint`, etc.) to intercept runtime events. Register with `install_new_debug_agent`.



.. _struct-debugapi-DapiDataWalker:

.. das:attribute:: DapiDataWalker

Base class for walking daslang data structures.  Subclass and override per-type visitor methods (`Int`, `Float`, `String`, `Array`, `Table`, etc.) to inspect values. Create with `make_data_walker`.



.. _struct-debugapi-DapiStackWalker:

.. das:attribute:: DapiStackWalker

Base class for walking the call stack. Subclass and override `onBeforeCall`, `onAfterPop`, `onArgument`, `onVariable`, etc. Create with `make_stack_walker`.




+++++++++++++++
Agent lifecycle
+++++++++++++++

  *  :ref:`delete_debug_agent_context (category: string) <function-debugapi_delete_debug_agent_context_string>`
  *  :ref:`fork_debug_agent_context (function: function\<():void\>) <function-debugapi_fork_debug_agent_context_function_ls__c_void_gr_>`
  *  :ref:`get_debug_agent_context (category: string) : Context& <function-debugapi_get_debug_agent_context_string>`
  *  :ref:`has_debug_agent_context (category: string) : bool <function-debugapi_has_debug_agent_context_string>`
  *  :ref:`install_debug_agent (agent: smart_ptr\<DebugAgent\>; category: string) <function-debugapi_install_debug_agent_smart_ptr_ls_DebugAgent_gr__string>`
  *  :ref:`install_debug_agent_thread_local (agent: smart_ptr\<DebugAgent\>) <function-debugapi_install_debug_agent_thread_local_smart_ptr_ls_DebugAgent_gr_>`
  *  :ref:`install_new_debug_agent (var agentPtr: auto; category: string) : auto <function-debugapi_install_new_debug_agent_auto_string_0x25>`
  *  :ref:`install_new_thread_local_debug_agent (var agentPtr: auto) : auto <function-debugapi_install_new_thread_local_debug_agent_auto_0x31>`
  *  :ref:`is_in_debug_agent_creation () : bool <function-debugapi_is_in_debug_agent_creation>`
  *  :ref:`lock_debug_agent (block: block\<():void\>) <function-debugapi_lock_debug_agent_block_ls__c_void_gr_>`

.. _function-debugapi_delete_debug_agent_context_string:

.. das:function:: delete_debug_agent_context(category: string)

Removes the debug agent with the given category name.  Notifies all other agents via `onUninstall`, then destroys the agent and its context.  Safe no-op if the agent does not exist.


:Arguments: * **category** : string implicit

.. _function-debugapi_fork_debug_agent_context_function_ls__c_void_gr_:

.. das:function:: fork_debug_agent_context(function: function<():void>)

Clones the current context and calls the setup function inside the clone.  The cloned context becomes an agent context that stays resident for the lifetime of the program.


:Arguments: * **function** : function<void>

.. _function-debugapi_get_debug_agent_context_string:

.. das:function:: get_debug_agent_context(category: string) : Context&

Returns a reference to the `Context` of the named debug agent.  Panics if the agent does not exist  check with `has_debug_agent_context` first.


:Arguments: * **category** : string implicit

.. _function-debugapi_has_debug_agent_context_string:

.. das:function:: has_debug_agent_context(category: string) : bool

Returns `true` if a debug agent with the given category name is currently installed.


:Arguments: * **category** : string implicit

.. _function-debugapi_install_debug_agent_smart_ptr_ls_DebugAgent_gr__string:

.. das:function:: install_debug_agent(agent: smart_ptr<DebugAgent>; category: string)

Installs a low-level `smart_ptr<DebugAgent>` under the given category name.  Prefer `install_new_debug_agent` for the high-level pattern.


:Arguments: * **agent** : smart_ptr< :ref:`DebugAgent <handle-debugapi-DebugAgent>`> implicit

            * **category** : string implicit

.. _function-debugapi_install_debug_agent_thread_local_smart_ptr_ls_DebugAgent_gr_:

.. das:function:: install_debug_agent_thread_local(agent: smart_ptr<DebugAgent>)

Installs a low-level ``smart_ptr<DebugAgent>`` as the thread-local debug agent.  There can be only one thread-local agent per thread — installing a new one replaces the previous.


:Arguments: * **agent** : smart_ptr< :ref:`DebugAgent <handle-debugapi-DebugAgent>`> implicit

.. _function-debugapi_install_new_debug_agent_auto_string_0x25:

.. das:function:: install_new_debug_agent(agentPtr: auto; category: string) : auto

Creates and installs a debug agent from a `DapiDebugAgent` subclass instance.  This is the recommended high-level function for agent installation.


:Arguments: * **agentPtr** : auto

            * **category** : string

.. _function-debugapi_install_new_thread_local_debug_agent_auto_0x31:

.. das:function:: install_new_thread_local_debug_agent(agentPtr: auto) : auto

Creates and installs a thread-local debug agent from a ``DapiDebugAgent`` subclass instance.  There can be only one thread-local agent per thread — installing a new one replaces the previous.  Use ``invoke_debug_agent_method("", ...)`` to communicate with it.


:Arguments: * **agentPtr** : auto

.. _function-debugapi_is_in_debug_agent_creation:

.. das:function:: is_in_debug_agent_creation() : bool

Returns `true` if the current thread is inside a `fork_debug_agent_context` call.  Used in auto-start guards to prevent recursive agent creation.


.. _function-debugapi_lock_debug_agent_block_ls__c_void_gr_:

.. das:function:: lock_debug_agent(block: block<():void>)

.. warning::
  This is unsafe operation.

Executes the block while holding the global debug agent mutex.  Use when iterating or modifying agent state that must be thread-safe.


:Arguments: * **block** : block<void> implicit


++++++++++++++++++++++++
Cross-context invocation
++++++++++++++++++++++++

  *  :ref:`invoke_debug_agent_function (arg0: string; arg1: string; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any) <function-debugapi_invoke_debug_agent_function_string_string_any_any_any_any_any>`
  *  :ref:`invoke_debug_agent_function (arg0: string; arg1: string; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any; arg8: any; arg9: any; arg10: any; arg11: any) <function-debugapi_invoke_debug_agent_function_string_string_any_any_any_any_any_any_any_any_any_any>`
  *  :ref:`invoke_debug_agent_function (arg0: string; arg1: string; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any; arg8: any; arg9: any) <function-debugapi_invoke_debug_agent_function_string_string_any_any_any_any_any_any_any_any>`
  *  :ref:`invoke_debug_agent_function (arg0: string; arg1: string; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any; arg8: any; arg9: any; arg10: any) <function-debugapi_invoke_debug_agent_function_string_string_any_any_any_any_any_any_any_any_any>`
  *  :ref:`invoke_debug_agent_function (arg0: string; arg1: string; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any; arg8: any) <function-debugapi_invoke_debug_agent_function_string_string_any_any_any_any_any_any_any>`
  *  :ref:`invoke_debug_agent_function (arg0: string; arg1: string; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any) <function-debugapi_invoke_debug_agent_function_string_string_any_any_any_any_any_any>`
  *  :ref:`invoke_debug_agent_function (arg0: string; arg1: string; arg2: any) <function-debugapi_invoke_debug_agent_function_string_string_any>`
  *  :ref:`invoke_debug_agent_function (arg0: string; arg1: string; arg2: any; arg3: any; arg4: any; arg5: any) <function-debugapi_invoke_debug_agent_function_string_string_any_any_any_any>`
  *  :ref:`invoke_debug_agent_function (arg0: string; arg1: string; arg2: any; arg3: any; arg4: any) <function-debugapi_invoke_debug_agent_function_string_string_any_any_any>`
  *  :ref:`invoke_debug_agent_function (arg0: string; arg1: string; arg2: any; arg3: any) <function-debugapi_invoke_debug_agent_function_string_string_any_any>`
  *  :ref:`invoke_debug_agent_function (arg0: string; arg1: string) <function-debugapi_invoke_debug_agent_function_string_string>`
  *  :ref:`invoke_debug_agent_method (arg0: string; arg1: string; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any; arg8: any; arg9: any; arg10: any; arg11: any) <function-debugapi_invoke_debug_agent_method_string_string_any_any_any_any_any_any_any_any_any_any>`
  *  :ref:`invoke_debug_agent_method (arg0: string; arg1: string; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any; arg8: any; arg9: any) <function-debugapi_invoke_debug_agent_method_string_string_any_any_any_any_any_any_any_any>`
  *  :ref:`invoke_debug_agent_method (arg0: string; arg1: string; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any; arg8: any) <function-debugapi_invoke_debug_agent_method_string_string_any_any_any_any_any_any_any>`
  *  :ref:`invoke_debug_agent_method (arg0: string; arg1: string; arg2: any) <function-debugapi_invoke_debug_agent_method_string_string_any>`
  *  :ref:`invoke_debug_agent_method (arg0: string; arg1: string; arg2: any; arg3: any; arg4: any; arg5: any) <function-debugapi_invoke_debug_agent_method_string_string_any_any_any_any>`
  *  :ref:`invoke_debug_agent_method (arg0: string; arg1: string; arg2: any; arg3: any) <function-debugapi_invoke_debug_agent_method_string_string_any_any>`
  *  :ref:`invoke_debug_agent_method (arg0: string; arg1: string; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any) <function-debugapi_invoke_debug_agent_method_string_string_any_any_any_any_any>`
  *  :ref:`invoke_debug_agent_method (arg0: string; arg1: string; arg2: any; arg3: any; arg4: any) <function-debugapi_invoke_debug_agent_method_string_string_any_any_any>`
  *  :ref:`invoke_debug_agent_method (arg0: string; arg1: string; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any) <function-debugapi_invoke_debug_agent_method_string_string_any_any_any_any_any_any>`
  *  :ref:`invoke_debug_agent_method (arg0: string; arg1: string) <function-debugapi_invoke_debug_agent_method_string_string>`
  *  :ref:`invoke_debug_agent_method (arg0: string; arg1: string; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any; arg8: any; arg9: any; arg10: any) <function-debugapi_invoke_debug_agent_method_string_string_any_any_any_any_any_any_any_any_any>`
  *  :ref:`invoke_in_context (arg0: any; arg1: function\<():void\>; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any; arg8: any; arg9: any; arg10: any; arg11: any) <function-debugapi_invoke_in_context_any_function_ls__c_void_gr__any_any_any_any_any_any_any_any_any_any>`
  *  :ref:`invoke_in_context (arg0: any; arg1: function\<():void\>; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any; arg8: any; arg9: any; arg10: any) <function-debugapi_invoke_in_context_any_function_ls__c_void_gr__any_any_any_any_any_any_any_any_any>`
  *  :ref:`invoke_in_context (arg0: any; arg1: lambda\<():void\>) <function-debugapi_invoke_in_context_any_lambda_ls__c_void_gr_>`
  *  :ref:`invoke_in_context (arg0: any; arg1: function\<():void\>; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any; arg8: any) <function-debugapi_invoke_in_context_any_function_ls__c_void_gr__any_any_any_any_any_any_any>`
  *  :ref:`invoke_in_context (arg0: any; arg1: function\<():void\>; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any) <function-debugapi_invoke_in_context_any_function_ls__c_void_gr__any_any_any_any_any>`
  *  :ref:`invoke_in_context (arg0: any; arg1: function\<():void\>; arg2: any; arg3: any; arg4: any; arg5: any) <function-debugapi_invoke_in_context_any_function_ls__c_void_gr__any_any_any_any>`
  *  :ref:`invoke_in_context (arg0: any; arg1: function\<():void\>; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any) <function-debugapi_invoke_in_context_any_function_ls__c_void_gr__any_any_any_any_any_any>`
  *  :ref:`invoke_in_context (arg0: any; arg1: function\<():void\>; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any; arg8: any; arg9: any) <function-debugapi_invoke_in_context_any_function_ls__c_void_gr__any_any_any_any_any_any_any_any>`
  *  :ref:`invoke_in_context (arg0: any; arg1: lambda\<():void\>; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any; arg8: any; arg9: any; arg10: any) <function-debugapi_invoke_in_context_any_lambda_ls__c_void_gr__any_any_any_any_any_any_any_any_any>`
  *  :ref:`invoke_in_context (arg0: any; arg1: lambda\<():void\>; arg2: any) <function-debugapi_invoke_in_context_any_lambda_ls__c_void_gr__any>`
  *  :ref:`invoke_in_context (arg0: any; arg1: lambda\<():void\>; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any) <function-debugapi_invoke_in_context_any_lambda_ls__c_void_gr__any_any_any_any_any>`
  *  :ref:`invoke_in_context (arg0: any; arg1: lambda\<():void\>; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any; arg8: any; arg9: any) <function-debugapi_invoke_in_context_any_lambda_ls__c_void_gr__any_any_any_any_any_any_any_any>`
  *  :ref:`invoke_in_context (arg0: any; arg1: lambda\<():void\>; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any; arg8: any; arg9: any; arg10: any; arg11: any) <function-debugapi_invoke_in_context_any_lambda_ls__c_void_gr__any_any_any_any_any_any_any_any_any_any>`
  *  :ref:`invoke_in_context (arg0: any; arg1: lambda\<():void\>; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any) <function-debugapi_invoke_in_context_any_lambda_ls__c_void_gr__any_any_any_any_any_any>`
  *  :ref:`invoke_in_context (arg0: any; arg1: lambda\<():void\>; arg2: any; arg3: any; arg4: any; arg5: any) <function-debugapi_invoke_in_context_any_lambda_ls__c_void_gr__any_any_any_any>`
  *  :ref:`invoke_in_context (arg0: any; arg1: lambda\<():void\>; arg2: any; arg3: any; arg4: any) <function-debugapi_invoke_in_context_any_lambda_ls__c_void_gr__any_any_any>`
  *  :ref:`invoke_in_context (arg0: any; arg1: lambda\<():void\>; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any; arg8: any) <function-debugapi_invoke_in_context_any_lambda_ls__c_void_gr__any_any_any_any_any_any_any>`
  *  :ref:`invoke_in_context (arg0: any; arg1: function\<():void\>; arg2: any) <function-debugapi_invoke_in_context_any_function_ls__c_void_gr__any>`
  *  :ref:`invoke_in_context (arg0: any; arg1: string; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any; arg8: any; arg9: any; arg10: any; arg11: any) <function-debugapi_invoke_in_context_any_string_any_any_any_any_any_any_any_any_any_any>`
  *  :ref:`invoke_in_context (arg0: any; arg1: string; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any; arg8: any; arg9: any; arg10: any) <function-debugapi_invoke_in_context_any_string_any_any_any_any_any_any_any_any_any>`
  *  :ref:`invoke_in_context (arg0: any; arg1: function\<():void\>) <function-debugapi_invoke_in_context_any_function_ls__c_void_gr_>`
  *  :ref:`invoke_in_context (arg0: any; arg1: function\<():void\>; arg2: any; arg3: any) <function-debugapi_invoke_in_context_any_function_ls__c_void_gr__any_any>`
  *  :ref:`invoke_in_context (arg0: any; arg1: string; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any) <function-debugapi_invoke_in_context_any_string_any_any_any_any_any_any>`
  *  :ref:`invoke_in_context (arg0: any; arg1: string; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any) <function-debugapi_invoke_in_context_any_string_any_any_any_any_any>`
  *  :ref:`invoke_in_context (arg0: any; arg1: string; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any; arg8: any) <function-debugapi_invoke_in_context_any_string_any_any_any_any_any_any_any>`
  *  :ref:`invoke_in_context (arg0: any; arg1: string; arg2: any; arg3: any; arg4: any) <function-debugapi_invoke_in_context_any_string_any_any_any>`
  *  :ref:`invoke_in_context (arg0: any; arg1: string; arg2: any) <function-debugapi_invoke_in_context_any_string_any>`
  *  :ref:`invoke_in_context (arg0: any; arg1: string) <function-debugapi_invoke_in_context_any_string>`
  *  :ref:`invoke_in_context (arg0: any; arg1: string; arg2: any; arg3: any) <function-debugapi_invoke_in_context_any_string_any_any>`
  *  :ref:`invoke_in_context (arg0: any; arg1: string; arg2: any; arg3: any; arg4: any; arg5: any) <function-debugapi_invoke_in_context_any_string_any_any_any_any>`
  *  :ref:`invoke_in_context (arg0: any; arg1: string; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any; arg8: any; arg9: any) <function-debugapi_invoke_in_context_any_string_any_any_any_any_any_any_any_any>`
  *  :ref:`invoke_in_context (arg0: any; arg1: function\<():void\>; arg2: any; arg3: any; arg4: any) <function-debugapi_invoke_in_context_any_function_ls__c_void_gr__any_any_any>`
  *  :ref:`invoke_in_context (arg0: any; arg1: lambda\<():void\>; arg2: any; arg3: any) <function-debugapi_invoke_in_context_any_lambda_ls__c_void_gr__any_any>`


invoke_debug_agent_function
^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. _function-debugapi_invoke_debug_agent_function_string_string_any_any_any_any_any:

.. das:function:: invoke_debug_agent_function(arg0: string; arg1: string; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any)

.. warning::
  This is unsafe operation.

Calls an ``[export, pinvoke]`` function in the named agent's context.  Similar to ``invoke_in_context`` but resolves the agent context automatically from the category name.

When *category* is an empty string (``""``), the call targets the **thread-local** debug agent's context instead of a globally named one.  There can be only one thread-local agent per thread, so no name is needed.  The thread-local path is faster because it skips the global agent map lookup.


:Arguments: * **arg0** : string

            * **arg1** : string

            * **arg2** : any

            * **arg3** : any

            * **arg4** : any

            * **arg5** : any

            * **arg6** : any

.. _function-debugapi_invoke_debug_agent_function_string_string_any_any_any_any_any_any_any_any_any_any:

.. das:function:: invoke_debug_agent_function(arg0: string; arg1: string; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any; arg8: any; arg9: any; arg10: any; arg11: any)

.. _function-debugapi_invoke_debug_agent_function_string_string_any_any_any_any_any_any_any_any:

.. das:function:: invoke_debug_agent_function(arg0: string; arg1: string; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any; arg8: any; arg9: any)

.. _function-debugapi_invoke_debug_agent_function_string_string_any_any_any_any_any_any_any_any_any:

.. das:function:: invoke_debug_agent_function(arg0: string; arg1: string; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any; arg8: any; arg9: any; arg10: any)

.. _function-debugapi_invoke_debug_agent_function_string_string_any_any_any_any_any_any_any:

.. das:function:: invoke_debug_agent_function(arg0: string; arg1: string; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any; arg8: any)

.. _function-debugapi_invoke_debug_agent_function_string_string_any_any_any_any_any_any:

.. das:function:: invoke_debug_agent_function(arg0: string; arg1: string; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any)

.. _function-debugapi_invoke_debug_agent_function_string_string_any:

.. das:function:: invoke_debug_agent_function(arg0: string; arg1: string; arg2: any)

.. _function-debugapi_invoke_debug_agent_function_string_string_any_any_any_any:

.. das:function:: invoke_debug_agent_function(arg0: string; arg1: string; arg2: any; arg3: any; arg4: any; arg5: any)

.. _function-debugapi_invoke_debug_agent_function_string_string_any_any_any:

.. das:function:: invoke_debug_agent_function(arg0: string; arg1: string; arg2: any; arg3: any; arg4: any)

.. _function-debugapi_invoke_debug_agent_function_string_string_any_any:

.. das:function:: invoke_debug_agent_function(arg0: string; arg1: string; arg2: any; arg3: any)

.. _function-debugapi_invoke_debug_agent_function_string_string:

.. das:function:: invoke_debug_agent_function(arg0: string; arg1: string)

----


invoke_debug_agent_method
^^^^^^^^^^^^^^^^^^^^^^^^^

.. _function-debugapi_invoke_debug_agent_method_string_string_any_any_any_any_any_any_any_any_any_any:

.. das:function:: invoke_debug_agent_method(arg0: string; arg1: string; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any; arg8: any; arg9: any; arg10: any; arg11: any)

.. warning::
  This is unsafe operation.

Calls a method on the debug agent's class instance by name.  The first argument is the agent category, the second is the method name, followed by up to 10 user arguments.  The agent's ``self`` is passed automatically.

When *category* is an empty string (``""``), the call targets the **thread-local** debug agent instead of a globally named one.  There can be only one thread-local agent per thread — that is why it needs no name.  The thread-local path is faster than a named agent lookup because it skips the global map search.


:Arguments: * **arg0** : string

            * **arg1** : string

            * **arg2** : any

            * **arg3** : any

            * **arg4** : any

            * **arg5** : any

            * **arg6** : any

            * **arg7** : any

            * **arg8** : any

            * **arg9** : any

            * **arg10** : any

            * **arg11** : any

.. _function-debugapi_invoke_debug_agent_method_string_string_any_any_any_any_any_any_any_any:

.. das:function:: invoke_debug_agent_method(arg0: string; arg1: string; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any; arg8: any; arg9: any)

.. _function-debugapi_invoke_debug_agent_method_string_string_any_any_any_any_any_any_any:

.. das:function:: invoke_debug_agent_method(arg0: string; arg1: string; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any; arg8: any)

.. _function-debugapi_invoke_debug_agent_method_string_string_any:

.. das:function:: invoke_debug_agent_method(arg0: string; arg1: string; arg2: any)

.. _function-debugapi_invoke_debug_agent_method_string_string_any_any_any_any:

.. das:function:: invoke_debug_agent_method(arg0: string; arg1: string; arg2: any; arg3: any; arg4: any; arg5: any)

.. _function-debugapi_invoke_debug_agent_method_string_string_any_any:

.. das:function:: invoke_debug_agent_method(arg0: string; arg1: string; arg2: any; arg3: any)

.. _function-debugapi_invoke_debug_agent_method_string_string_any_any_any_any_any:

.. das:function:: invoke_debug_agent_method(arg0: string; arg1: string; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any)

.. _function-debugapi_invoke_debug_agent_method_string_string_any_any_any:

.. das:function:: invoke_debug_agent_method(arg0: string; arg1: string; arg2: any; arg3: any; arg4: any)

.. _function-debugapi_invoke_debug_agent_method_string_string_any_any_any_any_any_any:

.. das:function:: invoke_debug_agent_method(arg0: string; arg1: string; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any)

.. _function-debugapi_invoke_debug_agent_method_string_string:

.. das:function:: invoke_debug_agent_method(arg0: string; arg1: string)

.. _function-debugapi_invoke_debug_agent_method_string_string_any_any_any_any_any_any_any_any_any:

.. das:function:: invoke_debug_agent_method(arg0: string; arg1: string; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any; arg8: any; arg9: any; arg10: any)

----


invoke_in_context
^^^^^^^^^^^^^^^^^

.. _function-debugapi_invoke_in_context_any_function_ls__c_void_gr__any_any_any_any_any_any_any_any_any_any:

.. das:function:: invoke_in_context(arg0: any; arg1: function<():void>; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any; arg8: any; arg9: any; arg10: any; arg11: any)

.. warning::
  This is unsafe operation.

Calls a function in another context.  Accepts a `Context` reference and either a function name (string), a `function` pointer, or a `lambda`, plus up to 10 extra arguments.  Target functions must be marked `[export, pinvoke]`.


:Arguments: * **arg0** : any

            * **arg1** : function<void>

            * **arg2** : any

            * **arg3** : any

            * **arg4** : any

            * **arg5** : any

            * **arg6** : any

            * **arg7** : any

            * **arg8** : any

            * **arg9** : any

            * **arg10** : any

            * **arg11** : any

.. _function-debugapi_invoke_in_context_any_function_ls__c_void_gr__any_any_any_any_any_any_any_any_any:

.. das:function:: invoke_in_context(arg0: any; arg1: function<():void>; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any; arg8: any; arg9: any; arg10: any)

.. _function-debugapi_invoke_in_context_any_lambda_ls__c_void_gr_:

.. das:function:: invoke_in_context(arg0: any; arg1: lambda<():void>)

.. _function-debugapi_invoke_in_context_any_function_ls__c_void_gr__any_any_any_any_any_any_any:

.. das:function:: invoke_in_context(arg0: any; arg1: function<():void>; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any; arg8: any)

.. _function-debugapi_invoke_in_context_any_function_ls__c_void_gr__any_any_any_any_any:

.. das:function:: invoke_in_context(arg0: any; arg1: function<():void>; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any)

.. _function-debugapi_invoke_in_context_any_function_ls__c_void_gr__any_any_any_any:

.. das:function:: invoke_in_context(arg0: any; arg1: function<():void>; arg2: any; arg3: any; arg4: any; arg5: any)

.. _function-debugapi_invoke_in_context_any_function_ls__c_void_gr__any_any_any_any_any_any:

.. das:function:: invoke_in_context(arg0: any; arg1: function<():void>; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any)

.. _function-debugapi_invoke_in_context_any_function_ls__c_void_gr__any_any_any_any_any_any_any_any:

.. das:function:: invoke_in_context(arg0: any; arg1: function<():void>; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any; arg8: any; arg9: any)

.. _function-debugapi_invoke_in_context_any_lambda_ls__c_void_gr__any_any_any_any_any_any_any_any_any:

.. das:function:: invoke_in_context(arg0: any; arg1: lambda<():void>; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any; arg8: any; arg9: any; arg10: any)

.. _function-debugapi_invoke_in_context_any_lambda_ls__c_void_gr__any:

.. das:function:: invoke_in_context(arg0: any; arg1: lambda<():void>; arg2: any)

.. _function-debugapi_invoke_in_context_any_lambda_ls__c_void_gr__any_any_any_any_any:

.. das:function:: invoke_in_context(arg0: any; arg1: lambda<():void>; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any)

.. _function-debugapi_invoke_in_context_any_lambda_ls__c_void_gr__any_any_any_any_any_any_any_any:

.. das:function:: invoke_in_context(arg0: any; arg1: lambda<():void>; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any; arg8: any; arg9: any)

.. _function-debugapi_invoke_in_context_any_lambda_ls__c_void_gr__any_any_any_any_any_any_any_any_any_any:

.. das:function:: invoke_in_context(arg0: any; arg1: lambda<():void>; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any; arg8: any; arg9: any; arg10: any; arg11: any)

.. _function-debugapi_invoke_in_context_any_lambda_ls__c_void_gr__any_any_any_any_any_any:

.. das:function:: invoke_in_context(arg0: any; arg1: lambda<():void>; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any)

.. _function-debugapi_invoke_in_context_any_lambda_ls__c_void_gr__any_any_any_any:

.. das:function:: invoke_in_context(arg0: any; arg1: lambda<():void>; arg2: any; arg3: any; arg4: any; arg5: any)

.. _function-debugapi_invoke_in_context_any_lambda_ls__c_void_gr__any_any_any:

.. das:function:: invoke_in_context(arg0: any; arg1: lambda<():void>; arg2: any; arg3: any; arg4: any)

.. _function-debugapi_invoke_in_context_any_lambda_ls__c_void_gr__any_any_any_any_any_any_any:

.. das:function:: invoke_in_context(arg0: any; arg1: lambda<():void>; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any; arg8: any)

.. _function-debugapi_invoke_in_context_any_function_ls__c_void_gr__any:

.. das:function:: invoke_in_context(arg0: any; arg1: function<():void>; arg2: any)

.. _function-debugapi_invoke_in_context_any_string_any_any_any_any_any_any_any_any_any_any:

.. das:function:: invoke_in_context(arg0: any; arg1: string; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any; arg8: any; arg9: any; arg10: any; arg11: any)

.. _function-debugapi_invoke_in_context_any_string_any_any_any_any_any_any_any_any_any:

.. das:function:: invoke_in_context(arg0: any; arg1: string; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any; arg8: any; arg9: any; arg10: any)

.. _function-debugapi_invoke_in_context_any_function_ls__c_void_gr_:

.. das:function:: invoke_in_context(arg0: any; arg1: function<():void>)

.. _function-debugapi_invoke_in_context_any_function_ls__c_void_gr__any_any:

.. das:function:: invoke_in_context(arg0: any; arg1: function<():void>; arg2: any; arg3: any)

.. _function-debugapi_invoke_in_context_any_string_any_any_any_any_any_any:

.. das:function:: invoke_in_context(arg0: any; arg1: string; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any)

.. _function-debugapi_invoke_in_context_any_string_any_any_any_any_any:

.. das:function:: invoke_in_context(arg0: any; arg1: string; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any)

.. _function-debugapi_invoke_in_context_any_string_any_any_any_any_any_any_any:

.. das:function:: invoke_in_context(arg0: any; arg1: string; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any; arg8: any)

.. _function-debugapi_invoke_in_context_any_string_any_any_any:

.. das:function:: invoke_in_context(arg0: any; arg1: string; arg2: any; arg3: any; arg4: any)

.. _function-debugapi_invoke_in_context_any_string_any:

.. das:function:: invoke_in_context(arg0: any; arg1: string; arg2: any)

.. _function-debugapi_invoke_in_context_any_string:

.. das:function:: invoke_in_context(arg0: any; arg1: string)

.. _function-debugapi_invoke_in_context_any_string_any_any:

.. das:function:: invoke_in_context(arg0: any; arg1: string; arg2: any; arg3: any)

.. _function-debugapi_invoke_in_context_any_string_any_any_any_any:

.. das:function:: invoke_in_context(arg0: any; arg1: string; arg2: any; arg3: any; arg4: any; arg5: any)

.. _function-debugapi_invoke_in_context_any_string_any_any_any_any_any_any_any_any:

.. das:function:: invoke_in_context(arg0: any; arg1: string; arg2: any; arg3: any; arg4: any; arg5: any; arg6: any; arg7: any; arg8: any; arg9: any)

.. _function-debugapi_invoke_in_context_any_function_ls__c_void_gr__any_any_any:

.. das:function:: invoke_in_context(arg0: any; arg1: function<():void>; arg2: any; arg3: any; arg4: any)

.. _function-debugapi_invoke_in_context_any_lambda_ls__c_void_gr__any_any:

.. das:function:: invoke_in_context(arg0: any; arg1: lambda<():void>; arg2: any; arg3: any)


++++++++++++++++++
Agent construction
++++++++++++++++++

  *  :ref:`make_data_walker (class: void?; info: StructInfo const?) : smart_ptr\<DataWalker\> <function-debugapi_make_data_walker_void_q__StructInfo_const_q_>`
  *  :ref:`make_data_walker (classPtr: auto) : smart_ptr\<DataWalker\> <function-debugapi_make_data_walker_auto_0xbf>`
  *  :ref:`make_debug_agent (class: void?; info: StructInfo const?) : smart_ptr\<DebugAgent\> <function-debugapi_make_debug_agent_void_q__StructInfo_const_q_>`
  *  :ref:`make_stack_walker (class: void?; info: StructInfo const?) : smart_ptr\<StackWalker\> <function-debugapi_make_stack_walker_void_q__StructInfo_const_q_>`
  *  :ref:`make_stack_walker (classPtr: auto) : smart_ptr\<StackWalker\> <function-debugapi_make_stack_walker_auto_0xda>`


make_data_walker
^^^^^^^^^^^^^^^^

.. _function-debugapi_make_data_walker_void_q__StructInfo_const_q_:

.. das:function:: make_data_walker(class: void?; info: StructInfo const?) : smart_ptr<DataWalker>

Wraps a `DapiDataWalker` class pointer into a `smart_ptr<DataWalker>` for use with `walk_data`.


:Arguments: * **class** : void? implicit

            * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>`? implicit

.. _function-debugapi_make_data_walker_auto_0xbf:

.. das:function:: make_data_walker(classPtr: auto) : smart_ptr<DataWalker>

----

.. _function-debugapi_make_debug_agent_void_q__StructInfo_const_q_:

.. das:function:: make_debug_agent(class: void?; info: StructInfo const?) : smart_ptr<DebugAgent>

Low-level constructor that wraps a `DapiDebugAgent` class pointer into a `smart_ptr<DebugAgent>`.  Called internally by `install_new_debug_agent`.


:Arguments: * **class** : void? implicit

            * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>`? implicit


make_stack_walker
^^^^^^^^^^^^^^^^^

.. _function-debugapi_make_stack_walker_void_q__StructInfo_const_q_:

.. das:function:: make_stack_walker(class: void?; info: StructInfo const?) : smart_ptr<StackWalker>

Wraps a `DapiStackWalker` class pointer into a `smart_ptr<StackWalker>` for use with `walk_stack`.


:Arguments: * **class** : void? implicit

            * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>`? implicit

.. _function-debugapi_make_stack_walker_auto_0xda:

.. das:function:: make_stack_walker(classPtr: auto) : smart_ptr<StackWalker>


+++++++++++++++++++++++++++++++
Agent tick and state collection
+++++++++++++++++++++++++++++++

  *  :ref:`collect_debug_agent_state (context: Context; at: LineInfo) <function-debugapi_collect_debug_agent_state_Context_LineInfo>`
  *  :ref:`debug_agent_command (command: string) <function-debugapi_debug_agent_command_string>`
  *  :ref:`debugger_stop_requested () : bool <function-debugapi_debugger_stop_requested>`
  *  :ref:`on_breakpoints_reset (file: string; breakpointsNum: int) <function-debugapi_on_breakpoints_reset_string_int>`
  *  :ref:`report_context_state (context: Context; category: string; name: string; info: TypeInfo const?; data: void?) <function-debugapi_report_context_state_Context_string_string_TypeInfo_const_q__void_q_>`
  *  :ref:`tick_debug_agent () <function-debugapi_tick_debug_agent>`
  *  :ref:`tick_debug_agent (agent: string) <function-debugapi_tick_debug_agent_string>`

.. _function-debugapi_collect_debug_agent_state_Context_LineInfo:

.. das:function:: collect_debug_agent_state(context: Context; at: LineInfo)

Triggers `onCollect` on all installed debug agents, passing the calling context and line info.  Agents can inspect the context and report variables via `report_context_state`.


:Arguments: * **context** :  :ref:`Context <handle-rtti-Context>` implicit

            * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` implicit

.. _function-debugapi_debug_agent_command_string:

.. das:function:: debug_agent_command(command: string)

Sends a user-defined command string to all debug agents.  Agents receive it in `onUserCommand`.


:Arguments: * **command** : string implicit

.. _function-debugapi_debugger_stop_requested:

.. das:function:: debugger_stop_requested() : bool

Returns `true` if any debug agent has requested the program to stop (e.g. via a breakpoint or user interrupt).


.. _function-debugapi_on_breakpoints_reset_string_int:

.. das:function:: on_breakpoints_reset(file: string; breakpointsNum: int)

Notifies all debug agents that breakpoints for the given file have been reset.  The `breakpointsNum` parameter indicates the new breakpoint count.


:Arguments: * **file** : string implicit

            * **breakpointsNum** : int

.. _function-debugapi_report_context_state_Context_string_string_TypeInfo_const_q__void_q_:

.. das:function:: report_context_state(context: Context; category: string; name: string; info: TypeInfo const?; data: void?)

Reports a named variable from inside `onCollect` back to the debug system.  Each call triggers `onVariable` on all agents with the category, name, type info, and data pointer.


:Arguments: * **context** :  :ref:`Context <handle-rtti-Context>` implicit

            * **category** : string implicit

            * **name** : string implicit

            * **info** :  :ref:`TypeInfo <handle-rtti-TypeInfo>`? implicit

            * **data** : void? implicit


tick_debug_agent
^^^^^^^^^^^^^^^^

.. _function-debugapi_tick_debug_agent:

.. das:function:: tick_debug_agent()

Calls `onTick` on all installed debug agents (no-arg variant), or on a specific named agent (string variant).  Typically called once per frame by the host application.


.. _function-debugapi_tick_debug_agent_string:

.. das:function:: tick_debug_agent(agent: string)


+++++++++++++++
Instrumentation
+++++++++++++++

  *  :ref:`clear_instruments (context: Context) <function-debugapi_clear_instruments_Context>`
  *  :ref:`instrument_all_functions (context: Context) <function-debugapi_instrument_all_functions_Context>`
  *  :ref:`instrument_all_functions (ctx: Context; block: block\<(function\<():void\>;SimFunction const?):uint64\>) <function-debugapi_instrument_all_functions_Context_block_ls_function_ls__c_void_gr_;SimFunction_const_q__c_uint64_gr_>`
  *  :ref:`instrument_all_functions_thread_local (ctx: Context; block: block\<(function\<():void\>;SimFunction const?):uint64\>) <function-debugapi_instrument_all_functions_thread_local_Context_block_ls_function_ls__c_void_gr_;SimFunction_const_q__c_uint64_gr_>`
  *  :ref:`instrument_all_functions_thread_local (context: Context) <function-debugapi_instrument_all_functions_thread_local_Context>`
  *  :ref:`instrument_context_allocations (context: Context; isInstrumenting: bool) <function-debugapi_instrument_context_allocations_Context_bool>`
  *  :ref:`instrument_function (context: Context; function: function\<():void\>; isInstrumenting: bool; userData: uint64) <function-debugapi_instrument_function_Context_function_ls__c_void_gr__bool_uint64>`
  *  :ref:`instrument_node (context: Context; isInstrumenting: bool; block: block\<(LineInfo):bool\>) <function-debugapi_instrument_node_Context_bool_block_ls_LineInfo_c_bool_gr_>`
  *  :ref:`set_single_step (context: Context; enabled: bool) <function-debugapi_set_single_step_Context_bool>`

.. _function-debugapi_clear_instruments_Context:

.. das:function:: clear_instruments(context: Context)

Removes all instrumentation from the given context, restoring original execution.


:Arguments: * **context** :  :ref:`Context <handle-rtti-Context>` implicit


instrument_all_functions
^^^^^^^^^^^^^^^^^^^^^^^^

.. _function-debugapi_instrument_all_functions_Context:

.. das:function:: instrument_all_functions(context: Context)

Enables or disables instrumentation for all functions in the given context.  Overloaded: the no-block variant instruments everything; the block variant calls a filter to select functions.


:Arguments: * **context** :  :ref:`Context <handle-rtti-Context>` implicit

.. _function-debugapi_instrument_all_functions_Context_block_ls_function_ls__c_void_gr_;SimFunction_const_q__c_uint64_gr_:

.. das:function:: instrument_all_functions(ctx: Context; block: block<(function<():void>;SimFunction const?):uint64>)

----


instrument_all_functions_thread_local
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. _function-debugapi_instrument_all_functions_thread_local_Context_block_ls_function_ls__c_void_gr_;SimFunction_const_q__c_uint64_gr_:

.. das:function:: instrument_all_functions_thread_local(ctx: Context; block: block<(function<():void>;SimFunction const?):uint64>)

Thread-local variant of `instrument_all_functions`.  Instruments functions only on the current thread.


:Arguments: * **ctx** :  :ref:`Context <handle-rtti-Context>` implicit

            * **block** : block<(function<void>; :ref:`SimFunction <handle-rtti-SimFunction>`?):uint64> implicit

.. _function-debugapi_instrument_all_functions_thread_local_Context:

.. das:function:: instrument_all_functions_thread_local(context: Context)

----

.. _function-debugapi_instrument_context_allocations_Context_bool:

.. das:function:: instrument_context_allocations(context: Context; isInstrumenting: bool)

Enables or disables allocation tracking on the given context.  When enabled, the debug agent receives `onAllocate` / `onReallocate` / `onFree` callbacks.


:Arguments: * **context** :  :ref:`Context <handle-rtti-Context>` implicit

            * **isInstrumenting** : bool

.. _function-debugapi_instrument_function_Context_function_ls__c_void_gr__bool_uint64:

.. das:function:: instrument_function(context: Context; function: function<():void>; isInstrumenting: bool; userData: uint64)

Enables or disables instrumentation for a specific function in the given context.  The `userData` value is passed to the instrumentation callback.


:Arguments: * **context** :  :ref:`Context <handle-rtti-Context>` implicit

            * **function** : function<void>

            * **isInstrumenting** : bool

            * **userData** : uint64

.. _function-debugapi_instrument_node_Context_bool_block_ls_LineInfo_c_bool_gr_:

.. das:function:: instrument_node(context: Context; isInstrumenting: bool; block: block<(LineInfo):bool>)

Enables or disables per-node instrumentation on the given context.  The block receives each `LineInfo` and returns `true` to instrument that node.


:Arguments: * **context** :  :ref:`Context <handle-rtti-Context>` implicit

            * **isInstrumenting** : bool

            * **block** : block<( :ref:`LineInfo <handle-rtti-LineInfo>`):bool> implicit

.. _function-debugapi_set_single_step_Context_bool:

.. das:function:: set_single_step(context: Context; enabled: bool)

Enables or disables single-step execution on the given context.  When enabled, the debug agent receives `onSingleStep` for each statement.


:Arguments: * **context** :  :ref:`Context <handle-rtti-Context>` implicit

            * **enabled** : bool


++++++++++++++++++++++
Data and stack walking
++++++++++++++++++++++

  *  :ref:`stack_depth (context: Context) : int <function-debugapi_stack_depth_Context>`
  *  :ref:`stackwalk (context: Context; line: LineInfo) <function-debugapi_stackwalk_Context_LineInfo>`
  *  :ref:`walk_data (walker: smart_ptr\<DataWalker\>; data: void?; struct_info: StructInfo) <function-debugapi_walk_data_smart_ptr_ls_DataWalker_gr__void_q__StructInfo>`
  *  :ref:`walk_data (walker: smart_ptr\<DataWalker\>; data: void?; info: TypeInfo) <function-debugapi_walk_data_smart_ptr_ls_DataWalker_gr__void_q__TypeInfo>`
  *  :ref:`walk_data (walker: smart_ptr\<DataWalker\>; data: float4; info: TypeInfo) <function-debugapi_walk_data_smart_ptr_ls_DataWalker_gr__float4_TypeInfo>`
  *  :ref:`walk_stack (walker: smart_ptr\<StackWalker\>; context: Context; line: LineInfo) <function-debugapi_walk_stack_smart_ptr_ls_StackWalker_gr__Context_LineInfo>`

.. _function-debugapi_stack_depth_Context:

.. das:function:: stack_depth(context: Context) : int

Returns the current call stack depth of the given context.


:Arguments: * **context** :  :ref:`Context <handle-rtti-Context>` implicit

.. _function-debugapi_stackwalk_Context_LineInfo:

.. das:function:: stackwalk(context: Context; line: LineInfo)

Prints a human-readable stack trace of the given context to the log output.


:Arguments: * **context** :  :ref:`Context <handle-rtti-Context>` implicit

            * **line** :  :ref:`LineInfo <handle-rtti-LineInfo>` implicit


walk_data
^^^^^^^^^

.. _function-debugapi_walk_data_smart_ptr_ls_DataWalker_gr__void_q__StructInfo:

.. das:function:: walk_data(walker: smart_ptr<DataWalker>; data: void?; struct_info: StructInfo)

Walks a daslang data structure using the provided `DataWalker`.  The walker receives typed callbacks for each value encountered.  Overloaded for raw data+`StructInfo`, `float4`+`TypeInfo`, and `void?`+`TypeInfo`.


:Arguments: * **walker** : smart_ptr< :ref:`DataWalker <handle-debugapi-DataWalker>`> implicit

            * **data** : void? implicit

            * **struct_info** :  :ref:`StructInfo <handle-rtti-StructInfo>` implicit

.. _function-debugapi_walk_data_smart_ptr_ls_DataWalker_gr__void_q__TypeInfo:

.. das:function:: walk_data(walker: smart_ptr<DataWalker>; data: void?; info: TypeInfo)

.. _function-debugapi_walk_data_smart_ptr_ls_DataWalker_gr__float4_TypeInfo:

.. das:function:: walk_data(walker: smart_ptr<DataWalker>; data: float4; info: TypeInfo)

----

.. _function-debugapi_walk_stack_smart_ptr_ls_StackWalker_gr__Context_LineInfo:

.. das:function:: walk_stack(walker: smart_ptr<StackWalker>; context: Context; line: LineInfo)

Walks the call stack of the given context using the provided `StackWalker`.  The walker receives callbacks for each stack frame, argument, and local variable.


:Arguments: * **walker** : smart_ptr< :ref:`StackWalker <handle-debugapi-StackWalker>`> implicit

            * **context** :  :ref:`Context <handle-rtti-Context>` implicit

            * **line** :  :ref:`LineInfo <handle-rtti-LineInfo>` implicit


++++++++++++++++++
Context inspection
++++++++++++++++++

  *  :ref:`get_context_global_variable (context: any; index: int) : void? <function-debugapi_get_context_global_variable_any_int>`
  *  :ref:`get_context_global_variable (context: any; name: string) : void? <function-debugapi_get_context_global_variable_any_string>`
  *  :ref:`get_heap_stats (context: Context; bytes: uint64?) <function-debugapi_get_heap_stats_Context_uint64_q_>`
  *  :ref:`has_function (context: Context; function_name: string) : bool <function-debugapi_has_function_Context_string>`


get_context_global_variable
^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. _function-debugapi_get_context_global_variable_any_int:

.. das:function:: get_context_global_variable(context: any; index: int) : void?

.. warning::
  This is unsafe operation.

Returns a pointer to a global variable in the given context, looked up by name (string variant) or by index (int variant).  Returns `null` if not found.


:Arguments: * **context** : any

            * **index** : int

.. _function-debugapi_get_context_global_variable_any_string:

.. das:function:: get_context_global_variable(context: any; name: string) : void?

----

.. _function-debugapi_get_heap_stats_Context_uint64_q_:

.. das:function:: get_heap_stats(context: Context; bytes: uint64?)

.. warning::
  This is unsafe operation.

Writes the heap allocation statistics of the given context into the provided `uint64` pointer.


:Arguments: * **context** :  :ref:`Context <handle-rtti-Context>` implicit

            * **bytes** : uint64? implicit

.. _function-debugapi_has_function_Context_string:

.. das:function:: has_function(context: Context; function_name: string) : bool

Returns `true` if the given context contains an exported function with the specified name.


:Arguments: * **context** :  :ref:`Context <handle-rtti-Context>` implicit

            * **function_name** : string implicit


+++++++++++
Breakpoints
+++++++++++

  *  :ref:`clear_hw_breakpoint (arg0: int) : bool <function-debugapi_clear_hw_breakpoint_int>`
  *  :ref:`set_hw_breakpoint (context: Context; address: void?; size: int; writeOnly: bool) : int <function-debugapi_set_hw_breakpoint_Context_void_q__int_bool>`

.. _function-debugapi_clear_hw_breakpoint_int:

.. das:function:: clear_hw_breakpoint(arg0: int) : bool

.. warning::
  This is unsafe operation.

Clears the hardware breakpoint with the given index.  Returns `true` on success.


:Arguments: * **arg0** : int

.. _function-debugapi_set_hw_breakpoint_Context_void_q__int_bool:

.. das:function:: set_hw_breakpoint(context: Context; address: void?; size: int; writeOnly: bool) : int

.. warning::
  This is unsafe operation.

Sets a hardware breakpoint at the given memory address.  Returns the breakpoint index, or -1 on failure.  The `writeOnly` flag selects write-only vs. read/write watch.


:Arguments: * **context** :  :ref:`Context <handle-rtti-Context>` implicit

            * **address** : void? implicit

            * **size** : int

            * **writeOnly** : bool


++++++
Memory
++++++

  *  :ref:`break_on_free (context: Context; ptr: void?; size: uint) <function-debugapi_break_on_free_Context_void_q__uint>`
  *  :ref:`free_temp_string (context: Context) <function-debugapi_free_temp_string_Context>`
  *  :ref:`temp_string_size (context: Context) : uint64 <function-debugapi_temp_string_size_Context>`
  *  :ref:`track_insane_pointer (ptr: void?) <function-debugapi_track_insane_pointer_void_q_>`

.. _function-debugapi_break_on_free_Context_void_q__uint:

.. das:function:: break_on_free(context: Context; ptr: void?; size: uint)

.. warning::
  This is unsafe operation.

Triggers a debug break when the specified memory region is freed.  Useful for tracking down use-after-free bugs.


:Arguments: * **context** :  :ref:`Context <handle-rtti-Context>` implicit

            * **ptr** : void? implicit

            * **size** : uint

.. _function-debugapi_free_temp_string_Context:

.. das:function:: free_temp_string(context: Context)

.. warning::
  This is unsafe operation.

Frees all temporary string allocations in the given context.


:Arguments: * **context** :  :ref:`Context <handle-rtti-Context>` implicit

.. _function-debugapi_temp_string_size_Context:

.. das:function:: temp_string_size(context: Context) : uint64

.. warning::
  This is unsafe operation.

Returns the total size in bytes of temporary string allocations in the given context.


:Arguments: * **context** :  :ref:`Context <handle-rtti-Context>` implicit

.. _function-debugapi_track_insane_pointer_void_q_:

.. das:function:: track_insane_pointer(ptr: void?)

.. warning::
  This is unsafe operation.

Begins tracking the specified pointer for dangling-reference detection.


:Arguments: * **ptr** : void? implicit


