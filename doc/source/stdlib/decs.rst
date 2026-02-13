
.. _stdlib_decs:

=====================================
DECS, Daslang entity component system
=====================================

The DECS module implements a Data-oriented Entity Component System.
Entities are identified by integer IDs and store components as typed data.
Systems query and process entities by their component signatures,
enabling cache-friendly batch processing of game objects.

All functions and symbols are in "decs" module, use require to get access to it. ::

    require daslib/decs

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

+++++++++
Constants
+++++++++

.. _global-decs-INVALID_ENTITY_ID:

.. das:attribute:: INVALID_ENTITY_ID = struct<decs::EntityId>(uninitialized )

Entity ID which represents invalid entity.

++++++++++
Structures
++++++++++

.. _struct-decs-CTypeInfo:

.. das:attribute:: CTypeInfo

Type information for the individual component subtype.
Consists of type name and collection of type-specific routines to control type values during its lifetime, serialization, etc.

:Fields: * **basicType** :  :ref:`Type <enum-rtti-Type>`  - basic type of the component

         * **mangledName** : string - mangled name of the type

         * **fullName** : string - full name of the type

         * **hash** :  :ref:`TypeHash <alias-TypeHash>`  - hash of the type

         * **size** : uint - size of the type

         * **eraser** : function<(arr:array<uint8>):void> - function to erase component value

         * **clonner** : function<(dst:array<uint8>;src:array<uint8>):void> - function to clone component value

         * **serializer** : function<(arch: :ref:`Archive <struct-archive-Archive>` ;arr:array<uint8>;name:string):void> - function to serialize component value

         * **dumper** : function<(elem:void?):string> - function to dump component value as text

         * **mkTypeInfo** : function<void> - function to make TypeInfo for the component type

         * **gc** : function<(src:array<uint8>):lambda<void>> - function to perform GC marking on the component value


.. _struct-decs-Component:

.. das:attribute:: Component

Single ECS component. Contains component name, data, and data layout.

:Fields: * **name** : string - name of the component

         * **hash** :  :ref:`ComponentHash <alias-ComponentHash>`  - hash of the component

         * **stride** : int - stride of the component data

         * **data** : array<uint8> - raw data of the component

         * **info** :  :ref:`CTypeInfo <struct-decs-CTypeInfo>`  - type information of the component

         * **gc_dummy** : lambda<void> - this is here so that GC can find real representation of data


.. _struct-decs-EntityId:

.. das:attribute:: EntityId

:Fields: * **id** : uint - Unique identifier of the entity. Consists of id (index in the data array) and generation.

         * **generation** : int - index of the entity


.. _struct-decs-Archetype:

.. das:attribute:: Archetype

ECS archetype. Archetype is unique combination of components.

:Fields: * **hash** :  :ref:`ComponentHash <alias-ComponentHash>`  - hash of the archetype (combination of component hashes)

         * **components** : array< :ref:`Component <struct-decs-Component>` > - list of components in the archetype

         * **size** : int - number of entities in the archetype

         * **eidIndex** : int - index of the 'eid' component in the components array


.. _struct-decs-ComponentValue:

.. das:attribute:: ComponentValue

Value of the component during creation or transformation.

:Fields: * **name** : string - name of the component

         * **info** :  :ref:`CTypeInfo <struct-decs-CTypeInfo>`  - type information of the component

         * **data** : float4[4] - raw data of the component


.. _struct-decs-EcsRequestPos:

.. das:attribute:: EcsRequestPos

Location of the ECS request in the code (source file and line number).

:Fields: * **file** : string - source file

         * **line** : uint - line number


.. _struct-decs-EcsRequest:

.. das:attribute:: EcsRequest

Individual ECS requests. Contains list of required components, list of components which are required to be absent.
Caches list of archetypes, which match the request.

