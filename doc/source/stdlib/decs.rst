
.. _stdlib_decs:

=====================================
DECS, Daslang entity component system
=====================================

The DECS module implements low level functionality of Daslang entity component system.

All functions and symbols are in "decs" module, use require to get access to it. ::

    require daslib/decs

Under normal circumstances, the module is not used without the boost package::

    require daslib/desc_boost


++++++++++++
Type aliases
++++++++++++

.. _alias-ComponentHash:

.. das:attribute:: ComponentHash = uint64

Hash value of the ECS component type

.. _alias-TypeHash:

.. das:attribute:: TypeHash = uint64

Hash value of the individual type

.. _alias-DeferEval:

.. das:attribute:: DeferEval = lambda<(var act:DeferAction):void>

Lambda which holds deferred action. Typically creation of destruction of an entity.

.. _alias-ComponentMap:

.. das:attribute:: ComponentMap = array<ComponentValue>

Table of component values for individual entity.

.. _alias-PassFunction:

.. das:attribute:: PassFunction = function<void>

One of the callbacks which form individual pass.

++++++++++
Structures
++++++++++

.. _struct-decs-CTypeInfo:

.. das:attribute:: CTypeInfo

Type information for the individual component subtype.
Consists of type name and collection of type-specific routines to control type values during its lifetime, serialization, etc.

:Fields: * **basicType** :  :ref:`Type <enum-rtti-Type>` 

         * **mangledName** : string

         * **fullName** : string

         * **hash** :  :ref:`TypeHash <alias-TypeHash>` 

         * **size** : uint

         * **eraser** : function<(arr:array<uint8>):void>

         * **clonner** : function<(dst:array<uint8>;src:array<uint8>):void>

         * **serializer** : function<(arch: :ref:`Archive <struct-archive-Archive>` ;arr:array<uint8>):void>

         * **dumper** : function<(elem:void?):string>

         * **mkTypeInfo** : function<void>

         * **gc** : function<(src:array<uint8>):lambda<void>>

Type information for the individual component subtype.
Consists of type name and collection of type-specific routines to control type values during its lifetime, serialization, etc.

.. _struct-decs-Component:

.. das:attribute:: Component

Single ECS component. Contains component name, data, and data layout.

:Fields: * **name** : string

         * **hash** :  :ref:`ComponentHash <alias-ComponentHash>` 

         * **stride** : int

         * **data** : array<uint8>

         * **info** :  :ref:`CTypeInfo <struct-decs-CTypeInfo>` 

         * **gc_dummy** : lambda<void>

Single ECS component. Contains component name, data, and data layout.

.. _struct-decs-EntityId:

.. das:attribute:: EntityId

Unique identifier of the entity. Consists of id (index in the data array) and generation.

:Fields: * **id** : uint

         * **generation** : int

Unique identifier of the entity. Consists of id (index in the data array) and generation.

.. _struct-decs-Archetype:

.. das:attribute:: Archetype

ECS archetype. Archetype is unique combination of components.

:Fields: * **hash** :  :ref:`ComponentHash <alias-ComponentHash>` 

         * **components** : array< :ref:`Component <struct-decs-Component>` >

         * **size** : int

         * **eidIndex** : int

ECS archetype. Archetype is unique combination of components.

.. _struct-decs-ComponentValue:

.. das:attribute:: ComponentValue

Value of the component during creation or transformation.

:Fields: * **name** : string

         * **info** :  :ref:`CTypeInfo <struct-decs-CTypeInfo>` 

         * **data** : float4[4]

Value of the component during creation or transformation.

.. _struct-decs-EcsRequestPos:

.. das:attribute:: EcsRequestPos

Location of the ECS request in the code (source file and line number).

:Fields: * **file** : string

         * **line** : uint

Location of the ECS request in the code (source file and line number).

.. _struct-decs-EcsRequest:

.. das:attribute:: EcsRequest

Individual ESC requests. Contains list of required components, list of components which are required to be absent.
Caches list of archetypes, which match the request.

:Fields: * **hash** :  :ref:`ComponentHash <alias-ComponentHash>` 

         * **req** : array<string>

         * **reqn** : array<string>

         * **archetypes** : array<int>

         * **at** :  :ref:`EcsRequestPos <struct-decs-EcsRequestPos>` 

Individual ESC requests. Contains list of required components, list of components which are required to be absent.
Caches list of archetypes, which match the request.

.. _struct-decs-DecsState:

.. das:attribute:: DecsState

Entire state of the ECS system.
Conntains archtypes, entities and entity free-list, entity lokup table, all archetypes and archetype lookups, etc.

:Fields: * **archetypeLookup** : table< :ref:`ComponentHash <alias-ComponentHash>` ;int>

         * **allArchetypes** : array< :ref:`Archetype <struct-decs-Archetype>` >

         * **entityFreeList** : array< :ref:`EntityId <struct-decs-EntityId>` >

         * **entityLookup** : array<tuple<generation:int;archetype: :ref:`ComponentHash <alias-ComponentHash>` ;index:int>>

         * **componentTypeCheck** : table<string; :ref:`CTypeInfo <struct-decs-CTypeInfo>` >

         * **ecsQueries** : array< :ref:`EcsRequest <struct-decs-EcsRequest>` >

         * **queryLookup** : table< :ref:`ComponentHash <alias-ComponentHash>` ;int>

Entire state of the ECS system.
Conntains archtypes, entities and entity free-list, entity lokup table, all archetypes and archetype lookups, etc.

