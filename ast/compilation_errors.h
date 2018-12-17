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
    ,   invalid_variable_type                   =   30108       //  a:void
    ,   invalid_new_type                        =   30109       //  new int&, new int
    ,   invalid_index_type                      =   30110       //  a[wtf]
        
    ,   function_already_declared               =   30201       //  func x .... func x
    ,   argument_already_declared               =   30202       //  func(...,a,...,a,....)
    ,   local_variable_already_declared         =   30203       //  let(...,x,...,x,...)
    ,   global_variable_already_declared        =   30204       //  let(...,x,...,x,...)
    ,   structure_field_already_declared        =   30205       //  struct ... x ... x
    ,   structure_already_declared              =   30206       //  ... struct x ... struct x ...
        
    ,   type_not_found                          =   30301       //  a:wtf
    ,   structure_not_found                     =   30302       //  new wtf
    ,   operator_not_found                      =   30303       //  1 + 1.0
    ,   function_not_found                      =   30304       //  wtf(x)
    ,   variable_not_found                      =   30305       //  wtf
        
    ,   cant_initialize                         =   30401       //  struct x=0
        
    ,   cant_dereference                        =   30501
    ,   cant_index                              =   30502       //  wtf[a]
    ,   cant_get_field                          =   30503       //  wtf.field
    ,   cant_write_to_const                     =   30504       //  const int & a = 5
    ,   cant_move_to_const                      =   30505       //  const array<int> & a <- b
    ,   cant_write_to_non_reference             =   30506       //  1 = blah
    ,   cant_copy                               =   30507       //  a = array<int>(x), expecting <-
    ,   cant_move                               =   30508       //  int a; a <- 1
        
    ,   condition_must_be_bool                  =   30601       //  if ( 10 ) ...
        
    ,   cant_pipe                               =   30701       //  wtf <| arg
        
    ,   invalid_block                           =   30801       //  fn({ return; }), fn ({ break; })
        
    ,   module_not_found                        =   30901       //  require wtf
        
    ,   cant_new_handle                         =   31001       //  new Handle
    
    ,   sizeof_reference                        =   39901       //  sizeof(int&)
    };
}
