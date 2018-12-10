#pragma once

namespace yzg
{
    enum class CompilationError : int
    {
        unspecified
        
// lexer errors
        
    ,   mismatching_parentheses                 =   10001
    ,   mismatching_curly_bracers               =   10002
        
// parser errors
        
    ,   syntax_error                            =   20000

// semantic erros
        
    ,   invalid_type                            =   30101       //  int & a[], int&&, int&*
    ,   invalid_return_type                     =   30102       //  func():blah&
    ,   invalid_argument_type                   =   30103       //  func(a:boxed&)
    ,   invalid_structure_field_type            =   30104       //  struct a:void
    ,   invalid_array_type                      =   30105       //  array<int&>
    ,   invalid_table_type                      =   30106       //  table<wtf,int> table<string&,int> table<int,string&>
    ,   invalid_argument_count                  =   30107       //  assert(), assert(blah,....)
        
    ,   function_already_declared               =   30201       //  func x .... func x
    ,   argument_already_declared               =   30202       //  func(...,a,...,a,....)
    ,   local_variable_already_declared         =   30203       //  let(...,x,...,x,...)
    ,   global_variable_already_declared        =   30204       //  let(...,x,...,x,...)
    ,   structure_field_already_declared        =   30205       //  struct ... x ... x
    ,   structure_already_declared              =   30206       //  ... struct x ... struct x ...
        
    ,   type_not_found                          =   30301       //  a:wtf
    ,   structure_not_found                     =   30302       //  new wtf
        
    ,   cant_initialize                         =   30401       //  struct x=0
        
    ,   cant_dereference                        =   30501
    
        
    ,   sizeof_reference                        =   39901       //  sizeof(int&)
    };
}
