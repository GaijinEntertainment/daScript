options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module decs shared public

require daslib/templates

require daslib/rtti public

[template(atype)]
def decs_array ( atype:auto(TT); src:array<uint8> )
    assert(length(src)>0)
    var dest : array<TT -const -&>
    unsafe
        _builtin_make_temp_array(dest, addr(src[0]), 25)
    __builtin_array_lock(dest)
    return <- dest

typedef ComponentHash = uint

def component_hash ( name:string; info:TypeInfo const? ) : ComponentHash
    return hash(name) ^ (info.hash)

struct private Component
    name : string
    hash : ComponentHash
    stride : int
    data : array<uint8>
    info : TypeInfo const?

struct public EntityId
    id : uint
    generation : int

struct private Archetype
    hash : ComponentHash
    components : array<Component>
    entities : array<EntityId>
    componentsLookup : table<ComponentHash;int>
    size : int

struct public ComponentValue
    info : TypeInfo const?
    data : float4

typedef ComponentMap = table<string; ComponentValue>

var private allArchetypes : table<ComponentHash; Archetype>
var private deferCreateEntity : array<lambda<(eid:EntityId; var cmp:ComponentMap):void>>
var private deferDeleteEntity : array<EntityId>
var private lastEntityId : uint
var private entityFreeList : array<EntityId>
var private entityLookup : array<tuple<generation:int;archetype:ComponentHash;index:int>>

def private new_entity_id()
    var eid : EntityId
    if !empty(entityFreeList)
        eid = entityFreeList[length(entityFreeList)-1]
        entityFreeList |> pop
    else
        entityLookup |> push([[auto 0,0u,0]])
        eid.id = lastEntityId++
    eid.generation ++
    return eid

def public debug_dump
    for arch in values(allArchetypes)
        print("archtype {arch.hash} : {arch.size}\n")
        // debug(arch)
        for e,index in arch.entities,range(INT_MAX)
            print("\tentity[{index}] {e.id}\n")
            for c in arch.components
                unsafe
                    print("\t\t{c.name} : {describe(c.info)} = {sprint_data(addr(c.data[index*c.stride]),c.info,print_flags humanReadable)}\n")

def private with_archetype ( hash:ComponentHash; blk : block<(var arch:Archetype; isNew:bool ):void> )
    var afound & = allArchetypes[hash]
    __builtin_table_lock(allArchetypes) // todo: more elegant table lock. with find?
    if afound.hash != [[ComponentHash]]
        invoke(blk, afound, false)
    else
        afound.hash = hash
        invoke(blk, afound, true)
    __builtin_table_unlock(allArchetypes)

def private create_archetype ( var arch:Archetype; cmp:ComponentMap )
    assert(length(arch.components)==0 && length(arch.entities)==0)
    for k,v in keys(cmp),values(cmp)
        let chash = component_hash(k,v.info)
        arch.componentsLookup[chash] = length(arch.components)
        arch.components |> emplace <| [[Component
            name=k,
            hash=chash,
            stride=int(v.info.size),
            info=v.info
        ]]

def private create_entity ( var arch:Archetype; eid:EntityId )
    for c in arch.components
        c.data |> resize(length(c.data) + c.stride)
    arch.entities |> push(eid)
    return arch.size++

def private create_entity_imm ( blk : lambda<(eid:EntityId; var cmp:ComponentMap):void> )
    var cmp : ComponentMap
    let eid = new_entity_id()
    invoke(blk, eid, cmp)
    if length(cmp)==0            // question: shoulw we allow empty entities?
        panic("can't create empty entity")
    var ahash : ComponentHash
    for name,value in keys(cmp),values(cmp)
        ahash ^= component_hash(name,value.info)
    with_archetype(ahash) <| $ ( var arch; isNew )
        if isNew
            arch |> create_archetype(cmp)
        let eidx = arch |> create_entity(eid)
        entityLookup[eid.id] = [[auto eid.generation,ahash,eidx]]
        for c in arch.components
            var comp & = cmp[c.name]
            unsafe
                memcpy ( addr(c.data[eidx*c.stride]), addr(comp.data), c.stride )

def public create_entity ( blk : lambda<(eid:EntityId; var cmp:ComponentMap):void> )
    deferCreateEntity |> emplace(blk)

def public delete_entity ( eid:EntityId )
    deferDeleteEntity |> push(eid)

def private delete_entities ( var arch:Archetype; var indices:array<int> )
    sort(indices)
    let li = length(indices)
    for i in range(li)
        arch.size --
        var di = indices[li-i-1]
        var eid = arch.entities[di]
        entityLookup[eid.id].generation = 0
        entityFreeList |> push(eid)
        if di!=arch.size    // copy last one in the hole
            var eid_last = arch.entities[arch.size]
            arch.entities[di] = eid_last
            entityLookup[eid_last.id].index = di
            for c in arch.components
                unsafe
                    memcpy ( addr(c.data[di*c.stride]), addr(c.data[arch.size*c.stride]), c.stride )
        arch.entities |> pop    // trim the tail
        for c in arch.components
            c.data |> resize ( arch.size * c.stride )

def public commit
    // create new ones
    for dc in deferCreateEntity
        create_entity_imm(dc)
    delete deferCreateEntity
    // delete old ones
    var d2a : table<ComponentHash; array<int>>
    for eid in deferDeleteEntity
        var lookup & = entityLookup[eid.id]
        if lookup.generation == eid.generation
            var arch & = allArchetypes[lookup.archetype]
            let index = entityLookup[eid.id].index
            d2a[lookup.archetype] |> push(index)
    for arch,a in keys(d2a),values(d2a)
        allArchetypes[arch] |> delete_entities(a)
    delete d2a
    delete deferDeleteEntity

def public set ( var cmp:ComponentMap; name:string; value:auto(TT) )
    concept_assert((typeinfo(is_pod value) || typeinfo(is_string value))
        && typeinfo(sizeof value)<=16, "unsupported component type {typeinfo(typename value)}")
    var cv : ComponentValue
    unsafe
        cv.info = addr(typeinfo(rtti_typeinfo type<TT-const-&>))
        memcpy ( addr(cv.data), addr(value), typeinfo(sizeof value))
    cmp[name] = cv