.. _struct-decs-DecsPass:

.. das:attribute:: DecsPass

:Fields: * **name** : string - Individual pass of the update of the ECS system.

         * **calls** : array< :ref:`PassFunction <alias-PassFunction>` > - Contains pass name and list of all pass calblacks.


+++++++++++++++++++++
Comparison and access
+++++++++++++++++++++

  *  :ref:`EntityId implicit== (a: EntityId implicit; b: EntityId implicit) : bool <function-_at_decs_c__c__eq__eq__CIS_ls_decs_c__c_EntityId_gr__CIS_ls_decs_c__c_EntityId_gr_>` 
  *  :ref:`EntityId implicit\!= (a: EntityId implicit; b: EntityId implicit) : bool <function-_at_decs_c__c__ex__eq__CIS_ls_decs_c__c_EntityId_gr__CIS_ls_decs_c__c_EntityId_gr_>` 
  *  :ref:`ComponentMap. (var cmp: ComponentMap; name: string) : ComponentValue& <function-_at_decs_c__c__dot__Y_ls_ComponentMap_gr_1_ls_S_ls_decs_c__c_ComponentValue_gr__gr_A_Cs>` 

.. _function-_at_decs_c__c__eq__eq__CIS_ls_decs_c__c_EntityId_gr__CIS_ls_decs_c__c_EntityId_gr_:

.. das:function:: EntityId implicit==(a: EntityId implicit; b: EntityId implicit) : bool

Equality operator for entity IDs.

:Arguments: * **a** :  :ref:`EntityId <struct-decs-EntityId>`  implicit

            * **b** :  :ref:`EntityId <struct-decs-EntityId>`  implicit

.. _function-_at_decs_c__c__ex__eq__CIS_ls_decs_c__c_EntityId_gr__CIS_ls_decs_c__c_EntityId_gr_:

.. das:function:: EntityId implicit!=(a: EntityId implicit; b: EntityId implicit) : bool

Inequality operator for entity IDs.

:Arguments: * **a** :  :ref:`EntityId <struct-decs-EntityId>`  implicit

            * **b** :  :ref:`EntityId <struct-decs-EntityId>`  implicit

.. _function-_at_decs_c__c__dot__Y_ls_ComponentMap_gr_1_ls_S_ls_decs_c__c_ComponentValue_gr__gr_A_Cs:

.. das:function:: ComponentMap.(cmp: ComponentMap; name: string) : ComponentValue&

Access to component value by name. For example::

    create_entity <| @ ( eid, cmp )
        cmp.pos := float3(i)    // same as cmp |> set("pos",float3(i))

:Arguments: * **cmp** :  :ref:`ComponentMap <alias-ComponentMap>` 

            * **name** : string

++++++++++++++++++++++
Access (get/set/clone)
++++++++++++++++++++++

  *  :ref:`clone (var cv: ComponentValue; val: EntityId) <function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__CS_ls_decs_c__c_EntityId_gr_>` 
  *  :ref:`clone (var cv: ComponentValue; val: bool) <function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Cb>` 
  *  :ref:`clone (var cv: ComponentValue; val: range) <function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Cr>` 
  *  :ref:`clone (var cv: ComponentValue; val: urange) <function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Cz>` 
  *  :ref:`clone (var cv: ComponentValue; val: range64) <function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Cr64>` 
  *  :ref:`clone (var cv: ComponentValue; val: urange64) <function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Cz64>` 
  *  :ref:`clone (var cv: ComponentValue; val: string) <function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Cs>` 
  *  :ref:`clone (var cv: ComponentValue; val: int) <function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Ci>` 
  *  :ref:`clone (var cv: ComponentValue; val: int8) <function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Ci8>` 
  *  :ref:`clone (var cv: ComponentValue; val: int16) <function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Ci16>` 
  *  :ref:`clone (var cv: ComponentValue; val: int64) <function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Ci64>` 
  *  :ref:`clone (var cv: ComponentValue; val: int2) <function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Ci2>` 
  *  :ref:`clone (var cv: ComponentValue; val: int3) <function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Ci3>` 
  *  :ref:`clone (var cv: ComponentValue; val: int4) <function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Ci4>` 
  *  :ref:`clone (var cv: ComponentValue; val: uint) <function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Cu>` 
  *  :ref:`clone (var cv: ComponentValue; val: uint8) <function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Cu8>` 
  *  :ref:`clone (var cv: ComponentValue; val: uint16) <function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Cu16>` 
  *  :ref:`clone (var cv: ComponentValue; val: uint64) <function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Cu64>` 
  *  :ref:`clone (var cv: ComponentValue; val: uint2) <function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Cu2>` 
  *  :ref:`clone (var cv: ComponentValue; val: uint3) <function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Cu3>` 
  *  :ref:`clone (var cv: ComponentValue; val: uint4) <function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Cu4>` 
  *  :ref:`clone (var cv: ComponentValue; val: float) <function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Cf>` 
  *  :ref:`clone (var cv: ComponentValue; val: float2) <function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Cf2>` 
  *  :ref:`clone (var cv: ComponentValue; val: float3) <function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Cf3>` 
  *  :ref:`clone (var cv: ComponentValue; val: float4) <function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Cf4>` 
  *  :ref:`clone (var cv: ComponentValue; val: float3x3) <function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__CH_ls_math_c__c_float3x3_gr_>` 
  *  :ref:`clone (var cv: ComponentValue; val: float3x4) <function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__CH_ls_math_c__c_float3x4_gr_>` 
  *  :ref:`clone (var cv: ComponentValue; val: float4x4) <function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__CH_ls_math_c__c_float4x4_gr_>` 
  *  :ref:`clone (var cv: ComponentValue; val: double) <function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Cd>` 
  *  :ref:`clone (var dst: Component; src: Component) <function-_at_decs_c__c_clone_S_ls_decs_c__c_Component_gr__CS_ls_decs_c__c_Component_gr_>` 
  *  :ref:`has (arch: Archetype; name: string) : bool <function-_at_decs_c__c_has_CS_ls_decs_c__c_Archetype_gr__Cs>` 
  *  :ref:`has (var cmp: ComponentMap; name: string) : bool <function-_at_decs_c__c_has_Y_ls_ComponentMap_gr_1_ls_S_ls_decs_c__c_ComponentValue_gr__gr_A_Cs>` 
  *  :ref:`remove (var cmp: ComponentMap; name: string) <function-_at_decs_c__c_remove_Y_ls_ComponentMap_gr_1_ls_S_ls_decs_c__c_ComponentValue_gr__gr_A_Cs>` 
  *  :ref:`set (var cv: ComponentValue; val: auto) : auto <function-_at_decs_c__c_set_S_ls_decs_c__c_ComponentValue_gr__C_dot_>` 
  *  :ref:`get (arch: Archetype; name: string; value: auto(TT)) : auto <function-_at_decs_c__c_get_CS_ls_decs_c__c_Archetype_gr__Cs_CY_ls_TT_gr__dot_>` 
  *  :ref:`get (var cmp: ComponentMap; name: string; var value: auto(TT)) : auto <function-_at_decs_c__c_get_Y_ls_ComponentMap_gr_1_ls_S_ls_decs_c__c_ComponentValue_gr__gr_A_Cs_Y_ls_TT_gr__dot_>` 
  *  :ref:`set (var cmp: ComponentMap; name: string; value: auto(TT)) : auto <function-_at_decs_c__c_set_Y_ls_ComponentMap_gr_1_ls_S_ls_decs_c__c_ComponentValue_gr__gr_A_Cs_CY_ls_TT_gr__dot_>` 

