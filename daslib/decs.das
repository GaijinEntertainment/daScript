options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options multiple_contexts

module decs shared public

require daslib/templates

require daslib/rtti public
require daslib/algorithm
require daslib/sort_boost
require daslib/functional
require math

typedef ComponentHash = uint

struct CTypeInfo
    basicType : Type
    mangledName : string
    hash : uint
    size : uint

struct public Component
    name : string
    hash : ComponentHash
    stride : int
    data : array<uint8>
    [[do_not_delete]] text : array<string>
    info : CTypeInfo

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
    info : CTypeInfo
    data : float4[4]

typedef DeferEval = lambda<(var act:DeferAction):void>

struct private DeferAction
    eid : EntityId
    action : DeferEval

typedef ComponentMap = array<ComponentValue>

struct EcsRequestPos
    file: string
    line: uint

def EcsRequestPos(at: rtti::LineInfo): EcsRequestPos
    return [[EcsRequestPos file=at.fileInfo != null ? string(at.fileInfo.name) : "", line=at.line]]

struct public EcsRequest
    hash : ComponentHash
    req : array<string>
    reqn : array<string>
    archetypes : array<int>     // sorted
    at: EcsRequestPos

struct public DecsState
    archetypeLookup : table<ComponentHash; int>
    allArchetypes : array<Archetype>
    entityFreeList : array<EntityId>
    entityLookup : array<tuple<generation:int;archetype:ComponentHash;index:int>>
    componentTypeCheck : table<string; CTypeInfo>
    ecsQueries : array<EcsRequest>
    queryLookup : table<uint; int>

typedef PassFunction = function<():void>

struct public DecsPass
    name : string
    calls : array<PassFunction>

var public decsState : DecsState
var private deferActions : array<DeferAction>
var private decsPasses : array<DecsPass>

let INVALID_ENTITY_ID = [[decs::EntityId]]

def operator ==(a, b: decs::EntityId implicit)
    return a.id == b.id && a.generation == b.generation

def operator !=(a, b: decs::EntityId implicit)
    return a.id != b.id || a.generation != b.generation

def public operator . ( var cmp:ComponentMap; name:string ) : ComponentValue &
    let idx = lower_bound(cmp,[[ComponentValue name=name]]) <| $ ( x,y ) => x.name < y.name
    if !(idx<length(cmp) && cmp[idx].name==name)
        cmp |> push([[ComponentValue name=name]], idx)
    unsafe
        return cmp[idx]

def public set ( var cv:ComponentValue; val:auto )
    var cval = make_component(cv.name, val)
    assert(cv.info.hash==0u || cv.info.hash==cval.info.hash)
    cv = cval

def public operator := ( var cv:ComponentValue; val:EntityId )    { set(cv, val); }
def public operator := ( var cv:ComponentValue; val:bool )        { set(cv, val); }
def public operator := ( var cv:ComponentValue; val:range )       { set(cv, val); }
def public operator := ( var cv:ComponentValue; val:urange )      { set(cv, val); }
def public operator := ( var cv:ComponentValue; val:string )      { set(cv, val); }
def public operator := ( var cv:ComponentValue; val:int )         { set(cv, val); }
def public operator := ( var cv:ComponentValue; val:int8 )        { set(cv, val); }
def public operator := ( var cv:ComponentValue; val:int16 )       { set(cv, val); }
def public operator := ( var cv:ComponentValue; val:int64 )       { set(cv, val); }
def public operator := ( var cv:ComponentValue; val:int2 )        { set(cv, val); }
def public operator := ( var cv:ComponentValue; val:int3 )        { set(cv, val); }
def public operator := ( var cv:ComponentValue; val:int4 )        { set(cv, val); }
def public operator := ( var cv:ComponentValue; val:uint )        { set(cv, val); }
def public operator := ( var cv:ComponentValue; val:uint8 )       { set(cv, val); }
def public operator := ( var cv:ComponentValue; val:uint16 )      { set(cv, val); }
def public operator := ( var cv:ComponentValue; val:uint64 )      { set(cv, val); }
def public operator := ( var cv:ComponentValue; val:uint2 )       { set(cv, val); }
def public operator := ( var cv:ComponentValue; val:uint3 )       { set(cv, val); }
def public operator := ( var cv:ComponentValue; val:uint4 )       { set(cv, val); }
def public operator := ( var cv:ComponentValue; val:float )       { set(cv, val); }
def public operator := ( var cv:ComponentValue; val:float2 )      { set(cv, val); }
def public operator := ( var cv:ComponentValue; val:float3 )      { set(cv, val); }
def public operator := ( var cv:ComponentValue; val:float4 )      { set(cv, val); }
def public operator := ( var cv:ComponentValue; val:float3x3 )    { set(cv, val); }
def public operator := ( var cv:ComponentValue; val:float3x4 )    { set(cv, val); }
def public operator := ( var cv:ComponentValue; val:float4x4 )    { set(cv, val); }
def public operator := ( var cv:ComponentValue; val:double )      { set(cv, val); }