:Fields: * **hash** :  :ref:`ComponentHash <alias-ComponentHash>`  - hash of the request

         * **req** : array<string> - required components

         * **reqn** : array<string> - required components which are not present

         * **archetypes** : array<int> - sorted list of matching archetypes

         * **at** :  :ref:`EcsRequestPos <struct-decs-EcsRequestPos>`  - location of the request in the code


.. _struct-decs-DecsState:

.. das:attribute:: DecsState

Entire state of the ECS system.
Contains archetypes, entities and entity free-list, entity lookup table, all archetypes and archetype lookups, etc.

:Fields: * **archetypeLookup** : table< :ref:`ComponentHash <alias-ComponentHash>` ;int> - lookup of archetype by its hash

         * **allArchetypes** : array< :ref:`Archetype <struct-decs-Archetype>` > - all archetypes in the system

         * **entityFreeList** : array< :ref:`EntityId <struct-decs-EntityId>` > - list of free entity IDs

         * **entityLookup** : array<tuple<generation:int;archetype: :ref:`ComponentHash <alias-ComponentHash>` ;index:int>> - lookup of entity by its ID

         * **componentTypeCheck** : table<string; :ref:`CTypeInfo <struct-decs-CTypeInfo>` > - lookup of component type info by its name

         * **ecsQueries** : array< :ref:`EcsRequest <struct-decs-EcsRequest>` > - all ECS requests

         * **queryLookup** : table< :ref:`ComponentHash <alias-ComponentHash>` ;int> - lookup of ECS request by its hash


.. _struct-decs-DecsPass:

.. das:attribute:: DecsPass

Individual pass of the update of the ECS system.
Contains pass name and list of all pass callbacks.

:Fields: * **name** : string - name of the pass

         * **calls** : array< :ref:`PassFunction <alias-PassFunction>` > - list of all pass callbacks


+++++++++++++++++++++
Comparison and access
+++++++++++++++++++++

  *  :ref:`ComponentMap. (var cmp: ComponentMap; name: string) : ComponentValue& <function-decs__dot__ComponentMap_string>` 
  *  :ref:`EntityId implicit\!= (a: EntityId implicit; b: EntityId implicit) : bool <function-decs__ex__eq__EntityId_implicit_EntityId_implicit>` 
  *  :ref:`EntityId implicit== (a: EntityId implicit; b: EntityId implicit) : bool <function-decs__eq__eq__EntityId_implicit_EntityId_implicit>` 

.. _function-decs__dot__ComponentMap_string:

.. das:function:: ComponentMap.(cmp: ComponentMap; name: string) : ComponentValue&

Access to component value by name. For example::

    create_entity <| @ ( eid, cmp )
        cmp.pos := float3(i)    // same as cmp |> set("pos",float3(i))

:Arguments: * **cmp** :  :ref:`ComponentMap <alias-ComponentMap>` 

            * **name** : string

.. _function-decs__ex__eq__EntityId_implicit_EntityId_implicit:

.. das:function:: EntityId implicit!=(a: EntityId implicit; b: EntityId implicit) : bool

Inequality operator for entity IDs.

:Arguments: * **a** :  :ref:`EntityId <struct-decs-EntityId>`  implicit

            * **b** :  :ref:`EntityId <struct-decs-EntityId>`  implicit

.. _function-decs__eq__eq__EntityId_implicit_EntityId_implicit:

.. das:function:: EntityId implicit==(a: EntityId implicit; b: EntityId implicit) : bool

Equality operator for entity IDs.

:Arguments: * **a** :  :ref:`EntityId <struct-decs-EntityId>`  implicit

            * **b** :  :ref:`EntityId <struct-decs-EntityId>`  implicit

