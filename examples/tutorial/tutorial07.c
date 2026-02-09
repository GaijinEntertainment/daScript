#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <assert.h>

#include "daScript/daScriptC.h"

#define TUTORIAL_NAME   "/examples/tutorial/tutorial07.das"

typedef struct {
    int     foo;
    float   bar;
} FooBar;

void tutorial () {
    das_text_writer * tout = das_text_make_printer();               // output stream for all compiler messages (stdout. for stringstream use TextWriter)
    das_module_group * dummyLibGroup = das_modulegroup_make();      // module group for compiled program
    das_file_access * fAccess = das_fileaccess_make_default();      // default file access
    das_context * ctx = NULL;
    // get path to tutorial
    char fileName[256];
    das_get_root(fileName, sizeof(fileName));
    int charsLeft = ((int)sizeof(fileName)) - ((int)strlen(fileName)) - 1;
    assert(charsLeft>=0 && "fileName buffer is too small ");
    strncat ( fileName, TUTORIAL_NAME, (size_t)charsLeft);
    // compile program
    das_program * program = das_program_compile(fileName, fAccess, tout, dummyLibGroup);
    int err_count = das_program_err_count(program);
    if ( err_count ) {
        // if compilation failed, report errors
        das_text_output(tout, "failed to compile\n");
        for ( int ei=0; ei!=err_count; ++ei ) {
            das_error * error = das_program_get_error(program, ei);
            das_error_output(error, tout);
        }
        goto shutdown;
    }
    // create daScript context
    ctx = das_context_make(das_program_context_stack_size(program));
    if ( !das_program_simulate(program,ctx,tout) ) {
        // if interpretation failed, report errors
        das_text_output(tout, "failed to simulate\n");
        err_count = das_program_err_count(program);
        for ( int ei=0; ei!=err_count; ++ei ) {
            das_error * error = das_program_get_error(program, ei);
            das_error_output(error, tout);
        }

        goto shutdown;
    }
    // find function 'test' in the context
    das_function * fnTest = das_context_find_function(ctx,"test");
    if ( !fnTest ) {
        das_text_output(tout, "function 'test' not found\n");
        goto shutdown;
    }
    // evaluate 'test' function in the context
    das_context_eval_with_catch(ctx, fnTest, NULL);
    char * ex = das_context_get_exception(ctx);
    if ( ex!=NULL ) {
        das_text_output(tout, "exception: ");
        das_text_output(tout, ex);
        das_text_output(tout, "\n");
        goto shutdown;
    }
    // evalute 'test_cmres' function in the context, which returns complex result (FooBar structure)
    das_function * fnTestCmres = das_context_find_function(ctx,"test_cmres");
    if ( !fnTestCmres ) {
        das_text_output(tout, "function 'test_cmres' not found\n");
        goto shutdown;
    }
    FooBar cmres;
    vec4f args[2];
    args[0] = das_result_int(42);
    args[1] = das_result_float(3.14f);
    das_context_eval_with_catch_cmres(ctx, fnTestCmres, args, &cmres);
    ex = das_context_get_exception(ctx);
    if ( ex!=NULL ) {
        das_text_output(tout, "exception: ");
        das_text_output(tout, ex);
        das_text_output(tout, "\n");
        goto shutdown;
    }
    printf("test_cmres returned: foo=%i, bar=%f\n", cmres.foo, cmres.bar);
shutdown:;
    das_program_release(program);
    das_fileaccess_release(fAccess);
    das_modulegroup_release(dummyLibGroup);
    das_text_release(tout);
}

// this is test function
vec4f das_c_func ( das_context * ctx, das_node * node, vec4f * args ) {
    (void)ctx; (void)node;
    printf("from das_c_func(%i)\n", das_argument_int(args[0]));     // access argument
    return das_result_void();                                       // return result
}

// this is test function (string)
vec4f das_c_str_func ( das_context * ctx, das_node * node, vec4f * args ) {
    (void)ctx; (void)node;
    printf("from das_c_str_func(%s)\n", das_argument_string(args[0]));  // access argument
    return das_result_void();                                       // return result
}

