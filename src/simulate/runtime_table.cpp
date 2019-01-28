#include "daScript/misc/platform.h"

#include "daScript/simulate/runtime_table.h"

namespace das
{
    void table_clear ( Context & context, Table & arr ) {
        if ( arr.lock ) {
            context.throw_error("clearing locked table");
            return;
        }
        memset(arr.hashes, 0, arr.capacity * sizeof(uint32_t));
        arr.size = 0;
    }

    void table_lock ( Context & context, Table & arr ) {
        arr.lock ++;
        if ( arr.lock==0 ) {
            context.throw_error("table lock overflow");
        }
    }

    void table_unlock ( Context & context, Table & arr ) {
        if ( arr.lock==0 ) {
            context.throw_error("table lock underflow");
        }
        arr.lock --;
    }

    // TableIterator

    size_t TableIterator::nextValid ( Table * tab, size_t index ) const {
        for (; index < tab->capacity; index++)
            if (tab->hashes[index] > HASH_KILLED32)
                break;
        return index;
    }

    bool TableIterator::first ( Context & context, IteratorContext & itc ) {
        vec4f ll = source->eval(context);
        auto pTable = cast<Table *>::to(ll);
        table_lock(context, *pTable);
        size_t index = nextValid(pTable, 0);
        char * data    = getData(pTable);
        itc.value      = cast<char *>::from(data + index * stride);
        itc.table_end  = data + pTable->capacity * stride;
        itc.table      = pTable;
        return (bool) pTable->size;
    }

    bool TableIterator::next  ( Context &, IteratorContext & itc ) {
        char * data = cast<char *>::to(itc.value);
        char * tableData = getData(itc.table);
        size_t index = nextValid(itc.table, (data - tableData) / stride + 1 );
        data = tableData + index * stride;
        itc.value = cast<char *>::from(data);
        return data != itc.table_end;
    }

    void TableIterator::close ( Context & context, IteratorContext & itc ) {
        if ( itc.table ) {
            table_unlock(context, *itc.table);
        }
    }

    // TableKeysIterator

    char * TableKeysIterator::getData ( Table * tab ) const {
        return tab->keys;
    }

    // TableValuesIterator

    char * TableValuesIterator::getData ( Table * tab ) const {
        return tab->data;
    }
}