++++++++++++++++++++++
Access (get/set/clone)
++++++++++++++++++++++

  *  :ref:`clone (var cv: ComponentValue; val: bool) <function-decs_clone_ComponentValue_bool>` 
  *  :ref:`clone (var cv: ComponentValue; val: EntityId) <function-decs_clone_ComponentValue_EntityId>` 
  *  :ref:`clone (var cv: ComponentValue; val: urange) <function-decs_clone_ComponentValue_urange>` 
  *  :ref:`clone (var cv: ComponentValue; val: range) <function-decs_clone_ComponentValue_range>` 
  *  :ref:`clone (var cv: ComponentValue; val: string) <function-decs_clone_ComponentValue_string>` 
  *  :ref:`clone (var cv: ComponentValue; val: urange64) <function-decs_clone_ComponentValue_urange64>` 
  *  :ref:`clone (var cv: ComponentValue; val: int) <function-decs_clone_ComponentValue_int>` 
  *  :ref:`clone (var cv: ComponentValue; val: range64) <function-decs_clone_ComponentValue_range64>` 
  *  :ref:`clone (var cv: ComponentValue; val: int64) <function-decs_clone_ComponentValue_int64>` 
  *  :ref:`clone (var cv: ComponentValue; val: int16) <function-decs_clone_ComponentValue_int16>` 
  *  :ref:`clone (var cv: ComponentValue; val: int2) <function-decs_clone_ComponentValue_int2>` 
  *  :ref:`clone (var cv: ComponentValue; val: int3) <function-decs_clone_ComponentValue_int3>` 
  *  :ref:`clone (var cv: ComponentValue; val: int4) <function-decs_clone_ComponentValue_int4>` 
  *  :ref:`clone (var cv: ComponentValue; val: int8) <function-decs_clone_ComponentValue_int8>` 
  *  :ref:`clone (var cv: ComponentValue; val: uint16) <function-decs_clone_ComponentValue_uint16>` 
  *  :ref:`clone (var cv: ComponentValue; val: uint8) <function-decs_clone_ComponentValue_uint8>` 
  *  :ref:`clone (var cv: ComponentValue; val: uint64) <function-decs_clone_ComponentValue_uint64>` 
  *  :ref:`clone (var cv: ComponentValue; val: uint2) <function-decs_clone_ComponentValue_uint2>` 
  *  :ref:`clone (var cv: ComponentValue; val: uint3) <function-decs_clone_ComponentValue_uint3>` 
  *  :ref:`clone (var cv: ComponentValue; val: float) <function-decs_clone_ComponentValue_float>` 
  *  :ref:`clone (var cv: ComponentValue; val: uint4) <function-decs_clone_ComponentValue_uint4>` 
  *  :ref:`clone (var cv: ComponentValue; val: float2) <function-decs_clone_ComponentValue_float2>` 
  *  :ref:`clone (var cv: ComponentValue; val: float3) <function-decs_clone_ComponentValue_float3>` 
  *  :ref:`clone (var cv: ComponentValue; val: uint) <function-decs_clone_ComponentValue_uint>` 
  *  :ref:`clone (var cv: ComponentValue; val: float3x4) <function-decs_clone_ComponentValue_float3x4>` 
  *  :ref:`clone (var cv: ComponentValue; val: float3x3) <function-decs_clone_ComponentValue_float3x3>` 
  *  :ref:`clone (var cv: ComponentValue; val: float4x4) <function-decs_clone_ComponentValue_float4x4>` 
  *  :ref:`clone (var cv: ComponentValue; val: double) <function-decs_clone_ComponentValue_double>` 
  *  :ref:`clone (var dst: Component; src: Component) <function-decs_clone_Component_Component>` 
  *  :ref:`clone (var cv: ComponentValue; val: float4) <function-decs_clone_ComponentValue_float4>` 
  *  :ref:`get (var cmp: ComponentMap; name: string; var value: auto(TT)) : auto <function-decs_get_ComponentMap_string_autoTT_0x3b5>` 
  *  :ref:`get (arch: Archetype; name: string; value: auto(TT)) : auto <function-decs_get_Archetype_string_autoTT_0x2e0>` 
  *  :ref:`has (var cmp: ComponentMap; name: string) : bool <function-decs_has_ComponentMap_string>` 
  *  :ref:`has (arch: Archetype; name: string) : bool <function-decs_has_Archetype_string>` 
  *  :ref:`remove (var cmp: ComponentMap; name: string) <function-decs_remove_ComponentMap_string>` 
  *  :ref:`set (var cmp: ComponentMap; name: string; value: auto(TT)) : auto <function-decs_set_ComponentMap_string_autoTT_0x48c>` 
  *  :ref:`set (var cv: ComponentValue; val: auto) : auto <function-decs_set_ComponentValue_auto_0xa7>` 


