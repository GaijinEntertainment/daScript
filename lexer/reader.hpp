//
//  reader.hpp
//  yzg
//
//  Created by Boris Batkin on 8/23/18.
//  Copyright © 2018 Boris Batkin. All rights reserved.
//

#ifndef reader_hpp
#define reader_hpp

#include <memory>
#include <vector>
#include <string>

namespace yzg
{
    using namespace std;
    
    // double 1 as 1.
    string to_string_ex ( double dnum );
    
    enum class NodeType
    {
        nil,
        list,
        name,
        string,
        boolean,
        inumber,
        unumber,
        dnumber,
        op
    };
    
    enum class Operator
    {
        none,
    // 2-char
        r2l,
        addEqu,
        subEqu,
        divEqu,
        mulEqu,
        andEqu,
        orEqu,
        xorEqu,
        eqEq,
        lessEqu,
        greaterEqu,
        notEqu,
        binNotEqu,
    // 1-char
        at,         // @
        dot,        // .
        binand,
        binor,
        binxor,
        add,
        sub,
        div,
        mul,
        eq,
        is,         // ?
        boolNot,    // !
        binNot,     // ~
        less,
        greater
    };
    
    string to_string ( Operator op );
    bool isUnaryOperator ( Operator op );
    bool isBinaryOperator ( Operator op );
    bool isTrinaryOperator ( Operator op );
    
    struct Node;
    typedef unique_ptr<Node> NodePtr;
    
    struct Node
    {
        NodeType        type;
        string          text;
        vector<NodePtr> list;
        union
        {
            bool        b;
            int32_t     inum;
            uint32_t    unum;
            double      dnum;
            Operator    op;
        };
        string::const_iterator at;
        
        Node(string::const_iterator & AT) : at(AT), type(NodeType::nil) {}
        Node(NodeType nt, const string & st, string::const_iterator & AT) : at(AT), type(nt), text(st) {}
        Node(int32_t val, string::const_iterator & AT) : at(AT), type(NodeType::inumber), inum(val) {}
        Node(uint32_t val, string::const_iterator & AT) : at(AT), type(NodeType::unumber), unum(val) {}
        Node(double val, string::const_iterator & AT) : at(AT), type(NodeType::dnumber), dnum(val) {}
        Node(bool val, string::const_iterator & AT) : at(AT), type(NodeType::boolean), b(val) {}
        Node(vector<NodePtr> && ll, string::const_iterator & AT) : at(AT), type(NodeType::list), list(move(ll)) {}
        Node(Operator o, string::const_iterator & AT) : at(AT), type(NodeType::op), op(o) {}
        
        bool isList() const { return type==NodeType::list; }
        bool isString() const { return type==NodeType::string; }
        bool isBoolean() const { return type==NodeType::boolean; }
        bool isListOfAtLeastSize(int n) const { return type==NodeType::list && list.size()>=n; }
        bool isName() const { return type==NodeType::name; }
        bool isName(const string & st) const { return type==NodeType::name && text==st; }
        bool isOperator() const { return type==NodeType::op; }
        bool isOperator(Operator opr) const { return type==NodeType::op && op==opr; }
        bool isNumericConstant() const { return type==NodeType::inumber || type==NodeType::unumber || type==NodeType::dnumber; }
    
        uint32_t getUnsigned(int n) const;
        string getName(int n) const;
        string getTailName(int nField = 0) const;
        
        int depth() const;
        bool isMathNode() const;    // (* (+ a b) c) etc
        
        friend ostream& operator<< (ostream& stream, const Node & node);
    };
    
    class read_error : public runtime_error
    {
    public:
        read_error ( const string & message, string::const_iterator error_at ) : runtime_error(message), at(error_at) {}
        string::const_iterator at;
    };
    
    NodePtr read ( const string & st );
    
    void reportError ( const string & st, const string::const_iterator & at, const string & message );
}

#endif /* reader_hpp */