def private cleanup_temp_string_array ( var comp:Component )
    assert(comp.info.basicType==Type tString,"not a string array")
    unsafe
        memzero(comp.text)

def private make_temp_string_array ( var comp:Component; size:int )
    assert(comp.info.basicType==Type tString,"not a string array")
    unsafe
        _builtin_make_temp_array(comp.text, addr(comp.data[0]), size )
        __builtin_array_lock(comp.text)

def public clone ( var dst:Component; src:Component )
    dst.name := src.name
    dst.hash = src.hash
    dst.stride = src.stride
    dst.data := src.data
    dst.info := src.info
    if src.info.basicType==Type tString
        let size = length(src.data) / src.stride
        var dtext, stext : array<string>
        unsafe
            _builtin_make_temp_array(dtext, addr(dst.data[0]), size )
            _builtin_make_temp_array(stext, addr(src.data[0]), size )
        for d,s in dtext,stext
            d := s

def public register_decs_stage_call ( name:string; pcall:PassFunction )
    var dpass <- [[DecsPass name=name]]
    let idx = lower_bound(decsPasses,dpass) <| $ ( x,y ) => x.name < y.name
    if idx<length(decsPasses) && decsPasses[idx].name==name
        decsPasses[idx].calls |> push(pcall)
    else
        dpass.calls |> push(pcall)
        decsPasses |> emplace(dpass, idx)    // insert new one

def public decs_stage ( name : string )
    commit()
    let idx = lower_bound(decsPasses,[[DecsPass name=name]]) <| $ ( x,y ) => x.name < y.name
    if idx<length(decsPasses) && decsPasses[idx].name==name
        for cll in decsPasses[idx].calls
            invoke ( cll )
    commit()

def public restart
    delete deferActions
    delete decsState

def private new_entity_id()
    var eid : EntityId
    if !empty(decsState.entityFreeList)
        eid = decsState.entityFreeList[length(decsState.entityFreeList)-1]
        decsState.entityFreeList |> pop
    else
        eid.id = uint(length(decsState.entityLookup))
        decsState.entityLookup |> push([[auto 0,0u,0]])
    eid.generation ++
    return eid

def public before_gc
    for arch in decsState.allArchetypes
        if arch.size > 0
            for comp in arch.components
                if comp.info.basicType==Type tString
                    unsafe
                        _builtin_make_temp_array(comp.text, addr(comp.data[0]), arch.size )
                        __builtin_array_lock(comp.text)


def public after_gc
    for arch in decsState.allArchetypes
        if arch.size > 0
            for comp in arch.components
                if comp.info.basicType==Type tString
                    unsafe
                        memzero(comp.text)

def public debug_dump
    for arch in decsState.allArchetypes
        print("archtype {arch.hash} : {arch.size}\n")
        // debug(arch)
        for index in range(arch.size)
            print("\tentity[{index}]\n")
            for c in arch.components
                unsafe
                    print("\t\t{c.name} : {c.info}\n")
    for erq in decsState.ecsQueries
        print("query {erq.hash}\n")
        print("\treq = {erq.req}\n\treqn = {erq.reqn}\n\tarchetypes = {erq.archetypes}\nat = {erq.at.file}:{int(erq.at.line)}\n")

def private with_archetype ( hash:ComponentHash; blk : block<(var arch:Archetype; idx:int; isNew:bool ):void> )
    var afound & = decsState.archetypeLookup[hash]
    if afound==0
        decsState.allArchetypes |> emplace([[Archetype hash=hash]])
        afound = length(decsState.allArchetypes)
        invoke(blk, decsState.allArchetypes[afound-1], afound-1, true)
    else
        invoke(blk, decsState.allArchetypes[afound-1], afound-1, false)

def private create_archetype ( var arch:Archetype; cmp:ComponentMap; idx:int )
    assert(length(arch.components)==0)
    arch.eidIndex = -1
    for kv,kvi in cmp,range(INT_MAX)
        var ct & = decsState.componentTypeCheck[kv.name]
        if ct.hash != 0u
            if kv.info.hash != ct.hash
                panic("component {kv.name} type mismatch {ct} vs {kv.info}")
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
    for erq in decsState.ecsQueries
        if erq |> can_process_request(arch)
            erq.archetypes |> push(idx)

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
        decsState.entityLookup[eid_last.id].index = di
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
    return arch.components |> binary_search ( [[Component name=name]] ) <| $ ( x, y ) => x.name < y.name

