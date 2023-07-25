
namespace das { class AnnotationFactory; }

// Use in the definition of the annotation class
// Do not use in abstract classes
#define ANNOTATION_DECLARE_SERIALIZABLE( annotation_type )                            \
    virtual const char * getFactoryTag () override { return #annotation_type; }       \
    static AnnotationPtr createInstance () { return make_smart<annotation_type>(); }  \
    static bool registered;


// Use in the .cpp file where the other annotation methods are defined
#define FACTORY_REGISTER_ANNOTATION( annotation_type )                          \
    bool annotation_type :: registered = AnnotationFactory::registerCreator (   \
        #annotation_type, annotation_type::createInstance                       \
    );


// Example:
//
// -- ast.h --
// class Annotation : public BasicAnnotation {
//     ...
//     ANNOTATION_DECLARE_SERIALIZABLE ( Annotation )
// };
//
// -- module_builtin_ast_serializable.cpp --
// ..
// FACTORY_REGISTER_ANNOTATION ( Annotation )
// ..