.. _function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__CS_ls_decs_c__c_EntityId_gr_:

.. das:function:: clone(cv: ComponentValue; val: EntityId)

|detail/function-decs-clone-0x620b30871767098a|

:Arguments: * **cv** :  :ref:`ComponentValue <struct-decs-ComponentValue>` 

            * **val** :  :ref:`EntityId <struct-decs-EntityId>` 

.. _function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Cb:

.. das:function:: clone(cv: ComponentValue; val: bool)

|detail/function-decs-clone-0xf9a2f5f617d86441|

:Arguments: * **cv** :  :ref:`ComponentValue <struct-decs-ComponentValue>` 

            * **val** : bool

.. _function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Cr:

.. das:function:: clone(cv: ComponentValue; val: range)

|detail/function-decs-clone-0xbabbfb43e1247e5a|

:Arguments: * **cv** :  :ref:`ComponentValue <struct-decs-ComponentValue>` 

            * **val** : range

.. _function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Cz:

.. das:function:: clone(cv: ComponentValue; val: urange)

|detail/function-decs-clone-0x9e0783c33915b72d|

:Arguments: * **cv** :  :ref:`ComponentValue <struct-decs-ComponentValue>` 

            * **val** : urange

.. _function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Cr64:

.. das:function:: clone(cv: ComponentValue; val: range64)

|detail/function-decs-clone-0x71b6605790aa7584|

:Arguments: * **cv** :  :ref:`ComponentValue <struct-decs-ComponentValue>` 

            * **val** : range64

.. _function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Cz64:

.. das:function:: clone(cv: ComponentValue; val: urange64)

|detail/function-decs-clone-0x27fdb97b14c17b|

:Arguments: * **cv** :  :ref:`ComponentValue <struct-decs-ComponentValue>` 

            * **val** : urange64

.. _function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Cs:

.. das:function:: clone(cv: ComponentValue; val: string)

|detail/function-decs-clone-0xf7cb3737e5c057e2|

:Arguments: * **cv** :  :ref:`ComponentValue <struct-decs-ComponentValue>` 

            * **val** : string

.. _function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Ci:

.. das:function:: clone(cv: ComponentValue; val: int)

|detail/function-decs-clone-0x1435a0ae1e5d4aa4|

:Arguments: * **cv** :  :ref:`ComponentValue <struct-decs-ComponentValue>` 

            * **val** : int

.. _function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Ci8:

.. das:function:: clone(cv: ComponentValue; val: int8)

|detail/function-decs-clone-0xb46aa3dd9885c714|

:Arguments: * **cv** :  :ref:`ComponentValue <struct-decs-ComponentValue>` 

            * **val** : int8

.. _function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Ci16:

.. das:function:: clone(cv: ComponentValue; val: int16)

|detail/function-decs-clone-0xb49c9cdd98dab12f|

:Arguments: * **cv** :  :ref:`ComponentValue <struct-decs-ComponentValue>` 

            * **val** : int16

.. _function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Ci64:

.. das:function:: clone(cv: ComponentValue; val: int64)

|detail/function-decs-clone-0xb49e99dd98de1216|

