def test : bool
    let(tab:table<string,int>;i,del:int;total:int=20)
        i = 0
        while i!=total
            tab[string(i)] = i++
        assert(length(tab)==total)
        // debug(tab)
        i = 0
        while i!=total
            assert(tab[string(i)]==i++)
        assert(length(tab)==total)
        // debug(tab)
        i = del = 0
        while i < total
            erase(tab,string(i))
            del ++
            i += 8
        assert(length(tab)==total-del)
    return true

//
//        for ( int i=0; i!=total; ++i ) {
//            auto it = Find<const char *,int>(tab, rhh, keys[i].c_str() );
//            assert(it.second);
//            assert(it.first == i);
//        }
//
//        for ( int i=0; i<total; ++i ) {
//            auto it = Find<const char *,int>(tab, rhh, keys[i].c_str() );
//            if ( i%7 == 0 ) {
//                assert(!it.second);
//            } else {
//                assert(it.second);
//                assert(it.first == i);
//            }
//        }
//
//        for ( int i=0; i<total; i+=7 ) {
//            Insert<const char *,int>(tab, rhh, keys[i].c_str(), i);
//        }
//
//        for ( int i=0; i!=total; ++i ) {
//            auto it = Find<const char *,int>(tab, rhh, keys[i].c_str() );
//            assert(it.second);
//            assert(it.first == i);
//        }
//    }

