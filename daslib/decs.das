options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module decs shared public

require daslib/templates

require daslib/rtti public
require daslib/algorithm
require daslib/sort_boost
require daslib/functional

typedef ComponentHash = uint

struct public Component
    name : string
    hash : ComponentHash
    stride : int
    data : array<uint8>
    info : TypeInfo const?

struct public EntityId
    id : uint
    generation : int

struct public Archetype
    hash : ComponentHash
    components : array<Component>
    size : int
    eidIndex : int

struct public ComponentValue
    name : string
    info : TypeInfo const?
    data : float4

typedef DeferEval = lambda<():void>

struct private DeferAction
    eid : EntityId
    action : DeferEval

typedef ComponentMap = array<ComponentValue>

struct public EcsRequest
    hash : ComponentHash
    req : array<string>
    reqn : array<string>

enum private ReqLookup
    dontKnow
    yes
    no

var private requestLookup : table<uint64; ReqLookup>
var private allArchetypes : table<ComponentHash; Archetype>
var private deferActions : array<DeferAction>
var private entityFreeList : array<EntityId>
var private entityLookup : array<tuple<generation:int;archetype:ComponentHash;index:int>>
var private componentTypeCheck : table<string; TypeInfo const?>

def restart
    delete deferActions
    delete allArchetypes
    delete entityFreeList
    delete entityLookup
    delete componentTypeCheck
    delete requestLookup

def private new_entity_id()
    var eid : EntityId
    if !empty(entityFreeList)
        eid = entityFreeList[length(entityFreeList)-1]
        entityFreeList |> pop
    else
        eid.id = uint(length(entityLookup))
        entityLookup |> push([[auto 0,0u,0]])
    eid.generation ++
    return eid

def public debug_dump
    for arch in values(allArchetypes)
        print("archtype {arch.hash} : {arch.size}\n")
        // debug(arch)
        for index in range(arch.size)
            print("\tentity[{index}]\n")
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
    assert(length(arch.components)==0)
    arch.eidIndex = -1
    for kv,kvi in cmp,range(100500)
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
        if kv.name=="eid"
            assert(arch.eidIndex==-1)
            arch.eidIndex = kvi
    assert(arch.eidIndex!=-1)

def private get_eid ( var arch:Archetype; index:int ) : EntityId &
    unsafe
        var ceid & = arch.components[arch.eidIndex]
        return *(reinterpret<EntityId?> addr(ceid.data[index*ceid.stride]))

def private create_entity ( var arch:Archetype; eid:EntityId; cmp:ComponentMap )
    let eidx = arch.size++
    for c,comp in arch.components,cmp
        c.data |> resize(length(c.data) + c.stride)
        unsafe
            memcpy ( addr(c.data[eidx*c.stride]), addr(comp.data), c.stride )
    return eidx

def private remove_entity ( var arch:Archetype; di:int )
    arch.size --
    if di!=arch.size    // copy last one in the hole
        var eid_last & = arch |> get_eid(arch.size)
        entityLookup[eid_last.id].index = di
        for c in arch.components
            unsafe
                memcpy ( addr(c.data[di*c.stride]), addr(c.data[arch.size*c.stride]), c.stride )
    for c in arch.components
        c.data |> resize ( arch.size * c.stride )

def private cmp_archetype_hash ( cmp:ComponentMap )
    var ahash : ComponentHash
    for kv in cmp
        ahash = (ahash<<<2u) ^ hash(kv.name)
    return ahash

def private req_hash ( erq : EcsRequest )
    var ahash : ComponentHash
    for kv in erq.req
        ahash = (ahash<<<2u) ^ hash(kv)
    for kv in erq.reqn
        ahash = (ahash<<<2u) ^ ~hash(kv)
    return ahash

def public has ( arch:Archetype; name:string )
    return arch.components |> binary_search ( [[Component name=name]] ) <| $ ( x, y )
        return x.name < y.name