:Arguments: * **cv** :  :ref:`ComponentValue <struct-decs-ComponentValue>` 

            * **val** : int64

.. _function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Ci2:

.. das:function:: clone(cv: ComponentValue; val: int2)

|detail/function-decs-clone-0xb46a9ddd9885bce2|

:Arguments: * **cv** :  :ref:`ComponentValue <struct-decs-ComponentValue>` 

            * **val** : int2

.. _function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Ci3:

.. das:function:: clone(cv: ComponentValue; val: int3)

|detail/function-decs-clone-0xb46a9edd9885be95|

:Arguments: * **cv** :  :ref:`ComponentValue <struct-decs-ComponentValue>` 

            * **val** : int3

.. _function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Ci4:

.. das:function:: clone(cv: ComponentValue; val: int4)

|detail/function-decs-clone-0xb46a97dd9885b2b0|

:Arguments: * **cv** :  :ref:`ComponentValue <struct-decs-ComponentValue>` 

            * **val** : int4

.. _function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Cu:

.. das:function:: clone(cv: ComponentValue; val: uint)

|detail/function-decs-clone-0xe46da66cff0afff|

:Arguments: * **cv** :  :ref:`ComponentValue <struct-decs-ComponentValue>` 

            * **val** : uint

.. _function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Cu8:

.. das:function:: clone(cv: ComponentValue; val: uint8)

|detail/function-decs-clone-0xe0eda66cf9187ff|

:Arguments: * **cv** :  :ref:`ComponentValue <struct-decs-ComponentValue>` 

            * **val** : uint8

.. _function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Cu16:

.. das:function:: clone(cv: ComponentValue; val: uint16)

|detail/function-decs-clone-0x935ae5b2ce462b8b|

:Arguments: * **cv** :  :ref:`ComponentValue <struct-decs-ComponentValue>` 

            * **val** : uint16

.. _function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Cu64:

.. das:function:: clone(cv: ComponentValue; val: uint64)

|detail/function-decs-clone-0x96c0e7b2d12957f1|

:Arguments: * **cv** :  :ref:`ComponentValue <struct-decs-ComponentValue>` 

            * **val** : uint64

.. _function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Cu2:

.. das:function:: clone(cv: ComponentValue; val: uint2)

|detail/function-decs-clone-0xe14da66cf9bb9ff|

:Arguments: * **cv** :  :ref:`ComponentValue <struct-decs-ComponentValue>` 

            * **val** : uint2

.. _function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Cu3:

.. das:function:: clone(cv: ComponentValue; val: uint3)

|detail/function-decs-clone-0xe15da66cf9d6cff|

:Arguments: * **cv** :  :ref:`ComponentValue <struct-decs-ComponentValue>` 

            * **val** : uint3

.. _function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Cu4:

.. das:function:: clone(cv: ComponentValue; val: uint4)

|detail/function-decs-clone-0xe1ada66cfa5ebff|

:Arguments: * **cv** :  :ref:`ComponentValue <struct-decs-ComponentValue>` 

            * **val** : uint4

.. _function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Cf:

.. das:function:: clone(cv: ComponentValue; val: float)

|detail/function-decs-clone-0x9aced908009e13d1|

:Arguments: * **cv** :  :ref:`ComponentValue <struct-decs-ComponentValue>` 

            * **val** : float

.. _function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Cf2:

.. das:function:: clone(cv: ComponentValue; val: float2)

|detail/function-decs-clone-0xab8eab990c9bcab9|

:Arguments: * **cv** :  :ref:`ComponentValue <struct-decs-ComponentValue>` 

            * **val** : float2

.. _function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Cf3:

.. das:function:: clone(cv: ComponentValue; val: float3)

|detail/function-decs-clone-0xab8eaa990c9bc906|

:Arguments: * **cv** :  :ref:`ComponentValue <struct-decs-ComponentValue>` 

            * **val** : float3

.. _function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Cf4:

.. das:function:: clone(cv: ComponentValue; val: float4)

|detail/function-decs-clone-0xab8ead990c9bce1f|

:Arguments: * **cv** :  :ref:`ComponentValue <struct-decs-ComponentValue>` 

            * **val** : float4

.. _function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__CH_ls_math_c__c_float3x3_gr_:

.. das:function:: clone(cv: ComponentValue; val: float3x3)

|detail/function-decs-clone-0x4a4517116accfd0f|

:Arguments: * **cv** :  :ref:`ComponentValue <struct-decs-ComponentValue>` 

            * **val** :  :ref:`float3x3 <handle-math-float3x3>` 

.. _function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__CH_ls_math_c__c_float3x4_gr_:

.. das:function:: clone(cv: ComponentValue; val: float3x4)

|detail/function-decs-clone-0x4a4514116accf7f6|

:Arguments: * **cv** :  :ref:`ComponentValue <struct-decs-ComponentValue>` 

            * **val** :  :ref:`float3x4 <handle-math-float3x4>` 

.. _function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__CH_ls_math_c__c_float4x4_gr_:

.. das:function:: clone(cv: ComponentValue; val: float4x4)

|detail/function-decs-clone-0x4a4f26116ad56b11|

:Arguments: * **cv** :  :ref:`ComponentValue <struct-decs-ComponentValue>` 

            * **val** :  :ref:`float4x4 <handle-math-float4x4>` 

.. _function-_at_decs_c__c_clone_S_ls_decs_c__c_ComponentValue_gr__Cd:

.. das:function:: clone(cv: ComponentValue; val: double)

