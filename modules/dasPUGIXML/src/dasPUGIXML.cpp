#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_typefactory_bind.h"
#include "daScript/ast/ast_handle.h"
#include "daScript/simulate/bind_enum.h"

#include "dasPUGIXML.h"

MAKE_TYPE_FACTORY(xml_document,pugi::xml_document)
MAKE_TYPE_FACTORY(xml_parse_result,pugi::xml_parse_result)
MAKE_TYPE_FACTORY(xml_node,pugi::xml_node)
MAKE_TYPE_FACTORY(xml_attribute,pugi::xml_attribute)

DAS_BASE_BIND_ENUM ( ::pugi::xml_node_type, xml_node_type, \
    node_null, \
    node_document, \
    node_element, \
    node_pcdata, \
    node_cdata, \
    node_comment, \
    node_pi, \
    node_declaration, \
    node_doctype \
);
DAS_BIND_ENUM_CAST ( ::pugi::xml_node_type );

DAS_BASE_BIND_ENUM ( ::pugi::xml_parse_status, xml_parse_status, \
    status_ok, \
    status_file_not_found, \
    status_io_error, \
    status_out_of_memory, \
    status_internal_error, \
    status_unrecognized_tag, \
    status_bad_pi, \
    status_bad_comment, \
    status_bad_cdata, \
    status_bad_doctype, \
    status_bad_pcdata, \
    status_bad_start_element, \
    status_bad_attribute, \
    status_bad_end_element, \
    status_end_element_mismatch, \
    status_append_invalid_root, \
    status_no_document_element \
);
DAS_BIND_ENUM_CAST ( ::pugi::xml_parse_status );

DAS_BASE_BIND_ENUM ( ::pugi::xml_encoding, xml_encoding, \
    encoding_auto, \
    encoding_utf8, \
    encoding_utf16_le, \
    encoding_utf16_be, \
    encoding_utf16, \
    encoding_utf32_le, \
    encoding_utf32_be, \
    encoding_utf32, \
    encoding_wchar, \
    encoding_latin1 \
);
DAS_BIND_ENUM_CAST ( ::pugi::xml_encoding );