clone
^^^^^

.. _function-decs_clone_ComponentValue_bool:

.. das:function:: clone(cv: ComponentValue; val: bool)

Sets individual component value. Verifies that the value is of the correct type.

:Arguments: * **cv** :  :ref:`ComponentValue <struct-decs-ComponentValue>` 

            * **val** : bool

.. _function-decs_clone_ComponentValue_EntityId:

.. das:function:: clone(cv: ComponentValue; val: EntityId)

.. _function-decs_clone_ComponentValue_urange:

.. das:function:: clone(cv: ComponentValue; val: urange)

.. _function-decs_clone_ComponentValue_range:

.. das:function:: clone(cv: ComponentValue; val: range)

.. _function-decs_clone_ComponentValue_string:

.. das:function:: clone(cv: ComponentValue; val: string)

.. _function-decs_clone_ComponentValue_urange64:

.. das:function:: clone(cv: ComponentValue; val: urange64)

.. _function-decs_clone_ComponentValue_int:

.. das:function:: clone(cv: ComponentValue; val: int)

.. _function-decs_clone_ComponentValue_range64:

.. das:function:: clone(cv: ComponentValue; val: range64)

.. _function-decs_clone_ComponentValue_int64:

.. das:function:: clone(cv: ComponentValue; val: int64)

.. _function-decs_clone_ComponentValue_int16:

.. das:function:: clone(cv: ComponentValue; val: int16)

.. _function-decs_clone_ComponentValue_int2:

.. das:function:: clone(cv: ComponentValue; val: int2)

.. _function-decs_clone_ComponentValue_int3:

.. das:function:: clone(cv: ComponentValue; val: int3)

.. _function-decs_clone_ComponentValue_int4:

.. das:function:: clone(cv: ComponentValue; val: int4)

.. _function-decs_clone_ComponentValue_int8:

.. das:function:: clone(cv: ComponentValue; val: int8)

.. _function-decs_clone_ComponentValue_uint16:

.. das:function:: clone(cv: ComponentValue; val: uint16)

.. _function-decs_clone_ComponentValue_uint8:

.. das:function:: clone(cv: ComponentValue; val: uint8)

.. _function-decs_clone_ComponentValue_uint64:

.. das:function:: clone(cv: ComponentValue; val: uint64)

.. _function-decs_clone_ComponentValue_uint2:

.. das:function:: clone(cv: ComponentValue; val: uint2)

.. _function-decs_clone_ComponentValue_uint3:

.. das:function:: clone(cv: ComponentValue; val: uint3)

.. _function-decs_clone_ComponentValue_float:

.. das:function:: clone(cv: ComponentValue; val: float)

.. _function-decs_clone_ComponentValue_uint4:

.. das:function:: clone(cv: ComponentValue; val: uint4)

.. _function-decs_clone_ComponentValue_float2:

.. das:function:: clone(cv: ComponentValue; val: float2)

.. _function-decs_clone_ComponentValue_float3:

.. das:function:: clone(cv: ComponentValue; val: float3)

.. _function-decs_clone_ComponentValue_uint:

.. das:function:: clone(cv: ComponentValue; val: uint)

.. _function-decs_clone_ComponentValue_float3x4:

.. das:function:: clone(cv: ComponentValue; val: float3x4)

.. _function-decs_clone_ComponentValue_float3x3:

