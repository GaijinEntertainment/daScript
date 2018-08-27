//
//  reader.cpp
//  yzg
//
//  Created by Boris Batkin on 8/23/18.
//  Copyright © 2018 Boris Batkin. All rights reserved.
//

#include "reader.hpp"

#include <regex>
#include <sstream>

namespace yzg
{
    struct OpEntry
    {
        Operator op;
        string text;
    } g_opTable [] = {
    // 2-char
        Operator::addEqu,   "+=",
        Operator::subEqu,   "-=",
        Operator::divEqu,   "/=",
        Operator::mulEqu,   "*=",
        Operator::eqEq,     "==",
    // 1-char
        Operator::add,      "+",
        Operator::sub,      "-",
        Operator::div,      "/",
        Operator::mul,      "*",
        Operator::eq,       "="
    };
    
    string to_string ( Operator o ) {
        for ( auto & op : g_opTable ) {
            if ( op.op == o )
                return op.text;
        }
        return "";
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
                stream << "0x" << hex << node.unum;
                break;
            case NodeType::dnumber:
                {
                    stringstream ss;
                    ss << node.dnum;
                    auto num = ss.str();
                    stream << node.dnum;
                    if ( num.find_first_of(".e")==string::npos )
                        stream << ".";
                }
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
            if ( *pos=='\\' )
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
            return make_unique<Node>(readNodes(it, end, true));
        } else if ( *it=='"' || *it=='\'') {
            stit at = it;
            it = parseString(it,end,*it);
            return make_unique<Node>(NodeType::string, string(at+1,it-1));
        } else if ( regex_search(it,end,what,REG_boolean,continues) ) {
            it += what.length();
            return make_unique<Node>(what[1].str()=="true");
        } else if ( regex_search(it,end,what,REG_name,continues) ) {
            it += what.length();
            return make_unique<Node>(NodeType::name, what[1].str());
        } else if ( regex_search(it,end,what,REG_hex,continues) ) {
            it += what.length();
            return make_unique<Node>(uint64_t(stoul(what[1].str(), 0, 16)));
        } else if ( regex_search(it,end,what,REG_uint,continues) ) {
            it += what.length();
            return make_unique<Node>(uint64_t(stoul(what[1].str())));
        } else if ( regex_search(it,end,what,REG_number_exp,continues) || regex_search(it,end,what,REG_number,continues) ) {
            it += what.length();
            string num = what[1].str();
            auto found = num.find_first_of(".e");
            return found!=string::npos ? make_unique<Node>(stod(num)) : make_unique<Node>(int64_t(stol(num)));
        } else {
            // TODO: implement effective operator search
            for ( auto & op : g_opTable ) {
                if ( equal(op.text.begin(), op.text.end(), it) ) {
                    it += op.text.length();
                    return make_unique<Node>(op.op);
                }
            }
            throw read_error("unexpected character", it);
        }
        return nullptr;
    }
    
    NodePtr read ( stit it, stit end )
    {
        return make_unique<Node>(readNodes(it, end, false));
    }
    
    NodePtr read ( const string & st )
    {
        return read(st.begin(), st.end());
    }
    
    NodePtr read ( ifstream & t )
    {
        string str;
        t.seekg(0, ios::end);
        str.reserve(t.tellg());
        t.seekg(0, ios::beg);
        str.assign((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());
        return read(str);
    }
}