def private can_process_request ( var erq : EcsRequest; var arch : Archetype )
    if erq.hash==arch.hash
        return true
    let key = (uint64(erq.hash)<<<32ul) ^ (uint64(arch.hash))
    var rlook & = requestLookup[key]
    if rlook==ReqLookup dontKnow
        rlook = ReqLookup yes
        for r in erq.req
            if ! arch |> has(r)
                rlook = ReqLookup no
                break
        if rlook == ReqLookup yes
            for r in erq.reqn
                if arch |> has(r)
                    rlook = ReqLookup no
                    break
    return rlook==ReqLookup yes

def public for_each_archetype ( var erq : EcsRequest; blk:block<(arch:Archetype):void> )
    if erq.hash==0u
        if !empty(erq.req)
            sort(erq.req)
        if !empty(erq.reqn)
            sort(erq.reqn)
        erq.hash = req_hash(erq)
    for arch in values(allArchetypes)
        if arch.size > 0
            if erq |> can_process_request(arch)
                invoke ( blk, arch )

[template(atype)]
def decs_array ( atype:auto(TT); src:array<uint8>; capacity:int ) : array<TT-const-&>#
    assert(length(src)>0)
    var dest : array<TT -const -&>#
    unsafe
        _builtin_make_temp_array(dest, addr(src[0]), capacity)
    __builtin_array_lock(dest)
    unsafe
        return <- dest

