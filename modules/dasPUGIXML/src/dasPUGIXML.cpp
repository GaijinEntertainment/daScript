#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_typefactory_bind.h"
#include "daScript/ast/ast_handle.h"
#include "daScript/simulate/bind_enum.h"

#include "dasPUGIXML.h"

#include <sstream>

MAKE_TYPE_FACTORY(xml_document,pugi::xml_document)
MAKE_TYPE_FACTORY(xml_parse_result,pugi::xml_parse_result)
MAKE_TYPE_FACTORY(xml_node,pugi::xml_node)
MAKE_TYPE_FACTORY(xml_attribute,pugi::xml_attribute)
MAKE_TYPE_FACTORY(xml_text,pugi::xml_text)
MAKE_TYPE_FACTORY(xpath_node,pugi::xpath_node)
MAKE_TYPE_FACTORY(xpath_node_set,pugi::xpath_node_set)
MAKE_TYPE_FACTORY(xpath_query,pugi::xpath_query)
MAKE_TYPE_FACTORY(xpath_variable_set,pugi::xpath_variable_set)

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

DAS_BASE_BIND_ENUM ( ::pugi::xpath_value_type, xpath_value_type, \
    xpath_type_none, \
    xpath_type_node_set, \
    xpath_type_number, \
    xpath_type_string, \
    xpath_type_boolean \
);
DAS_BIND_ENUM_CAST ( ::pugi::xpath_value_type );