// this function receives function pointer and calls it with int and float args
// def callback_func ( callback : function<(int,float):string), int, float ) : string
vec4f das_callback_func ( das_context * ctx, das_node * node, vec4f * args ) {
    (void)ctx; (void)node;
    das_function * callback = das_argument_function(args[0]);
    if ( callback ) {
        int iArg = das_argument_int(args[1]);
        float fArg = das_argument_float(args[2]);
        printf("calling callback function(%i,%f)...\n", iArg, fArg);
        vec4f callback_args[2];
        callback_args[0] = das_result_int(iArg);
        callback_args[1] = das_result_float(fArg);
        vec4f result = das_context_eval_with_catch(ctx, callback, callback_args);
        char * ex = das_context_get_exception(ctx);
        if ( ex!=NULL ) {
            das_text_writer * tout = das_text_make_printer();
            das_text_output(tout, "exception in callback: ");
            das_text_output(tout, ex);
            das_text_output(tout, "\n");
            das_text_release(tout);
        }
        char * str_result = das_argument_string(result);
        char * prefix = "callback function returned: ";
        char output[256];
        snprintf(output, sizeof(output), "%s%s\n", prefix, str_result);
        output[255] = '\0';
        char * out_str = das_allocate_string(ctx, output);
        printf("%s", out_str);
        return das_result_string(out_str);
    } else {
        printf("callback function is null\n");
        return das_result_string(NULL);
    }
}

// this function receives lambda and calls it with int and float args
// def lambda_func ( lambda : lambda<(int,float):string), int, float ) : string
vec4f das_lambda_func ( das_context * ctx, das_node * node, vec4f * args ) {
    (void)ctx; (void)node;
    das_lambda * lambda = das_argument_lambda(args[0]);
    if ( lambda ) {
        int iArg = das_argument_int(args[1]);
        float fArg = das_argument_float(args[2]);
        printf("calling lambda (%i,%f)...\n", iArg, fArg);
        vec4f lambda_args[3];
        lambda_args[0] = das_result_lambda(lambda);
        lambda_args[1] = das_result_int(iArg);
        lambda_args[2] = das_result_float(fArg);
        vec4f result = das_context_eval_lambda(ctx, lambda, lambda_args);
        char * ex = das_context_get_exception(ctx);
        if ( ex!=NULL ) {
            das_text_writer * tout = das_text_make_printer();
            das_text_output(tout, "exception in lambda: ");
            das_text_output(tout, ex);
            das_text_output(tout, "\n");
            das_text_release(tout);
        }
        char * str_result = das_argument_string(result);
        char * prefix = "lambda function returned: ";
        char output[256];
        snprintf(output, sizeof(output), "%s%s\n", prefix, str_result);
        output[255] = '\0';
        char * out_str = das_allocate_string(ctx, output);
        printf("%s", out_str);
        return das_result_string(out_str);
    } else {
        printf("lambda function is null\n");
        return das_result_string(NULL);
    }
}

// this function receives lambda and calls it with int and float args
// def lambda_func ( block : block<(int,float):string), int, float ) : string
vec4f das_block_func ( das_context * ctx, das_node * node, vec4f * args ) {
    (void)ctx; (void)node;
    das_block * block = das_argument_block(args[0]);
    if ( block ) {
        int iArg = das_argument_int(args[1]);
        float fArg = das_argument_float(args[2]);
        printf("calling block (%i,%f)...\n", iArg, fArg);
        vec4f block_args[2];
        block_args[0] = das_result_int(iArg);
        block_args[1] = das_result_float(fArg);
        vec4f result = das_context_eval_block(ctx, block, block_args);
        char * ex = das_context_get_exception(ctx);
        if ( ex!=NULL ) {
            das_text_writer * tout = das_text_make_printer();
            das_text_output(tout, "exception in block: ");
            das_text_output(tout, ex);
            das_text_output(tout, "\n");
            das_text_release(tout);
        }
        char * str_result = das_argument_string(result);
        char * prefix = "block returned: ";
        char output[256];
        snprintf(output, sizeof(output), "%s%s\n", prefix, str_result);
        output[255] = '\0';
        char * out_str = das_allocate_string(ctx, output);
        printf("%s", out_str);
        return das_result_string(out_str);
    } else {
        printf("block is null\n");
        return das_result_string(NULL);
    }
}

typedef enum {
    OneTwo_one = 1,
    OneTwo_two = 2
} OneTwo;

// model
typedef struct {
    float *     vertices;
    uint32_t    vertexCount;
    int         smoothShading;
} model;