|detail/function-decs-clone-0xf10e5962d149721e|

:Arguments: * **cv** :  :ref:`ComponentValue <struct-decs-ComponentValue>` 

            * **val** : double

.. _function-_at_decs_c__c_clone_S_ls_decs_c__c_Component_gr__CS_ls_decs_c__c_Component_gr_:

.. das:function:: clone(dst: Component; src: Component)

Clones component value.

:Arguments: * **dst** :  :ref:`Component <struct-decs-Component>` 

            * **src** :  :ref:`Component <struct-decs-Component>` 

.. _function-_at_decs_c__c_has_CS_ls_decs_c__c_Archetype_gr__Cs:

.. das:function:: has(arch: Archetype; name: string) : bool

Returns true if object has specified subobjec.

:Arguments: * **arch** :  :ref:`Archetype <struct-decs-Archetype>` 

            * **name** : string

.. _function-_at_decs_c__c_has_Y_ls_ComponentMap_gr_1_ls_S_ls_decs_c__c_ComponentValue_gr__gr_A_Cs:

.. das:function:: has(cmp: ComponentMap; name: string) : bool

|detail/function-decs-has-0x8f4b2bc860fc05e6|

:Arguments: * **cmp** :  :ref:`ComponentMap <alias-ComponentMap>` 

            * **name** : string

.. _function-_at_decs_c__c_remove_Y_ls_ComponentMap_gr_1_ls_S_ls_decs_c__c_ComponentValue_gr__gr_A_Cs:

.. das:function:: remove(cmp: ComponentMap; name: string)

Removes speicified value from the component map.

:Arguments: * **cmp** :  :ref:`ComponentMap <alias-ComponentMap>` 

            * **name** : string

.. _function-_at_decs_c__c_set_S_ls_decs_c__c_ComponentValue_gr__C_dot_:

.. das:function:: set(cv: ComponentValue; val: auto) : auto

Sets individual component value. Verifies that the value is of the correct type.

:Arguments: * **cv** :  :ref:`ComponentValue <struct-decs-ComponentValue>` 

            * **val** : auto

.. _function-_at_decs_c__c_get_CS_ls_decs_c__c_Archetype_gr__Cs_CY_ls_TT_gr__dot_:

.. das:function:: get(arch: Archetype; name: string; value: auto(TT)) : auto

Creates temporary array of component given specific name and type of component.
If component is not found - panic.

:Arguments: * **arch** :  :ref:`Archetype <struct-decs-Archetype>` 

            * **name** : string

            * **value** : auto(TT)

.. _function-_at_decs_c__c_get_Y_ls_ComponentMap_gr_1_ls_S_ls_decs_c__c_ComponentValue_gr__gr_A_Cs_Y_ls_TT_gr__dot_:

.. das:function:: get(cmp: ComponentMap; name: string; value: auto(TT)) : auto

Gets component value specified by name and type.
Will panic if name matches but type does not.

:Arguments: * **cmp** :  :ref:`ComponentMap <alias-ComponentMap>` 

            * **name** : string

            * **value** : auto(TT)

.. _function-_at_decs_c__c_set_Y_ls_ComponentMap_gr_1_ls_S_ls_decs_c__c_ComponentValue_gr__gr_A_Cs_CY_ls_TT_gr__dot_:

.. das:function:: set(cmp: ComponentMap; name: string; value: auto(TT)) : auto

Set component value specified by name and type.
If value already exists, it is overwritten. If already existing value type is not the same - panic.

:Arguments: * **cmp** :  :ref:`ComponentMap <alias-ComponentMap>` 

            * **name** : string

            * **value** : auto(TT)

+++++++++++++++++++++++
Deubg and serialization
+++++++++++++++++++++++

  *  :ref:`describe (info: CTypeInfo) : string <function-_at_decs_c__c_describe_CS_ls_decs_c__c_CTypeInfo_gr_>` 
  *  :ref:`serialize (var arch: Archive; var src: Component) <function-_at_decs_c__c_serialize_S_ls_archive_c__c_Archive_gr__S_ls_decs_c__c_Component_gr_>` 
  *  :ref:`finalize (var cmp: Component) <function-_at_decs_c__c_finalize_S_ls_decs_c__c_Component_gr_>` 
  *  :ref:`debug_dump () <function-_at_decs_c__c_debug_dump>` 

.. _function-_at_decs_c__c_describe_CS_ls_decs_c__c_CTypeInfo_gr_:

.. das:function:: describe(info: CTypeInfo) : string

Returns textual description of the type.

:Arguments: * **info** :  :ref:`CTypeInfo <struct-decs-CTypeInfo>` 

.. _function-_at_decs_c__c_serialize_S_ls_archive_c__c_Archive_gr__S_ls_decs_c__c_Component_gr_:

.. das:function:: serialize(arch: Archive; src: Component)

Serializes component value.

:Arguments: * **arch** :  :ref:`Archive <struct-archive-Archive>` 

            * **src** :  :ref:`Component <struct-decs-Component>` 

.. _function-_at_decs_c__c_finalize_S_ls_decs_c__c_Component_gr_:

.. das:function:: finalize(cmp: Component)

Deletes component.

:Arguments: * **cmp** :  :ref:`Component <struct-decs-Component>` 

.. _function-_at_decs_c__c_debug_dump:

.. das:function:: debug_dump()

Prints out state of the ECS system.