namespace das {

// ============ Type Annotations ============

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
    virtual bool isLocal() const override { return true; }
    virtual bool canCopy() const override { return true; }
    virtual bool canMove() const override { return true; }
    virtual bool hasNonTrivialCtor() const override { return false; }
    virtual bool canBePlacedInContainer() const override { return true; }
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

struct XmlTextAnnotation : ManagedStructureAnnotation<pugi::xml_text,true,true> {
    XmlTextAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation("xml_text",ml,"pugi::xml_text") {
        addProperty<DAS_BIND_MANAGED_PROP(empty)>("empty");
        addProperty<DAS_BIND_MANAGED_PROP(get)>("get");
    }
    virtual bool isLocal() const override { return true; }
    virtual bool canCopy() const override { return true; }
    virtual bool canMove() const override { return true; }
    virtual bool hasNonTrivialCtor() const override { return false; }
    virtual bool canBePlacedInContainer() const override { return true; }
};

struct XPathNodeAnnotation : ManagedStructureAnnotation<pugi::xpath_node,true,true> {
    XPathNodeAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation("xpath_node",ml,"pugi::xpath_node") {
    }
    virtual bool isLocal() const override { return true; }
    virtual bool canCopy() const override { return true; }
    virtual bool canMove() const override { return true; }
    virtual bool hasNonTrivialCtor() const override { return false; }
    virtual bool canBePlacedInContainer() const override { return true; }
};

struct XPathNodeSetAnnotation : ManagedStructureAnnotation<pugi::xpath_node_set,true,true> {
    XPathNodeSetAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation("xpath_node_set",ml,"pugi::xpath_node_set") {
    }
};

struct XPathQueryAnnotation : ManagedStructureAnnotation<pugi::xpath_query,true,true> {
    XPathQueryAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation("xpath_query",ml,"pugi::xpath_query") {
    }
};

struct XPathVariableSetAnnotation : ManagedStructureAnnotation<pugi::xpath_variable_set,true,true> {
    XPathVariableSetAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation("xpath_variable_set",ml,"pugi::xpath_variable_set") {
    }
};

// ============ Document functions ============

pugi::xml_node pugiDocumentAsNode ( pugi::xml_document * doc ) {
    return doc ? static_cast<pugi::xml_node>(*doc) : pugi::xml_node();
}

bool pugiLoadDocumentFromFile ( pugi::xml_document * doc, char * filename, pugi::xml_parse_result & status ) {
    if ( !filename || !doc ) return false;
    status = doc->load_file(filename);
    return status;
}

bool pugiLoadDocumentFromString ( pugi::xml_document * doc, char * content, pugi::xml_parse_result & status, uint32_t options ) {
    if ( !content || !doc ) return false;
    status = doc->load_string(content, options);
    return status;
}

pugi::xml_node pugiDocumentElement ( const pugi::xml_document * doc ) {
    return doc ? doc->document_element() : pugi::xml_node();
}

bool pugiSaveDocumentToFile ( const pugi::xml_document * doc, char * filename, char * indent, uint32_t flags, pugi::xml_encoding encoding ) {
    if ( !filename || !doc ) return false;
    return doc->save_file(filename, indent ? indent : "\t", flags, encoding);
}

char * pugiDocumentSaveToString ( const pugi::xml_document * doc, char * indent, uint32_t flags, pugi::xml_encoding encoding, Context * context, LineInfoArg * at ) {
    if ( !doc ) return nullptr;
    std::ostringstream oss;
    doc->save(oss, indent ? indent : "\t", flags, encoding);
    auto str = oss.str();
    return context->allocateString(str.c_str(), uint32_t(str.length()), at);
}

void pugiDocumentReset ( pugi::xml_document * doc ) {
    if ( doc ) doc->reset();
}

// ============ Node navigation functions ============

bool pugiNodeOk ( const pugi::xml_node & node ) {
    return node;
}

pugi::xml_node pugiNodeChild ( const pugi::xml_node & node, char * name ) {
    return node.child(name ? name : "");
}

pugi::xml_attribute pugiNodeAttribute ( const pugi::xml_node & node, char * name ) {
    return node.attribute(name ? name : "");
}

pugi::xml_node pugiNodeFirstChild ( const pugi::xml_node & node ) {
    return node.first_child();
}

pugi::xml_node pugiNodeLastChild ( const pugi::xml_node & node ) {
    return node.last_child();
}

pugi::xml_node pugiNodeNextSibling ( const pugi::xml_node & node ) {
    return node.next_sibling();
}

pugi::xml_node pugiNodeNextSiblingByName ( const pugi::xml_node & node, char * name ) {
    return node.next_sibling(name ? name : "");
}

pugi::xml_node pugiNodePreviousSibling ( const pugi::xml_node & node ) {
    return node.previous_sibling();
}

pugi::xml_node pugiNodePreviousSiblingByName ( const pugi::xml_node & node, char * name ) {
    return node.previous_sibling(name ? name : "");
}

pugi::xml_node pugiNodeParent ( const pugi::xml_node & node ) {
    return node.parent();
}

pugi::xml_node pugiNodeRoot ( const pugi::xml_node & node ) {
    return node.root();
}

pugi::xml_attribute pugiNodeFirstAttribute ( const pugi::xml_node & node ) {
    return node.first_attribute();
}

pugi::xml_attribute pugiNodeLastAttribute ( const pugi::xml_node & node ) {
    return node.last_attribute();
}

pugi::xml_text pugiNodeText ( const pugi::xml_node & node ) {
    return node.text();
}

char * pugiNodeChildValue ( const pugi::xml_node & node ) {
    return (char *) node.child_value();
}

char * pugiNodeChildValueByName ( const pugi::xml_node & node, char * name ) {
    return (char *) node.child_value(name ? name : "");
}

pugi::xml_node pugiNodeFindChildByAttribute ( const pugi::xml_node & node, char * element_name, char * attr_name, char * attr_value ) {
    return node.find_child_by_attribute(
        element_name ? element_name : "",
        attr_name ? attr_name : "",
        attr_value ? attr_value : "");
}

pugi::xml_node pugiNodeFindChildByAttribute2 ( const pugi::xml_node & node, char * attr_name, char * attr_value ) {
    return node.find_child_by_attribute(
        attr_name ? attr_name : "",
        attr_value ? attr_value : "");
}

pugi::xml_node pugiNodeFirstElementByPath ( const pugi::xml_node & node, char * path ) {
    return node.first_element_by_path(path ? path : "");
}

int32_t pugiNodeOffsetDebug ( const pugi::xml_node & node ) {
    return int32_t(node.offset_debug());
}

uint64_t pugiNodeHashValue ( const pugi::xml_node & node ) {
    return uint64_t(node.hash_value());
}

char * pugiNodePath ( const pugi::xml_node & node, char * delimiter, Context * context, LineInfoArg * at ) {
    auto path = node.path(delimiter && delimiter[0] ? delimiter[0] : '/');
    return context->allocateString(path.c_str(), uint32_t(path.length()), at);
}

char * pugiNodePrint ( const pugi::xml_node & node, char * indent, uint32_t flags, pugi::xml_encoding encoding, Context * context, LineInfoArg * at ) {
    std::ostringstream oss;
    node.print(oss, indent ? indent : "\t", flags, encoding);
    auto str = oss.str();
    return context->allocateString(str.c_str(), uint32_t(str.length()), at);
}

bool pugiNodeEqu ( const pugi::xml_node & a, const pugi::xml_node & b ) {
    return a == b;
}

bool pugiNodeNotEqu ( const pugi::xml_node & a, const pugi::xml_node & b ) {
    return a != b;
}

// ============ Node mutation functions ============

bool pugiNodeSetName ( pugi::xml_node & node, char * name ) {
    return node.set_name(name ? name : "");
}

bool pugiNodeSetValue ( pugi::xml_node & node, char * value ) {
    return node.set_value(value ? value : "");
}

pugi::xml_node pugiNodeAppendChild ( pugi::xml_node & node, char * name ) {
    return node.append_child(name ? name : "");
}

pugi::xml_node pugiNodeAppendChildByType ( pugi::xml_node & node, pugi::xml_node_type type ) {
    return node.append_child(type);
}

pugi::xml_node pugiNodePrependChild ( pugi::xml_node & node, char * name ) {
    return node.prepend_child(name ? name : "");
}

pugi::xml_node pugiNodePrependChildByType ( pugi::xml_node & node, pugi::xml_node_type type ) {
    return node.prepend_child(type);
}

pugi::xml_node pugiNodeInsertChildAfter ( pugi::xml_node & node, char * name, const pugi::xml_node & after ) {
    return node.insert_child_after(name ? name : "", after);
}

pugi::xml_node pugiNodeInsertChildBefore ( pugi::xml_node & node, char * name, const pugi::xml_node & before ) {
    return node.insert_child_before(name ? name : "", before);
}

bool pugiNodeRemoveChild ( pugi::xml_node & node, const pugi::xml_node & child ) {
    return node.remove_child(child);
}

bool pugiNodeRemoveChildByName ( pugi::xml_node & node, char * name ) {
    return node.remove_child(name ? name : "");
}

bool pugiNodeRemoveChildren ( pugi::xml_node & node ) {
    return node.remove_children();
}

pugi::xml_attribute pugiNodeAppendAttribute ( pugi::xml_node & node, char * name ) {
    return node.append_attribute(name ? name : "");
}

pugi::xml_attribute pugiNodePrependAttribute ( pugi::xml_node & node, char * name ) {
    return node.prepend_attribute(name ? name : "");
}

pugi::xml_attribute pugiNodeInsertAttributeAfter ( pugi::xml_node & node, char * name, const pugi::xml_attribute & after ) {
    return node.insert_attribute_after(name ? name : "", after);
}

pugi::xml_attribute pugiNodeInsertAttributeBefore ( pugi::xml_node & node, char * name, const pugi::xml_attribute & before ) {
    return node.insert_attribute_before(name ? name : "", before);
}

bool pugiNodeRemoveAttribute ( pugi::xml_node & node, const pugi::xml_attribute & attr ) {
    return node.remove_attribute(attr);
}

bool pugiNodeRemoveAttributeByName ( pugi::xml_node & node, char * name ) {
    return node.remove_attribute(name ? name : "");
}

bool pugiNodeRemoveAttributes ( pugi::xml_node & node ) {
    return node.remove_attributes();
}

pugi::xml_node pugiNodeAppendCopyNode ( pugi::xml_node & node, const pugi::xml_node & proto ) {
    return node.append_copy(proto);
}

pugi::xml_node pugiNodePrependCopyNode ( pugi::xml_node & node, const pugi::xml_node & proto ) {
    return node.prepend_copy(proto);
}

pugi::xml_attribute pugiNodeAppendCopyAttr ( pugi::xml_node & node, const pugi::xml_attribute & proto ) {
    return node.append_copy(proto);
}

pugi::xml_attribute pugiNodePrependCopyAttr ( pugi::xml_node & node, const pugi::xml_attribute & proto ) {
    return node.prepend_copy(proto);
}

pugi::xml_node pugiNodeAppendMoveNode ( pugi::xml_node & node, const pugi::xml_node & moved ) {
    return node.append_move(moved);
}

pugi::xml_node pugiNodePrependMoveNode ( pugi::xml_node & node, const pugi::xml_node & moved ) {
    return node.prepend_move(moved);
}

// ============ Attribute functions ============

bool pugiAttributeOk ( const pugi::xml_attribute & attr ) {
    return attr;
}

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

pugi::xml_attribute pugiAttributeNextAttribute ( const pugi::xml_attribute & attr ) {
    return attr.next_attribute();
}

pugi::xml_attribute pugiAttributePreviousAttribute ( const pugi::xml_attribute & attr ) {
    return attr.previous_attribute();
}

uint64_t pugiAttributeHashValue ( const pugi::xml_attribute & attr ) {
    return uint64_t(attr.hash_value());
}

bool pugiAttributeSetName ( pugi::xml_attribute & attr, char * name ) {
    return attr.set_name(name ? name : "");
}

bool pugiAttributeSetValueString ( pugi::xml_attribute & attr, char * value ) {
    return attr.set_value(value ? value : "");
}

bool pugiAttributeSetValueInt ( pugi::xml_attribute & attr, int32_t value ) {
    return attr.set_value(value);
}

bool pugiAttributeSetValueUint ( pugi::xml_attribute & attr, uint32_t value ) {
    return attr.set_value(value);
}

bool pugiAttributeSetValueDouble ( pugi::xml_attribute & attr, double value ) {
    return attr.set_value(value);
}

bool pugiAttributeSetValueFloat ( pugi::xml_attribute & attr, float value ) {
    return attr.set_value(value);
}

bool pugiAttributeSetValueBool ( pugi::xml_attribute & attr, bool value ) {
    return attr.set_value(value);
}

bool pugiAttributeEqu ( const pugi::xml_attribute & a, const pugi::xml_attribute & b ) {
    return a == b;
}

bool pugiAttributeNotEqu ( const pugi::xml_attribute & a, const pugi::xml_attribute & b ) {
    return a != b;
}

// ============ Text functions ============

bool pugiTextOk ( const pugi::xml_text & text ) {
    return text;
}

char * pugiText_as_string ( const pugi::xml_text & text, char * def ) {
    return (char *) text.as_string(def ? def : "");
}

int32_t pugiText_as_int ( const pugi::xml_text & text, int32_t def ) {
    return text.as_int(def);
}

uint32_t pugiText_as_uint ( const pugi::xml_text & text, uint32_t def ) {
    return text.as_uint(def);
}

double pugiText_as_double ( const pugi::xml_text & text, double def ) {
    return text.as_double(def);
}

float pugiText_as_float ( const pugi::xml_text & text, float def ) {
    return text.as_float(def);
}

bool pugiText_as_bool ( const pugi::xml_text & text, bool def ) {
    return text.as_bool(def);
}

bool pugiTextSetString ( pugi::xml_text & text, char * value ) {
    return text.set(value ? value : "");
}

bool pugiTextSetInt ( pugi::xml_text & text, int32_t value ) {
    return text.set(value);
}

bool pugiTextSetUint ( pugi::xml_text & text, uint32_t value ) {
    return text.set(value);
}

bool pugiTextSetDouble ( pugi::xml_text & text, double value ) {
    return text.set(value);
}

bool pugiTextSetFloat ( pugi::xml_text & text, float value ) {
    return text.set(value);
}

bool pugiTextSetBool ( pugi::xml_text & text, bool value ) {
    return text.set(value);
}

bool pugiTextSetInt8 ( pugi::xml_text & text, int8_t value ) {
    return text.set((int)value);
}

bool pugiTextSetUint8 ( pugi::xml_text & text, uint8_t value ) {
    return text.set((unsigned int)value);
}

bool pugiTextSetInt16 ( pugi::xml_text & text, int16_t value ) {
    return text.set((int)value);
}

bool pugiTextSetUint16 ( pugi::xml_text & text, uint16_t value ) {
    return text.set((unsigned int)value);
}

bool pugiTextSetInt64 ( pugi::xml_text & text, int64_t value ) {
    return text.set((long long)value);
}

bool pugiTextSetUint64 ( pugi::xml_text & text, uint64_t value ) {
    return text.set((unsigned long long)value);
}

int64_t pugiText_as_int64 ( const pugi::xml_text & text, int64_t def ) {
    return text.as_llong(def);
}

uint64_t pugiText_as_uint64 ( const pugi::xml_text & text, uint64_t def ) {
    return text.as_ullong(def);
}

pugi::xml_node pugiTextData ( const pugi::xml_text & text ) {
    return text.data();
}

// ============ XPath functions ============

pugi::xpath_query * pugiXPathCompile ( char * query ) {
    return new pugi::xpath_query(query ? query : "");
}

bool pugiXPathQueryOk ( const pugi::xpath_query * query ) {
    return query && *query;
}

pugi::xpath_value_type pugiXPathQueryReturnType ( const pugi::xpath_query * query ) {
    return query ? query->return_type() : pugi::xpath_type_none;
}

char * pugiXPathQueryResultDescription ( const pugi::xpath_query * query ) {
    return query ? (char *) query->result().description() : nullptr;
}

int32_t pugiXPathQueryResultOffset ( const pugi::xpath_query * query ) {
    return query ? int32_t(query->result().offset) : 0;
}

bool pugiXPathEvaluateBoolean ( const pugi::xpath_query * query, const pugi::xml_node & node ) {
    return query ? query->evaluate_boolean(pugi::xpath_node(node)) : false;
}

double pugiXPathEvaluateNumber ( const pugi::xpath_query * query, const pugi::xml_node & node ) {
    return query ? query->evaluate_number(pugi::xpath_node(node)) : 0.0;
}

char * pugiXPathEvaluateString ( const pugi::xpath_query * query, const pugi::xml_node & node, Context * context, LineInfoArg * at ) {
    if ( !query ) return nullptr;
    auto str = query->evaluate_string(pugi::xpath_node(node));
    return context->allocateString(str.c_str(), uint32_t(str.length()), at);
}

pugi::xpath_node_set * pugiXPathEvaluateNodeSet ( const pugi::xpath_query * query, const pugi::xml_node & node ) {
    return query ? new pugi::xpath_node_set(query->evaluate_node_set(pugi::xpath_node(node))) : new pugi::xpath_node_set();
}

pugi::xpath_node pugiXPathEvaluateNode ( const pugi::xpath_query * query, const pugi::xml_node & node ) {
    return query ? query->evaluate_node(pugi::xpath_node(node)) : pugi::xpath_node();
}

pugi::xpath_node pugiNodeSelectNode ( const pugi::xml_node & node, char * query ) {
    return node.select_node(query ? query : "");
}

pugi::xpath_node pugiNodeSelectNodeQuery ( const pugi::xml_node & node, const pugi::xpath_query * query ) {
    return query ? node.select_node(*query) : pugi::xpath_node();
}

pugi::xpath_node_set * pugiNodeSelectNodes ( const pugi::xml_node & node, char * query ) {
    return new pugi::xpath_node_set(node.select_nodes(query ? query : ""));
}

pugi::xpath_node_set * pugiNodeSelectNodesQuery ( const pugi::xml_node & node, const pugi::xpath_query * query ) {
    return query ? new pugi::xpath_node_set(node.select_nodes(*query)) : new pugi::xpath_node_set();
}

// xpath_node accessors

pugi::xml_node pugiXPathNodeNode ( const pugi::xpath_node & xnode ) {
    return xnode.node();
}

pugi::xml_attribute pugiXPathNodeAttribute ( const pugi::xpath_node & xnode ) {
    return xnode.attribute();
}

pugi::xml_node pugiXPathNodeParent ( const pugi::xpath_node & xnode ) {
    return xnode.parent();
}

bool pugiXPathNodeOk ( const pugi::xpath_node & xnode ) {
    return xnode;
}

// xpath_node_set accessors

int32_t pugiXPathNodeSetSize ( const pugi::xpath_node_set * set ) {
    return set ? int32_t(set->size()) : 0;
}

bool pugiXPathNodeSetEmpty ( const pugi::xpath_node_set * set ) {
    return !set || set->empty();
}

pugi::xpath_node pugiXPathNodeSetFirst ( const pugi::xpath_node_set * set ) {
    return set ? set->first() : pugi::xpath_node();
}

pugi::xpath_node pugiXPathNodeSetAt ( const pugi::xpath_node_set * set, int32_t index ) {
    if ( !set || index < 0 || index >= int32_t(set->size()) ) return pugi::xpath_node();
    return (*set)[size_t(index)];
}

void pugiXPathNodeSetSort ( pugi::xpath_node_set * set, bool reverse ) {
    if ( set ) set->sort(reverse);
}

// xpath_variable_set

bool pugiXPathVarSetSetBool ( pugi::xpath_variable_set * vars, char * name, bool value ) {
    return vars ? vars->set(name ? name : "", value) : false;
}

bool pugiXPathVarSetSetNumber ( pugi::xpath_variable_set * vars, char * name, double value ) {
    return vars ? vars->set(name ? name : "", value) : false;
}

bool pugiXPathVarSetSetString ( pugi::xpath_variable_set * vars, char * name, char * value ) {
    return vars ? vars->set(name ? name : "", value ? value : "") : false;
}

pugi::xpath_query * pugiXPathCompileWithVars ( char * query, pugi::xpath_variable_set * vars ) {
    return new pugi::xpath_query(query ? query : "", vars);
}

// ============ Module ============

class Module_PUGIXML : public Module {
public:
    Module_PUGIXML() : Module("pugixml") {
        ModuleLibrary lib;
        lib.addModule(this);
        lib.addBuiltInModule();
        // enumerations
        addEnumeration(make_smart<Enumerationxml_parse_status>());
        addEnumeration(make_smart<Enumerationxml_node_type>());
        addEnumeration(make_smart<Enumerationxml_encoding>());
        addEnumeration(make_smart<Enumerationxpath_value_type>());
        // type annotations
        addAnnotation(make_smart<XmlAttributeAnnotation>(lib));
        addAnnotation(make_smart<XmlTextAnnotation>(lib));
        addAnnotation(make_smart<XmlNodeAnnotation>(lib));
        addAnnotation(make_smart<XPathNodeAnnotation>(lib));
        addAnnotation(make_smart<XPathNodeSetAnnotation>(lib));
        addAnnotation(make_smart<XPathQueryAnnotation>(lib));
        addAnnotation(make_smart<XPathVariableSetAnnotation>(lib));
        addAnnotation(make_smart<XmlDocumentAnnotation>(lib));
        addAnnotation(make_smart<XmlParseResultAnnotation>(lib));
        // using declarations for lightweight handle types
        addCtorAndUsing<pugi::xml_document>(*this,lib,"xml_document","pugi::xml_document");
        addCtorAndUsing<pugi::xpath_node_set>(*this,lib,"xpath_node_set","pugi::xpath_node_set");
        addCtorAndUsing<pugi::xpath_variable_set>(*this,lib,"xpath_variable_set","pugi::xpath_variable_set");
        // parse flag constants
        addConstant<uint32_t>(*this, "parse_minimal",           pugi::parse_minimal);
        addConstant<uint32_t>(*this, "parse_pi",                pugi::parse_pi);
        addConstant<uint32_t>(*this, "parse_comments",          pugi::parse_comments);
        addConstant<uint32_t>(*this, "parse_cdata",             pugi::parse_cdata);
        addConstant<uint32_t>(*this, "parse_ws_pcdata",         pugi::parse_ws_pcdata);
        addConstant<uint32_t>(*this, "parse_escapes",           pugi::parse_escapes);
        addConstant<uint32_t>(*this, "parse_eol",               pugi::parse_eol);
        addConstant<uint32_t>(*this, "parse_wconv_attribute",   pugi::parse_wconv_attribute);
        addConstant<uint32_t>(*this, "parse_wnorm_attribute",   pugi::parse_wnorm_attribute);
        addConstant<uint32_t>(*this, "parse_declaration",       pugi::parse_declaration);
        addConstant<uint32_t>(*this, "parse_doctype",           pugi::parse_doctype);
        addConstant<uint32_t>(*this, "parse_ws_pcdata_single",  pugi::parse_ws_pcdata_single);
        addConstant<uint32_t>(*this, "parse_trim_pcdata",       pugi::parse_trim_pcdata);
        addConstant<uint32_t>(*this, "parse_fragment",          pugi::parse_fragment);
        addConstant<uint32_t>(*this, "parse_embed_pcdata",      pugi::parse_embed_pcdata);
        addConstant<uint32_t>(*this, "parse_merge_pcdata",      pugi::parse_merge_pcdata);
        addConstant<uint32_t>(*this, "parse_default",           pugi::parse_default);
        addConstant<uint32_t>(*this, "parse_full",              pugi::parse_full);
        // format flag constants
        addConstant<uint32_t>(*this, "format_indent",                  pugi::format_indent);
        addConstant<uint32_t>(*this, "format_write_bom",               pugi::format_write_bom);
        addConstant<uint32_t>(*this, "format_raw",                     pugi::format_raw);
        addConstant<uint32_t>(*this, "format_no_declaration",          pugi::format_no_declaration);
        addConstant<uint32_t>(*this, "format_no_escapes",              pugi::format_no_escapes);
        addConstant<uint32_t>(*this, "format_save_file_text",          pugi::format_save_file_text);
        addConstant<uint32_t>(*this, "format_indent_attributes",       pugi::format_indent_attributes);
        addConstant<uint32_t>(*this, "format_no_empty_element_tags",   pugi::format_no_empty_element_tags);
        addConstant<uint32_t>(*this, "format_skip_control_chars",      pugi::format_skip_control_chars);
        addConstant<uint32_t>(*this, "format_attribute_single_quote",  pugi::format_attribute_single_quote);
        addConstant<uint32_t>(*this, "format_default",                 pugi::format_default);

        // ---- document ----
        addExtern<DAS_BIND_FUN(pugiDocumentAsNode),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, "document_as_node",
            SideEffects::none, "pugiDocumentAsNode")
                ->args({"document"});
        addExtern<DAS_BIND_FUN(pugiLoadDocumentFromFile)> (*this, lib, "load_document",
            SideEffects::modifyArgumentAndAccessExternal, "pugiLoadDocumentFromFile")
                ->args({"doc","filename","result"});
        addExtern<DAS_BIND_FUN(pugiLoadDocumentFromString)> (*this, lib, "load_string",
            SideEffects::modifyArgument, "pugiLoadDocumentFromString")
                ->args({"doc","content","result","options"});
        addExtern<DAS_BIND_FUN(pugiDocumentElement),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, ".`document_element",
            SideEffects::none, "pugiDocumentElement")
                ->args({"document"})->setCustomProperty();
        addExtern<DAS_BIND_FUN(pugiSaveDocumentToFile)> (*this, lib, "save_file",
            SideEffects::modifyExternal, "pugiSaveDocumentToFile")
                ->args({"document","filename","indent","flags","encoding"});
        addExtern<DAS_BIND_FUN(pugiDocumentSaveToString)> (*this, lib, "save_string",
            SideEffects::none, "pugiDocumentSaveToString")
                ->args({"document","indent","flags","encoding","context","at"});
        addExtern<DAS_BIND_FUN(pugiDocumentReset)> (*this, lib, "reset",
            SideEffects::modifyArgument, "pugiDocumentReset")
                ->args({"document"});

