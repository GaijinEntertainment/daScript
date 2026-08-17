options gen2
options _comment_hygiene = true
options indenting = 4

module vk_model shared

require pugixml/PUGIXML_boost
require strings
require daslib/strings_boost

// every string stored in the model must be cloned — pugixml accessors return pointers into the document, which dies at the open_xml block exit
def private cs(s : string) : string {
    return clone_string(s)
}

//! One struct member or command parameter, as parsed from vk.xml mixed content.
struct public VkMember {
    name : string
    type_name : string          //! the <type> tag content (e.g. "VkStructureType", "char")
    num_ptrs : int              //! count of '*' in the declaration
    is_const : bool
    arr_dims : array<string>    //! fixed-array dims, outermost first; numeric text or VK_* constant name
    len : string                //! raw len= attribute
    optional_attr : string      //! raw optional= attribute
    noautovalidity : string     //! raw noautovalidity= attribute (count not derivable from this array's length)
    stype_value : string        //! values= attribute (sType)
    bit_width : int             //! C bitfield width (:24), 0 if none
}

struct public VkStructDef {
    name : string
    alias : string              //! non-empty: this is just an alias of another struct
    is_union : bool
    returned_only : bool
    struct_extends : string     //! raw structextends= attribute (comma list)
    members : array<VkMember>
    protect : string            //! platform guard macro ("" if none)
    guard : string              //! feature/extension define guarding this type
}

struct public VkHandleDef {
    name : string
    alias : string
    parent : string
    dispatchable : bool
    protect : string
    guard : string
}

struct public VkEnumEntry {
    name : string               //! full C name (VK_SUCCESS)
    bitpos : int                //! bit position for bitmask entries, -1 otherwise
    is_alias : bool
    guard : string              //! extension define that introduced it ("" = core)
}

struct public VkEnumDef {
    name : string
    is_bitmask : bool
    bit_width : int             //! 32 or 64
    entries : array<VkEnumEntry>
}

//! VkAccessFlags -> VkAccessFlagBits association
struct public VkBitmaskDef {
    name : string               //! the Flags name (used in struct fields / params)
    bits_enum : string          //! the FlagBits enum holding the values ("" if reserved-empty)
    is64 : bool
    alias : string
    protect : string
    guard : string
}

struct public VkCommandDef {
    name : string
    alias : string              //! non-empty: alias of another command
    ret_type : string
    params : array<VkMember>
    protect : string
    guard : string
}

struct public VkConstantDef {
    name : string               //! C name, emitted by reference
    ctype : string              //! uint32_t / uint64_t / float
    is_alias : bool
}

struct public VkRegistry {
    vendor_tags : array<string>
    platform_protect : table<string; string>    //! platform name -> protect macro
    base_types : table<string; string>          //! VkBool32 -> uint32_t etc.
    func_pointers : table<string>               //! PFN_* names (mapped to void?)
    handles : array<VkHandleDef>
    enums : array<VkEnumDef>                    //! plain enums AND flag-bits enums
    bitmasks : array<VkBitmaskDef>
    structs : array<VkStructDef>                //! structs and unions, registry order
    commands : array<VkCommandDef>
    constants : array<VkConstantDef>
    // indexes
    enum_index : table<string; int>
    struct_index : table<string; int>
    handle_index : table<string; int>
    bitmask_index : table<string; int>
    command_index : table<string; int>
    // what features/extensions enabled, name -> guard define
    enabled_types : table<string; string>
    enabled_commands : table<string; string>
    enabled_constants : table<string; string>   //! constant name -> guard define (extension that requires it)
    ext_protect : table<string; string>         //! extension name -> platform protect ("" common)
    skipped : table<string; string>             //! type/command name -> reason
}

