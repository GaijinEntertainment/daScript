#pragma once

#include <pugixml.hpp>

#include "daScript/simulate/simulate.h"

namespace das {

    // document
    pugi::xml_node pugiDocumentAsNode ( pugi::xml_document * doc );
    bool pugiLoadDocumentFromFile ( pugi::xml_document * doc, char * filename, pugi::xml_parse_result & status );
    bool pugiLoadDocumentFromString ( pugi::xml_document * doc, char * content, pugi::xml_parse_result & status, uint32_t options );
    pugi::xml_node pugiDocumentElement ( const pugi::xml_document * doc );
    bool pugiSaveDocumentToFile ( const pugi::xml_document * doc, char * filename, char * indent, uint32_t flags, pugi::xml_encoding encoding );
    char * pugiDocumentSaveToString ( const pugi::xml_document * doc, char * indent, uint32_t flags, pugi::xml_encoding encoding, Context * context, LineInfoArg * at );
    void pugiDocumentReset ( pugi::xml_document * doc );

    // node navigation
    bool pugiNodeOk ( const pugi::xml_node & node );
    pugi::xml_node pugiNodeChild ( const pugi::xml_node & node, char * name );
    pugi::xml_attribute pugiNodeAttribute ( const pugi::xml_node & node, char * name );
    pugi::xml_node pugiNodeFirstChild ( const pugi::xml_node & node );
    pugi::xml_node pugiNodeLastChild ( const pugi::xml_node & node );
    pugi::xml_node pugiNodeNextSibling ( const pugi::xml_node & node );
    pugi::xml_node pugiNodeNextSiblingByName ( const pugi::xml_node & node, char * name );
    pugi::xml_node pugiNodePreviousSibling ( const pugi::xml_node & node );
    pugi::xml_node pugiNodePreviousSiblingByName ( const pugi::xml_node & node, char * name );
    pugi::xml_node pugiNodeParent ( const pugi::xml_node & node );
    pugi::xml_node pugiNodeRoot ( const pugi::xml_node & node );
    pugi::xml_attribute pugiNodeFirstAttribute ( const pugi::xml_node & node );
    pugi::xml_attribute pugiNodeLastAttribute ( const pugi::xml_node & node );
    pugi::xml_text pugiNodeText ( const pugi::xml_node & node );
    char * pugiNodeChildValue ( const pugi::xml_node & node );
    char * pugiNodeChildValueByName ( const pugi::xml_node & node, char * name );
    pugi::xml_node pugiNodeFindChildByAttribute ( const pugi::xml_node & node, char * element_name, char * attr_name, char * attr_value );
    pugi::xml_node pugiNodeFindChildByAttribute2 ( const pugi::xml_node & node, char * attr_name, char * attr_value );
    pugi::xml_node pugiNodeFirstElementByPath ( const pugi::xml_node & node, char * path );
    int32_t pugiNodeOffsetDebug ( const pugi::xml_node & node );
    uint64_t pugiNodeHashValue ( const pugi::xml_node & node );
    char * pugiNodePath ( const pugi::xml_node & node, char * delimiter, Context * context, LineInfoArg * at );
    char * pugiNodePrint ( const pugi::xml_node & node, char * indent, uint32_t flags, pugi::xml_encoding encoding, Context * context, LineInfoArg * at );
    bool pugiNodeEqu ( const pugi::xml_node & a, const pugi::xml_node & b );
    bool pugiNodeNotEqu ( const pugi::xml_node & a, const pugi::xml_node & b );

