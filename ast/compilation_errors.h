#pragma once

namespace yzg
{
    enum class CompilationError
    {
        unspecified
        
// lexer errors
        
    ,   mismatching_parentheses
    ,   mismatching_curly_bracers
        
// parser errors
        
    ,   syntax_error

// semantic erros
        
    ,   invalid_type                        //  int & a[], [], int&&, &, *, int&*
    ,   invalid_return_type                 //  func():blah&
    ,   invalid_argument_type               //  func(a:boxed&)
    ,   invalid_structure_field_type        //  struct a:void
    ,   invalid_array_type                  //  array<int&>
    ,   invalid_table_type                  //  table<wtf,int> table<string&,int> table<int,string&>
        
    ,   function_already_declared           //  func x .... func x
    ,   argument_already_declared           //  func(...,a,...,a,....)
    ,   local_variable_already_declared     //  let(...,x,...,x,...)
    ,   global_variable_already_declared    //  let(...,x,...,x,...)
    ,   structure_field_already_declared    //  struct ... x ... x
    ,   structure_already_declared          //  ... struct x ... struct x ...
        
    ,   type_not_found                      //  a:wtf
    ,   structure_not_found
        
    ,   cant_initialize
        
    ,   sizeof_reference                    //  sizeof(int&)
    };
}
