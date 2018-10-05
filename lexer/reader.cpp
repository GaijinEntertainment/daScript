//
//  reader.cpp
//  yzg
//
//  Created by Boris Batkin on 8/23/18.
//  Copyright © 2018 Boris Batkin. All rights reserved.
//

#include "reader.hpp"

#include "enums.h"

#include <regex>
#include <sstream>
#include <iostream>

namespace yzg
{
    Enum<Operator> g_opTable2 = {
        {   Operator::addEqu,       "+="    },
        {   Operator::subEqu,       "-="    },
        {   Operator::divEqu,       "/="    },
        {   Operator::mulEqu,       "*="    },
        {   Operator::eqEq,         "=="    },
        {   Operator::lessEqu,      "<="    },
        {   Operator::greaterEqu,   ">="    },
        {   Operator::notEqu,       "!="    },
        {   Operator::binNotEqu,    "~="    },
        {   Operator::andEqu,       "&="    },
        {   Operator::orEqu,        "|="    },
        {   Operator::xorEqu,       "^="    },
    };
    
    Enum<Operator> g_opTable1 = {
        {   Operator::dot,      "."    },
        {   Operator::add,      "+"    },
        {   Operator::sub,      "-"    },
        {   Operator::div,      "/"    },
        {   Operator::mul,      "*"    },
        {   Operator::eq,       "="    },
        {   Operator::is,       "?"    },
        {   Operator::boolNot,  "!"    },
        {   Operator::binNot,   "~"    },
        {   Operator::less,     "<"    },
        {   Operator::greater,  ">"    },
        {   Operator::binand,   "&"    },
        {   Operator::binor,    "|"    },
        {   Operator::binxor,   "^"    },
    };
    
    string to_string ( Operator o ) {
        string t = g_opTable2.find(o);
        if ( t.empty() )
            t = g_opTable1.find(o);
        return t;
    }
    
    bool isUnaryOperator ( Operator op )
    {
        return
            (op==Operator::add)
        ||  (op==Operator::sub)
        ||  (op==Operator::boolNot)
        ||  (op==Operator::binNot)
        ;
    }
    
    bool isBinaryOperator ( Operator op )
    {
        return
            (op!=Operator::is)
        &&  (op!=Operator::boolNot)
        &&  (op!=Operator::binNot)
        ;
    }
    
    bool isTrinaryOperator ( Operator op )
    {
        return (op==Operator::is);
    }
    
    // Node
    
    uint64_t Node::getUnsigned(int n) const
    {
        if ( !isListOfAtLeastSize(n+1) )
            return -1;
        auto & name = list[n];
        if ( name->type==NodeType::unumber )
            return name->unum;
        if ( name->type==NodeType::inumber && name->inum>=0 )
            return name->inum;
        return -1U;
    }
    
    string Node::getName(int n) const
    {
        if ( !isListOfAtLeastSize(n+1) )
            return "";
        auto & name = list[n];
        if ( name->type!=NodeType::name )
            return "";
        return name->text;
    }
    
    string Node::getTailName(int nField) const
    {
        if ( !isListOfAtLeastSize(1 + nField) )
            return "";
        auto & tail = list[list.size()-1-nField];
        if ( tail->type!=NodeType::name )
            return "";
        return tail->text;
    }
    
    int Node::depth() const
    {
        if ( type!=NodeType::list )
            return 1;
        int maxD = 0;
        for ( auto & node : list ) {
            maxD = max(maxD, node->depth());
        }
        return maxD + 1;
    }
    
    bool Node::isMathNode() const
    {
        if ( type!=NodeType::list || list.size()==0 )
            return true;
        if ( list[0]->type != NodeType::op )
            return false;
        for ( auto & node : list ) {
            if ( !node->isMathNode() )
                return false;
        }
        return true;
    }
    
    string to_string_ex ( double dnum )
    {
        stringstream ss;
        ss << dnum;
        if ( ss.str().find_first_of(".e")==string::npos )
            ss << ".";
        return ss.str();
    }
    
    void print (ostream & stream, const Node & node, int depth )
    {
        switch(node.type) {
            case NodeType::nil:
                throw "implement";
                break;
            case NodeType::list:
                if ( depth==0 || (node.depth()>2 && !node.isMathNode()) ) {
                    stream << "(";
                    for ( int i=0; i != node.list.size(); ++i ) {
                        if ( i )
                            stream << string(depth+1, '\t');
                        else if ( node.list[i]->type!=NodeType::name )
                            stream << '\t';
                        print(stream, *(node.list[i]), depth+1);
                        if ( i != node.list.size()-1 )
                            stream << "\n";
                    }
                    stream << ")\n";
                } else {
                    stream << "(";
                    for ( int i=0; i != node.list.size(); ++i ) {
                        if ( i )
                            stream << " ";
                        print(stream, *(node.list[i]), depth+1);
                    }
                    stream << ")";
                }
                break;
            case NodeType::name:
                stream << node.text;
                break;
            case NodeType::string:
                stream << '"' << node.text << '"';
                break;
            case NodeType::boolean:
                stream << (node.b ? "true" : "false");
                break;
            case NodeType::inumber:
                stream << node.inum;
                break;
            case NodeType::unumber:
                stream << "0x" << hex << node.unum << dec;
                break;
            case NodeType::dnumber:
                stream << to_string_ex(node.dnum);
                break;
            case NodeType::op:
                stream << to_string(node.op);
                break;
        }
    }
    