def private parse_decl_into(m : xml_node; var mem : VkMember) {
    let txt = build_string() $(var w) {
        for (ch in each_child(m)) {
            let nm = ch.name
            if (nm == "type") {
                mem.type_name = cs(ch.child_value)
                w |> write(ch.child_value)
            } elif (nm == "name") {
                mem.name = cs(ch.child_value)
            } elif (nm == "enum") {
                mem.arr_dims |> push(cs(ch.child_value))
            } elif (nm == "comment") {
                pass
            } elif (empty(nm)) {
                w |> write(ch.value)
            }
        }
    }
    mem.num_ptrs = txt |> count("*")
    mem.is_const = txt |> contains("const")
    // numeric array dims: [4] or [3][4] in the raw text (enum-named dims handled above)
    peek_data(txt) $(d) {
        var pos = 0
        while (true) {
            let lb = d |> find("[", pos)
            break if (lb < 0)
            let rb = d |> find("]", pos)
            break if (rb < lb)
            let dim = d |> slice(lb + 1, rb) |> trim()
            if (!empty(dim)) {
                mem.arr_dims |> push(dim)
            }
            pos = rb + 1
        }
    }
    // C bitfield width -- ":24"
    let colon = txt |> find(":")
    if (colon >= 0) {
        let w = to_int(txt |> slice(colon + 1) |> trim())
        if (w > 0) {
            mem.bit_width = w
        }
    }
}

def private api_ok(node : xml_node) : bool {
    let api = node["api"] as string
    return true if (empty(api))
    // api is a comma list; we want plain "vulkan" present
    for (part in split(api, ",")) {
        return true if (part == "vulkan")
    }
    return false
}

def private parse_types(root : xml_node; var reg : VkRegistry) {
    let types_node = root |> child("types")
    for (t in each_child(types_node, "type")) {
        continue if (!api_ok(t))
        let category = t["category"] as string
        let alias = cs(t["alias"] as string)
        if (category == "handle") {
            var h = VkHandleDef(alias = alias)
            if (!empty(alias)) {
                h.name = cs(t["name"] as string)
            } else {
                h.name = cs(node_text(t, "name"))
                h.dispatchable = node_text(t, "type") == "VK_DEFINE_HANDLE"
                h.parent = cs(t["parent"] as string)
            }
            reg.handle_index[h.name] = length(reg.handles)
            reg.handles |> emplace(h)
        } elif (category == "bitmask") {
            var b = VkBitmaskDef(alias = alias)
            if (!empty(alias)) {
                b.name = cs(t["name"] as string)
            } else {
                b.name = cs(node_text(t, "name"))
                b.is64 = node_text(t, "type") == "VkFlags64"
                var bits = t["requires"] as string
                if (empty(bits)) {
                    bits = t["bitvalues"] as string
                }
                b.bits_enum = cs(bits)
            }
            reg.bitmask_index[b.name] = length(reg.bitmasks)
            reg.bitmasks |> emplace(b)
        } elif (category == "struct" || category == "union") {
            var s = VkStructDef(
                alias = alias,
                is_union = category == "union",
                returned_only = (t["returnedonly"] as string) == "true",
                struct_extends = cs(t["structextends"] as string)
            )
            s.name = cs(t["name"] as string)
            if (empty(alias)) {
                for (m in each_child(t, "member")) {
                    continue if (!api_ok(m))
                    var mem = VkMember(
                        len = cs(m["len"] as string),
                        optional_attr = cs(m["optional"] as string),
                        noautovalidity = cs(m["noautovalidity"] as string),
                        stype_value = cs(m["values"] as string)
                    )
                    parse_decl_into(m, mem)
                    s.members |> emplace(mem)
                }
            }
            reg.struct_index[s.name] = length(reg.structs)
            reg.structs |> emplace(s)
        } elif (category == "basetype") {
            let nm = node_text(t, "name")
            let ct = node_text(t, "type")
            if (!empty(nm) && !empty(ct)) {
                reg.base_types[cs(nm)] = cs(ct)
            }
        } elif (category == "funcpointer") {
            // modern registry format: <proto><type>void</type> <name>PFN_x</name></proto>
            let nm = node_text(t |> child("proto"), "name")
            if (!empty(nm)) {
                reg.func_pointers |> insert(cs(nm))
            }
        }
    }
}

