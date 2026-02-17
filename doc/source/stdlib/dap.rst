
.. _stdlib_dap:

======================================
Debug Adapter Protocol data structures
======================================

.. das:module:: dap

The DAP module implements the Debug Adapter Protocol (DAP) for integrating
daslang with external debuggers. It provides the message types, serialization,
and communication infrastructure needed for IDE debugging support.

All functions and symbols are in "dap" module, use require to get access to it. ::

    require daslib/dap



++++++++++
Structures
++++++++++

.. _struct-dap-InitializeRequestArguments:

.. das:attribute:: InitializeRequestArguments

Arguments for the DAP initialize request.



.. _struct-dap-DisconnectArguments:

.. das:attribute:: DisconnectArguments

Arguments for the DAP disconnect request.

:Fields: * **restart** : bool - Whether to restart the debuggee after disconnecting.

         * **terminateDebuggee** : bool - Whether to terminate the debuggee when disconnecting.

         * **suspendDebuggee** : bool - Whether to suspend the debuggee when disconnecting.



.. _struct-dap-Capabilities:

.. das:attribute:: Capabilities

Debugger capabilities reported in the initialize response.

:Fields: * **supportsConfigurationDoneRequest** : bool - Whether the adapter supports the configurationDone request.

         * **supportsRestartRequest** : bool - Whether the adapter supports the restart request.

         * **supportTerminateDebuggee** : bool - Whether the adapter supports terminating the debuggee.

         * **supportsTerminateRequest** : bool - Whether the adapter supports the terminate request.

         * **supportsExceptionOptions** : bool - Whether the adapter supports exception options.

         * **supportsExceptionFilterOptions** : bool - Whether the adapter supports exception filter options.

         * **supportsDelayedStackTraceLoading** : bool - Whether the adapter supports delayed stack trace loading.

         * **supportsDataBreakpoints** : bool - Whether the adapter supports data breakpoints.



.. _struct-dap-DataBreakpoint:

.. das:attribute:: DataBreakpoint

A data breakpoint that triggers on memory access.

:Fields: * **dataId** : string - Identifier for the data to watch.

         * **accessType** : string - Access type that triggers the breakpoint: read, write, or readWrite.

         * **condition** : string - Optional expression condition for the breakpoint.

         * **hitCondition** : string - Optional hit count condition for the breakpoint.

         * **description** : string - Human-readable description of the breakpoint.

         * **enabled** : bool - Whether the breakpoint is enabled.



.. _struct-dap-SetDataBreakpointsArguments:

.. das:attribute:: SetDataBreakpointsArguments

Arguments for the setDataBreakpoints request.

:Fields: * **breakpoints** : array< :ref:`DataBreakpoint <struct-dap-DataBreakpoint>`> - Array of data breakpoints to set.



.. _struct-dap-DataBreakpointInfoArguments:

.. das:attribute:: DataBreakpointInfoArguments

Arguments for the dataBreakpointInfo request.

:Fields: * **variablesReference** : double - Reference to the variable container.

         * **name** : string - Name of the variable.



.. _struct-dap-DataBreakpointInfoResponse:

.. das:attribute:: DataBreakpointInfoResponse

Response body for the dataBreakpointInfo request.

:Fields: * **dataId** : string - Identifier for the data, used when setting a data breakpoint.

         * **description** : string - Human-readable description of the data.



.. _struct-dap-SourceBreakpoint:

.. das:attribute:: SourceBreakpoint

A breakpoint specified by source location line number.

:Fields: * **line** : double - Line number of the breakpoint.



.. _struct-dap-Source:

.. das:attribute:: Source

A source file descriptor with name and path.

:Fields: * **name** : string - Short name of the source.

         * **path** : string - Full file-system path of the source.



.. _struct-dap-SetBreakpointsArguments:

.. das:attribute:: SetBreakpointsArguments

Arguments for the setBreakpoints request.