++++++
Stages
++++++

  *  :ref:`register_decs_stage_call (name: string; pcall: PassFunction) <function-_at_decs_c__c_register_decs_stage_call_Cs_CY_ls_PassFunction_gr_1_ls_v_gr__at__at_>` 
  *  :ref:`decs_stage (name: string) <function-_at_decs_c__c_decs_stage_Cs>` 
  *  :ref:`commit () <function-_at_decs_c__c_commit>` 

.. _function-_at_decs_c__c_register_decs_stage_call_Cs_CY_ls_PassFunction_gr_1_ls_v_gr__at__at_:

.. das:function:: register_decs_stage_call(name: string; pcall: PassFunction)

Registration of a single pass callback. This is a low-level function, used by decs_boost macros.

:Arguments: * **name** : string

            * **pcall** :  :ref:`PassFunction <alias-PassFunction>` 

.. _function-_at_decs_c__c_decs_stage_Cs:

.. das:function:: decs_stage(name: string)

Invokes specific ECS pass.
`commit` is called before and after the invocation.

:Arguments: * **name** : string

.. _function-_at_decs_c__c_commit:

.. das:function:: commit()

Finishes all deferred actions.

++++++++++++++++
Deferred actions
++++++++++++++++

  *  :ref:`update_entity (entityid: EntityId implicit; var blk: lambda\<(eid:EntityId const;var cmp:array\<ComponentValue\>):void\>) <function-_at_decs_c__c_update_entity_CIS_ls_decs_c__c_EntityId_gr__N_ls_eid;cmp_gr_0_ls_CS_ls_decs_c__c_EntityId_gr_;Y_ls_ComponentMap_gr_1_ls_S_ls_decs_c__c_ComponentValue_gr__gr_A_gr_1_ls_v_gr__at_>` 
  *  :ref:`create_entity (var blk: lambda\<(eid:EntityId const;var cmp:array\<ComponentValue\>):void\>) : EntityId <function-_at_decs_c__c_create_entity_N_ls_eid;cmp_gr_0_ls_CS_ls_decs_c__c_EntityId_gr_;Y_ls_ComponentMap_gr_1_ls_S_ls_decs_c__c_ComponentValue_gr__gr_A_gr_1_ls_v_gr__at_>` 
  *  :ref:`delete_entity (entityid: EntityId implicit) <function-_at_decs_c__c_delete_entity_CIS_ls_decs_c__c_EntityId_gr_>` 

.. _function-_at_decs_c__c_update_entity_CIS_ls_decs_c__c_EntityId_gr__N_ls_eid;cmp_gr_0_ls_CS_ls_decs_c__c_EntityId_gr_;Y_ls_ComponentMap_gr_1_ls_S_ls_decs_c__c_ComponentValue_gr__gr_A_gr_1_ls_v_gr__at_:

.. das:function:: update_entity(entityid: EntityId implicit; blk: lambda<(eid:EntityId const;var cmp:array<ComponentValue>):void>)

Creates deferred action to update entity specified by id.

:Arguments: * **entityid** :  :ref:`EntityId <struct-decs-EntityId>`  implicit

            * **blk** : lambda<(eid: :ref:`EntityId <struct-decs-EntityId>` ;cmp: :ref:`ComponentMap <alias-ComponentMap>` ):void>

.. _function-_at_decs_c__c_create_entity_N_ls_eid;cmp_gr_0_ls_CS_ls_decs_c__c_EntityId_gr_;Y_ls_ComponentMap_gr_1_ls_S_ls_decs_c__c_ComponentValue_gr__gr_A_gr_1_ls_v_gr__at_:

.. das:function:: create_entity(blk: lambda<(eid:EntityId const;var cmp:array<ComponentValue>):void>) : EntityId

Creates deferred action to create entity.

:Arguments: * **blk** : lambda<(eid: :ref:`EntityId <struct-decs-EntityId>` ;cmp: :ref:`ComponentMap <alias-ComponentMap>` ):void>

.. _function-_at_decs_c__c_delete_entity_CIS_ls_decs_c__c_EntityId_gr_:

.. das:function:: delete_entity(entityid: EntityId implicit)

Creates deferred action to delete entity specified by id.

:Arguments: * **entityid** :  :ref:`EntityId <struct-decs-EntityId>`  implicit

++++++++++++
GC and reset
++++++++++++

  *  :ref:`restart () <function-_at_decs_c__c_restart>` 
  *  :ref:`before_gc () <function-_at_decs_c__c_before_gc>` 
  *  :ref:`after_gc () <function-_at_decs_c__c_after_gc>` 

.. _function-_at_decs_c__c_restart:

.. das:function:: restart()

Restarts ECS by erasing all deferred actions and entire state.

.. _function-_at_decs_c__c_before_gc:

.. das:function:: before_gc()

Low level callback to be called before the garbage collection.
This is a low-level function typically used by `live`.

.. _function-_at_decs_c__c_after_gc:

.. das:function:: after_gc()

Low level callback to be called after the garbage collection.
This is a low-level function typically used by `live`.

