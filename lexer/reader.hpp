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
#include <fstream>

namespace yzg
{
    using namespace std;
    
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
    // 2-char
        addEqu,
        subEqu,
        divEqu,
        mulEqu,
        eqEq,
    // 1-char
        add,
        sub,
        div,
        mul,
        eq
    };
    
    string to_string ( Operator op );
    
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
            int64_t     inum;
            uint64_t    unum;
            double      dnum;
            Operator    op;
        };
        Node() : type(NodeType::nil) {}
        Node(NodeType nt, const string & st) : type(nt), text(st) {}
        Node(int64_t val) : type(NodeType::inumber), inum(val) {}
        Node(uint64_t val) : type(NodeType::unumber), unum(val) {}
        Node(double val) : type(NodeType::dnumber), dnum(val) {}
        Node(bool val) : type(NodeType::boolean), b(val) {}
        Node(vector<NodePtr> && ll) : type(NodeType::list), list(move(ll)) {}
        Node(Operator o) : type(NodeType::op), op(o) {}
        
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
    NodePtr read ( ifstream & t );
}

#endif /* reader_hpp */
