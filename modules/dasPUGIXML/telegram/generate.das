require pugixml

require strings
require fio

var g_FOUT = fstdout()

struct Section
    found : bool = false
    stack : array<xml_node>
    name  : string

def find_section ( node:xml_node; var section:Section )
    section.stack |> push(node)
    if node |> attribute("id") |> as_string("") == section.name
        section.found = true
        return
    var ch = node.first_child
    while ch.ok
        find_section(ch,section)
        if section.found
            return
        ch = ch.next_sibling
    section.stack |> pop

def find_section ( doc : xml_document ; name : string )
    var inscope section <- [[Section() name=name]]
    doc |> find_section(section)
    if !section.found
        panic("section not found\n")
    if length(section.stack) < 2
        panic("section stack too short\n")
    var node = section.stack[length(section.stack)-2]
    if node.name != "h3"
        panic("section is not <h3>")
    return <- node

def collect_comment_rec ( node:xml_node )
    var comment = ""
    var ch = node.first_child
    while ch.ok
        if ch._type == xml_node_type node_pcdata
            if comment|>empty && ch.value=="Yes"
                comment = "Required"
            else
                comment = "{comment}{ch.value}"
        else
            comment = "{comment}{collect_comment_rec(ch)}"
        ch = ch.next_sibling
    return comment

def collect_comment ( node : xml_node )
    var comment = collect_comment_rec(node)
    return build_string <| $ ( writer )
        var spaces = 0
        for Ch in comment
            if Ch==' ' || Ch=='\t' || Ch=='\r' || Ch=='\n'
                spaces ++
            else
                if spaces != 0
                    writer |> write(" ")
                    spaces = 0
                writer |> write_char(Ch)

def sanitize_name ( name:string )
    return "_type"  if name=="type"
    return "_options" if name=="options"
    return "_label" if name=="label"
    return build_string <| $ ( writer )
        for Ch in name
            if Ch==' ' || Ch=='\t' || Ch=='\r' || Ch=='\n' || Ch=='-' || Ch=='.' || Ch=='(' || Ch==')'
                writer |> write("_")
            else
                writer |> write_char(Ch)

def process_type ( node:xml_node; is_optional:bool=false )
    if node.name == "a"
        var href = node |> attribute("href") |> as_string("")
        var classname = ""
        if href |> starts_with("#")
            classname = href |> slice(1) |> sanitize_name()
        else
            classname = href |> sanitize_name()
        return is_optional ? "{classname}?" : "{classname}"
    else
        let value = node.value
        return "int64"      if value=="Integer"
        return "float"      if value=="Float"
        return "float"      if value=="Float number"
        return "string"     if value=="String"
        return "string"     if value=="Integer or String"
        return "bool"       if value=="Boolean"
        return "bool"       if value=="True"       // sometimes there is an error in documentation
        return "array<string>"  if value=="Array of String"
        return "array<int64>"     if value=="Array of Integer"
        return "array<{process_type(node.next_sibling)}>"           if value == "Array of "
        return "array<array<{process_type(node.next_sibling)}>>"    if value == "Array of Array of "
        panic("unsupported type {value}")
        return value

def generate_fromJV ( name:string; fields:array<tuple<field:string;_type:string>> )
    // from value
    g_FOUT |> fprint("def from_JV ( var v:JsonValue?; t:{name} ) : {name}\n")
    g_FOUT |> fprint("    var q : {name}\n")
    for field in fields
        if field._type=="int64" || field._type=="float" || field._type=="string" || field._type=="bool" || field._type |> ends_with("?")
            g_FOUT |> fprint("    q.{field.field} = _::from_JV((v as _object)[\"{field.field}\"], type<{field._type}>)\n")
        else
            g_FOUT |> fprint("    q.{field.field} <- _::from_JV((v as _object)[\"{field.field}\"], type<{field._type}>)\n")
    g_FOUT |> fprint("    return <- q\n\n")
    // from pointer
    g_FOUT |> fprint("def from_JV ( var v:JsonValue?; t:{name}? ) : {name}?\n")
    g_FOUT |> fprint("    return null if v==null\n")
    g_FOUT |> fprint("    var q = new {name}\n")
    g_FOUT |> fprint("    *q <- from_JV(v,type<{name}>)\n")
    g_FOUT |> fprint("    return q\n\n")
    // to value
    g_FOUT |> fprint("def JV ( v:{name} ) : JsonValue?\n")
    g_FOUT |> fprint("    var map: table<string; JsonValue?>\n")
    for field in fields
        if field._type |> ends_with("?")
            g_FOUT |> fprint("    if v.{field.field}!=null\n")
            g_FOUT |> fprint("        map[\"{field.field}\"] = _::JV(v.{field.field})\n")
        else
            g_FOUT |> fprint("    map[\"{field.field}\"] = _::JV(v.{field.field})\n")
    g_FOUT |> fprint("    return _::JV(map)\n\n")

