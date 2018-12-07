#pragma once

#include "simulate.h"
#include "arraytype.h"
#include "hash.h"

namespace yzg
{
    template <typename KeyType>
    class RobinHoodHash
    {
        Context *   context = nullptr;
        uint32_t    valueTypeSize = 0;
        
        constexpr static uint32_t minCapacity = 64u;
        constexpr static uint32_t minLookups = 4u;
        
    public:
        RobinHoodHash () = delete;
        RobinHoodHash ( const RobinHoodHash & ) = delete;
        RobinHoodHash ( Context * ctx, uint32_t vs ) : context(ctx), valueTypeSize(vs) {}
        
        __forceinline void swap_value ( Table & tab, size_t index, void * b ) {
            char * a = tab.data + index * valueTypeSize;
            swap_ranges(a, a + valueTypeSize, (char *) b);
        }
        
        __forceinline void copy_value ( Table & tab, size_t index, void * b ) {
            memcpy ( tab.data + index*valueTypeSize, b, valueTypeSize );
        }
        
        __forceinline size_t indexForHash ( const Table & tab, size_t hash ) const {
            // tail end of the table is 'special' elements, no less than max lookup
            return hash % (tab.capacity - tab.maxLookups - 1);
        }
        
        __forceinline void computeMaxLookups(Table & tab) {
            uint32_t desired = 32 - __builtin_clz (tab.capacity-1);
            tab.maxLookups = std::max(minLookups, desired);
        }
        
        pair<size_t,bool> find ( const Table & tab, const KeyType & key ) const {
            if ( tab.capacity==0 ) {
                return { 0, false };
            }
            size_t index = indexForHash ( tab, hash_function(key) );
            const KeyType * keys = (const KeyType *)(tab.keys);
            for ( int8_t dist = 0; tab.distance[index] >= dist; ++dist, ++index ) {
                if ( keys[index] == key ) {
                    return { index, true };
                }
            }
            return { 0, false };
        }
        
        void grow ( Table & tab ) {
            uint32_t newCapacity = max(minCapacity, tab.capacity*2);
            Table newTab;
            newTab.data = (char *) context->allocate(newCapacity * (valueTypeSize + sizeof(KeyType) + sizeof(uint8_t)));
            newTab.keys = newTab.data + newCapacity * valueTypeSize;
            newTab.distance = (int8_t *)( newTab.data + newCapacity * (valueTypeSize + sizeof(KeyType)) );
            newTab.size = 0;
            newTab.capacity = newCapacity;
            newTab.lock = tab.lock;
            computeMaxLookups(newTab);
            memset ( newTab.distance, -1, newCapacity * sizeof(uint8_t) );
            if ( tab.size ) {
                KeyType * keys = (KeyType *)(tab.keys);
                for ( uint32_t index=0; index != tab.capacity; ++ index ) {
                    if ( tab.distance[index] >=0 ) {
                        auto at = insert(newTab, keys[index], tab.data + index*valueTypeSize);
                        assert(at.second && "expected for it to be inserted fine");
                        memcpy ( newTab.data + at.first*valueTypeSize, tab.data + index*valueTypeSize, valueTypeSize );
                    }
                }
            }
            swap(newTab, tab);
        }
        
        // this moves on insert
        pair<size_t,bool> insert ( Table & tab, const KeyType & key, void * value )
        {
            if ( tab.capacity==0 ) {
                grow(tab);
            }
            auto hash = hash_function(key);
            size_t index = indexForHash(tab, hash );
            KeyType * keys = (KeyType *)(tab.keys);
            int8_t dist = 0;
            for ( ; tab.distance[index] >= dist; index++, dist++ ) {
                if ( keys[index] == key ) {
                    return { index, false };
                }
            }
            // insert_new
            return insert_new(tab, dist, index, key, value);
        }
        
        pair<size_t,bool> erase ( Table & tab, const KeyType & key ) {
            auto at = find(tab, key);
            if ( at.second ) {
                erase_existing(tab, at.first);
                return at;
            } else {
                return { 0, false };
            }
        }
        
    protected:
        
        pair<size_t,bool> insert_new ( Table & tab, int8_t dist, size_t index, const KeyType & key, void * value ) {
            KeyType * keys = (KeyType *)(tab.keys);
            if ( tab.capacity==0 || dist==tab.maxLookups || (tab.size+1)>(tab.capacity/2) ) {
                grow(tab);
                return insert(tab, key, value);
            } else if ( tab.distance[index]<0 ) {
                tab.size ++;
                tab.distance[index] = dist;
                keys[index] = key;
                copy_value ( tab, index, value );
                return { index, true };
            }
            KeyType insertKey = key;
            swap ( dist, tab.distance[index] );
            swap ( insertKey, keys[index] );
            swap_value ( tab, index, value );
            for ( ++dist, ++index; ; ++index ) {
                if ( tab.distance[index]<0 ) {
                    tab.size ++;
                    tab.distance[index] = dist;
                    keys[index] = insertKey;
                    copy_value ( tab, index, value );
                    return { index, true };
                } else if ( tab.distance[index]<dist ) {
                    swap(dist, tab.distance[index]);
                    swap(insertKey, keys[index]);
                    swap_value(tab, index, value);
                    ++ dist;
                } else {
                    ++ dist;
                    if ( dist == tab.maxLookups ) {
                        swap(insertKey, keys[index]);
                        swap_value(tab, index, value);
                        grow(tab);
                        return insert(tab, insertKey, value);
                    }
                }
            }
        }
        
        void erase_existing ( Table & tab, size_t to_erase ) {
            size_t current = to_erase;
            tab.distance[current] = -1;
            tab.size --;
            KeyType * keys = (KeyType *)(tab.keys);
            for ( size_t next = current + 1; tab.distance[next]>0 ; ++ current, ++next ) {
                tab.distance[current] = tab.distance[next] - 1;
                keys[current] = keys[next];
                memcpy ( tab.data + current*valueTypeSize, tab.data + next*valueTypeSize, valueTypeSize );
                tab.distance[next] = -1;
            }
        }
    };
}