.. das:function:: clone(cv: ComponentValue; val: float3x3)

.. _function-decs_clone_ComponentValue_float4x4:

.. das:function:: clone(cv: ComponentValue; val: float4x4)

.. _function-decs_clone_ComponentValue_double:

.. das:function:: clone(cv: ComponentValue; val: double)

.. _function-decs_clone_Component_Component:

.. das:function:: clone(dst: Component; src: Component)

.. _function-decs_clone_ComponentValue_float4:

.. das:function:: clone(cv: ComponentValue; val: float4)

----


get
^^^

.. _function-decs_get_ComponentMap_string_autoTT_0x3b5:

.. das:function:: get(cmp: ComponentMap; name: string; value: auto(TT)) : auto

Gets component value specified by name and type.
Will panic if name matches but type does not.

:Arguments: * **cmp** :  :ref:`ComponentMap <alias-ComponentMap>` 

            * **name** : string

            * **value** : auto(TT)

.. _function-decs_get_Archetype_string_autoTT_0x2e0:

.. das:function:: get(arch: Archetype; name: string; value: auto(TT)) : auto

----


has
^^^

.. _function-decs_has_ComponentMap_string:

.. das:function:: has(cmp: ComponentMap; name: string) : bool

Returns true if component map has specified component.

:Arguments: * **cmp** :  :ref:`ComponentMap <alias-ComponentMap>` 

            * **name** : string

.. _function-decs_has_Archetype_string:

.. das:function:: has(arch: Archetype; name: string) : bool

----

.. _function-decs_remove_ComponentMap_string:

.. das:function:: remove(cmp: ComponentMap; name: string)

Removes specified value from the component map.

:Arguments: * **cmp** :  :ref:`ComponentMap <alias-ComponentMap>` 

            * **name** : string


set
^^^

.. _function-decs_set_ComponentMap_string_autoTT_0x48c:

.. das:function:: set(cmp: ComponentMap; name: string; value: auto(TT)) : auto

Set component value specified by name and type.
If value already exists, it is overwritten. If already existing value type is not the same - panic.

:Arguments: * **cmp** :  :ref:`ComponentMap <alias-ComponentMap>` 

            * **name** : string

            * **value** : auto(TT)

.. _function-decs_set_ComponentValue_auto_0xa7:

.. das:function:: set(cv: ComponentValue; val: auto) : auto

----

+++++++++++++++++++++++
Debug and serialization
+++++++++++++++++++++++

  *  :ref:`debug_dump () <function-decs_debug_dump>` 
  *  :ref:`describe (info: CTypeInfo) : string <function-decs_describe_CTypeInfo>` 
  *  :ref:`finalize (var cmp: Component) <function-decs_finalize_Component>` 
  *  :ref:`serialize (var arch: Archive; var src: Component) <function-decs_serialize_Archive_Component>` 

.. _function-decs_debug_dump:

.. das:function:: debug_dump()

Prints out state of the ECS system.

.. _function-decs_describe_CTypeInfo:

.. das:function:: describe(info: CTypeInfo) : string

Returns textual description of the type.

:Arguments: * **info** :  :ref:`CTypeInfo <struct-decs-CTypeInfo>` 

.. _function-decs_finalize_Component:

.. das:function:: finalize(cmp: Component)

Deletes component.

:Arguments: * **cmp** :  :ref:`Component <struct-decs-Component>` 

.. _function-decs_serialize_Archive_Component:

.. das:function:: serialize(arch: Archive; src: Component)

Serializes component value.

:Arguments: * **arch** :  :ref:`Archive <struct-archive-Archive>` 

            * **src** :  :ref:`Component <struct-decs-Component>` 

++++++
Stages
++++++

  *  :ref:`commit () <function-decs_commit>` 
  *  :ref:`decs_stage (name: string) <function-decs_decs_stage_string>` 
  *  :ref:`register_decs_stage_call (name: string; pcall: PassFunction) <function-decs_register_decs_stage_call_string_PassFunction>` 