+++++++++
Iteration
+++++++++

  *  :ref:`for_each_archetype (var erq: EcsRequest; blk: block\<(arch:Archetype const):void\>) <function-_at_decs_c__c_for_each_archetype_S_ls_decs_c__c_EcsRequest_gr__CN_ls_arch_gr_0_ls_CS_ls_decs_c__c_Archetype_gr__gr_1_ls_v_gr__builtin_>` 
  *  :ref:`for_eid_archetype (eid: EntityId implicit; hash: ComponentHash; var erq: function\<EcsRequest\>; blk: block\<(arch:Archetype const;index:int const):void\>) : bool <function-_at_decs_c__c_for_eid_archetype_CIS_ls_decs_c__c_EntityId_gr__CY_ls_ComponentHash_gr_u64_1_ls_S_ls_decs_c__c_EcsRequest_gr__gr__at__at__CN_ls_arch;index_gr_0_ls_CS_ls_decs_c__c_Archetype_gr_;Ci_gr_1_ls_v_gr__builtin_>` 
  *  :ref:`for_each_archetype (hash: ComponentHash; var erq: function\<EcsRequest\>; blk: block\<(arch:Archetype const):void\>) <function-_at_decs_c__c_for_each_archetype_CY_ls_ComponentHash_gr_u64_1_ls_S_ls_decs_c__c_EcsRequest_gr__gr__at__at__CN_ls_arch_gr_0_ls_CS_ls_decs_c__c_Archetype_gr__gr_1_ls_v_gr__builtin_>` 
  *  :ref:`for_each_archetype_find (hash: ComponentHash; var erq: function\<EcsRequest\>; blk: block\<(arch:Archetype const):bool\>) : bool <function-_at_decs_c__c_for_each_archetype_find_CY_ls_ComponentHash_gr_u64_1_ls_S_ls_decs_c__c_EcsRequest_gr__gr__at__at__CN_ls_arch_gr_0_ls_CS_ls_decs_c__c_Archetype_gr__gr_1_ls_b_gr__builtin_>` 
  *  :ref:`decs_array (atype: auto(TT); src: array\<uint8\>; capacity: int) : auto <function-_at_decs_c__c_decs_array_CY_ls_TT_gr__dot__C1_ls_u8_gr_A_Ci>` 
  *  :ref:`get_ro (arch: Archetype; name: string; value: auto(TT)[]) : array\<TT[-2]\> <function-_at_decs_c__c_get_ro_CS_ls_decs_c__c_Archetype_gr__Cs_C_lb_-1_rb_Y_ls_TT_gr__dot_>` 
  *  :ref:`get_ro (arch: Archetype; name: string; value: auto(TT)) : array\<TT\> <function-_at_decs_c__c_get_ro_CS_ls_decs_c__c_Archetype_gr__Cs_CY_ls_TT_gr__dot__%_ls__ex_()_gr_>` 
  *  :ref:`get_default_ro (arch: Archetype; name: string; value: auto(TT)) : iterator\<TT const&\> <function-_at_decs_c__c_get_default_ro_CS_ls_decs_c__c_Archetype_gr__Cs_CY_ls_TT_gr__dot_>` 
  *  :ref:`get_optional (arch: Archetype; name: string; value: auto(TT)?) : iterator\<TT?\> <function-_at_decs_c__c_get_optional_CS_ls_decs_c__c_Archetype_gr__Cs_C1_ls_Y_ls_TT_gr__dot__gr__qm_>` 

.. _function-_at_decs_c__c_for_each_archetype_S_ls_decs_c__c_EcsRequest_gr__CN_ls_arch_gr_0_ls_CS_ls_decs_c__c_Archetype_gr__gr_1_ls_v_gr__builtin_:

.. das:function:: for_each_archetype(erq: EcsRequest; blk: block<(arch:Archetype const):void>)

Invokes block for each entity of each archetype that can be processed by the request.

:Arguments: * **erq** :  :ref:`EcsRequest <struct-decs-EcsRequest>` 

            * **blk** : block<(arch: :ref:`Archetype <struct-decs-Archetype>` ):void>

.. _function-_at_decs_c__c_for_eid_archetype_CIS_ls_decs_c__c_EntityId_gr__CY_ls_ComponentHash_gr_u64_1_ls_S_ls_decs_c__c_EcsRequest_gr__gr__at__at__CN_ls_arch;index_gr_0_ls_CS_ls_decs_c__c_Archetype_gr_;Ci_gr_1_ls_v_gr__builtin_:

.. das:function:: for_eid_archetype(eid: EntityId implicit; hash: ComponentHash; erq: function<EcsRequest>; blk: block<(arch:Archetype const;index:int const):void>) : bool

Invokes block for the specific entity id, given request.
Request is returned by a specified function.

:Arguments: * **eid** :  :ref:`EntityId <struct-decs-EntityId>`  implicit

            * **hash** :  :ref:`ComponentHash <alias-ComponentHash>` 

            * **erq** : function<void>

            * **blk** : block<(arch: :ref:`Archetype <struct-decs-Archetype>` ;index:int):void>

.. _function-_at_decs_c__c_for_each_archetype_CY_ls_ComponentHash_gr_u64_1_ls_S_ls_decs_c__c_EcsRequest_gr__gr__at__at__CN_ls_arch_gr_0_ls_CS_ls_decs_c__c_Archetype_gr__gr_1_ls_v_gr__builtin_:

.. das:function:: for_each_archetype(hash: ComponentHash; erq: function<EcsRequest>; blk: block<(arch:Archetype const):void>)

Invokes block for each entity of each archetype that can be processed by the request.
Request is returned by a specified function.

:Arguments: * **hash** :  :ref:`ComponentHash <alias-ComponentHash>` 

            * **erq** : function<void>

            * **blk** : block<(arch: :ref:`Archetype <struct-decs-Archetype>` ):void>