:Fields: * **source** :  :ref:`Source <struct-dap-Source>` - Source file for which breakpoints are set.

         * **breakpoints** : array< :ref:`SourceBreakpoint <struct-dap-SourceBreakpoint>`> - Array of source breakpoints to set.

         * **sourceModified** : bool - Whether the source has been modified since last build.



.. _struct-dap-Breakpoint:

.. das:attribute:: Breakpoint

A breakpoint with verification status and location.

:Fields: * **id** : double - Unique identifier for the breakpoint.

         * **verified** : bool - Whether the breakpoint has been verified by the debugger.

         * **source** :  :ref:`Source <struct-dap-Source>` - Source file containing the breakpoint.

         * **line** : double - Actual line number of the breakpoint.

         * **message** : string - Optional message about the breakpoint state.



.. _struct-dap-SetBreakpointsResponse:

.. das:attribute:: SetBreakpointsResponse

Response body for the setBreakpoints request.

:Fields: * **breakpoints** : array< :ref:`Breakpoint <struct-dap-Breakpoint>`> - Array of breakpoints with their verification status.



.. _struct-dap-Thread:

.. das:attribute:: Thread

A thread with an identifier and name.

:Fields: * **id** : double - Unique identifier for the thread.

         * **name** : string - Human-readable name of the thread.



.. _struct-dap-ThreadsResponseBody:

.. das:attribute:: ThreadsResponseBody

Response body for the threads request.

:Fields: * **threads** : array< :ref:`Thread <struct-dap-Thread>`> - Array of threads.



.. _struct-dap-StackTraceArguments:

.. das:attribute:: StackTraceArguments

Arguments for the stackTrace request.

:Fields: * **threadId** : double - Thread for which to retrieve the stack trace.

         * **startFrame** : double - Index of the first frame to return.

         * **levels** : double - Maximum number of frames to return.



.. _struct-dap-StackFrame:

.. das:attribute:: StackFrame

A stack frame with source location and identifier.

:Fields: * **id** : double - Unique identifier for the stack frame.

         * **name** : string - Name of the frame, typically the function name.

         * **source** :  :ref:`Source <struct-dap-Source>` - Source file of the frame.

         * **line** : double - Line number in the source file.

         * **column** : double - Column number in the source file.



.. _struct-dap-StackTraceResponseBody:

.. das:attribute:: StackTraceResponseBody

Response body for the stackTrace request.

:Fields: * **stackFrames** : array< :ref:`StackFrame <struct-dap-StackFrame>`> - Array of stack frames.

         * **totalFrames** : double - Total number of frames available.



.. _struct-dap-ScopesArguments:

.. das:attribute:: ScopesArguments

Arguments for the scopes request.

:Fields: * **frameId** : double - Stack frame for which to retrieve scopes.



.. _struct-dap-Scope:

.. das:attribute:: Scope

A named variable scope with a variables reference.

:Fields: * **name** : string - Name of the scope (e.g. Locals, Arguments).

         * **variablesReference** : double - Reference used to retrieve the variables of this scope.



.. _struct-dap-ScopesResponseBody:

.. das:attribute:: ScopesResponseBody

Response body for the scopes request.

:Fields: * **scopes** : array< :ref:`Scope <struct-dap-Scope>`> - Array of scopes for the given frame.



.. _struct-dap-VariablesArguments:

.. das:attribute:: VariablesArguments

Arguments for the variables request.

:Fields: * **variablesReference** : double - Reference to the variable container to expand.

         * **start** : double - Start index of variables to return (for paging).

         * **count** : double - Number of variables to return (for paging).



.. _struct-dap-Variable:

.. das:attribute:: Variable

A variable with name, value, and type information.

:Fields: * **name** : string - Name of the variable.

         * **value** : string - String representation of the variable's value.

         * **variablesReference** : double - Reference to child variables, if any.

         * **_type** : string - Type of the variable.

         * **indexedVariables** : double - Number of indexed child variables.



.. _struct-dap-VariablesResponseBody:

.. das:attribute:: VariablesResponseBody

Response body for the variables request.

:Fields: * **variables** : array< :ref:`Variable <struct-dap-Variable>`> - Array of variables.