.. _function-decs_commit:

.. das:function:: commit()

Finishes all deferred actions.

.. _function-decs_decs_stage_string:

.. das:function:: decs_stage(name: string)

Invokes specific ECS pass.
`commit` is called before and after the invocation.

:Arguments: * **name** : string

.. _function-decs_register_decs_stage_call_string_PassFunction:

.. das:function:: register_decs_stage_call(name: string; pcall: PassFunction)

Registration of a single pass callback. This is a low-level function, used by decs_boost macros.

:Arguments: * **name** : string

            * **pcall** :  :ref:`PassFunction <alias-PassFunction>` 

++++++++++++++++
Deferred actions
++++++++++++++++

  *  :ref:`create_entity (var blk: lambda\<(eid:EntityId;var cmp:ComponentMap):void\>) : EntityId <function-decs_create_entity_lambda_ls_eid_c_EntityId;var_cmp_c_ComponentMap_c_void_gr_>` 
  *  :ref:`delete_entity (entityid: EntityId implicit) <function-decs_delete_entity_EntityId_implicit>` 
  *  :ref:`update_entity (entityid: EntityId implicit; var blk: lambda\<(eid:EntityId;var cmp:ComponentMap):void\>) <function-decs_update_entity_EntityId_implicit_lambda_ls_eid_c_EntityId;var_cmp_c_ComponentMap_c_void_gr_>` 

.. _function-decs_create_entity_lambda_ls_eid_c_EntityId;var_cmp_c_ComponentMap_c_void_gr_:

.. das:function:: create_entity(blk: lambda<(eid:EntityId;var cmp:ComponentMap):void>) : EntityId

Creates deferred action to create entity.

:Arguments: * **blk** : lambda<(eid: :ref:`EntityId <struct-decs-EntityId>` ;cmp: :ref:`ComponentMap <alias-ComponentMap>` ):void>

.. _function-decs_delete_entity_EntityId_implicit:

.. das:function:: delete_entity(entityid: EntityId implicit)

Creates deferred action to delete entity specified by id.

:Arguments: * **entityid** :  :ref:`EntityId <struct-decs-EntityId>`  implicit

.. _function-decs_update_entity_EntityId_implicit_lambda_ls_eid_c_EntityId;var_cmp_c_ComponentMap_c_void_gr_:

.. das:function:: update_entity(entityid: EntityId implicit; blk: lambda<(eid:EntityId;var cmp:ComponentMap):void>)

Creates deferred action to update entity specified by id.

:Arguments: * **entityid** :  :ref:`EntityId <struct-decs-EntityId>`  implicit

            * **blk** : lambda<(eid: :ref:`EntityId <struct-decs-EntityId>` ;cmp: :ref:`ComponentMap <alias-ComponentMap>` ):void>

++++++++++++
GC and reset
++++++++++++

  *  :ref:`after_gc () <function-decs_after_gc>` 
  *  :ref:`before_gc () <function-decs_before_gc>` 
  *  :ref:`restart () <function-decs_restart>` 

.. _function-decs_after_gc:

.. das:function:: after_gc()

Low level callback to be called after the garbage collection.
This is a low-level function typically used by `live`.

.. _function-decs_before_gc:

.. das:function:: before_gc()

Low level callback to be called before the garbage collection.
This is a low-level function typically used by `live`.

.. _function-decs_restart:

.. das:function:: restart()

Restarts ECS by erasing all deferred actions and entire state.

