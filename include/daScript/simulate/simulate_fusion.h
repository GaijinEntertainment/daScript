#pragma once

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/simulate_nodes.h"

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
        virtual SimNode * fuse ( const SimNodeInfoLookup &, SimNode * node, Context * ) {
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

    const char * getSimSourceName(SimSourceType st);

    string fuseName ( const string & name, const string & typeName );
    void resetFusionEngine();
    void createFusionEngine();

    // fusion engine subsections
    // misc (note, misc before everything)
    void createFusionEngine_misc_copy_reference();
    // op1
    void createFusionEngine_op1();
    // scalar
    void createFusionEngine_op2();
    void createFusionEngine_op2_set();
    void createFusionEngine_op2_bool();
    void createFusionEngine_op2_bin();
    // vector
    void createFusionEngine_op2_vec();
    void createFusionEngine_op2_set_vec();
    void createFusionEngine_op2_bool_vec();
    void createFusionEngine_op2_bin_vec();
    void createFusionEngine_op2_scalar_vec();
    // at
    void createFusionEngine_at();
}