def public get ( arch:Archetype; name:string; value:auto(TT) ) : array<TT-const-&>#
    let idx = arch.components |> lower_bound([[Component name=name]]) <| $ ( x,y )
        return x.name < y.name
    if idx<length(arch.components)
        let comp & = arch.components[idx]
        if comp.name==name
            unsafe
                let cvinfo = addr(typeinfo(rtti_typeinfo type<TT-const-&>))
                if comp.info.hash != cvinfo.hash
                    panic("component array {name} type mismatch, expecting {comp.info} vs {describe(cvinfo)}")
            unsafe
                return <- decs_array(type<TT>, comp.data, arch.size)
    panic("component array {name} not found")
    unsafe
        return <- [[array<TT-const-&># ]]

def public get_ro ( arch:Archetype; name:string; value:auto(TT) ) : array<TT-const-&> const#
    unsafe
        return <- get(arch, name, value)

def public get_default_ro ( arch:Archetype; name:string; value:auto(TT) ) : iterator<TT const &>
    let idx = arch.components |> lower_bound([[Component name=name]]) <| $ ( x,y )
        return x.name < y.name
    if idx<length(arch.components)
        let comp & = arch.components[idx]
        if comp.name==name
            unsafe
                let cvinfo = addr(typeinfo(rtti_typeinfo type<TT-const-&>))
                if comp.info.hash != cvinfo.hash
                    panic("component array {name} type mismatch, expecting {comp.info} vs {describe(cvinfo)}")
            var it : iterator<TT const &>
            unsafe
                _builtin_make_fixed_array_iterator(it,addr(comp.data[0]),arch.size,comp.stride)
            return <- it
    return <- repeat_ref(value,arch.size)

def private update_entity_imm ( eid:EntityId; blk : lambda<(eid:EntityId; var cmp:ComponentMap):void> )
    var lookup & = entityLookup[eid.id]
    if lookup.generation != eid.generation
        return
    var cmp : ComponentMap
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
    cmp |> set("eid", eid)  // necessary?
    var new_ahash = cmp_archetype_hash(cmp)
    if old_ahash == new_ahash
        for c,comp in arch.components,cmp
            unsafe
                memcpy ( addr(c.data[eidx*c.stride]), addr(comp.data), c.stride )
    else
        arch |> remove_entity ( eidx )
        with_archetype(new_ahash) <| $ ( var narch; isNew )
            if isNew
                narch |> create_archetype(cmp)
            let neidx = narch |> create_entity(eid, cmp)
            entityLookup[eid.id] = [[auto eid.generation,new_ahash,neidx]]
    delete cmp

def private create_entity_imm ( eid:EntityId; blk : lambda<(eid:EntityId; var cmp:ComponentMap):void> )
    var cmp : ComponentMap
    cmp |> push <| make_component("eid", eid)
    invoke(blk, eid, cmp)
    cmp |> set("eid", eid)       // necessary?
    var ahash = cmp_archetype_hash(cmp)
    with_archetype(ahash) <| $ ( var arch; isNew )
        if isNew
            arch |> create_archetype(cmp)
        let eidx = arch |> create_entity(eid,cmp)
        entityLookup[eid.id] = [[auto eid.generation,ahash,eidx]]
    delete cmp

def private delete_entity_imm ( eid:EntityId )
    var lookup & = entityLookup[eid.id]
    if lookup.generation == eid.generation
        var arch & = allArchetypes[lookup.archetype]
        let di = entityLookup[eid.id].index
        entityLookup[eid.id].generation = 0
        entityFreeList |> push(eid)
        arch |> remove_entity(di)

def public update_entity ( entityid:EntityId; var blk : lambda<(eid:EntityId; var cmp:ComponentMap):void> )
    var eid = entityid
    var deval <- @ <| [[<-blk]]
        update_entity_imm(eid,blk)
    deferActions |> emplace([[DeferAction action<-deval]])

def public create_entity ( var blk : lambda<(eid:EntityId; var cmp:ComponentMap):void> )
    var eid = new_entity_id()
    var deval <- @ <| [[<-blk]]
        create_entity_imm(eid, blk)
    deferActions |> emplace([[DeferAction action<-deval]])
    return eid

def public delete_entity ( entityid:EntityId )
    var eid = entityid
    var deval <- @ <|
        delete_entity_imm(eid)
    deferActions |> emplace([[DeferAction action<-deval]])

def public commit
    for da in deferActions
        da.action |> invoke
    delete deferActions

def public get ( var cmp:ComponentMap; name:string; var value:auto(TT) )
    let idx = lower_bound(cmp,[[ComponentValue name=name]]) <| $ ( x,y )
        return x.name < y.name
    if idx<length(cmp) && cmp[idx].name==name
        unsafe
            let cvinfo = addr(typeinfo(rtti_typeinfo type<TT-const-&-#>))
            if cmp[idx].info.hash != cvinfo.hash
                panic("get component {name} type mismatch, expecting {describe(cmp[idx].info)} vs {describe(cvinfo)}")
            memcpy ( addr(value), addr(cmp[idx].data), typeinfo(sizeof type<TT const-&>))
    return value

def private make_component ( name:string; value:auto(TT) )
    var cv = [[ComponentValue name=name]]
    unsafe
        cv.info = addr(typeinfo(rtti_typeinfo type<TT-const-&-#>))
        memcpy ( addr(cv.data), addr(value), typeinfo(sizeof value))
    return cv

def public set ( var cmp:ComponentMap; name:string; value:auto(TT) )
    concept_assert((typeinfo(is_pod value) || typeinfo(is_string value))
        && typeinfo(sizeof value)<=16, "unsupported component type {typeinfo(typename value)}")
    let cv = make_component(name, value)
    let idx = lower_bound(cmp,[[ComponentValue name=name]]) <| $ ( x,y )
        return x.name < y.name
    if idx<length(cmp) && cmp[idx].name==name
        if cmp[idx].info.hash != cv.info.hash
            panic("set component {name} type mismatch, expecting {describe(cmp[idx].info)} vs {describe(cv.info)}")
        cmp[idx] = cv           // overwrite
    else
        cmp |> push(cv, idx)    // insert new one

def public has ( var cmp:ComponentMap; name:string )
    return binary_search(cmp,[[ComponentValue name=name]]) <| $ ( x,y )
        return x.name < y.name

def public remove ( var cmp:ComponentMap; name:string )
    let idx = lower_bound(cmp,[[ComponentValue name=name]]) <| $ ( x,y )
        return x.name < y.name
    if idx<length(cmp) && cmp[idx].name==name
        cmp |> erase(idx)
