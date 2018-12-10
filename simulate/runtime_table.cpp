#include "precomp.h"

#include "runtime_table.h"

namespace yzg
{
    void table_lock ( Context & context, Array & arr ) {
        arr.lock ++;
        if ( arr.lock==0 ) {
            context.throw_error("table lock overflow");
        }
    }
    
    void table_unlock ( Context & context, Array & arr ) {
        if ( arr.lock==0 ) {
            context.throw_error("table lock underflow");
        }
        arr.lock --;
    }
    
    // SimNode_Table
    
    __m128 SimNode_Table::eval ( Context & context ) {
        __m128 xtab = tabExpr->eval(context);
        YZG_EXCEPTION_POINT;
        __m128 xkey = keyExpr->eval(context);
        YZG_EXCEPTION_POINT;
        Table * tab = cast<Table *>::to(xtab);
        return tabEval ( context, tab, xkey );
    }

    // TableIterator
    
    size_t TableIterator::nextValid ( Table * tab, size_t index ) const {
        for ( ; index < tab->capacity; index++ )
            if ( tab->distance[index]>=0 )
                break;
        return index;
    }
    
    bool TableIterator::first ( Context & context, IteratorContext & itc ) {
        __m128 ll = source->eval(context);
        YZG_ITERATOR_EXCEPTION_POINT;
        auto pTable = cast<Table *>::to(ll);
        table_lock(context, *pTable);
        YZG_ITERATOR_EXCEPTION_POINT;
        size_t index = nextValid(pTable, 0);
        char * data    = getData(pTable);
        itc.value      = cast<char *>::from(data + index * stride);
        itc.table_end  = data + pTable->capacity * stride;
        itc.table      = pTable;
        return pTable->size != 0;
    }
    
    bool TableIterator::next  ( Context & context, IteratorContext & itc ) {
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