def private parse_enum_blocks(root : xml_node; var reg : VkRegistry) {
    for (eb in each_child(root, "enums")) {
        let nm = eb["name"] as string
        let etype = eb["type"] as string
        if (etype == "constants" || nm == "API Constants") {
            for (e in each_child(eb, "enum")) {
                continue if (!api_ok(e))
                var c = VkConstantDef(
                    name = cs(e["name"] as string),
                    ctype = cs(e["type"] as string),
                    is_alias = !empty(e["alias"] as string)
                )
                reg.constants |> emplace(c)
            }
            continue
        }
        continue if (etype != "enum" && etype != "bitmask")
        var ed = VkEnumDef(
            name = cs(nm),
            is_bitmask = etype == "bitmask",
            bit_width = node_attr_int(eb, "bitwidth", 32)
        )
        for (e in each_child(eb, "enum")) {
            continue if (!api_ok(e))
            var entry = VkEnumEntry(
                name = cs(e["name"] as string),
                bitpos = node_attr_int(e, "bitpos", -1),
                is_alias = !empty(e["alias"] as string)
            )
            if (entry.bitpos < 0 && ed.is_bitmask && !entry.is_alias) {
                entry.bitpos = single_bit_pos(e["value"] as string)
            }
            ed.entries |> emplace(entry)
        }
        reg.enum_index[ed.name] = length(reg.enums)
        reg.enums |> emplace(ed)
    }
}

//! parse "0x00000040" / "64" -- generator-trusted registry text, hand-rolled to keep hex support explicit
def public parse_value_u64(value_text : string) : uint64 {
    var v = 0ul
    if (value_text |> starts_with("0x")) {
        peek_data(value_text |> slice(2)) $(arr) {
            for (c in arr) {
                let ic = int(c)
                if (is_number(ic)) {
                    v = v * 16ul + uint64(ic - '0')
                } elif (ic >= 'a' && ic <= 'f') {
                    v = v * 16ul + uint64(ic - 'a' + 10)
                } elif (ic >= 'A' && ic <= 'F') {
                    v = v * 16ul + uint64(ic - 'A' + 10)
                }
            }
        }
    } else {
        v = uint64(to_int64(value_text))
    }
    return v
}

//! bit position when the value text is a single set bit, else -1
def public single_bit_pos(value_text : string) : int {
    return -1 if (empty(value_text))
    let v = parse_value_u64(value_text)
    return -1 if (v == 0ul)
    var bit = -1
    for (i in range(64)) {
        if ((v & (1ul << uint64(i))) != 0ul) {
            return -1 if (bit >= 0)
            bit = i
        }
    }
    return bit
}

def private parse_commands(root : xml_node; var reg : VkRegistry) {
    let commands_node = root |> child("commands")
    for (c in each_child(commands_node, "command")) {
        continue if (!api_ok(c))
        let alias = cs(c["alias"] as string)
        var cmd = VkCommandDef(alias = alias)
        if (!empty(alias)) {
            cmd.name = cs(c["name"] as string)
        } else {
            let proto = c |> child("proto")
            cmd.name = cs(node_text(proto, "name"))
            cmd.ret_type = cs(node_text(proto, "type"))
            for (p in each_child(c, "param")) {
                continue if (!api_ok(p))
                var mem = VkMember(
                    len = cs(p["len"] as string),
                    optional_attr = cs(p["optional"] as string)
                )
                parse_decl_into(p, mem)
                cmd.params |> emplace(mem)
            }
        }
        reg.command_index[cmd.name] = length(reg.commands)
        reg.commands |> emplace(cmd)
    }
}

[unused_argument(ext_number)]
def private process_require_block(req : xml_node; guard : string; ext_number : int; var reg : VkRegistry) {
    for (item in each_child(req)) {
        let inm = item.name
        if (inm == "type") {
            let tn = item["name"] as string
            if (!key_exists(reg.enabled_types, tn)) {
                reg.enabled_types[cs(tn)] = guard
            }
        } elif (inm == "command") {
            let cn = item["name"] as string
            if (!key_exists(reg.enabled_commands, cn)) {
                reg.enabled_commands[cs(cn)] = guard
            }
        } elif (inm == "enum") {
            let extends = item["extends"] as string
            if (empty(extends)) {
                // plain constant/value reference -- record the enabling guard so emit can #if-gate it
                let cn = item["name"] as string
                if (!empty(cn) && !key_exists(reg.enabled_constants, cn)) {
                    reg.enabled_constants[cs(cn)] = guard
                }
                continue
            }
            continue if (!api_ok(item))
            let target = reg.enum_index?[extends] ?? -1
            continue if (target < 0)
            var entry = VkEnumEntry(
                name = cs(item["name"] as string),
                bitpos = node_attr_int(item, "bitpos", -1),
                is_alias = !empty(item["alias"] as string),
                guard = guard
            )
            if (entry.bitpos < 0 && !entry.is_alias && reg.enums[target].is_bitmask) {
                entry.bitpos = single_bit_pos(item["value"] as string)
            }
            // dedup -- the same promoted entry appears in several extensions
            var dup = false
            for (ex in reg.enums[target].entries) {
                if (ex.name == entry.name) {
                    dup = true
                    break
                }
            }
            if (!dup) {
                reg.enums[target].entries |> emplace(entry)
            }
        }
    }
}