def process_section ( doc:xml_document; name:string )
    var node = doc |> find_section(name)
    var elem = node
    var api_call = ""
    var comment = ""
    var any_fields = false
    while elem.ok
        if elem.name == "h3" && elem != node
            break
        // print("<{elem.name}> = {elem.value} // {elem._type}\n")
        if elem.name == "h4"
            if !any_fields && !api_call |> empty
                g_FOUT |> fprint("\n    //! {comment}\n    object_{api_call}_has_no_fields:bool\n\n")
                generate_fromJV(api_call,[[array<tuple<string;string>>]])
            comment = ""
            api_call = elem |> child("a") |> attribute("id") |> as_string("") |> sanitize_name()
            g_FOUT |> fprint("struct {api_call}")
            any_fields = false
        elif elem.name == "p"
            comment = "{comment} {collect_comment(elem)}"
        elif elem.name == "table"
            g_FOUT |> fprint("\n    //! {comment}\n")
            var tbody = elem |> child("tbody")
            var fields : array<tuple<string;string>>
            var tr = tbody.first_child
            while tr.ok
                any_fields = true
                var td = tr.first_child
                let api_field = td.first_child.value |> sanitize_name()
                td = td.next_sibling
                var field_type_node = td.first_child
                td = td.next_sibling
                var api_field_comment = collect_comment(td)
                td = td.next_sibling
                if td.ok
                    api_field_comment = "{api_field_comment}. {collect_comment(td)}"
                let is_optional = api_field_comment |> starts_with("Optional")
                let api_field_type = process_type(field_type_node,is_optional)
                g_FOUT |> fprint("    ")
                g_FOUT |> fprint("{api_field} : {api_field_type}\t//! {api_field_comment}\n")
                fields |> push([[auto api_field,api_field_type]])
                tr = tr.next_sibling
            g_FOUT |> fprint("\n")
            generate_fromJV(api_call,fields)
            api_call = ""
        elem = elem.next_sibling
    if !any_fields && !api_call |> empty
        g_FOUT |> fprint("\n    //! {comment}\n    object_{api_call}_has_no_fields:bool\n\n")
        generate_fromJV(api_call,[[array<tuple<string;string>>]])

[export]
def main
    using <| $ ( var doc : xml_document )
        using <| $ ( var status : xml_parse_result# )
            let res = doc |> load_document("{get_das_root()}/modules/dasPUGIXML/telegram/tbotapi.xhtml",status)
            if status.status != xml_parse_status status_ok
                panic("failed to load document\n{status.description}\n")
            fopen("{get_das_root()}/modules/dasPUGIXML/telegram/tbotapi.das","w") <| $ ( ff )
                if ff==null
                    panic("failed to open output file\n")
                g_FOUT = ff
                g_FOUT |> fprint("options indenting = 4\n")
                g_FOUT |> fprint("options no_unused_block_arguments = false\n")
                g_FOUT |> fprint("options no_unused_function_arguments = false\n")
                g_FOUT |> fprint("\n")
                g_FOUT |> fprint("module tbotapi public\n")
                g_FOUT |> fprint("\n")
                g_FOUT |> fprint("require daslib/json_boost\n")
                g_FOUT |> fprint("\n")
                doc |> process_section("getting-updates")
                doc |> process_section("available-types")
                doc |> process_section("available-methods")
                doc |> process_section("updating-messages")
                doc |> process_section("stickers")
                doc |> process_section("inline-mode")
                doc |> process_section("payments")
                doc |> process_section("telegram-passport")
                doc |> process_section("games")