+++++++++
Iteration
+++++++++

  *  :ref:`decs_array (atype: auto(TT); src: array\<uint8\>; capacity: int) : auto <function-decs_decs_array_autoTT_array_ls_uint8_gr__int_0x2cd>` 
  *  :ref:`for_each_archetype (hash: ComponentHash; var erq: function\<():void\>; blk: block\<(arch:Archetype):void\>) <function-decs_for_each_archetype_ComponentHash_function_ls__c_void_gr__block_ls_arch_c_Archetype_c_void_gr_>` 
  *  :ref:`for_each_archetype (var erq: EcsRequest; blk: block\<(arch:Archetype):void\>) <function-decs_for_each_archetype_EcsRequest_block_ls_arch_c_Archetype_c_void_gr_>` 
  *  :ref:`for_each_archetype_find (hash: ComponentHash; var erq: function\<():void\>; blk: block\<(arch:Archetype):bool\>) : bool <function-decs_for_each_archetype_find_ComponentHash_function_ls__c_void_gr__block_ls_arch_c_Archetype_c_bool_gr_>` 
  *  :ref:`for_eid_archetype (eid: EntityId implicit; hash: ComponentHash; var erq: function\<():void\>; blk: block\<(arch:Archetype;index:int):void\>) : bool <function-decs_for_eid_archetype_EntityId_implicit_ComponentHash_function_ls__c_void_gr__block_ls_arch_c_Archetype;index_c_int_c_void_gr_>` 
  *  :ref:`get_default_ro (arch: Archetype; name: string; value: auto(TT)) : iterator\<TT const&\> <function-decs_get_default_ro_Archetype_string_autoTT_0x313>` 
  *  :ref:`get_optional (arch: Archetype; name: string; value: auto(TT)?) : iterator\<TT?\> <function-decs_get_optional_Archetype_string_autoTT_q_>` 
  *  :ref:`get_ro (arch: Archetype; name: string; value: auto(TT)) : array\<TT\> <function-decs_get_ro_Archetype_string_autoTT_0x30c>` 
  *  :ref:`get_ro (arch: Archetype; name: string; value: auto(TT)[]) : array\<TT[-2]\> <function-decs_get_ro_Archetype_string_autoTT_0x304>` 

.. _function-decs_decs_array_autoTT_array_ls_uint8_gr__int_0x2cd:

.. das:function:: decs_array(atype: auto(TT); src: array<uint8>; capacity: int) : auto

.. warning:: 
  This is unsafe operation.

Low level function returns temporary array of component given specific type of component.

:Arguments: * **atype** : auto(TT)

            * **src** : array<uint8>

            * **capacity** : int


for_each_archetype
^^^^^^^^^^^^^^^^^^

.. _function-decs_for_each_archetype_ComponentHash_function_ls__c_void_gr__block_ls_arch_c_Archetype_c_void_gr_:

.. das:function:: for_each_archetype(hash: ComponentHash; erq: function<():void>; blk: block<(arch:Archetype):void>)

Invokes block for each entity of each archetype that can be processed by the request.
Request is returned by a specified function.

:Arguments: * **hash** :  :ref:`ComponentHash <alias-ComponentHash>` 

            * **erq** : function<void>

            * **blk** : block<(arch: :ref:`Archetype <struct-decs-Archetype>` ):void>

.. _function-decs_for_each_archetype_EcsRequest_block_ls_arch_c_Archetype_c_void_gr_:

.. das:function:: for_each_archetype(erq: EcsRequest; blk: block<(arch:Archetype):void>)

----

.. _function-decs_for_each_archetype_find_ComponentHash_function_ls__c_void_gr__block_ls_arch_c_Archetype_c_bool_gr_:

.. das:function:: for_each_archetype_find(hash: ComponentHash; erq: function<():void>; blk: block<(arch:Archetype):bool>) : bool

Invokes block for each entity of each archetype that can be processed by the request.
Request is returned by a specified function.
If block returns true, iteration is stopped.

:Arguments: * **hash** :  :ref:`ComponentHash <alias-ComponentHash>` 

            * **erq** : function<void>

            * **blk** : block<(arch: :ref:`Archetype <struct-decs-Archetype>` ):bool>