.. _struct-dap-OutputEventBody:

.. das:attribute:: OutputEventBody

Body of the output event for debugger console messages.

:Fields: * **category** : string - Category of the output (e.g. console, stdout, stderr).

         * **output** : string - The output text.



.. _struct-dap-ContinueArguments:

.. das:attribute:: ContinueArguments

Arguments for the continue request.

:Fields: * **threadId** : double - Thread to continue.



.. _struct-dap-PauseArguments:

.. das:attribute:: PauseArguments

Arguments for the pause request.

:Fields: * **threadId** : double - Thread to pause.



.. _struct-dap-StepInArguments:

.. das:attribute:: StepInArguments

Arguments for the stepIn request.

:Fields: * **threadId** : double - Thread to step into.



.. _struct-dap-NextArguments:

.. das:attribute:: NextArguments

Arguments for the next (step over) request.

:Fields: * **threadId** : double - Thread to step over.



.. _struct-dap-StepOutArguments:

.. das:attribute:: StepOutArguments

Arguments for the stepOut request.

:Fields: * **threadId** : double - Thread to step out of.



.. _struct-dap-EvaluateArguments:

.. das:attribute:: EvaluateArguments

Arguments for the evaluate request.

:Fields: * **expression** : string - Expression to evaluate.

         * **frameId** : double - Stack frame in which to evaluate the expression.

         * **context** : string - Context in which the expression is evaluated (e.g. watch, repl, hover).



.. _struct-dap-EvaluateResponse:

.. das:attribute:: EvaluateResponse

Response body for the evaluate request.

:Fields: * **result** : string - Type of the evaluation result.

         * **_type** : string - String representation of the evaluation result.

         * **variablesReference** : double - Reference to child variables of the result, if any.

         * **indexedVariables** : double - Number of indexed child variables in the result.



.. _struct-dap-BreakpointEvent:

.. das:attribute:: BreakpointEvent

Event body indicating a breakpoint status change.

:Fields: * **reason** : string - Reason for the event: changed, new, or removed.

         * **breakpoint** :  :ref:`Breakpoint <struct-dap-Breakpoint>` - The breakpoint whose status changed.



.. _struct-dap-ThreadEvent:

.. das:attribute:: ThreadEvent

Event body indicating a thread started or exited.

:Fields: * **reason** : string - Reason for the event: started or exited.

         * **threadId** : double - Thread identifier.




++++++++++++++++++++
JSON deserialization
++++++++++++++++++++

  *  :ref:`ContinueArguments (data: JsonValue?) : ContinueArguments <function-dap_ContinueArguments_JsonValue_q_>`
  *  :ref:`DataBreakpoint (data: JsonValue?) : DataBreakpoint <function-dap_DataBreakpoint_JsonValue_q_>`
  *  :ref:`DataBreakpointInfoArguments (data: JsonValue?) : DataBreakpointInfoArguments <function-dap_DataBreakpointInfoArguments_JsonValue_q_>`
  *  :ref:`DisconnectArguments (data: JsonValue?) : DisconnectArguments <function-dap_DisconnectArguments_JsonValue_q_>`
  *  :ref:`EvaluateArguments (data: JsonValue?) : EvaluateArguments <function-dap_EvaluateArguments_JsonValue_q_>`
  *  :ref:`InitializeRequestArguments (data: JsonValue?) : InitializeRequestArguments <function-dap_InitializeRequestArguments_JsonValue_q_>`
  *  :ref:`NextArguments (data: JsonValue?) : NextArguments <function-dap_NextArguments_JsonValue_q_>`
  *  :ref:`PauseArguments (data: JsonValue?) : PauseArguments <function-dap_PauseArguments_JsonValue_q_>`
  *  :ref:`ScopesArguments (data: JsonValue?) : ScopesArguments <function-dap_ScopesArguments_JsonValue_q_>`
  *  :ref:`SetBreakpointsArguments (data: JsonValue?) : SetBreakpointsArguments <function-dap_SetBreakpointsArguments_JsonValue_q_>`
  *  :ref:`SetDataBreakpointsArguments (data: JsonValue?) : SetDataBreakpointsArguments <function-dap_SetDataBreakpointsArguments_JsonValue_q_>`
  *  :ref:`Source (data: JsonValue?) : Source <function-dap_Source_JsonValue_q_>`
  *  :ref:`SourceBreakpoint (data: JsonValue?) : SourceBreakpoint <function-dap_SourceBreakpoint_JsonValue_q_>`
  *  :ref:`StackTraceArguments (data: JsonValue?) : StackTraceArguments <function-dap_StackTraceArguments_JsonValue_q_>`
  *  :ref:`StepInArguments (data: JsonValue?) : StepInArguments <function-dap_StepInArguments_JsonValue_q_>`
  *  :ref:`StepOutArguments (data: JsonValue?) : StepOutArguments <function-dap_StepOutArguments_JsonValue_q_>`
  *  :ref:`VariablesArguments (data: JsonValue?) : VariablesArguments <function-dap_VariablesArguments_JsonValue_q_>`