    // node mutation
    bool pugiNodeSetName ( pugi::xml_node & node, char * name );
    bool pugiNodeSetValue ( pugi::xml_node & node, char * value );
    pugi::xml_node pugiNodeAppendChild ( pugi::xml_node & node, char * name );
    pugi::xml_node pugiNodeAppendChildByType ( pugi::xml_node & node, pugi::xml_node_type type );
    pugi::xml_node pugiNodePrependChild ( pugi::xml_node & node, char * name );
    pugi::xml_node pugiNodePrependChildByType ( pugi::xml_node & node, pugi::xml_node_type type );
    pugi::xml_node pugiNodeInsertChildAfter ( pugi::xml_node & node, char * name, const pugi::xml_node & after );
    pugi::xml_node pugiNodeInsertChildBefore ( pugi::xml_node & node, char * name, const pugi::xml_node & before );
    bool pugiNodeRemoveChild ( pugi::xml_node & node, const pugi::xml_node & child );
    bool pugiNodeRemoveChildByName ( pugi::xml_node & node, char * name );
    bool pugiNodeRemoveChildren ( pugi::xml_node & node );
    pugi::xml_attribute pugiNodeAppendAttribute ( pugi::xml_node & node, char * name );
    pugi::xml_attribute pugiNodePrependAttribute ( pugi::xml_node & node, char * name );
    pugi::xml_attribute pugiNodeInsertAttributeAfter ( pugi::xml_node & node, char * name, const pugi::xml_attribute & after );
    pugi::xml_attribute pugiNodeInsertAttributeBefore ( pugi::xml_node & node, char * name, const pugi::xml_attribute & before );
    bool pugiNodeRemoveAttribute ( pugi::xml_node & node, const pugi::xml_attribute & attr );
    bool pugiNodeRemoveAttributeByName ( pugi::xml_node & node, char * name );
    bool pugiNodeRemoveAttributes ( pugi::xml_node & node );
    pugi::xml_node pugiNodeAppendCopyNode ( pugi::xml_node & node, const pugi::xml_node & proto );
    pugi::xml_node pugiNodePrependCopyNode ( pugi::xml_node & node, const pugi::xml_node & proto );
    pugi::xml_attribute pugiNodeAppendCopyAttr ( pugi::xml_node & node, const pugi::xml_attribute & proto );
    pugi::xml_attribute pugiNodePrependCopyAttr ( pugi::xml_node & node, const pugi::xml_attribute & proto );
    pugi::xml_node pugiNodeAppendMoveNode ( pugi::xml_node & node, const pugi::xml_node & moved );
    pugi::xml_node pugiNodePrependMoveNode ( pugi::xml_node & node, const pugi::xml_node & moved );

    // attributes
    bool pugiAttributeOk ( const pugi::xml_attribute & attr );
    char * pugiAttribute_as_string ( const pugi::xml_attribute & attr, char * def );
    int32_t pugiAttribute_as_int ( const pugi::xml_attribute & attr, int32_t def );
    uint32_t pugiAttribute_as_uint ( const pugi::xml_attribute & attr, uint32_t def );
    double pugiAttribute_as_double ( const pugi::xml_attribute & attr, double def );
    float pugiAttribute_as_float ( const pugi::xml_attribute & attr, float def );
    bool pugiAttribute_as_bool ( const pugi::xml_attribute & attr, bool def );
    pugi::xml_attribute pugiAttributeNextAttribute ( const pugi::xml_attribute & attr );
    pugi::xml_attribute pugiAttributePreviousAttribute ( const pugi::xml_attribute & attr );
    uint64_t pugiAttributeHashValue ( const pugi::xml_attribute & attr );
    bool pugiAttributeSetName ( pugi::xml_attribute & attr, char * name );
    bool pugiAttributeSetValueString ( pugi::xml_attribute & attr, char * value );
    bool pugiAttributeSetValueInt ( pugi::xml_attribute & attr, int32_t value );
    bool pugiAttributeSetValueUint ( pugi::xml_attribute & attr, uint32_t value );
    bool pugiAttributeSetValueDouble ( pugi::xml_attribute & attr, double value );
    bool pugiAttributeSetValueFloat ( pugi::xml_attribute & attr, float value );
    bool pugiAttributeSetValueBool ( pugi::xml_attribute & attr, bool value );
    bool pugiAttributeEqu ( const pugi::xml_attribute & a, const pugi::xml_attribute & b );
    bool pugiAttributeNotEqu ( const pugi::xml_attribute & a, const pugi::xml_attribute & b );