def private parse_features(root : xml_node; var reg : VkRegistry) {
    for (f in each_child(root, "feature")) {
        continue if (!api_ok(f))
        var guard = cs(f["name"] as string)      // VK_VERSION_1_0 etc -- also a header define
        // internal sub-features (registry >= 1.4.3xx) are not header defines -- fold into VK_VERSION_x_y
        if ((f["apitype"] as string) == "internal") {
            let vpos = guard |> find("VERSION_")
            if (vpos >= 0) {
                guard = "VK_" + (guard |> slice(vpos))
            }
        }
        for (req in each_child(f, "require")) {
            process_require_block(req, guard, 0, reg)
        }
    }
}

def private parse_extensions(root : xml_node; var reg : VkRegistry) {
    let exts = root |> child("extensions")
    for (e in each_child(exts, "extension")) {
        let supported = e["supported"] as string
        var ok = false
        for (part in split(supported, ",")) {
            if (part == "vulkan") {
                ok = true
            }
        }
        continue if (!ok)
        let ename = cs(e["name"] as string)
        let platform = e["platform"] as string
        if (!empty(platform)) {
            reg.ext_protect[ename] = reg.platform_protect?[platform] ?? ""
        }
        let ext_number = node_attr_int(e, "number", 0)
        for (req in each_child(e, "require")) {
            continue if (!api_ok(req))
            process_require_block(req, ename, ext_number, reg)
        }
    }
}

def private parse_platforms_and_tags(root : xml_node; var reg : VkRegistry) {
    let platforms = root |> child("platforms")
    for (p in each_child(platforms, "platform")) {
        reg.platform_protect[cs(p["name"] as string)] = cs(p["protect"] as string)
    }
    let tags = root |> child("tags")
    for (t in each_child(tags, "tag")) {
        reg.vendor_tags |> push(cs(t["name"] as string))
    }
}

//! stamp guard + protect onto every registry item from the enabled sets
def private stamp_guards(var reg : VkRegistry) {
    for (h in reg.handles) {
        h.guard = reg.enabled_types?[h.name] ?? ""
        h.protect = reg.ext_protect?[h.guard] ?? ""
    }
    for (b in reg.bitmasks) {
        b.guard = reg.enabled_types?[b.name] ?? ""
        b.protect = reg.ext_protect?[b.guard] ?? ""
    }
    for (s in reg.structs) {
        s.guard = reg.enabled_types?[s.name] ?? ""
        s.protect = reg.ext_protect?[s.guard] ?? ""
    }
    for (c in reg.commands) {
        c.guard = reg.enabled_commands?[c.name] ?? ""
        c.protect = reg.ext_protect?[c.guard] ?? ""
    }
}

//! Parse vk.xml into a VkRegistry. Panics on parse failure.
def public parse_vk_xml(path : string) : VkRegistry {
    var reg : VkRegistry
    var parsed = false
    open_xml(path) $(doc, ok) {
        if (!ok) {
            return
        }
        let root = doc.document_element
        parse_platforms_and_tags(root, reg)
        parse_types(root, reg)
        parse_enum_blocks(root, reg)
        parse_commands(root, reg)
        parse_features(root, reg)
        parse_extensions(root, reg)
        stamp_guards(reg)
        parsed = true
    }
    if (!parsed) {
        panic("can't parse {path}")
    }
    return <- reg
}