.. _function-decs_for_eid_archetype_EntityId_implicit_ComponentHash_function_ls__c_void_gr__block_ls_arch_c_Archetype;index_c_int_c_void_gr_:

.. das:function:: for_eid_archetype(eid: EntityId implicit; hash: ComponentHash; erq: function<():void>; blk: block<(arch:Archetype;index:int):void>) : bool

Invokes block for the specific entity id, given request.
Request is returned by a specified function.

:Arguments: * **eid** :  :ref:`EntityId <struct-decs-EntityId>`  implicit

            * **hash** :  :ref:`ComponentHash <alias-ComponentHash>` 

            * **erq** : function<void>

            * **blk** : block<(arch: :ref:`Archetype <struct-decs-Archetype>` ;index:int):void>

.. _function-decs_get_default_ro_Archetype_string_autoTT_0x313:

.. das:function:: get_default_ro(arch: Archetype; name: string; value: auto(TT)) : iterator<TT const&>

Returns const iterator of component given specific name and type of component.
If component is not found - iterator will keep returning the specified value.

:Arguments: * **arch** :  :ref:`Archetype <struct-decs-Archetype>` 

            * **name** : string

            * **value** : auto(TT)

.. _function-decs_get_optional_Archetype_string_autoTT_q_:

.. das:function:: get_optional(arch: Archetype; name: string; value: auto(TT)?) : iterator<TT?>

Returns const iterator of component given specific name and type of component.
If component is not found - iterator will keep returning default value for the component type.

:Arguments: * **arch** :  :ref:`Archetype <struct-decs-Archetype>` 

            * **name** : string

            * **value** : auto(TT)?


get_ro
^^^^^^

.. _function-decs_get_ro_Archetype_string_autoTT_0x30c:

.. das:function:: get_ro(arch: Archetype; name: string; value: auto(TT)) : array<TT>

Returns const temporary array of component given specific name and type of component for regular components.

:Arguments: * **arch** :  :ref:`Archetype <struct-decs-Archetype>` 

            * **name** : string

            * **value** : auto(TT)

.. _function-decs_get_ro_Archetype_string_autoTT_0x304:

.. das:function:: get_ro(arch: Archetype; name: string; value: auto(TT)[]) : array<TT[-2]>

----

+++++++
Request
+++++++

  *  :ref:`EcsRequestPos (at: LineInfo) : EcsRequestPos <function-decs_EcsRequestPos_LineInfo>` 
  *  :ref:`compile_request (var erq: EcsRequest) <function-decs_compile_request_EcsRequest>` 
  *  :ref:`lookup_request (var erq: EcsRequest) : int <function-decs_lookup_request_EcsRequest>` 
  *  :ref:`verify_request (var erq: EcsRequest) : tuple\<ok:bool;error:string\> <function-decs_verify_request_EcsRequest>` 

.. _function-decs_EcsRequestPos_LineInfo:

.. das:function:: EcsRequestPos(at: LineInfo) : EcsRequestPos

Constructs EcsRequestPos from rtti::LineInfo.

:Arguments: * **at** :  :ref:`LineInfo <handle-rtti-LineInfo>` 

.. _function-decs_compile_request_EcsRequest:

.. das:function:: compile_request(erq: EcsRequest)

Compiles ECS request, by creating request hash.

:Arguments: * **erq** :  :ref:`EcsRequest <struct-decs-EcsRequest>` 

.. _function-decs_lookup_request_EcsRequest:

.. das:function:: lookup_request(erq: EcsRequest) : int

Looks up ECS request in the request cache.

:Arguments: * **erq** :  :ref:`EcsRequest <struct-decs-EcsRequest>` 

.. _function-decs_verify_request_EcsRequest:

.. das:function:: verify_request(erq: EcsRequest) : tuple<ok:bool;error:string>

Verifies ECS request. Returns pair of boolean (true for OK) and error message.

:Arguments: * **erq** :  :ref:`EcsRequest <struct-decs-EcsRequest>` 


