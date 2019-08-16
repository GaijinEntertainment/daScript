#pragma once

#include "daScript/simulate/simulate.h"

namespace das {

    struct SimNodeInfo {
        string  name;
        string  typeName;
        size_t  typeSize;
    };

    typedef map<SimNode *,SimNodeInfo> SimNodeInfoLookup;

    struct FusionPoint {
        FusionPoint () {}
        virtual ~FusionPoint() {}
        virtual SimNode * fuse ( const SimNodeInfoLookup & info, SimNode * node, Context * ) {
            return node;
        }
        static bool is ( const SimNodeInfoLookup & info, SimNode * node, const char * name ) {
            auto it = info.find(node);
            if ( it==info.end() ) return false;
            return it->second.name == name;
        }
        static bool is ( const SimNodeInfoLookup & info, SimNode * node, const char * name, const char * typeName ) {
            auto it = info.find(node);
            if ( it==info.end() ) return false;
            return (it->second.name == name) && (it->second.typeName==typeName);
        }
    };
    typedef shared_ptr<FusionPoint> FusionPointPtr;

    typedef map<string,vector<FusionPointPtr>> FusionEngine;
    extern unique_ptr<FusionEngine> g_fusionEngine;

    string fuseName ( const string & name, const string & typeName );
    void resetFusionEngine();
    void createFusionEngine();

    // fusion engine subsections
    // scalar
    void createFusionEngine_op2();
    void createFusionEngine_op2_set();
    void createFusionEngine_op2_bool();
    void createFusionEngine_op2_bin();
    // vector
    void createFusionEngine_op2_vec();
    void createFusionEngine_op2_set_vec();
    void createFusionEngine_op2_bool_vec();
}
