#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"

namespace das {

    struct SimPrint : SimVisitor {
        SimPrint ( TextWriter & wr ) : ss(wr) {
        }
        void crlf() {
            if ( CR ) {
                ss << "\n" << string(tab,'\t');
            } else {
                ss << " ";
            }
        }
        virtual void preVisit ( SimNode * node ) override {
            xcr.push_back(CR);
            CR = false;
            tab ++;
            SimVisitor::preVisit(node);
            ss << "(";
        }
        virtual SimNode * visit ( SimNode * node ) override {
            ss << ")";
            tab --;
            CR = xcr.back();
            xcr.pop_back();
            return SimVisitor::visit(node);
        }
        virtual void cr () override {
            CR = true;
        }
        virtual void op ( const char * name, size_t typeSize, const string & typeName ) override {
            SimVisitor::op(name);
            ss << name;
            if ( !typeName.empty() ) {
                ss << "_TT<" << typeName << ">";
            } else if ( typeSize ) {
                ss << "_TT<(" << typeSize << ")>";
            }
        }
        virtual void sp ( uint32_t stackTop, const char * name ) override {
            SimVisitor::sp(stackTop,name);
            crlf();
            ss << "#" << stackTop;
        }
        virtual void arg ( int32_t argV,  const char * argN ) override {
            SimVisitor::arg(argV,argN);
            crlf();
            ss << argV;
        }
        virtual void arg ( uint32_t argV,  const char * argN ) override {
            SimVisitor::arg(argV,argN);
            crlf();
            ss << argV;
        }
        virtual void arg ( const char * argV,  const char * argN ) override {
            SimVisitor::arg(argV,argN);
            crlf();
            if ( argV ) {
                ss << "\"" << argV << "\"";
            } else {
                ss << "null";
            }
        }
        virtual void arg ( int64_t argV,  const char * argN ) override {
            SimVisitor::arg(argV,argN);
            crlf();
            ss << argV;
        }
        virtual void arg ( uint64_t argV,  const char * argN ) override {
            SimVisitor::arg(argV,argN);
            crlf();
            ss << argV;
        }
        virtual void arg ( float argV,  const char * argN ) override {
            SimVisitor::arg(argV,argN);
            crlf();
            ss << argV;
        }
        virtual void arg ( double argV,  const char * argN ) override {
            SimVisitor::arg(argV,argN);
            crlf();
            ss << argV;
        }
        virtual void arg ( bool argV,  const char * argN ) override {
            SimVisitor::arg(argV,argN);
            crlf();
            ss << (argV ? "true" : "false");
        }
        virtual void arg ( vec4f argV,  const char * argN ) override {
            SimVisitor::arg(argV,argN);
            union {
                uint32_t    ui[4];
                vec4f       v;
            } X; X.v = argV;
            crlf();
            ss << "{" << X.ui[0] << "," << X.ui[1] << "," << X.ui[2] << "," << X.ui[3] << "}";
        }

        virtual void sub ( SimNode ** nodes, uint32_t count, const char * ) override {
            if ( count==0 ) return;
            crlf();
            for ( uint32_t t = 0; t!=count; ++t ) {
                if ( t ) crlf();
                nodes[t] = nodes[t]->visit(*this);
            }
        }
        virtual SimNode * sub ( SimNode * node, const char *  ) override {
            crlf();
            return node ? node->visit(*this) : nullptr;
        }
        TextWriter & ss;
        int tab = 0;
        bool CR = false;
        vector<bool> xcr;
    };

    void printSimNode ( TextWriter & ss, SimNode * node ) {
        SimPrint prv(ss);
        node->visit(prv);
    }

}