namespace das {

struct XmlDocumentAnnotation : ManagedStructureAnnotation<pugi::xml_document,true,true> {
    XmlDocumentAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation("xml_document",ml,"pugi::xml_document") {
        from("xml_node");
    }
};

struct XmlParseResultAnnotation : ManagedStructureAnnotation<pugi::xml_parse_result,true,true> {
    XmlParseResultAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation("xml_parse_result",ml,"pugi::xml_parse_result") {
        addField<DAS_BIND_MANAGED_FIELD(status)>("status");
        addField<DAS_BIND_MANAGED_FIELD(offset)>("offset");
        addField<DAS_BIND_MANAGED_FIELD(encoding)>("encoding");
        addProperty<DAS_BIND_MANAGED_PROP(description)>("description");
    }
};

struct XmlNodeAnnotation : ManagedStructureAnnotation<pugi::xml_node,true,true> {
    XmlNodeAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation("xml_node",ml,"pugi::xml_node") {
        addProperty<DAS_BIND_MANAGED_PROP(name)>("name");
        addProperty<DAS_BIND_MANAGED_PROP(value)>("value");
        addProperty<DAS_BIND_MANAGED_PROP(type)>("_type","type");
        addProperty<DAS_BIND_MANAGED_PROP(empty)>("empty");
    }
    virtual bool isLocal() const override { return true; }
    virtual bool canCopy() const override { return true; }
    virtual bool canMove() const override { return true; }
    virtual bool hasNonTrivialCtor() const override { return false; }
    virtual bool canBePlacedInContainer() const override { return true; }
};

struct XmlAttributeAnnotation : ManagedStructureAnnotation<pugi::xml_attribute,true,true> {
    XmlAttributeAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation("xml_attribute",ml,"pugi::xml_attribute") {
        addProperty<DAS_BIND_MANAGED_PROP(name)>("name");
        addProperty<DAS_BIND_MANAGED_PROP(value)>("value");
        addProperty<DAS_BIND_MANAGED_PROP(empty)>("empty");
    }
    virtual bool isLocal() const override { return true; }
    virtual bool canCopy() const override { return true; }
    virtual bool canMove() const override { return true; }
    virtual bool hasNonTrivialCtor() const override { return false; }
    virtual bool canBePlacedInContainer() const override { return true; }
};

// document

bool pugiLoadDocumentFromFile ( pugi::xml_document & doc, char * filename, pugi::xml_parse_result & status ) {
    if ( !filename ) return false;
    status = doc.load_file(filename);
    return status;
}

pugi::xml_node pugiDocumentElement ( const pugi::xml_document & doc ) {
    return doc.document_element();
}

// node

pugi::xml_node pugiNodeChild ( const pugi::xml_node & doc, char * name ) {
    return doc.child(name ? name : "");
}

pugi::xml_attribute pugiNodeAttribute ( const pugi::xml_node & doc, char * name ) {
    return doc.attribute(name ? name : "");
}

bool pugiNodeOk ( const pugi::xml_node & doc ) {
    return doc;
}

pugi::xml_node pugiNodeFirstChild ( const pugi::xml_node & doc ) {
    return doc.first_child();
}

pugi::xml_node pugiNodeNextSibling ( const pugi::xml_node & doc ) {
    return doc.next_sibling();
}

pugi::xml_node pugiNodeParent ( const pugi::xml_node & doc ) {
    return doc.parent();
}

bool pugiNodeEqu ( const pugi::xml_node & doc, const pugi::xml_node & other ) {
    return doc == other;
}

bool pugiNodeNotEqu ( const pugi::xml_node & doc, const pugi::xml_node & other ) {
    return doc != other;
}

// attribute

char * pugiAttribute_as_string ( const pugi::xml_attribute & attr, char * def ) {
    return (char *) attr.as_string(def ? def : "");
}

int32_t pugiAttribute_as_int ( const pugi::xml_attribute & attr, int32_t def ) {
    return attr.as_int(def);
}

uint32_t pugiAttribute_as_uint ( const pugi::xml_attribute & attr, uint32_t def ) {
    return attr.as_uint(def);
}

double pugiAttribute_as_double ( const pugi::xml_attribute & attr, double def ) {
    return attr.as_double(def);
}

float pugiAttribute_as_float ( const pugi::xml_attribute & attr, float def ) {
    return attr.as_float(def);
}

bool pugiAttribute_as_bool ( const pugi::xml_attribute & attr, bool def ) {
    return attr.as_bool(def);
}

class Module_PUGIXML : public Module {
public:
    Module_PUGIXML() : Module("pugixml") {
        ModuleLibrary lib;
        lib.addModule(this);
        lib.addBuiltInModule();
        addEnumeration(make_smart<Enumerationxml_parse_status>());
        addEnumeration(make_smart<Enumerationxml_node_type>());
        addEnumeration(make_smart<Enumerationxml_encoding>());
        addAnnotation(make_smart<XmlAttributeAnnotation>(lib));
        addAnnotation(make_smart<XmlNodeAnnotation>(lib));
        addAnnotation(make_smart<XmlDocumentAnnotation>(lib));
        addAnnotation(make_smart<XmlParseResultAnnotation>(lib));
        addUsing<pugi::xml_document>(*this,lib,"pugi::xml_document");
        addUsing<pugi::xml_parse_result>(*this,lib,"pugi::xml_parse_result");
        // document
        addExtern<DAS_BIND_FUN(pugiLoadDocumentFromFile)> (*this, lib, "load_document",
            SideEffects::modifyArgumentAndAccessExternal, "pugiLoadDocumentFromFile")
                ->args({"doc","filename","result"});
        addExtern<DAS_BIND_FUN(pugiDocumentElement),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, ".`document_element",
            SideEffects::none, "pugiDocumentElement")
                ->args({"document"});
        // node
        addExtern<DAS_BIND_FUN(pugiNodeOk)> (*this, lib, ".`ok",
            SideEffects::none, "pugiNodeOk")
                ->args({"node"});
        addExtern<DAS_BIND_FUN(pugiNodeChild),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, "child",
            SideEffects::none, "pugiNodeChild")
                ->args({"node","name"});
        addExtern<DAS_BIND_FUN(pugiNodeAttribute),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, "attribute",
            SideEffects::none, "pugiNodeAttribute")
                ->args({"node","name"});
        addExtern<DAS_BIND_FUN(pugiNodeNextSibling),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, ".`next_sibling",
            SideEffects::none, "pugiNodeNextSibling")
                ->args({"node"});
        addExtern<DAS_BIND_FUN(pugiNodeFirstChild),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, ".`first_child",
            SideEffects::none, "pugiNodeFirstChild")
                ->args({"node"});
        addExtern<DAS_BIND_FUN(pugiNodeParent),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, ".`parent",
            SideEffects::none, "pugiNodeParent")
                ->args({"node"});
        addExtern<DAS_BIND_FUN(pugiNodeEqu)> (*this, lib, "==",
            SideEffects::none, "pugiNodeEqu")
                ->args({"node_a","node_b"});
        addExtern<DAS_BIND_FUN(pugiNodeNotEqu)> (*this, lib, "!=",
            SideEffects::none, "pugiNodeNotEqu")
                ->args({"node_a","node_b"});
        // attributes
        addExtern<DAS_BIND_FUN(pugiAttribute_as_string)> (*this, lib, "as_string",
            SideEffects::none, "pugiAttribute_as_string")
                ->args({"attribute","default_value"});
        addExtern<DAS_BIND_FUN(pugiAttribute_as_int)> (*this, lib, "as_int",
            SideEffects::none, "pugiAttribute_as_int")
                ->args({"attribute","default_value"});
        addExtern<DAS_BIND_FUN(pugiAttribute_as_uint)> (*this, lib, "as_uint",
            SideEffects::none, "pugiAttribute_as_uint")
                ->args({"attribute","default_value"});
        addExtern<DAS_BIND_FUN(pugiAttribute_as_double)> (*this, lib, "as_double",
            SideEffects::none, "pugiAttribute_as_double")
                ->args({"attribute","default_value"});
        addExtern<DAS_BIND_FUN(pugiAttribute_as_float)> (*this, lib, "as_float",
            SideEffects::none, "pugiAttribute_as_float")
                ->args({"attribute","default_value"});
        addExtern<DAS_BIND_FUN(pugiAttribute_as_bool)> (*this, lib, "as_bool",
            SideEffects::none, "pugiAttribute_as_bool")
                ->args({"attribute","default_value"});
    }
    virtual ModuleAotType aotRequire ( TextWriter & tw ) const override {
        tw << "#include \"../modules/dasPUGIXML/src/dasPUGIXML.h\"\n";
        return ModuleAotType::cpp;
    }
};

}

REGISTER_MODULE_IN_NAMESPACE(Module_PUGIXML,das);

