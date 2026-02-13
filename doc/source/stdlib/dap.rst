
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

|detail/function-dap-InitializeRequestArguments-0xcfda5e72467f24c8|

:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>` ?

.. _function-dap_DisconnectArguments_JsonValue_q_:

.. das:function:: DisconnectArguments(data: JsonValue?) : DisconnectArguments

|detail/function-dap-DisconnectArguments-0x9be176d413d449e|

:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>` ?

.. _function-dap_DataBreakpoint_JsonValue_q_:

.. das:function:: DataBreakpoint(data: JsonValue?) : DataBreakpoint

|detail/function-dap-DataBreakpoint-0x603e46207abd3b58|

:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>` ?

.. _function-dap_SetDataBreakpointsArguments_JsonValue_q_:

.. das:function:: SetDataBreakpointsArguments(data: JsonValue?) : SetDataBreakpointsArguments

|detail/function-dap-SetDataBreakpointsArguments-0xf5cb33aef6c0896a|

:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>` ?

.. _function-dap_DataBreakpointInfoArguments_JsonValue_q_:

.. das:function:: DataBreakpointInfoArguments(data: JsonValue?) : DataBreakpointInfoArguments

|detail/function-dap-DataBreakpointInfoArguments-0xe64cbed1c6652ce3|

:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>` ?

.. _function-dap_SourceBreakpoint_JsonValue_q_:

.. das:function:: SourceBreakpoint(data: JsonValue?) : SourceBreakpoint

|detail/function-dap-SourceBreakpoint-0x4524798dd369cb53|

:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>` ?

.. _function-dap_Source_JsonValue_q_:

.. das:function:: Source(data: JsonValue?) : Source

|detail/function-dap-Source-0x5ab400eaa5ff1f5e|

:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>` ?

.. _function-dap_SetBreakpointsArguments_JsonValue_q_:

.. das:function:: SetBreakpointsArguments(data: JsonValue?) : SetBreakpointsArguments

|detail/function-dap-SetBreakpointsArguments-0xf8d315f1db853494|

:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>` ?

.. _function-dap_StackTraceArguments_JsonValue_q_:

.. das:function:: StackTraceArguments(data: JsonValue?) : StackTraceArguments

|detail/function-dap-StackTraceArguments-0x73166280aac7ce65|

:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>` ?

.. _function-dap_ScopesArguments_JsonValue_q_:

.. das:function:: ScopesArguments(data: JsonValue?) : ScopesArguments

|detail/function-dap-ScopesArguments-0x6bdcd049ac80a256|

:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>` ?

.. _function-dap_VariablesArguments_JsonValue_q_:

.. das:function:: VariablesArguments(data: JsonValue?) : VariablesArguments

|detail/function-dap-VariablesArguments-0x5cf903e7126b3367|

:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>` ?

.. _function-dap_JV_Variable:

.. das:function:: JV(data: Variable) : JsonValue?

|detail/function-dap-JV-0x486b32165ac0d5cc|

:Arguments: * **data** :  :ref:`Variable <struct-dap-Variable>` 

.. _function-dap_ContinueArguments_JsonValue_q_:

.. das:function:: ContinueArguments(data: JsonValue?) : ContinueArguments

|detail/function-dap-ContinueArguments-0x6679b7846ef3beb6|

:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>` ?

.. _function-dap_PauseArguments_JsonValue_q_:

.. das:function:: PauseArguments(data: JsonValue?) : PauseArguments

|detail/function-dap-PauseArguments-0x579d107b61f9dcf3|

:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>` ?

.. _function-dap_StepInArguments_JsonValue_q_:

.. das:function:: StepInArguments(data: JsonValue?) : StepInArguments

|detail/function-dap-StepInArguments-0x390039c5c5ee3b1c|

:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>` ?

.. _function-dap_NextArguments_JsonValue_q_:

.. das:function:: NextArguments(data: JsonValue?) : NextArguments

|detail/function-dap-NextArguments-0xa2308a0436bd5ac3|

:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>` ?

.. _function-dap_StepOutArguments_JsonValue_q_:

.. das:function:: StepOutArguments(data: JsonValue?) : StepOutArguments

|detail/function-dap-StepOutArguments-0x9e9761e1594e0dee|

:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>` ?

.. _function-dap_EvaluateArguments_JsonValue_q_:

.. das:function:: EvaluateArguments(data: JsonValue?) : EvaluateArguments

|detail/function-dap-EvaluateArguments-0xc1bfcdd2690cf312|

:Arguments: * **data** :  :ref:`JsonValue <struct-json-JsonValue>` ?

.. _function-dap_JV_EvaluateResponse:

.. das:function:: JV(data: EvaluateResponse) : JsonValue?

|detail/function-dap-JV-0x8e9c49df66f20bbb|

:Arguments: * **data** :  :ref:`EvaluateResponse <struct-dap-EvaluateResponse>` 

.. _function-dap_joj_JsonValue_q__string:

.. das:function:: joj(val: JsonValue?; id: string) : JsonValue?

|detail/function-dap-joj-0x994db487d35b8589|

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


