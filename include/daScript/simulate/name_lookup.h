#pragma once

#include "daScript/misc/platform.h"

namespace das {

    //! the function or global table of a Context, indexed by mangled-name hash and by plain name.
    //! Built at runtime - insert every entry, seal once; the sealed blob is owned and freed with
    //! the object - or adopted from constant data an emitter sealed at code-generation time, which
    //! the object never owns. Every lookup after that is one perfect-hash probe plus one 64-bit
    //! key compare
    struct DAS_API NameLookup {
        static constexpr uint32_t NOT_FOUND = 0xffffffffu;

        struct Entry {
            uint64_t    mnh;
            uint32_t    value;      //! what the mangled-name probe answers: a function index, a global's byte offset
            uint32_t    index;      //! position in the Context's functions or globalVariables array
            int32_t     next;       //! entries slot of the next same-name entry, -1 at the chain end
            int32_t     pad_;
        };
        struct NameSlot {
            uint64_t    nameHash;
            int32_t     head;       //! entries slot of the first same-name entry, -1 when empty
            int32_t     pad_;
        };
        //! compress-hash-displace: a key's bucket picks a displacement, the displaced mix of the key
        //! picks the slot; built once over distinct keys so no two land on one slot. The key is
        //! scrambled first: an FNV hash of two names that differ only in their last characters
        //! differs only in its low bits, and the bucket reads the high ones
        struct PerfectHash {
            uint32_t            nbuckets = 1;
            uint32_t            nslots = 1;
            const uint32_t *    disp = nullptr;
            //! slack is the number of five-percent empty-slot margins; a build that fails to
            //! converge is retried with more
            bool build ( const vector<uint64_t> & keys, vector<uint32_t> & dispOut, uint32_t slack = 1 );
            __forceinline uint32_t slotOf ( uint64_t key ) const {
                uint64_t h = scramble(key);
                uint32_t bucket = fastRange(uint32_t(h >> 32), nbuckets);
                return fastRange(mix(h, disp[bucket]), nslots);
            }
            static __forceinline uint64_t scramble ( uint64_t key ) {
                return key * 0x9E3779B97F4A7C15ull;
            }
            static __forceinline uint32_t fastRange ( uint32_t x, uint32_t n ) {
                return uint32_t((uint64_t(x) * uint64_t(n)) >> 32);
            }
            static __forceinline uint32_t mix ( uint64_t h, uint32_t d ) {
                uint64_t x = h ^ (uint64_t(d) * 0xD6E8FEB86659FD93ull);
                x *= 0xBF58476D1CE4E5B9ull;
                x ^= x >> 32;
                return uint32_t(x);
            }
        };
        //! a sealed lookup as an emitter writes it into an artifact: the four arrays and their sizes
        struct StaticTable {
            uint32_t            mnhBuckets;
            uint32_t            mnhSlots;
            uint32_t            nameBuckets;
            uint32_t            nameSlots;
            uint32_t            count;
            uint32_t            pad_;
            const uint32_t *    mnhDisp;
            const uint32_t *    nameDisp;
            const Entry *       entries;
            const NameSlot *    names;
        };

        //! a fresh object, and one whose seal failed, answers every probe with a miss
        NameLookup ();
        NameLookup ( const NameLookup & ) = delete;
        NameLookup & operator = ( const NameLookup & ) = delete;
        ~NameLookup ();

        //! value is what the mangled-name probe answers (a function index, a global byte offset);
        //! index is the position in the Context's functions or globalVariables array
        void insert ( uint64_t mnh, const char * name, uint32_t index, uint32_t value );
        //! false when two entries share a mangled-name hash or two different names share a name
        //! hash; failure then names both entries
        bool seal ( string * failure = nullptr );
        //! seal from constant data: the table and everything it points at outlive this object
        void adopt ( const StaticTable & table );

        __forceinline uint32_t valueByMnh ( uint64_t mnh ) const {
            const auto & e = entries[byMnh.slotOf(mnh)];
            return e.mnh==mnh ? e.value : NOT_FOUND;
        }
        //! entries slot of the first entry with this name, -1 when none
        __forceinline int32_t headByName ( const char * name ) const {
            uint64_t nameHash = hashName(name);
            const auto & n = names[byName.slotOf(nameHash)];
            return n.nameHash==nameHash ? n.head : -1;
        }
        __forceinline int32_t nextSameName ( int32_t slot ) const { return entries[slot].next; }
        __forceinline uint32_t indexAt ( int32_t slot ) const { return entries[slot].index; }
        __forceinline uint32_t valueAt ( int32_t slot ) const { return entries[slot].value; }

        uint32_t size () const { return count; }
        bool isSealed () const { return sealed; }
        bool isOwned () const { return blob!=nullptr; }

        static uint64_t hashName ( const char * name );

        //! the sealed state, readable so an emitter can write it out as a StaticTable
        PerfectHash         byMnh;
        PerfectHash         byName;
        const Entry *       entries = nullptr;
        const NameSlot *    names = nullptr;
        uint32_t            count = 0;
        bool                sealed = false;

    private:
        //! the name is copied into the staging arena: a caller's string need only outlive the insert
        struct Staged {
            uint64_t        mnh;
            uint64_t        nameHash;
            uint32_t        nameOffset;
            uint32_t        index;
            uint32_t        value;
        };
        const char * stagedName ( const Staged & s ) const { return nameBytes.data() + s.nameOffset; }
        void pointAtEmpty ();
        vector<Staged>      staged;
        vector<char>        nameBytes;
        void *              blob = nullptr;
    };

    //! the emitters write these layouts as words: an entry is {mnh, value | index << 32, next} and
    //! a name slot is {nameHash, head}, little-endian, so the pins hold them to that shape
    static_assert(sizeof(NameLookup::Entry)==24, "NameLookup::Entry layout is emitted as three 64-bit words");
    static_assert(offsetof(NameLookup::Entry, value)==8 && offsetof(NameLookup::Entry, index)==12 && offsetof(NameLookup::Entry, next)==16, "NameLookup::Entry layout is emitted as three 64-bit words");
    static_assert(sizeof(NameLookup::NameSlot)==16 && offsetof(NameLookup::NameSlot, head)==8, "NameLookup::NameSlot layout is emitted as two 64-bit words");
    static_assert(sizeof(NameLookup::StaticTable)==6*4+4*sizeof(void *), "NameLookup::StaticTable layout is emitted as six 32-bit words and four pointers");
    static_assert(offsetof(NameLookup::StaticTable, mnhBuckets)==0 && offsetof(NameLookup::StaticTable, mnhSlots)==4
        && offsetof(NameLookup::StaticTable, nameBuckets)==8 && offsetof(NameLookup::StaticTable, nameSlots)==12
        && offsetof(NameLookup::StaticTable, count)==16, "NameLookup::StaticTable layout is emitted as six 32-bit words and four pointers");
    static_assert(offsetof(NameLookup::StaticTable, mnhDisp)==24 && offsetof(NameLookup::StaticTable, nameDisp)==24+sizeof(void *)
        && offsetof(NameLookup::StaticTable, entries)==24+2*sizeof(void *) && offsetof(NameLookup::StaticTable, names)==24+3*sizeof(void *),
        "NameLookup::StaticTable layout is emitted as six 32-bit words and four pointers");

}
