options rtti=true

require fio
require rtti
require ast
require math
require uriparser
require daslib/rst
require daslib/ast_boost
require daslib/strings
require daslib/functional
require daslib/json
require daslib/json_boost
require daslib/regex
require daslib/regex_boost

def document_module_math(root:string)
    var mod = get_module("math")
    var groups <- [{DocGroup
        group_by_regex("all numerics (uint*, int*, float*, double)", mod, %regex~(min|max)$%%);
        group_by_regex("float* and double", mod, %regex~(abs|acos|asin|atan|atan2|cos|sin|tan|exp|log|exp2|log2|pow|sqrt|rcp|ceil|floor|abs|saturate)$%%);
        group_by_regex("float* only", mod, %regex~(atan2_est|rcp_est|ceili|floori|roundi|trunci)$%%);
        group_by_regex("Noise functions", mod, %regex~(uint32_hash|uint_noise_1D|uint_noise_2D|uint_noise_3D)$%%)
    }]
    document("Math library",mod,"{root}/math.rst",groups)

def document_module_builtin(root:string)
    var mod = get_module("$")
    var groups <- [{DocGroup
        group_by_regex("stub0", mod, %regex~(stub0)$%%);
        group_by_regex("stub1", mod, %regex~(stub1)$%%)
    }]
    document("Built-in runtime",mod,"{root}/builtin.rst",groups)

def document_module_fio(root:string)
    var mod = get_module("fio")
    var groups <- [{DocGroup
        group_by_regex("stub0", mod, %regex~(stub0)$%%);
        group_by_regex("stub1", mod, %regex~(stub1)$%%)
    }]
    document("File input output library",mod,"{root}/fio.rst",groups)

def document_module_random(root:string)
    var mod = get_module("random")
    var groups <- [{DocGroup
        group_by_regex("stub0", mod, %regex~(stub0)$%%);
        group_by_regex("stub1", mod, %regex~(stub1)$%%)
    }]
    document("Random generator library",mod,"{root}/random.rst",groups)

def document_module_network(root:string)
    var mod = get_module("network")
    var groups <- [{DocGroup
        group_by_regex("stub0", mod, %regex~(stub0)$%%);
        group_by_regex("stub1", mod, %regex~(stub1)$%%)
    }]
    document("Network socket library",mod,"{root}/network.rst",groups)

def document_module_uriparser(root:string)
    var mod = get_module("uriparser")
    var groups <- [{DocGroup
        group_by_regex("stub0", mod, %regex~(stub0)$%%);
        group_by_regex("stub1", mod, %regex~(stub1)$%%)
    }]
    document("URI manipulation library based on UriParser",mod,"{root}/uriparser.rst",groups)

def document_module_rtti(root:string)
    var mod = get_module("rtti")
    var groups <- [{DocGroup
        group_by_regex("stub0", mod, %regex~(stub0)$%%);
        group_by_regex("stub1", mod, %regex~(stub1)$%%)
    }]
    document("Runtime type information library",mod,"{root}/rtti.rst",groups)

def document_module_ast(root:string)
    var mod = get_module("ast")
    var groups <- [{DocGroup
        group_by_regex("stub0", mod, %regex~(stub0)$%%);
        group_by_regex("stub1", mod, %regex~(stub1)$%%)
    }]
    document("AST manipulation library",mod,"{root}/ast.rst",groups)

def document_module_ast_boost(root:string)
    var mod = find_module("ast_boost")
    var groups <- [{DocGroup
        group_by_regex("stub0", mod, %regex~(stub0)$%%);
        group_by_regex("stub1", mod, %regex~(stub1)$%%)
    }]
    document("Boost package for the AST",mod,"{root}/ast_boost.rst",groups)

def document_module_strings(root:string)
    var mod = find_module("strings")
    var groups <- [{DocGroup
        group_by_regex("stub0", mod, %regex~(stub0)$%%);
        group_by_regex("stub1", mod, %regex~(stub1)$%%)
    }]
    document("String manipulation library",mod,"{root}/strings.rst",groups)

def document_module_functional(root:string)
    var mod = find_module("functional")
    var groups <- [{DocGroup
        group_by_regex("stub0", mod, %regex~(stub0)$%%);
        group_by_regex("stub1", mod, %regex~(stub1)$%%)
    }]
    document("Functional programming library",mod,"{root}/functional.rst",groups)

def document_module_json(root:string)
    var mod = find_module("json")
    var groups <- [{DocGroup
        group_by_regex("stub0", mod, %regex~(stub0)$%%);
        group_by_regex("stub1", mod, %regex~(stub1)$%%)
    }]
    document("JSON manipulation library",mod,"{root}/json.rst",groups)

def document_module_json_boost(root:string)
    var mod = find_module("json_boost")
    var groups <- [{DocGroup
        group_by_regex("stub0", mod, %regex~(stub0)$%%);
        group_by_regex("stub1", mod, %regex~(stub1)$%%)
    }]
    document("Boost package for JSON",mod,"{root}/json_boost.rst",groups)

def document_module_regex(root:string)
    var mod = find_module("regex")
    var groups <- [{DocGroup
        group_by_regex("stub0", mod, %regex~(stub0)$%%);
        group_by_regex("stub1", mod, %regex~(stub1)$%%)
    }]
    document("Regular expression library",mod,"{root}/regex.rst",groups)

def document_module_regex_boost(root:string)
    var mod = find_module("regex_boost")
    var groups <- [{DocGroup
        group_by_regex("stub0", mod, %regex~(stub0)$%%);
        group_by_regex("stub1", mod, %regex~(stub1)$%%)
    }]
    document("Boost package for REGEX",mod,"{root}/regex_boost.rst",groups)

[export]
def test
    let root = get_das_root()+"/doc/source/stdlib"  // todo: modify to output /temp so that we can merge changes
    // document embedded modules
    document_module_builtin(root)
    document_module_math(root)
    document_module_fio(root)
    document_module_random(root)
    document_module_network(root)
    document_module_uriparser(root)
    document_module_rtti(root)
    document_module_ast(root)
    // document daslib modules
    document_module_ast_boost(root)
    document_module_strings(root)
    document_module_functional(root)
    document_module_json(root)
    document_module_json_boost(root)
    document_module_regex(root)
    document_module_regex_boost(root)
    return true