    // text
    bool pugiTextOk ( const pugi::xml_text & text );
    char * pugiText_as_string ( const pugi::xml_text & text, char * def );
    int32_t pugiText_as_int ( const pugi::xml_text & text, int32_t def );
    uint32_t pugiText_as_uint ( const pugi::xml_text & text, uint32_t def );
    double pugiText_as_double ( const pugi::xml_text & text, double def );
    float pugiText_as_float ( const pugi::xml_text & text, float def );
    bool pugiText_as_bool ( const pugi::xml_text & text, bool def );
    bool pugiTextSetString ( pugi::xml_text & text, char * value );
    bool pugiTextSetInt ( pugi::xml_text & text, int32_t value );
    bool pugiTextSetUint ( pugi::xml_text & text, uint32_t value );
    bool pugiTextSetDouble ( pugi::xml_text & text, double value );
    bool pugiTextSetFloat ( pugi::xml_text & text, float value );
    bool pugiTextSetBool ( pugi::xml_text & text, bool value );
    bool pugiTextSetInt8 ( pugi::xml_text & text, int8_t value );
    bool pugiTextSetUint8 ( pugi::xml_text & text, uint8_t value );
    bool pugiTextSetInt16 ( pugi::xml_text & text, int16_t value );
    bool pugiTextSetUint16 ( pugi::xml_text & text, uint16_t value );
    bool pugiTextSetInt64 ( pugi::xml_text & text, int64_t value );
    bool pugiTextSetUint64 ( pugi::xml_text & text, uint64_t value );
    int64_t pugiText_as_int64 ( const pugi::xml_text & text, int64_t def );
    uint64_t pugiText_as_uint64 ( const pugi::xml_text & text, uint64_t def );
    pugi::xml_node pugiTextData ( const pugi::xml_text & text );

    // xpath
    pugi::xpath_query * pugiXPathCompile ( char * query );
    pugi::xpath_query * pugiXPathCompileWithVars ( char * query, pugi::xpath_variable_set * vars );
    bool pugiXPathQueryOk ( const pugi::xpath_query * query );
    pugi::xpath_value_type pugiXPathQueryReturnType ( const pugi::xpath_query * query );
    char * pugiXPathQueryResultDescription ( const pugi::xpath_query * query );
    int32_t pugiXPathQueryResultOffset ( const pugi::xpath_query * query );
    bool pugiXPathEvaluateBoolean ( const pugi::xpath_query * query, const pugi::xml_node & node );
    double pugiXPathEvaluateNumber ( const pugi::xpath_query * query, const pugi::xml_node & node );
    char * pugiXPathEvaluateString ( const pugi::xpath_query * query, const pugi::xml_node & node, Context * context, LineInfoArg * at );
    pugi::xpath_node_set * pugiXPathEvaluateNodeSet ( const pugi::xpath_query * query, const pugi::xml_node & node );
    pugi::xpath_node pugiXPathEvaluateNode ( const pugi::xpath_query * query, const pugi::xml_node & node );
    pugi::xpath_node pugiNodeSelectNode ( const pugi::xml_node & node, char * query );
    pugi::xpath_node pugiNodeSelectNodeQuery ( const pugi::xml_node & node, const pugi::xpath_query * query );
    pugi::xpath_node_set * pugiNodeSelectNodes ( const pugi::xml_node & node, char * query );
    pugi::xpath_node_set * pugiNodeSelectNodesQuery ( const pugi::xml_node & node, const pugi::xpath_query * query );
    pugi::xml_node pugiXPathNodeNode ( const pugi::xpath_node & xnode );
    pugi::xml_attribute pugiXPathNodeAttribute ( const pugi::xpath_node & xnode );
    pugi::xml_node pugiXPathNodeParent ( const pugi::xpath_node & xnode );
    bool pugiXPathNodeOk ( const pugi::xpath_node & xnode );
    int32_t pugiXPathNodeSetSize ( const pugi::xpath_node_set * set );
    bool pugiXPathNodeSetEmpty ( const pugi::xpath_node_set * set );
    pugi::xpath_node pugiXPathNodeSetFirst ( const pugi::xpath_node_set * set );
    pugi::xpath_node pugiXPathNodeSetAt ( const pugi::xpath_node_set * set, int32_t index );
    void pugiXPathNodeSetSort ( pugi::xpath_node_set * set, bool reverse );
    bool pugiXPathVarSetSetBool ( pugi::xpath_variable_set * vars, char * name, bool value );
    bool pugiXPathVarSetSetNumber ( pugi::xpath_variable_set * vars, char * name, double value );
    bool pugiXPathVarSetSetString ( pugi::xpath_variable_set * vars, char * name, char * value );
}
