#include "precomp.h"

#include "runtime_table.h"

namespace yzg
{
    void TableIterator::close ( Context & context, IteratorContext & itc ) {
        if ( itc.table ) {
            table_unlock(context, *itc.table);
        }
    }
    
    // TableKeysIterator
    
    bool TableKeysIterator::first ( Context & context, IteratorContext & itc ) {
        __m128 ll = source->eval(context);
        YZG_ITERATOR_EXCEPTION_POINT;
        auto pTable = cast<Table *>::to(ll);
        table_lock(context, *pTable);
        YZG_ITERATOR_EXCEPTION_POINT;
        size_t index = nextValid(pTable, 0);
        char * data    = pTable->keys;
        itc.value      = cast<char *>::from(data + index * stride);
        itc.table_end  = data + pTable->capacity * stride;
        itc.table      = pTable;
        return pTable->size != 0;
    }
    
    bool TableKeysIterator::next  ( Context & context, IteratorContext & itc ) {
        char * data = cast<char *>::to(itc.value);
        size_t index = nextValid(itc.table, (data - itc.table->keys) / stride + 1 );
        data = itc.table->keys + index * stride;
        itc.value = cast<char *>::from(data);
        return data != itc.table_end;
    }
    
    // TableValuesIterator
    
    bool TableValuesIterator::first ( Context & context, IteratorContext & itc ) {
        __m128 ll = source->eval(context);
        YZG_ITERATOR_EXCEPTION_POINT;
        auto pTable = cast<Table *>::to(ll);
        table_lock(context, *pTable);
        YZG_ITERATOR_EXCEPTION_POINT;
        size_t index = nextValid(pTable, 0);
        char * data    = pTable->data;
        itc.value      = cast<char *>::from(data + index * stride);
        itc.table_end  = data + pTable->capacity * stride;
        itc.table      = pTable;
        return pTable->size != 0;
    }
    
    bool TableValuesIterator::next  ( Context & context, IteratorContext & itc ) {
        char * data = cast<char *>::to(itc.value);
        size_t index = nextValid(itc.table, (data - itc.table->data) / stride + 1 );
        data = itc.table->data + index * stride;
        itc.value = cast<char *>::from(data);
        return data != itc.table_end;
    }
}

