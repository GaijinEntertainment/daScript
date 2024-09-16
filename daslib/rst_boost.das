options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true
options strict_smart_pointers = true

module rst_boost private

require daslib/regex_boost
require daslib/rst
require daslib/ast_boost

def public append_to_group_by_regex(var group:DocGroup; mod:Module?; var reg:Regex) : DocGroup&
    for_each_function(mod, "") <| $(func)
        if function_needs_documenting(func)
            if regex_match(reg,string(func.name)) != -1
                push(group.func,[[auto get_ptr(func), mod]])
    for_each_generic(mod) <| $(func)
        if function_needs_documenting(func)
            if regex_match(reg,string(func.name)) != -1
                push(group.func,[[auto get_ptr(func), mod]])
    unsafe
        return group

def public group_by_regex(name:string; mod:Module?; var reg:Regex)
    //! Creates a group of functions with shared category.
    //! Functions will be added to the group if they match the regular expression.
    var group <- [[DocGroup name=name, hidden=false]]
    append_to_group_by_regex(group,mod,reg)
    return <- group

def public group_by_regex(name:string; mods:array<Module?>; var reg:Regex)
    //! Creates a group of functions with shared category.
    //! Functions will be added to the group if they match the regular expression.
    var group <- [[DocGroup name=name, hidden=false]]
    for mod in mods
        append_to_group_by_regex(group,mod,reg)
    return <- group
