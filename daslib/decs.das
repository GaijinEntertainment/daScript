options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module decs shared public

require daslib/templates

require daslib/rtti public
require daslib/algorithm

[template(atype)]
def decs_array ( atype:auto(TT); src:array<uint8> )
    assert(length(src)>0)
    var dest : array<TT -const -&>
    unsafe
        _builtin_make_temp_array(dest, addr(src[0]), 25)
    __builtin_array_lock(dest)
    return <- dest

typedef ComponentHash = uint

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
    size : int

struct public ComponentValue
    name : string
    info : TypeInfo const?
    data : float4

typedef ComponentMap = array<ComponentValue>

var private allArchetypes : table<ComponentHash; Archetype>
var private deferCreateEntity : array<lambda<(eid:EntityId; var cmp:ComponentMap):void>>
var private deferUpdateEntity : array<tuple<eid:EntityId; update:lambda<(eid:EntityId; var cmp:ComponentMap):void>>>
var private deferDeleteEntity : array<EntityId>
var private lastEntityId : uint
var private entityFreeList : array<EntityId>
var private entityLookup : array<tuple<generation:int;archetype:ComponentHash;index:int>>
var private componentTypeCheck : table<string; TypeInfo const?>

def restart
    delete allArchetypes
    delete deferCreateEntity
    delete deferUpdateEntity
    delete deferDeleteEntity
    delete entityFreeList
    delete entityLookup
    delete componentTypeCheck
    lastEntityId = 0u

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
    for kv in cmp
        var ct & = componentTypeCheck[kv.name]
        if ct != null
            if kv.info.hash != (*ct).hash
                panic("component {kv.name} type mismatch {describe(ct)} vs {describe(kv.info)}")
        else
            ct = kv.info
        let chash = hash(kv.name)
        arch.components |> emplace <| [[Component
            name=kv.name,
            hash=chash,
            stride=int(kv.info.size),
            info=kv.info
        ]]

def private create_entity ( var arch:Archetype; eid:EntityId; cmp:ComponentMap )
    let eidx = arch.size++
    for c,comp in arch.components,cmp
        c.data |> resize(length(c.data) + c.stride)
        unsafe
            memcpy ( addr(c.data[eidx*c.stride]), addr(comp.data), c.stride )
    arch.entities |> push(eid)
    return eidx

def private remove_entity ( var arch:Archetype; di:int )
    arch.size --
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

def private cmp_archetype_hash ( cmp:ComponentMap )
    var ahash : ComponentHash
    for kv in cmp
        ahash = (ahash<<<2u) ^ hash(kv.name)
    return ahash

def private update_entity_imm ( eid:EntityId; blk : lambda<(eid:EntityId; var cmp:ComponentMap):void> )
    var cmp : ComponentMap
    var lookup & = entityLookup[eid.id]
    if lookup.generation == eid.generation
        var arch & = allArchetypes[lookup.archetype]
        let eidx = entityLookup[eid.id].index
        cmp |> reserve ( length(arch.components) )
        for c in arch.components
            var value = [[ComponentValue name=c.name, info=c.info]]
            unsafe
                memcpy ( addr(value.data), addr(c.data[eidx*c.stride]), c.stride)
            cmp |> push(value)
        var old_ahash = arch.hash
        invoke(blk, eid, cmp)
        if length(cmp)==0
            panic("can't migrate to an empty entity")
        var new_ahash = cmp_archetype_hash(cmp)
        if old_ahash == new_ahash
            pass
        else
            arch |> remove_entity ( eidx )
            with_archetype(new_ahash) <| $ ( var narch; isNew )
                if isNew
                    narch |> create_archetype(cmp)
                let neidx = narch |> create_entity(eid, cmp)
                entityLookup[eid.id] = [[auto eid.generation,new_ahash,neidx]]

def private create_entity_imm ( blk : lambda<(eid:EntityId; var cmp:ComponentMap):void> )
    var cmp : ComponentMap
    let eid = new_entity_id()
    invoke(blk, eid, cmp)
    if length(cmp)==0            // question: shoulw we allow empty entities?
        panic("can't create empty entity")
    var ahash = cmp_archetype_hash(cmp)
    with_archetype(ahash) <| $ ( var arch; isNew )
        if isNew
            arch |> create_archetype(cmp)
        let eidx = arch |> create_entity(eid,cmp)
        entityLookup[eid.id] = [[auto eid.generation,ahash,eidx]]

def public update_entity ( eid:EntityId; var blk : lambda<(eid:EntityId; var cmp:ComponentMap):void> )
    deferUpdateEntity |> emplace([[auto eid, blk]])

def public create_entity ( var blk : lambda<(eid:EntityId; var cmp:ComponentMap):void> )
    deferCreateEntity |> emplace(blk)

def public delete_entity ( eid:EntityId )
    deferDeleteEntity |> push(eid)

def private delete_entities ( var arch:Archetype; var indices:array<int> )
    sort(indices)
    let li = length(indices)
    for i in range(li)
        var di = indices[li-i-1]
        var eid = arch.entities[di]
        entityLookup[eid.id].generation = 0
        entityFreeList |> push(eid)
        remove_entity ( arch, di )

def public commit
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
    // update
    for du in deferUpdateEntity
        update_entity_imm(du.eid, du.update)
    delete deferUpdateEntity
    // create new ones
    for dc in deferCreateEntity
        create_entity_imm(dc)
    delete deferCreateEntity

def public get ( var cmp:ComponentMap; name:string; var value:auto(TT) )
    let idx = lower_bound(cmp) <| $ ( kv )
        return kv.name < name
    if idx<length(cmp) && cmp[idx].name==name
        unsafe
            let cvinfo = addr(typeinfo(rtti_typeinfo type<TT-const-&-#>))
            if cmp[idx].info.hash != cvinfo.hash
                panic("get component {name} type mismatch, expecting {describe(cmp[idx].info)} vs {describe(cvinfo)}")
            memcpy ( addr(value), addr(cmp[idx].data), typeinfo(sizeof type<TT const-&>))
    return value

def public set ( var cmp:ComponentMap; name:string; value:auto(TT) )
    concept_assert((typeinfo(is_pod value) || typeinfo(is_string value))
        && typeinfo(sizeof value)<=16, "unsupported component type {typeinfo(typename value)}")
    var cv = [[ComponentValue name=name]]
    unsafe
        cv.info = addr(typeinfo(rtti_typeinfo type<TT-const-&-#>))
        memcpy ( addr(cv.data), addr(value), typeinfo(sizeof value))
    let idx = lower_bound(cmp) <| $ ( kv )
        return kv.name < name
    if idx<length(cmp) && cmp[idx].name==name
        if cmp[idx].info.hash != cv.info.hash
            panic("set component {name} type mismatch, expecting {describe(cmp[idx].info)} vs {describe(cv.info)}")
        cmp[idx] = cv           // overwrite
    else
        cmp |> push(cv, idx)    // insert new one

def public has ( var cmp:ComponentMap; name:string )
    let idx = lower_bound(cmp) <| $ ( kv )
        return kv.name < name
    return idx<length(cmp) && cmp[idx].name==name

def public remove ( var cmp:ComponentMap; name:string )
    let idx = lower_bound(cmp) <| $ ( kv )
        return kv.name < name
    if idx<length(cmp) && cmp[idx].name==name
        cmp |> erase(idx)