// def make_model ( vtx : array<float> ) : model?
vec4f make_model ( das_context * ctx, das_node * node, vec4f * args ) {
    (void)ctx; (void)node;
    // get array from argument
    das_array * arr = (das_array *) das_argument_ptr(args[0]);
    uint32_t size = arr->size;
    float * data = (float *) arr->data;
    printf("make_model called with array of size %u\n", size);
    for ( uint32_t i=0; i!=size; ++i ) {
        printf("  vtx[%u] = %f\n", i, data[i]);
    }
    model * mdl = (model *) malloc ( sizeof(model) );
    mdl->vertexCount = size;
    mdl->vertices = (float *) malloc ( sizeof(float) * size );
    memcpy ( mdl->vertices, data, sizeof(float) * size );
    mdl->smoothShading = 0;
    return das_result_ptr(mdl);   // return null model
}

// def set_smooth_shading ( mdl : model, smooth : bool ) : void
vec4f set_smooth_shading ( das_context * ctx, das_node * node, vec4f * args ) {
    (void)ctx; (void)node;
    model * mdl = (model *) das_argument_ptr(args[0]);
    int smooth = das_argument_bool(args[1]);
    printf("set_smooth_shading called with model %p and smooth=%s\n", mdl, smooth ? "true" : "false");
    if ( mdl ) {
        mdl->smoothShading = smooth ? 1 : 0;
    }
    return das_result_void();
}

das_module * register_module_tutorial_07() {
    // create module and library
    das_module * mod = das_module_create ("tutorial_07");
    das_module_group * lib = das_modulegroup_make();
    das_modulegroup_add_module(lib, mod);
    // alias
    das_module_bind_alias (mod, lib, "intarray", "1<i>A" );
    // enumeration
    das_enumeration * en = das_enumeration_make("OneTwo", "OneTwo", 1);
    das_enumeration_add_value(en, "one", "OneTwo_one", OneTwo_one);
    das_enumeration_add_value(en, "two", "OneTwo_two", OneTwo_two);
    das_module_bind_enumeration(mod, en);
    // handled structure
    das_structure * st = das_structure_make(lib, "FooBar", "FooBar", sizeof(FooBar), _Alignof(FooBar));
    das_structure_add_field(st, mod, lib, "foo", "foo", offsetof(FooBar,foo), "i");
    das_structure_add_field(st, mod, lib, "bar", "bar", offsetof(FooBar,bar), "f");
    das_module_bind_structure(mod, st);
    // bind das_c_func
    das_module_bind_interop_function(mod, lib, &das_c_func, "das_c_func", "das_c_func", SIDEEFFECTS_modifyExternal, "v i");
    // bind das_c_str_func
    das_module_bind_interop_function(mod, lib, &das_c_str_func, "das_c_str_func", "das_c_str_func", SIDEEFFECTS_modifyExternal, "v s");
    // bind das_callback_func
    das_module_bind_interop_function(mod, lib, &das_callback_func, "das_callback_func", "das_callback_func", SIDEEFFECTS_modifyExternal, "s 0<i;f>@@ i f");
    // bind das_lambda_func
    das_module_bind_interop_function(mod, lib, &das_lambda_func, "das_lambda_func", "das_lambda_func", SIDEEFFECTS_modifyExternal, "s 0<i;f>@ i f");
    // bind das_block_func
    das_module_bind_interop_function(mod, lib, &das_block_func, "das_block_func", "das_block_func", SIDEEFFECTS_modifyExternal, "s 0<i;f>$ i f");

    ////////////////
    // model example
    ////////////////

    // handled structure 'model'
    das_structure * mst = das_structure_make(lib, "model", "model", sizeof(model), _Alignof(model));
    das_structure_add_field(mst, mod, lib, "vertices", "vertices", offsetof(model,vertices), "1<f>?");
    das_structure_add_field(mst, mod, lib, "vertexCount", "vertexCount", offsetof(model,vertexCount), "u");
    das_structure_add_field(mst, mod, lib, "smoothShading", "smoothShading", offsetof(model,smoothShading), "i");
    // bind model functions
    das_module_bind_interop_function(mod, lib, &make_model, "make_model", "make_model", SIDEEFFECTS_modifyExternal, "1<H<model>>? C1<f>A");
    das_module_bind_interop_function(mod, lib, &set_smooth_shading, "set_smooth_shading", "set_smooth_shading", SIDEEFFECTS_modifyExternal, "v 1<H<model>>? b");
    // cleanup
    das_modulegroup_release(lib);
    return mod;
}

int main( int argc, char ** argv ) {
    (void)argc; (void)argv;
    // Initialize all default modules
    das_initialize();
    // register modules
    register_module_tutorial_07();
    // run the tutorial
    tutorial();
    // shut-down daScript, free all memory
    das_shutdown();
    return 0;
}