def private can_process_request ( var erq : EcsRequest; var arch : Archetype )
    if erq.hash==arch.hash
        return true
    for r in erq.req
        if ! arch |> has(r)
            return false
    for r in erq.reqn
        if arch |> has(r)
            return false
    return true

def public verify_request ( var erq : EcsRequest ) : tuple<ok:bool;error:string>
    if erq.hash==0u || (empty(erq.req) && empty(erq.reqn))
        return [[auto false, "missing components"]]
    for N in erq.reqn   // assuming require_not is typically shorter
        if erq.req |> binary_search(N)
            return [[auto false, "duplicate req and neq {N}"]]
    return [[auto true,""]]

def public compile_request ( var erq : EcsRequest )
    sort_unique(erq.req)
    sort_unique(erq.reqn)
    erq.hash = req_hash(erq)

def public lookup_request ( var erq : EcsRequest )
    if erq.hash==0u
        compile_request(erq)
    var ql & = decsState.queryLookup[erq.hash]
    if ql == 0
        for arch,archi in decsState.allArchetypes,range(INT_MAX)
            if erq |> can_process_request(arch)
                erq.archetypes |> push(archi)
        decsState.ecsQueries |> push_clone(erq)
        ql = length(decsState.ecsQueries)
    return ql - 1

def public for_each_archetype ( var erq : EcsRequest; blk:block<(arch:Archetype):void> )
    let qi = lookup_request(erq)
    for aidx in decsState.ecsQueries[qi].archetypes
        var arch & = decsState.allArchetypes[aidx]
        if arch.size > 0
            invoke ( blk, arch )

def public for_eid_archetype ( eid:EntityId implicit; hash:ComponentHash; var erq : function<():EcsRequest>; blk:block<(arch:Archetype;index:int):void> )
    var lookup & = decsState.entityLookup[eid.id]
    if lookup.generation != eid.generation
        return false
    var qi = -1
    decsState.queryLookup |> find_if_exists(hash) <| $ ( ql )
        qi = *ql - 1
    if qi == -1
        qi = lookup_request(invoke(erq))
    var req & = decsState.ecsQueries[qi]
    let aidx = decsState.archetypeLookup[lookup.archetype]
    if aidx == 0
        panic("archetype not found")
    if binary_search(req.archetypes,aidx-1)
        var arch & = decsState.allArchetypes[aidx-1]
        assert(arch.size > 0)
        invoke(blk, arch, lookup.index)
        return true
    else
        panic("archetype not found")
        return false

def public for_each_archetype ( hash:ComponentHash; var erq : function<():EcsRequest>; blk:block<(arch:Archetype):void> )
    var qi = -1
    decsState.queryLookup |> find_if_exists(hash) <| $ ( ql )
        qi = *ql - 1
    if qi == -1
        qi = lookup_request(invoke(erq))
    for aidx in decsState.ecsQueries[qi].archetypes
        var arch & = decsState.allArchetypes[aidx]
        if arch.size > 0
            invoke ( blk, arch )

def public for_each_archetype_find ( hash:ComponentHash; var erq : function<():EcsRequest>; blk:block<(arch:Archetype):bool> )
    var qi = -1
    decsState.queryLookup |> find_if_exists(hash) <| $ ( ql )
        qi = *ql - 1
    if qi == -1
        qi = lookup_request(invoke(erq))
    for aidx in decsState.ecsQueries[qi].archetypes
        var arch & = decsState.allArchetypes[aidx]
        if arch.size > 0
            if invoke ( blk, arch )
                return true
    return false

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
    let idx = arch.components |> lower_bound([[Component name=name]]) <| $ ( x,y ) => x.name < y.name
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
    let idx = arch.components |> lower_bound([[Component name=name]]) <| $ ( x,y ) => x.name < y.name
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
    var lookup & = decsState.entityLookup[eid.id]
    if lookup.generation != eid.generation
        return
    var cmp : ComponentMap
    var arch & = decsState.allArchetypes[decsState.archetypeLookup[lookup.archetype]-1]
    let eidx = decsState.entityLookup[eid.id].index
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
        with_archetype(new_ahash) <| $ ( var narch; idx; isNew )
            if isNew
                narch |> create_archetype(cmp, idx)
            let neidx = narch |> create_entity(eid, cmp)
            decsState.entityLookup[eid.id] = [[auto eid.generation,new_ahash,neidx]]
    delete cmp

