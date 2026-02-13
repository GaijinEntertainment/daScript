
.. _stdlib_dap:

======================================
Debug Adapter Protocol data structures
======================================

The DAP module implements the Debug Adapter Protocol (DAP) for integrating
daScript with external debuggers. It provides the message types, serialization,
and communication infrastructure needed for IDE debugging support.

All functions and symbols are in "dap" module, use require to get access to it. ::

    require daslib/dap

++++++++++
Structures
++++++++++

.. _struct-dap-InitializeRequestArguments:

.. das:attribute:: InitializeRequestArguments

|detail/structure-dap-InitializeRequestArguments|

.. _struct-dap-DisconnectArguments:

.. das:attribute:: DisconnectArguments

|detail/structure-dap-DisconnectArguments|

.. _struct-dap-Capabilities:

.. das:attribute:: Capabilities

|detail/structure-dap-Capabilities|

.. _struct-dap-DataBreakpoint:

.. das:attribute:: DataBreakpoint

|detail/structure-dap-DataBreakpoint|

.. _struct-dap-SetDataBreakpointsArguments:

.. das:attribute:: SetDataBreakpointsArguments

|detail/structure-dap-SetDataBreakpointsArguments|

.. _struct-dap-DataBreakpointInfoArguments:

.. das:attribute:: DataBreakpointInfoArguments

|detail/structure-dap-DataBreakpointInfoArguments|

.. _struct-dap-DataBreakpointInfoResponse:

.. das:attribute:: DataBreakpointInfoResponse

|detail/structure-dap-DataBreakpointInfoResponse|

.. _struct-dap-SourceBreakpoint:

.. das:attribute:: SourceBreakpoint

|detail/structure-dap-SourceBreakpoint|

.. _struct-dap-Source:

.. das:attribute:: Source

|detail/structure-dap-Source|

.. _struct-dap-SetBreakpointsArguments:

.. das:attribute:: SetBreakpointsArguments

|detail/structure-dap-SetBreakpointsArguments|

.. _struct-dap-Breakpoint:

.. das:attribute:: Breakpoint

|detail/structure-dap-Breakpoint|

.. _struct-dap-SetBreakpointsResponse:

.. das:attribute:: SetBreakpointsResponse

|detail/structure-dap-SetBreakpointsResponse|

.. _struct-dap-Thread:

.. das:attribute:: Thread

|detail/structure-dap-Thread|

.. _struct-dap-ThreadsResponseBody:

.. das:attribute:: ThreadsResponseBody

|detail/structure-dap-ThreadsResponseBody|

.. _struct-dap-StackTraceArguments:

.. das:attribute:: StackTraceArguments

|detail/structure-dap-StackTraceArguments|

.. _struct-dap-StackFrame:

.. das:attribute:: StackFrame

|detail/structure-dap-StackFrame|

.. _struct-dap-StackTraceResponseBody:

.. das:attribute:: StackTraceResponseBody

|detail/structure-dap-StackTraceResponseBody|

.. _struct-dap-ScopesArguments:

.. das:attribute:: ScopesArguments

|detail/structure-dap-ScopesArguments|

.. _struct-dap-Scope:

.. das:attribute:: Scope

|detail/structure-dap-Scope|

.. _struct-dap-ScopesResponseBody:

.. das:attribute:: ScopesResponseBody

|detail/structure-dap-ScopesResponseBody|

.. _struct-dap-VariablesArguments:

.. das:attribute:: VariablesArguments

|detail/structure-dap-VariablesArguments|

.. _struct-dap-Variable:

.. das:attribute:: Variable

|detail/structure-dap-Variable|

.. _struct-dap-VariablesResponseBody:

.. das:attribute:: VariablesResponseBody

|detail/structure-dap-VariablesResponseBody|

.. _struct-dap-OutputEventBody:

.. das:attribute:: OutputEventBody

|detail/structure-dap-OutputEventBody|

.. _struct-dap-ContinueArguments:

.. das:attribute:: ContinueArguments

|detail/structure-dap-ContinueArguments|

.. _struct-dap-PauseArguments:

.. das:attribute:: PauseArguments

|detail/structure-dap-PauseArguments|

.. _struct-dap-StepInArguments:

.. das:attribute:: StepInArguments

|detail/structure-dap-StepInArguments|

.. _struct-dap-NextArguments:

.. das:attribute:: NextArguments

|detail/structure-dap-NextArguments|

.. _struct-dap-StepOutArguments:

.. das:attribute:: StepOutArguments

|detail/structure-dap-StepOutArguments|

.. _struct-dap-EvaluateArguments:

.. das:attribute:: EvaluateArguments

|detail/structure-dap-EvaluateArguments|

.. _struct-dap-EvaluateResponse:

.. das:attribute:: EvaluateResponse

|detail/structure-dap-EvaluateResponse|

.. _struct-dap-BreakpointEvent:

.. das:attribute:: BreakpointEvent

|detail/structure-dap-BreakpointEvent|

.. _struct-dap-ThreadEvent:

.. das:attribute:: ThreadEvent

|detail/structure-dap-ThreadEvent|

+++++++++++++
Uncategorized
+++++++++++++

.. _function-dap_InitializeRequestArguments_JsonValue_q_:

.. das:function:: InitializeRequestArguments(data: JsonValue?) : InitializeRequestArguments

Constructs an InitializeRequestArguments from a JSON value.

:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>` ?

.. _function-dap_DisconnectArguments_JsonValue_q_:

.. das:function:: DisconnectArguments(data: JsonValue?) : DisconnectArguments

Constructs a DisconnectArguments from a JSON value.

:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>` ?

.. _function-dap_DataBreakpoint_JsonValue_q_:

.. das:function:: DataBreakpoint(data: JsonValue?) : DataBreakpoint

Constructs a DataBreakpoint from a JSON value.

:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>` ?

.. _function-dap_SetDataBreakpointsArguments_JsonValue_q_:

.. das:function:: SetDataBreakpointsArguments(data: JsonValue?) : SetDataBreakpointsArguments

Constructs a SetDataBreakpointsArguments from a JSON value, parsing the breakpoints array.

:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>` ?

.. _function-dap_DataBreakpointInfoArguments_JsonValue_q_:

.. das:function:: DataBreakpointInfoArguments(data: JsonValue?) : DataBreakpointInfoArguments

Constructs a DataBreakpointInfoArguments from a JSON value.

:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>` ?

.. _function-dap_SourceBreakpoint_JsonValue_q_:

.. das:function:: SourceBreakpoint(data: JsonValue?) : SourceBreakpoint

Constructs a SourceBreakpoint from a JSON value.

:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>` ?

.. _function-dap_Source_JsonValue_q_:

.. das:function:: Source(data: JsonValue?) : Source

Constructs a Source from a JSON value.

:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>` ?

.. _function-dap_SetBreakpointsArguments_JsonValue_q_:

.. das:function:: SetBreakpointsArguments(data: JsonValue?) : SetBreakpointsArguments

Constructs a SetBreakpointsArguments from a JSON value, parsing source and breakpoints.

:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>` ?

.. _function-dap_StackTraceArguments_JsonValue_q_:

.. das:function:: StackTraceArguments(data: JsonValue?) : StackTraceArguments

Constructs a StackTraceArguments from a JSON value.

:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>` ?

.. _function-dap_ScopesArguments_JsonValue_q_:

.. das:function:: ScopesArguments(data: JsonValue?) : ScopesArguments

Constructs a ScopesArguments from a JSON value.

:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>` ?

.. _function-dap_VariablesArguments_JsonValue_q_:

.. das:function:: VariablesArguments(data: JsonValue?) : VariablesArguments

Constructs a VariablesArguments from a JSON value.

:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>` ?

.. _function-dap_JV_Variable:

.. das:function:: JV(data: Variable) : JsonValue?

Converts a Variable struct to its DAP JSON representation.

:Arguments: * **data** :  :ref:`Variable <struct-dap-Variable>` 

.. _function-dap_ContinueArguments_JsonValue_q_:

.. das:function:: ContinueArguments(data: JsonValue?) : ContinueArguments

Constructs a ContinueArguments from a JSON value.

:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>` ?

.. _function-dap_PauseArguments_JsonValue_q_:

.. das:function:: PauseArguments(data: JsonValue?) : PauseArguments

Constructs a PauseArguments from a JSON value.

:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>` ?

.. _function-dap_StepInArguments_JsonValue_q_:

.. das:function:: StepInArguments(data: JsonValue?) : StepInArguments

Constructs a StepInArguments from a JSON value.

:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>` ?

.. _function-dap_NextArguments_JsonValue_q_:

.. das:function:: NextArguments(data: JsonValue?) : NextArguments

Constructs a NextArguments from a JSON value.

:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>` ?

.. _function-dap_StepOutArguments_JsonValue_q_:

.. das:function:: StepOutArguments(data: JsonValue?) : StepOutArguments

Constructs a StepOutArguments from a JSON value.

:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>` ?

.. _function-dap_EvaluateArguments_JsonValue_q_:

.. das:function:: EvaluateArguments(data: JsonValue?) : EvaluateArguments

Constructs an EvaluateArguments from a JSON value.

:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>` ?

.. _function-dap_JV_EvaluateResponse:

.. das:function:: JV(data: EvaluateResponse) : JsonValue?

Converts an EvaluateResponse struct to its DAP JSON representation.

:Arguments: * **data** :  :ref:`EvaluateResponse <struct-dap-EvaluateResponse>` 

.. _function-dap_joj_JsonValue_q__string:

.. das:function:: joj(val: JsonValue?; id: string) : JsonValue?

Returns a nested JSON object field by name, or null if not found.

:Arguments: * **val** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **id** : string

.. _function-dap_jon_JsonValue_q__string_double:

.. das:function:: jon(val: JsonValue?; id: string; defVal: double = 0lf) : double

|detail/function-dap-jon-0x9fdf0ee20ba20dd3|

:Arguments: * **val** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **id** : string

            * **defVal** : double

.. _function-dap_j_s_JsonValue_q__string:

.. das:function:: j_s(val: JsonValue?; defVal: string = "") : string

|detail/function-dap-j_s-0xab30853761d7730e|

:Arguments: * **val** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **defVal** : string

.. _function-dap_jos_JsonValue_q__string_string:

.. das:function:: jos(val: JsonValue?; id: string; defVal: string = "") : string

|detail/function-dap-jos-0x78c9ca8c4b8bacc6|

:Arguments: * **val** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **id** : string

            * **defVal** : string

.. _function-dap_job_JsonValue_q__string_bool:

.. das:function:: job(val: JsonValue?; id: string; defVal: bool = false) : bool

|detail/function-dap-job-0x478c3f582c0ab7e2|

:Arguments: * **val** :  :ref:`JsonValue <struct-json-JsonValue>` ?

            * **id** : string

            * **defVal** : bool


