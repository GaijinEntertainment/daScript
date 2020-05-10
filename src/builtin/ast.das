options indenting=4

require ast

[unsafe]
def describe ( decl : smart_ptr<TypeDecl>; extra, contracts, modules : bool = true )
    return ast_describe(decl,extra,contracts,modules)

[unsafe]
def describe ( decl : TypeDecl const?; extra, contracts, modules : bool = true )
    return ast_describe(reinterpret<smart_ptr<TypeDecl>>(decl),extra,contracts,modules)






