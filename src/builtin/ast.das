options indenting=4

require ast

[unsafe]
def describe ( decl : smart_ptr<TypeDecl>; extra, contracts, modules : bool = true )
    return ast_describe_typedecl(decl,extra,contracts,modules)

[unsafe]
def describe ( expr : smart_ptr<Expression> )
    return ast_describe_expression(expr)

