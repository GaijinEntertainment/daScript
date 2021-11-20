#pragma once

namespace das {

    template <typename KeyType, typename DataType>
    struct safebox {
    public:
        typedef smart_ptr<DataType> ValueType;
        typedef safebox<KeyType,ValueType> this_type;
    public:
        safebox() {}
        safebox( this_type && sb ) {
            objects = move(sb.objects);
            objectsInOrder = move(sb.objectsInOrder);
            postponed = move(sb.postponed);
        }
        safebox ( const this_type & sb ) {
            objects = sb.objects;
            objectsInOrder = sb.objectsInOrder;
            postponed = sb.postponed;
        }
        this_type & operator = ( this_type && sb ) {
            objects = move(sb.objects);
            objectsInOrder = move(sb.objectsInOrder);
            postponed = move(sb.postponed);
            return * this;
        }
        this_type & operator = ( const this_type & sb ) {
            objects = sb.objects;
            objectsInOrder = sb.objectsInOrder;
            postponed = sb.postponed;
            return * this;
        }
        void clear() {
            DAS_ASSERT(!locked);
            objects.clear();
            objectsInOrder.clear();
            postponed.clear();
        }
        void tryResolve() {
            if ( !locked ) {
                for ( auto & p : postponed ) {
                    objectsInOrder.push_back(p.second);
                    objects[p.first] = p.second;
                }
                postponed.clear();
            }
        }
        template <typename TT>
        void foreach ( TT && closure ) {
            auto saveLock = locked;
            locked = true;
            for ( auto & obj : objectsInOrder ) {
                closure(obj);
            }
            locked = saveLock;
            tryResolve();
        }
        template <typename TT>
        void find_first ( TT && closure ) {
            auto saveLock = locked;
            locked = true;
            for ( auto & obj : objectsInOrder ) {
                if ( closure(obj) ) break;
            }
            locked = saveLock;
            tryResolve();
        }
        template <typename TT>
        void foreach_kv ( TT && closure ) {
            auto saveLock = locked;
            locked = true;
            for ( auto & obj : objects ) {
                closure(obj.first, obj.second);
            }
            locked = saveLock;
            tryResolve();
        }
        bool insert ( const KeyType & key, const ValueType & value ) {
            if ( locked ) {
                if ( objects.find(key)!=objects.end() ) return false;
                for ( auto & pp : postponed ) {
                    if ( pp.first==key ) {
                        return false;
                    }
                }
                postponed.push_back(make_pair(key,value));
                return true;
            } else {
                objectsInOrder.push_back(value);
                return objects.insert(make_pair(key,value)).second;
            }
        }
        void replace ( const KeyType & key, const ValueType & value ) {
            auto it = objects.find(key);
            DAS_ASSERT(it!=objects.end());
            it->second = value;
        }
        ValueType find ( const KeyType & key ) const {
            auto it = objects.find(key);
            if ( it!=objects.end() ) {
                return it->second;
             } else {
                return ValueType();
             }
        }
        bool remove ( const KeyType & key ) {
            DAS_ASSERT(!locked);
            auto it = objects.find(key);
            if ( it != objects.end() ) {
                auto pObj = it->second;
                objects.erase(it);
                auto ito = das::find ( objectsInOrder.begin(), objectsInOrder.end(), pObj );
                DAS_ASSERT(ito != objectsInOrder.end() );
                objectsInOrder.erase(ito);
                return true;
            } else {
                return false;
            }
        }
    protected:
        das_hash_map<KeyType,ValueType>     objects;
        vector<ValueType>                   objectsInOrder;
        vector<pair<KeyType,ValueType>>     postponed;
        bool                                locked = false;
    };
}