.. _function-dap_ContinueArguments_JsonValue_q_:

.. das:function:: ContinueArguments(data: JsonValue?) : ContinueArguments

Constructs a ContinueArguments from a JSON value.


:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>`?

.. _function-dap_DataBreakpoint_JsonValue_q_:

.. das:function:: DataBreakpoint(data: JsonValue?) : DataBreakpoint

Constructs a DataBreakpoint from a JSON value.


:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>`?

.. _function-dap_DataBreakpointInfoArguments_JsonValue_q_:

.. das:function:: DataBreakpointInfoArguments(data: JsonValue?) : DataBreakpointInfoArguments

Constructs a DataBreakpointInfoArguments from a JSON value.


:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>`?

.. _function-dap_DisconnectArguments_JsonValue_q_:

.. das:function:: DisconnectArguments(data: JsonValue?) : DisconnectArguments

Constructs a DisconnectArguments from a JSON value.


:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>`?

.. _function-dap_EvaluateArguments_JsonValue_q_:

.. das:function:: EvaluateArguments(data: JsonValue?) : EvaluateArguments

Constructs an EvaluateArguments from a JSON value.


:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>`?

.. _function-dap_InitializeRequestArguments_JsonValue_q_:

.. das:function:: InitializeRequestArguments(data: JsonValue?) : InitializeRequestArguments

Constructs an InitializeRequestArguments from a JSON value.


:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>`?

.. _function-dap_NextArguments_JsonValue_q_:

.. das:function:: NextArguments(data: JsonValue?) : NextArguments

Constructs a NextArguments from a JSON value.


:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>`?

.. _function-dap_PauseArguments_JsonValue_q_:

.. das:function:: PauseArguments(data: JsonValue?) : PauseArguments

Constructs a PauseArguments from a JSON value.


:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>`?

.. _function-dap_ScopesArguments_JsonValue_q_:

.. das:function:: ScopesArguments(data: JsonValue?) : ScopesArguments

Constructs a ScopesArguments from a JSON value.


:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>`?

.. _function-dap_SetBreakpointsArguments_JsonValue_q_:

.. das:function:: SetBreakpointsArguments(data: JsonValue?) : SetBreakpointsArguments

Constructs a SetBreakpointsArguments from a JSON value, parsing source and breakpoints.


:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>`?

.. _function-dap_SetDataBreakpointsArguments_JsonValue_q_:

.. das:function:: SetDataBreakpointsArguments(data: JsonValue?) : SetDataBreakpointsArguments

Constructs a SetDataBreakpointsArguments from a JSON value, parsing the breakpoints array.


:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>`?

.. _function-dap_Source_JsonValue_q_:

.. das:function:: Source(data: JsonValue?) : Source

Constructs a Source from a JSON value.


:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>`?

.. _function-dap_SourceBreakpoint_JsonValue_q_:

.. das:function:: SourceBreakpoint(data: JsonValue?) : SourceBreakpoint

Constructs a SourceBreakpoint from a JSON value.


:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>`?

.. _function-dap_StackTraceArguments_JsonValue_q_:

.. das:function:: StackTraceArguments(data: JsonValue?) : StackTraceArguments

Constructs a StackTraceArguments from a JSON value.


:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>`?

.. _function-dap_StepInArguments_JsonValue_q_:

.. das:function:: StepInArguments(data: JsonValue?) : StepInArguments

Constructs a StepInArguments from a JSON value.


:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>`?

.. _function-dap_StepOutArguments_JsonValue_q_:

.. das:function:: StepOutArguments(data: JsonValue?) : StepOutArguments

Constructs a StepOutArguments from a JSON value.


:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>`?

.. _function-dap_VariablesArguments_JsonValue_q_:

.. das:function:: VariablesArguments(data: JsonValue?) : VariablesArguments

Constructs a VariablesArguments from a JSON value.


:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>`?


++++++++++++++++++
JSON serialization
++++++++++++++++++

  *  :ref:`JV (data: EvaluateResponse) : JsonValue? <function-dap_JV_EvaluateResponse>`
  *  :ref:`JV (data: Variable) : JsonValue? <function-dap_JV_Variable>`


JV
^^

.. _function-dap_JV_EvaluateResponse:

.. das:function:: JV(data: EvaluateResponse) : JsonValue?

Converts an EvaluateResponse struct to its DAP JSON representation.


:Arguments: * **data** :  :ref:`EvaluateResponse <struct-dap-EvaluateResponse>`

.. _function-dap_JV_Variable:

.. das:function:: JV(data: Variable) : JsonValue?

----


++++++++++++++++++++
JSON field accessors
++++++++++++++++++++

  *  :ref:`j_s (val: JsonValue?; defVal: string = "") : string <function-dap_j_s_JsonValue_q__string>`
  *  :ref:`job (val: JsonValue?; id: string; defVal: bool = false) : bool <function-dap_job_JsonValue_q__string_bool>`
  *  :ref:`joj (val: JsonValue?; id: string) : JsonValue? <function-dap_joj_JsonValue_q__string>`
  *  :ref:`jon (val: JsonValue?; id: string; defVal: double = 0lf) : double <function-dap_jon_JsonValue_q__string_double>`
  *  :ref:`jos (val: JsonValue?; id: string; defVal: string = "") : string <function-dap_jos_JsonValue_q__string_string>`

.. _function-dap_j_s_JsonValue_q__string:

.. das:function:: j_s(val: JsonValue?; defVal: string = "") : string

Returns the string value of a JSON value, or `defVal` if not a string.


:Arguments: * **val** :  :ref:`JsonValue <struct-json-JsonValue>`?

            * **defVal** : string

.. _function-dap_job_JsonValue_q__string_bool:

.. das:function:: job(val: JsonValue?; id: string; defVal: bool = false) : bool

Returns a boolean JSON field by name, or `defVal` if not found.


:Arguments: * **val** :  :ref:`JsonValue <struct-json-JsonValue>`?

            * **id** : string

            * **defVal** : bool

.. _function-dap_joj_JsonValue_q__string:

.. das:function:: joj(val: JsonValue?; id: string) : JsonValue?

Returns a nested JSON object field by name, or null if not found.


:Arguments: * **val** :  :ref:`JsonValue <struct-json-JsonValue>`?

            * **id** : string

.. _function-dap_jon_JsonValue_q__string_double:

.. das:function:: jon(val: JsonValue?; id: string; defVal: double = 0lf) : double

Returns a numeric JSON field by name, or `defVal` if not found.


:Arguments: * **val** :  :ref:`JsonValue <struct-json-JsonValue>`?

            * **id** : string

            * **defVal** : double

.. _function-dap_jos_JsonValue_q__string_string:

.. das:function:: jos(val: JsonValue?; id: string; defVal: string = "") : string

Returns a string JSON field by name, or `defVal` if not found.


:Arguments: * **val** :  :ref:`JsonValue <struct-json-JsonValue>`?

            * **id** : string

            * **defVal** : string


