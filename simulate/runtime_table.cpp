#include "precomp.h"

#include "runtime_table.h"

namespace yzg
{
    template <typename KeyType, typename ValueType>
    void Insert ( Table & tab, RobinHoodHash<KeyType> & rhh, const KeyType & key, const ValueType & value ) {
        ValueType sv = value;
        rhh.insert(tab, key, (void *)&sv);
    }
    
    template <typename KeyType, typename ValueType>
    pair<ValueType,bool> Find ( Table & tab, RobinHoodHash<KeyType> & rhh, const KeyType & key ) {
        auto at = rhh.find(tab, key);
        if ( at.second ) {
            ValueType * values = (ValueType *) tab.data;
            return { values[at.first], true };
        } else {
            return { ValueType(), false };
        }
    }
    
    void test_robin_hood() {
        Context ctx(nullptr);
        Table tab;
        memset ( &tab, 0, sizeof(Table) );
        RobinHoodHash<char *> rhh(&ctx, sizeof(int));
        
        constexpr int total = 10000;
        
        string keys[total];
        for ( int i=0; i!=total; ++i ) {
            stringstream ss;
            ss << i;
            keys[i] = ss.str();
            Insert<char *,int>(tab, rhh, (char *) keys[i].c_str(), i);
        }
        assert(tab.size==total);
        
        for ( int i=0; i!=total; ++i ) {
            Insert<char *,int>(tab, rhh, (char *) keys[i].c_str(), i);
        }
        assert(tab.size==total);
        
        for ( int i=0; i!=total; ++i ) {
            auto it = Find<char *,int>(tab, rhh, (char *) keys[i].c_str() );
            assert(it.second);
            assert(it.first == i);
        }
        
        int del = 0;
        for ( int i=0; i<total; i+= 7 ) {
            rhh.erase(tab, (char *) keys[i].c_str());
            del ++;
        }
        assert(tab.size==total-del);
        
        for ( int i=0; i<total; ++i ) {
            auto it = Find<char *,int>(tab, rhh, (char *) keys[i].c_str() );
            if ( i%7 == 0 ) {
                assert(!it.second);
            } else {
                assert(it.second);
                assert(it.first == i);
            }
        }
        
        for ( int i=0; i<total; i+=7 ) {
            Insert<char *,int>(tab, rhh, (char *) keys[i].c_str(), i);
        }
        
        for ( int i=0; i!=total; ++i ) {
            auto it = Find<char *,int>(tab, rhh, (char *) keys[i].c_str() );
            assert(it.second);
            assert(it.first == i);
        }
    }
}

