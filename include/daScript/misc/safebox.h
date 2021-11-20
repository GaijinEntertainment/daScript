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
                // note - the reason for the 'neworder', as oppose to regular push_back into objectsInOrder
                //  is that there is an order dependency for compiling things like templates, where otherwise
                //  it would require us to actually check, if the template aliasing has been addressed
                //  before switching to template`
                vector<ValueType> neworder;
                neworder.reserve(postponed.size() + objectsInOrder.size());
                for ( auto & p : postponed ) {
                    neworder.push_back(p.second);
                    objects[p.first] = p.second;
                }
                for ( auto & p : objectsInOrder ) {
                    neworder.push_back(p);
                }
                postponed.clear();
                swap(objectsInOrder, neworder);
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
                if ( objects.find(key)!=objects.end() ) {
                    return false;
                }
                for ( auto & pp : postponed ) {
                    if ( pp.first==key ) {
                        return false;
                    }
                }
                postponed.push_back(make_pair(key,value));
                return true;
            } else {
                if ( objects.insert(make_pair(key,value)).second ) {
                    objectsInOrder.push_back(value);
                    return true;
                } else {
                    return false;
                }
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
                for ( auto & kv : postponed ) {
                    if ( kv.first==key ) {
                        return kv.second;
                    }
                }
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