def private create_entity_imm ( eid:EntityId; blk : lambda<(eid:EntityId; var cmp:ComponentMap):void> )
    var cmp : ComponentMap
    cmp |> push <| make_component("eid", eid)
    invoke(blk, eid, cmp)
    cmp |> set("eid", eid)       // necessary?
    var ahash = cmp_archetype_hash(cmp)
    with_archetype(ahash) <| $ ( var arch; idx; isNew )
        if isNew
            arch |> create_archetype(cmp, idx)
        let eidx = arch |> create_entity(eid,cmp)
        decsState.entityLookup[eid.id] = [[auto eid.generation,ahash,eidx]]
    delete cmp

def private delete_entity_imm ( eid:EntityId )
    var lookup & = decsState.entityLookup[eid.id]
    if lookup.generation == eid.generation
        var arch & = decsState.allArchetypes[decsState.archetypeLookup[lookup.archetype]-1]
        let di = decsState.entityLookup[eid.id].index
        decsState.entityLookup[eid.id].generation = 0
        decsState.entityFreeList |> push(eid)
        arch |> remove_entity(di)

def public update_entity ( entityid:EntityId implicit; var blk : lambda<(eid:EntityId; var cmp:ComponentMap):void> )
    var deval <- @ <| [[<-blk]] ( var act:DeferAction )
        update_entity_imm(act.eid,blk)
    deferActions |> emplace([[DeferAction action<-deval, eid=entityid]])

def public create_entity ( var blk : lambda<(eid:EntityId; var cmp:ComponentMap):void> )
    var deval <- @ <| [[<-blk]] ( var act:DeferAction )
        create_entity_imm(act.eid, blk)
    var eid = new_entity_id()
    deferActions |> emplace([[DeferAction action<-deval, eid=eid]])
    return eid

def public delete_entity ( entityid:EntityId implicit )
    var deval <- @ <| ( var act:DeferAction )
        delete_entity_imm(act.eid)
    deferActions |> emplace([[DeferAction action<-deval, eid=entityid]])

def public commit
    for da in deferActions
        da.action |> invoke(da)
    delete deferActions

def public get ( var cmp:ComponentMap; name:string; var value:auto(TT) )
    let idx = lower_bound(cmp,[[ComponentValue name=name]]) <| $ ( x,y ) => x.name < y.name
    if idx<length(cmp) && cmp[idx].name==name
        unsafe
            let cvinfo = addr(typeinfo(rtti_typeinfo type<TT-const-&-#>))
            if cmp[idx].info.hash != cvinfo.hash
                panic("get component {name} type mismatch, expecting {cmp[idx].info} vs {describe(cvinfo)}")
            memcpy ( addr(value), addr(cmp[idx].data), typeinfo(sizeof type<TT const-&>))
    return value

def private make_component ( name:string; value:auto(TT) )
    var cv = [[ComponentValue name=name]]
    unsafe
        var tinfo = addr(typeinfo(rtti_typeinfo type<TT-const-&-#>))
        cv.info = [[CTypeInfo basicType=tinfo.basicType, mangledName=get_mangled_name(tinfo), hash=tinfo.hash, size=tinfo.size]]
        memcpy ( addr(cv.data), addr(value), typeinfo(sizeof value))
    return cv

def public set ( var cmp:ComponentMap; name:string; value:auto(TT) )
    concept_assert((typeinfo(is_pod value) || typeinfo(is_string value))
        && typeinfo(sizeof value)<=typeinfo(sizeof cmp[0].data), "unsupported component type {typeinfo(typename value)}")
    let cv = make_component(name, value)
    let idx = lower_bound(cmp,[[ComponentValue name=name]]) <| $ ( x,y ) => x.name < y.name
    if idx<length(cmp) && cmp[idx].name==name
        if cmp[idx].info.hash != cv.info.hash
            panic("set component {name} type mismatch, expecting {cmp[idx].info} vs {cv.info}")
        cmp[idx] = cv           // overwrite
    else
        cmp |> push(cv, idx)    // insert new one

def public has ( var cmp:ComponentMap; name:string )
    return binary_search(cmp,[[ComponentValue name=name]]) <| $ ( x,y ) => x.name < y.name

def public remove ( var cmp:ComponentMap; name:string )
    let idx = lower_bound(cmp,[[ComponentValue name=name]]) <| $ ( x,y ) => x.name < y.name
    if idx<length(cmp) && cmp[idx].name==name
        cmp |> erase(idx)