.. _function-_at_decs_c__c_for_each_archetype_find_CY_ls_ComponentHash_gr_u64_1_ls_S_ls_decs_c__c_EcsRequest_gr__gr__at__at__CN_ls_arch_gr_0_ls_CS_ls_decs_c__c_Archetype_gr__gr_1_ls_b_gr__builtin_:

.. das:function:: for_each_archetype_find(hash: ComponentHash; erq: function<EcsRequest>; blk: block<(arch:Archetype const):bool>) : bool

Invokes block for each entity of each archetype that can be processed by the request.
Request is returned by a specified function.
If block returns true, iteration is stopped.

:Arguments: * **hash** :  :ref:`ComponentHash <alias-ComponentHash>` 

            * **erq** : function<void>

            * **blk** : block<(arch: :ref:`Archetype <struct-decs-Archetype>` ):bool>

.. _function-_at_decs_c__c_decs_array_CY_ls_TT_gr__dot__C1_ls_u8_gr_A_Ci:

.. das:function:: decs_array(atype: auto(TT); src: array<uint8>; capacity: int) : auto

Low level function returns temporary array of component given specific type of component.

:Arguments: * **atype** : auto(TT)

            * **src** : array<uint8>

            * **capacity** : int

.. _function-_at_decs_c__c_get_ro_CS_ls_decs_c__c_Archetype_gr__Cs_C_lb_-1_rb_Y_ls_TT_gr__dot_:

.. das:function:: get_ro(arch: Archetype; name: string; value: auto(TT)[]) : array<TT[-2]>

Returns const temporary array of component given specific name and type of component for array components.

:Arguments: * **arch** :  :ref:`Archetype <struct-decs-Archetype>` 

            * **name** : string

            * **value** : auto(TT)[-1]

.. _function-_at_decs_c__c_get_ro_CS_ls_decs_c__c_Archetype_gr__Cs_CY_ls_TT_gr__dot__%_ls__ex_()_gr_:

.. das:function:: get_ro(arch: Archetype; name: string; value: auto(TT)) : array<TT>

Returns const temporary array of component given specific name and type of component for regular components.

:Arguments: * **arch** :  :ref:`Archetype <struct-decs-Archetype>` 

            * **name** : string

            * **value** : auto(TT)

.. _function-_at_decs_c__c_get_default_ro_CS_ls_decs_c__c_Archetype_gr__Cs_CY_ls_TT_gr__dot_:

.. das:function:: get_default_ro(arch: Archetype; name: string; value: auto(TT)) : iterator<TT const&>

Returns const iterator of component given specific name and type of component.
If component is not found - iterator will kepp returning the specified value.

:Arguments: * **arch** :  :ref:`Archetype <struct-decs-Archetype>` 

            * **name** : string

            * **value** : auto(TT)

.. _function-_at_decs_c__c_get_optional_CS_ls_decs_c__c_Archetype_gr__Cs_C1_ls_Y_ls_TT_gr__dot__gr__qm_:

.. das:function:: get_optional(arch: Archetype; name: string; value: auto(TT)?) : iterator<TT?>

Returns const iterator of component given specific name and type of component.
If component is not found - iterator will kepp returning default value for the component type.

:Arguments: * **arch** :  :ref:`Archetype <struct-decs-Archetype>` 

            * **name** : string

            * **value** : auto(TT)?

+++++++
Request
+++++++

  *  :ref:`EcsRequestPos (at: LineInfo) : EcsRequestPos <function-_at_decs_c__c_EcsRequestPos_CH_ls_rtti_c__c_LineInfo_gr_>` 
  *  :ref:`verify_request (var erq: EcsRequest) : tuple\<ok:bool;error:string\> <function-_at_decs_c__c_verify_request_S_ls_decs_c__c_EcsRequest_gr_>` 
  *  :ref:`compile_request (var erq: EcsRequest) <function-_at_decs_c__c_compile_request_S_ls_decs_c__c_EcsRequest_gr_>` 
  *  :ref:`lookup_request (var erq: EcsRequest) : int <function-_at_decs_c__c_lookup_request_S_ls_decs_c__c_EcsRequest_gr_>` 

.. _function-_at_decs_c__c_EcsRequestPos_CH_ls_rtti_c__c_LineInfo_gr_:

.. das:function:: EcsRequestPos(at: LineInfo) : EcsRequestPos

Constructs EcsRequestPos from rtti::LineInfo.

:Arguments: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

.. _function-_at_decs_c__c_verify_request_S_ls_decs_c__c_EcsRequest_gr_:

.. das:function:: verify_request(erq: EcsRequest) : tuple<ok:bool;error:string>

Verifies ESC request. Returns pair of boolean (true for OK) and error message.

:Arguments: * **erq** :  :ref:`EcsRequest <struct-decs-EcsRequest>` 

.. _function-_at_decs_c__c_compile_request_S_ls_decs_c__c_EcsRequest_gr_:

.. das:function:: compile_request(erq: EcsRequest)

Compiles ESC request, by creating request hash.

:Arguments: * **erq** :  :ref:`EcsRequest <struct-decs-EcsRequest>` 

.. _function-_at_decs_c__c_lookup_request_S_ls_decs_c__c_EcsRequest_gr_:

.. das:function:: lookup_request(erq: EcsRequest) : int

Looks up ESC request in the request cache.

:Arguments: * **erq** :  :ref:`EcsRequest <struct-decs-EcsRequest>` 


