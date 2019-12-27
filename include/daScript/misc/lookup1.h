#pragma once

namespace das {
    template <typename TT>
    vector<TT> buildLookup ( const das_hash_map<uint32_t,TT> & a2b, uint32_t & tab_mask, uint32_t & tab_rot ) {
        vector<TT> tab;
        // now, lets build this table
        for ( size_t ts = 1; ; ts *= 2 ) {
            if ( ts<a2b.size() ) {
                continue;
            }
            tab_mask = uint32_t(ts - 1);
            tab.resize(ts);
            for ( tab_rot=0; tab_rot!=32; ++tab_rot ) {
                fill(tab.begin(), tab.end(), 0);
                bool failed = false;
                for ( auto val = a2b.begin(); val!=a2b.end(); ++val ) {
                    uint32_t mnh = val->first;
                    TT       fni = val->second;
                    uint32_t idx = rotl_c(mnh,tab_rot) & tab_mask;
                    if ( tab[idx]==0 ) {
                        tab[idx] = fni;
                    } else {
                        failed = true;
                        break;
                    }
                }
                if ( !failed ) {
                    return tab;;
                }
            }
        }
        return tab;
    }
}
