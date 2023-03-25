#pragma once

#include <pugixml.hpp>

namespace das {
    // document
    bool pugiLoadDocumentFromFile ( pugi::xml_document & doc, char * filename, pugi::xml_parse_result & status );
    pugi::xml_node pugiDocumentElement ( const pugi::xml_document & doc );
    // node
    bool pugiNodeOk ( const pugi::xml_node & doc );
    pugi::xml_node pugiNodeChild ( const pugi::xml_node & doc, char * name );
    pugi::xml_attribute pugiNodeAttribute ( const pugi::xml_node & doc, char * name );
    pugi::xml_node pugiNodeNextSibling ( const pugi::xml_node & doc );
    pugi::xml_node pugiNodeFirstChild ( const pugi::xml_node & doc );
    pugi::xml_node pugiNodeParent ( const pugi::xml_node & doc );
    bool pugiNodeEqu ( const pugi::xml_node & doc, const pugi::xml_node & other );
    bool pugiNodeNotEqu ( const pugi::xml_node & doc, const pugi::xml_node & other );
    // attributes
    char * pugiAttribute_as_string ( const pugi::xml_attribute & attr, char * def );
    int32_t pugiAttribute_as_int ( const pugi::xml_attribute & attr, int32_t def );
    uint32_t pugiAttribute_as_uint ( const pugi::xml_attribute & attr, uint32_t def );
    double pugiAttribute_as_double ( const pugi::xml_attribute & attr, double def );
    float pugiAttribute_as_float ( const pugi::xml_attribute & attr, float def );
    bool pugiAttribute_as_bool ( const pugi::xml_attribute & attr, bool def );
}
