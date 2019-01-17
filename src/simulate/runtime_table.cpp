#include "daScript/misc/platform.h"

#include "daScript/simulate/runtime_table.h"

namespace das
{
    void table_clear ( Context & context, Table & arr ) {
        if ( arr.lock ) {
            context.throw_error("clearing locked table");
            return;
        }
#if USE_ROBIN_HOOD
        memset(arr.distance, -1, arr.capacity * sizeof(uint8_t));
#else
		memset(arr.hashes, 0, arr.capacity * sizeof(uint32_t));
#endif
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
    
    // SimNode_Table
    
    vec4f SimNode_Table::eval ( Context & context ) {
		Table * tab = (Table *)tabExpr->evalPtr(context);
        DAS_EXCEPTION_POINT;
        vec4f xkey = keyExpr->eval(context);
        DAS_EXCEPTION_POINT;
        return tabEval ( context, tab, xkey );
    }

    // TableIterator
    
    size_t TableIterator::nextValid ( Table * tab, size_t index ) const {
		for (; index < tab->capacity; index++)
#if USE_ROBIN_HOOD
			if (tab->distance[index] >= 0)
				break;
#else
			if (tab->hashes[index] != HASH_EMPTY32 && tab->hashes[index] != HASH_KILLED32)
				break;
#endif
        return index;
    }
    
    bool TableIterator::first ( Context & context, IteratorContext & itc ) {
        vec4f ll = source->eval(context);
        DAS_ITERATOR_EXCEPTION_POINT;
        auto pTable = cast<Table *>::to(ll);
        table_lock(context, *pTable);
        DAS_ITERATOR_EXCEPTION_POINT;
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