    ostream& operator << (ostream& stream, const Node & node)
    {
        print(stream, node, 0);
        return stream;
    }
    
    typedef string::const_iterator stit;
    
    const regex_constants::match_flag_type continues = regex_constants::match_continuous;
    regex_constants::syntax_option_type static_regex_init = regex_constants::optimize;
    
    static regex REG_name          ( "([_a-zA-Z]\\w*)", static_regex_init );
    static regex REG_hex           ( "(0x[a-fA-F0-9]+)", static_regex_init );
    static regex REG_uint          ( "([0-9]+[uU])", static_regex_init );
    static regex REG_number        ( "([-+]?\\d+\\.?\\d*)", static_regex_init );
    static regex REG_number_exp    ( "([-+]?\\d+\\.?\\d*e[-+]?\\d*)", static_regex_init );
    static regex REG_boolean       ( "\\b(true|false)\\b", static_regex_init );
    
    //  return -1 if not match
    stit parseString( stit begin, stit end, char quotes )
    {
        // regex REG_string        ( "\"(([^\"\\\\]|\\\\.)*?)\"", static_regex_init );
        //  1. "
        //  2. anything or \", repeated as many times, as necessary
        //  3. "
        auto pos = begin;
        // 1
        if ( *pos!=quotes )
            throw read_error("expecting quotes", pos);
        ++ pos;
        // 2
        while ( pos<end )
        {
            if ( *pos==quotes )
                return pos + 1;
            else if ( *pos=='\n' )
                throw read_error("string constant exceeds line", pos );
            else if ( *pos=='\\' )
                pos += 2;
            else
                ++ pos;
        }
        throw read_error("string constant exceeds file", pos);
    }
    
    stit skipWhiteSpace ( stit it, stit end )
    {
        for ( ; it != end && isspace(*it); ++it ) ;
        return it;
    }
    
    NodePtr readNode ( stit & it, stit end, bool isList );
    
    vector<NodePtr> readNodes ( stit & it, stit end, bool isList )
    {
        vector<NodePtr> nodes;
        while ( auto node = readNode(it, end, isList) )
            nodes.emplace_back(move(node));
        return nodes;
    }
    
    NodePtr readNode ( stit & it, stit end, bool isList )
    {
        smatch what;
        it = skipWhiteSpace(it, end);
        stit at = it;
        if ( it==end ) {
            if ( isList )
                throw read_error("list exceeds file", it);
            return nullptr;
        } else if ( *it==')' ) {
            if ( !isList )
                throw read_error("unexpected end of list", it);
            ++ it;
            return nullptr;
        } else if ( *it=='(' ) {
            ++ it;
            return make_unique<Node>(readNodes(it, end, true), at);
        } else if ( *it=='$' ) {
            ++ it;
            readNode(it, end, isList);  // skip node, and return next one
            return readNode(it, end, isList);
        } else if ( *it=='"' || *it=='\'') {
            it = parseString(it,end,*it);
            return make_unique<Node>(NodeType::string, string(at+1,it-1), at);
        } else if ( regex_search(it,end,what,REG_boolean,continues) ) {
            it += what.length();
            return make_unique<Node>(what[1].str()=="true", at);
        } else if ( regex_search(it,end,what,REG_name,continues) ) {
            it += what.length();
            return make_unique<Node>(NodeType::name, what[1].str(), at);
        } else if ( regex_search(it,end,what,REG_hex,continues) ) {
            it += what.length();
            return make_unique<Node>(uint64_t(stoul(what[1].str(), 0, 16)), at);
        } else if ( regex_search(it,end,what,REG_uint,continues) ) {
            it += what.length();
            return make_unique<Node>(uint64_t(stoul(what[1].str())), at);
        } else if ( regex_search(it,end,what,REG_number_exp,continues) || regex_search(it,end,what,REG_number,continues) ) {
            it += what.length();
            string num = what[1].str();
            auto found = num.find_first_of(".e");
            return found!=string::npos ? make_unique<Node>(stod(num), at) : make_unique<Node>(int64_t(stol(num)), at);
        } else {
            auto op = g_opTable2.parse(it, Operator::none);
            if ( op==Operator::none )
                op = g_opTable1.parse(it, Operator::none);
            if ( op==Operator::none )
                throw read_error("unexpected character", it);
            return make_unique<Node>(op, at);
        }
        return nullptr;
    }
    
    NodePtr read ( stit it, stit end )
    {
        return make_unique<Node>(readNodes(it, end, false), it);
    }
    
    void reportError ( const string & st, const string::const_iterator & at, const string & message )
    {
        int col = 1, row = 1;
        auto text = st.begin();
        auto it = st.begin();
        while ( it != at && it != st.end() ) {
            if ( *it=='\n' ) {
                text = it + 1;
                row ++;
                col = 1;
            }
            ++ it;
            col ++;
        }
        auto endtext = text;
        while ( *endtext!='\n' && endtext!=st.end() )
            ++endtext;
        cout
            << "error at line " << row << "\n"
            << string(text, endtext) << "\n"
            << string(max(col-2,0), ' ') << "^" << "\n"
            << message << "\n";
    }
    
    NodePtr read ( const string & st )
    {
        return read(st.begin(), st.end());
    }
}