        // ---- node navigation ----
        addExtern<DAS_BIND_FUN(pugiNodeOk)> (*this, lib, ".`ok",
            SideEffects::none, "pugiNodeOk")
                ->args({"node"});
        addExtern<DAS_BIND_FUN(pugiNodeChild),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, "child",
            SideEffects::none, "pugiNodeChild")
                ->args({"node","name"});
        addExtern<DAS_BIND_FUN(pugiNodeAttribute),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, "attribute",
            SideEffects::none, "pugiNodeAttribute")
                ->args({"node","name"});
        addExtern<DAS_BIND_FUN(pugiNodeFirstChild),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, ".`first_child",
            SideEffects::none, "pugiNodeFirstChild")
                ->args({"node"})->setCustomProperty();
        addExtern<DAS_BIND_FUN(pugiNodeLastChild),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, ".`last_child",
            SideEffects::none, "pugiNodeLastChild")
                ->args({"node"})->setCustomProperty();
        addExtern<DAS_BIND_FUN(pugiNodeNextSibling),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, ".`next_sibling",
            SideEffects::none, "pugiNodeNextSibling")
                ->args({"node"})->setCustomProperty();
        addExtern<DAS_BIND_FUN(pugiNodeNextSiblingByName),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, "next_sibling",
            SideEffects::none, "pugiNodeNextSiblingByName")
                ->args({"node","name"});
        addExtern<DAS_BIND_FUN(pugiNodePreviousSibling),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, ".`previous_sibling",
            SideEffects::none, "pugiNodePreviousSibling")
                ->args({"node"})->setCustomProperty();
        addExtern<DAS_BIND_FUN(pugiNodePreviousSiblingByName),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, "previous_sibling",
            SideEffects::none, "pugiNodePreviousSiblingByName")
                ->args({"node","name"});
        addExtern<DAS_BIND_FUN(pugiNodeParent),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, ".`parent",
            SideEffects::none, "pugiNodeParent")
                ->args({"node"})->setCustomProperty();
        addExtern<DAS_BIND_FUN(pugiNodeRoot),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, ".`root",
            SideEffects::none, "pugiNodeRoot")
                ->args({"node"})->setCustomProperty();
        addExtern<DAS_BIND_FUN(pugiNodeFirstAttribute),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, ".`first_attribute",
            SideEffects::none, "pugiNodeFirstAttribute")
                ->args({"node"})->setCustomProperty();
        addExtern<DAS_BIND_FUN(pugiNodeLastAttribute),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, ".`last_attribute",
            SideEffects::none, "pugiNodeLastAttribute")
                ->args({"node"})->setCustomProperty();
        addExtern<DAS_BIND_FUN(pugiNodeText),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, ".`text",
            SideEffects::none, "pugiNodeText")
                ->args({"node"})->setCustomProperty();
        addExtern<DAS_BIND_FUN(pugiNodeChildValue)> (*this, lib, ".`child_value",
            SideEffects::none, "pugiNodeChildValue")
                ->args({"node"});
        addExtern<DAS_BIND_FUN(pugiNodeChildValueByName)> (*this, lib, "child_value",
            SideEffects::none, "pugiNodeChildValueByName")
                ->args({"node","name"});
        addExtern<DAS_BIND_FUN(pugiNodeFindChildByAttribute),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, "find_child_by_attribute",
            SideEffects::none, "pugiNodeFindChildByAttribute")
                ->args({"node","element_name","attr_name","attr_value"});
        addExtern<DAS_BIND_FUN(pugiNodeFindChildByAttribute2),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, "find_child_by_attribute",
            SideEffects::none, "pugiNodeFindChildByAttribute2")
                ->args({"node","attr_name","attr_value"});
        addExtern<DAS_BIND_FUN(pugiNodeFirstElementByPath),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, "first_element_by_path",
            SideEffects::none, "pugiNodeFirstElementByPath")
                ->args({"node","path"});
        addExtern<DAS_BIND_FUN(pugiNodeOffsetDebug)> (*this, lib, ".`offset_debug",
            SideEffects::none, "pugiNodeOffsetDebug")
                ->args({"node"});
        addExtern<DAS_BIND_FUN(pugiNodeHashValue)> (*this, lib, ".`hash_value",
            SideEffects::none, "pugiNodeHashValue")
                ->args({"node"});
        addExtern<DAS_BIND_FUN(pugiNodePath)> (*this, lib, "path",
            SideEffects::none, "pugiNodePath")
                ->args({"node","delimiter","context","at"});
        addExtern<DAS_BIND_FUN(pugiNodePrint)> (*this, lib, "print_to_string",
            SideEffects::none, "pugiNodePrint")
                ->args({"node","indent","flags","encoding","context","at"});
        addExtern<DAS_BIND_FUN(pugiNodeEqu)> (*this, lib, "==",
            SideEffects::none, "pugiNodeEqu")
                ->args({"node_a","node_b"});
        addExtern<DAS_BIND_FUN(pugiNodeNotEqu)> (*this, lib, "!=",
            SideEffects::none, "pugiNodeNotEqu")
                ->args({"node_a","node_b"});

        // ---- node mutation ----
        addExtern<DAS_BIND_FUN(pugiNodeSetName)> (*this, lib, "set_name",
            SideEffects::modifyArgument, "pugiNodeSetName")
                ->args({"node","name"});
        addExtern<DAS_BIND_FUN(pugiNodeSetValue)> (*this, lib, "set_value",
            SideEffects::modifyArgument, "pugiNodeSetValue")
                ->args({"node","value"});
        addExtern<DAS_BIND_FUN(pugiNodeAppendChild),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, "append_child",
            SideEffects::modifyArgument, "pugiNodeAppendChild")
                ->args({"node","name"});
        addExtern<DAS_BIND_FUN(pugiNodeAppendChildByType),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, "append_child",
            SideEffects::modifyArgument, "pugiNodeAppendChildByType")
                ->args({"node","type"});
        addExtern<DAS_BIND_FUN(pugiNodePrependChild),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, "prepend_child",
            SideEffects::modifyArgument, "pugiNodePrependChild")
                ->args({"node","name"});
        addExtern<DAS_BIND_FUN(pugiNodePrependChildByType),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, "prepend_child",
            SideEffects::modifyArgument, "pugiNodePrependChildByType")
                ->args({"node","type"});
        addExtern<DAS_BIND_FUN(pugiNodeInsertChildAfter),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, "insert_child_after",
            SideEffects::modifyArgument, "pugiNodeInsertChildAfter")
                ->args({"node","name","after"});
        addExtern<DAS_BIND_FUN(pugiNodeInsertChildBefore),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, "insert_child_before",
            SideEffects::modifyArgument, "pugiNodeInsertChildBefore")
                ->args({"node","name","before"});
        addExtern<DAS_BIND_FUN(pugiNodeRemoveChild)> (*this, lib, "remove_child",
            SideEffects::modifyArgument, "pugiNodeRemoveChild")
                ->args({"node","child"});
        addExtern<DAS_BIND_FUN(pugiNodeRemoveChildByName)> (*this, lib, "remove_child",
            SideEffects::modifyArgument, "pugiNodeRemoveChildByName")
                ->args({"node","name"});
        addExtern<DAS_BIND_FUN(pugiNodeRemoveChildren)> (*this, lib, "remove_children",
            SideEffects::modifyArgument, "pugiNodeRemoveChildren")
                ->args({"node"});
        addExtern<DAS_BIND_FUN(pugiNodeAppendAttribute),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, "append_attribute",
            SideEffects::modifyArgument, "pugiNodeAppendAttribute")
                ->args({"node","name"});
        addExtern<DAS_BIND_FUN(pugiNodePrependAttribute),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, "prepend_attribute",
            SideEffects::modifyArgument, "pugiNodePrependAttribute")
                ->args({"node","name"});
        addExtern<DAS_BIND_FUN(pugiNodeInsertAttributeAfter),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, "insert_attribute_after",
            SideEffects::modifyArgument, "pugiNodeInsertAttributeAfter")
                ->args({"node","name","after"});
        addExtern<DAS_BIND_FUN(pugiNodeInsertAttributeBefore),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, "insert_attribute_before",
            SideEffects::modifyArgument, "pugiNodeInsertAttributeBefore")
                ->args({"node","name","before"});
        addExtern<DAS_BIND_FUN(pugiNodeRemoveAttribute)> (*this, lib, "remove_attribute",
            SideEffects::modifyArgument, "pugiNodeRemoveAttribute")
                ->args({"node","attribute"});
        addExtern<DAS_BIND_FUN(pugiNodeRemoveAttributeByName)> (*this, lib, "remove_attribute",
            SideEffects::modifyArgument, "pugiNodeRemoveAttributeByName")
                ->args({"node","name"});
        addExtern<DAS_BIND_FUN(pugiNodeRemoveAttributes)> (*this, lib, "remove_attributes",
            SideEffects::modifyArgument, "pugiNodeRemoveAttributes")
                ->args({"node"});
        addExtern<DAS_BIND_FUN(pugiNodeAppendCopyNode),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, "append_copy",
            SideEffects::modifyArgument, "pugiNodeAppendCopyNode")
                ->args({"node","proto"});
        addExtern<DAS_BIND_FUN(pugiNodePrependCopyNode),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, "prepend_copy",
            SideEffects::modifyArgument, "pugiNodePrependCopyNode")
                ->args({"node","proto"});
        addExtern<DAS_BIND_FUN(pugiNodeAppendCopyAttr),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, "append_copy",
            SideEffects::modifyArgument, "pugiNodeAppendCopyAttr")
                ->args({"node","proto"});
        addExtern<DAS_BIND_FUN(pugiNodePrependCopyAttr),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, "prepend_copy",
            SideEffects::modifyArgument, "pugiNodePrependCopyAttr")
                ->args({"node","proto"});
        addExtern<DAS_BIND_FUN(pugiNodeAppendMoveNode),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, "append_move",
            SideEffects::modifyArgument, "pugiNodeAppendMoveNode")
                ->args({"node","moved"});
        addExtern<DAS_BIND_FUN(pugiNodePrependMoveNode),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, "prepend_move",
            SideEffects::modifyArgument, "pugiNodePrependMoveNode")
                ->args({"node","moved"});

        // ---- attributes ----
        addExtern<DAS_BIND_FUN(pugiAttributeOk)> (*this, lib, ".`ok",
            SideEffects::none, "pugiAttributeOk")
                ->args({"attribute"});
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
        addExtern<DAS_BIND_FUN(pugiAttributeNextAttribute),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, ".`next_attribute",
            SideEffects::none, "pugiAttributeNextAttribute")
                ->args({"attribute"})->setCustomProperty();
        addExtern<DAS_BIND_FUN(pugiAttributePreviousAttribute),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, ".`previous_attribute",
            SideEffects::none, "pugiAttributePreviousAttribute")
                ->args({"attribute"})->setCustomProperty();
        addExtern<DAS_BIND_FUN(pugiAttributeHashValue)> (*this, lib, ".`hash_value",
            SideEffects::none, "pugiAttributeHashValue")
                ->args({"attribute"});
        addExtern<DAS_BIND_FUN(pugiAttributeSetName)> (*this, lib, "set_name",
            SideEffects::modifyArgument, "pugiAttributeSetName")
                ->args({"attribute","name"});
        addExtern<DAS_BIND_FUN(pugiAttributeSetValueString)> (*this, lib, "set_value",
            SideEffects::modifyArgument, "pugiAttributeSetValueString")
                ->args({"attribute","value"});
        addExtern<DAS_BIND_FUN(pugiAttributeSetValueInt)> (*this, lib, "set_value",
            SideEffects::modifyArgument, "pugiAttributeSetValueInt")
                ->args({"attribute","value"});
        addExtern<DAS_BIND_FUN(pugiAttributeSetValueUint)> (*this, lib, "set_value",
            SideEffects::modifyArgument, "pugiAttributeSetValueUint")
                ->args({"attribute","value"});
        addExtern<DAS_BIND_FUN(pugiAttributeSetValueDouble)> (*this, lib, "set_value",
            SideEffects::modifyArgument, "pugiAttributeSetValueDouble")
                ->args({"attribute","value"});
        addExtern<DAS_BIND_FUN(pugiAttributeSetValueFloat)> (*this, lib, "set_value",
            SideEffects::modifyArgument, "pugiAttributeSetValueFloat")
                ->args({"attribute","value"});
        addExtern<DAS_BIND_FUN(pugiAttributeSetValueBool)> (*this, lib, "set_value",
            SideEffects::modifyArgument, "pugiAttributeSetValueBool")
                ->args({"attribute","value"});
        addExtern<DAS_BIND_FUN(pugiAttributeEqu)> (*this, lib, "==",
            SideEffects::none, "pugiAttributeEqu")
                ->args({"attr_a","attr_b"});
        addExtern<DAS_BIND_FUN(pugiAttributeNotEqu)> (*this, lib, "!=",
            SideEffects::none, "pugiAttributeNotEqu")
                ->args({"attr_a","attr_b"});

        // ---- text ----
        addExtern<DAS_BIND_FUN(pugiTextOk)> (*this, lib, ".`ok",
            SideEffects::none, "pugiTextOk")
                ->args({"text"});
        addExtern<DAS_BIND_FUN(pugiText_as_string)> (*this, lib, "as_string",
            SideEffects::none, "pugiText_as_string")
                ->args({"text","default_value"});
        addExtern<DAS_BIND_FUN(pugiText_as_int)> (*this, lib, "as_int",
            SideEffects::none, "pugiText_as_int")
                ->args({"text","default_value"});
        addExtern<DAS_BIND_FUN(pugiText_as_uint)> (*this, lib, "as_uint",
            SideEffects::none, "pugiText_as_uint")
                ->args({"text","default_value"});
        addExtern<DAS_BIND_FUN(pugiText_as_double)> (*this, lib, "as_double",
            SideEffects::none, "pugiText_as_double")
                ->args({"text","default_value"});
        addExtern<DAS_BIND_FUN(pugiText_as_float)> (*this, lib, "as_float",
            SideEffects::none, "pugiText_as_float")
                ->args({"text","default_value"});
        addExtern<DAS_BIND_FUN(pugiText_as_bool)> (*this, lib, "as_bool",
            SideEffects::none, "pugiText_as_bool")
                ->args({"text","default_value"});
        addExtern<DAS_BIND_FUN(pugiTextSetString)> (*this, lib, "set",
            SideEffects::modifyArgumentAndExternal, "pugiTextSetString")
                ->args({"text","value"});
        addExtern<DAS_BIND_FUN(pugiTextSetInt)> (*this, lib, "set",
            SideEffects::modifyArgumentAndExternal, "pugiTextSetInt")
                ->args({"text","value"});
        addExtern<DAS_BIND_FUN(pugiTextSetUint)> (*this, lib, "set",
            SideEffects::modifyArgumentAndExternal, "pugiTextSetUint")
                ->args({"text","value"});
        addExtern<DAS_BIND_FUN(pugiTextSetDouble)> (*this, lib, "set",
            SideEffects::modifyArgumentAndExternal, "pugiTextSetDouble")
                ->args({"text","value"});
        addExtern<DAS_BIND_FUN(pugiTextSetFloat)> (*this, lib, "set",
            SideEffects::modifyArgumentAndExternal, "pugiTextSetFloat")
                ->args({"text","value"});
        addExtern<DAS_BIND_FUN(pugiTextSetBool)> (*this, lib, "set",
            SideEffects::modifyArgumentAndExternal, "pugiTextSetBool")
                ->args({"text","value"});
        addExtern<DAS_BIND_FUN(pugiTextSetInt8)> (*this, lib, "set",
            SideEffects::modifyArgumentAndExternal, "pugiTextSetInt8")
                ->args({"text","value"});
        addExtern<DAS_BIND_FUN(pugiTextSetUint8)> (*this, lib, "set",
            SideEffects::modifyArgumentAndExternal, "pugiTextSetUint8")
                ->args({"text","value"});
        addExtern<DAS_BIND_FUN(pugiTextSetInt16)> (*this, lib, "set",
            SideEffects::modifyArgumentAndExternal, "pugiTextSetInt16")
                ->args({"text","value"});
        addExtern<DAS_BIND_FUN(pugiTextSetUint16)> (*this, lib, "set",
            SideEffects::modifyArgumentAndExternal, "pugiTextSetUint16")
                ->args({"text","value"});
        addExtern<DAS_BIND_FUN(pugiTextSetInt64)> (*this, lib, "set",
            SideEffects::modifyArgumentAndExternal, "pugiTextSetInt64")
                ->args({"text","value"});
        addExtern<DAS_BIND_FUN(pugiTextSetUint64)> (*this, lib, "set",
            SideEffects::modifyArgumentAndExternal, "pugiTextSetUint64")
                ->args({"text","value"});
        addExtern<DAS_BIND_FUN(pugiText_as_int64)> (*this, lib, "as_int64",
            SideEffects::none, "pugiText_as_int64")
                ->args({"text","default_value"});
        addExtern<DAS_BIND_FUN(pugiText_as_uint64)> (*this, lib, "as_uint64",
            SideEffects::none, "pugiText_as_uint64")
                ->args({"text","default_value"});
        addExtern<DAS_BIND_FUN(pugiTextData),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, ".`data",
            SideEffects::none, "pugiTextData")
                ->args({"text"});

        // ---- xpath ----
        addExtern<DAS_BIND_FUN(pugiXPathCompile)> (*this, lib, "xpath_compile",
            SideEffects::none, "pugiXPathCompile")
                ->args({"query"});
        addExtern<DAS_BIND_FUN(pugiXPathCompileWithVars)> (*this, lib, "xpath_compile",
            SideEffects::none, "pugiXPathCompileWithVars")
                ->args({"query","variables"});
        addExtern<DAS_BIND_FUN(pugiXPathQueryOk)> (*this, lib, ".`ok",
            SideEffects::none, "pugiXPathQueryOk")
                ->args({"query"});
        addExtern<DAS_BIND_FUN(pugiXPathQueryReturnType)> (*this, lib, ".`return_type",
            SideEffects::none, "pugiXPathQueryReturnType")
                ->args({"query"});
        addExtern<DAS_BIND_FUN(pugiXPathQueryResultDescription)> (*this, lib, ".`result_description",
            SideEffects::none, "pugiXPathQueryResultDescription")
                ->args({"query"});
        addExtern<DAS_BIND_FUN(pugiXPathQueryResultOffset)> (*this, lib, ".`result_offset",
            SideEffects::none, "pugiXPathQueryResultOffset")
                ->args({"query"});
        addExtern<DAS_BIND_FUN(pugiXPathEvaluateBoolean)> (*this, lib, "evaluate_boolean",
            SideEffects::none, "pugiXPathEvaluateBoolean")
                ->args({"query","node"});
        addExtern<DAS_BIND_FUN(pugiXPathEvaluateNumber)> (*this, lib, "evaluate_number",
            SideEffects::none, "pugiXPathEvaluateNumber")
                ->args({"query","node"});
        addExtern<DAS_BIND_FUN(pugiXPathEvaluateString)> (*this, lib, "evaluate_string",
            SideEffects::none, "pugiXPathEvaluateString")
                ->args({"query","node","context","at"});
        addExtern<DAS_BIND_FUN(pugiXPathEvaluateNodeSet)> (*this, lib, "evaluate_node_set",
            SideEffects::none, "pugiXPathEvaluateNodeSet")
                ->args({"query","node"});
        addExtern<DAS_BIND_FUN(pugiXPathEvaluateNode),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, "evaluate_node",
            SideEffects::none, "pugiXPathEvaluateNode")
                ->args({"query","node"});
        addExtern<DAS_BIND_FUN(pugiNodeSelectNode),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, "select_node",
            SideEffects::none, "pugiNodeSelectNode")
                ->args({"node","query"});
        addExtern<DAS_BIND_FUN(pugiNodeSelectNodeQuery),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, "select_node",
            SideEffects::none, "pugiNodeSelectNodeQuery")
                ->args({"node","query"});
        addExtern<DAS_BIND_FUN(pugiNodeSelectNodes)> (*this, lib, "select_nodes",
            SideEffects::none, "pugiNodeSelectNodes")
                ->args({"node","query"});
        addExtern<DAS_BIND_FUN(pugiNodeSelectNodesQuery)> (*this, lib, "select_nodes",
            SideEffects::none, "pugiNodeSelectNodesQuery")
                ->args({"node","query"});
        // xpath_node accessors
        addExtern<DAS_BIND_FUN(pugiXPathNodeNode),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, ".`node",
            SideEffects::none, "pugiXPathNodeNode")
                ->args({"xpath_node"})->setCustomProperty();
        addExtern<DAS_BIND_FUN(pugiXPathNodeAttribute),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, ".`attribute",
            SideEffects::none, "pugiXPathNodeAttribute")
                ->args({"xpath_node"})->setCustomProperty();
        addExtern<DAS_BIND_FUN(pugiXPathNodeParent),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, ".`parent",
            SideEffects::none, "pugiXPathNodeParent")
                ->args({"xpath_node"})->setCustomProperty();
        addExtern<DAS_BIND_FUN(pugiXPathNodeOk)> (*this, lib, ".`ok",
            SideEffects::none, "pugiXPathNodeOk")
                ->args({"xpath_node"});
        // xpath_node_set accessors
        addExtern<DAS_BIND_FUN(pugiXPathNodeSetSize)> (*this, lib, ".`size",
            SideEffects::none, "pugiXPathNodeSetSize")
                ->args({"set"});
        addExtern<DAS_BIND_FUN(pugiXPathNodeSetEmpty)> (*this, lib, ".`empty",
            SideEffects::none, "pugiXPathNodeSetEmpty")
                ->args({"set"});
        addExtern<DAS_BIND_FUN(pugiXPathNodeSetFirst),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, ".`first",
            SideEffects::none, "pugiXPathNodeSetFirst")
                ->args({"set"});
        addExtern<DAS_BIND_FUN(pugiXPathNodeSetAt),SimNode_ExtFuncCallAndCopyOrMove> (*this, lib, "at",
            SideEffects::none, "pugiXPathNodeSetAt")
                ->args({"set","index"});
        addExtern<DAS_BIND_FUN(pugiXPathNodeSetSort)> (*this, lib, "sort",
            SideEffects::modifyArgument, "pugiXPathNodeSetSort")
                ->args({"set","reverse"});
        // xpath_variable_set
        addExtern<DAS_BIND_FUN(pugiXPathVarSetSetBool)> (*this, lib, "set",
            SideEffects::modifyArgument, "pugiXPathVarSetSetBool")
                ->args({"variables","name","value"});
        addExtern<DAS_BIND_FUN(pugiXPathVarSetSetNumber)> (*this, lib, "set",
            SideEffects::modifyArgument, "pugiXPathVarSetSetNumber")
                ->args({"variables","name","value"});
        addExtern<DAS_BIND_FUN(pugiXPathVarSetSetString)> (*this, lib, "set",
            SideEffects::modifyArgument, "pugiXPathVarSetSetString")
                ->args({"variables","name","value"});
    }
    virtual ModuleAotType aotRequire ( TextWriter & tw ) const override {
        tw << "#include \"../modules/dasPUGIXML/src/dasPUGIXML.h\"\n";
        return ModuleAotType::cpp;
    }
};

REGISTER_DYN_MODULE(Module_PUGIXML,Module_PUGIXML);

}

REGISTER_MODULE_IN_NAMESPACE(Module_PUGIXML,das);
