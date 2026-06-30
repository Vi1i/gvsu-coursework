#ifndef NODE_HPP
#define NODE_HPP

#include <iostream>
#include <sstream>
#include <vector>

namespace chow {
    class Node {
        public:
            virtual ~Node() {}
    };

    class Identifier {
        public:
            Identifier(std::string name) : mName(name) {}
            Identifier(const Identifier& nId);

            std::string GetName() const;

            std::string mName;
    };

    inline std::ostream& operator<<(std::ostream& os, const chow::Identifier& id) {
        return os << id.GetName();
    }

    enum class GraphType {
        GRAPH,
        DIGRAPH,
    };

    inline std::ostream& operator<<(std::ostream& os, const chow::GraphType& graphType) {
        switch(graphType) {
            case chow::GraphType::GRAPH:    return os << "GRAPH"; break;
            case chow::GraphType::DIGRAPH:  return os << "DIGRAPH"; break;
        }
        return os;
    }

    class Type {
        public:
            Type(GraphType type) : mType(type) {}

            chow::GraphType GetType() const;

        private:
            chow::GraphType mType;
    };

    inline std::ostream& operator<<(std::ostream& os, const chow::Type& rhs) {
        return os << rhs.GetType();
    }

    enum class StatementType {
        NODE,
        EDGE,
        ATTR,
        IDASSIGNID,
        SUBGRAPH,
        NONE,
    };

    inline std::ostream& operator<<(std::ostream& os, const chow::StatementType& statementType) {
        switch(statementType) {
            case chow::StatementType::NODE:       return os << "NODE"; break;
            case chow::StatementType::EDGE:       return os << "EDGE"; break;
            case chow::StatementType::ATTR:       return os << "ATTR"; break;
            case chow::StatementType::IDASSIGNID: return os << "IDASSIGNID"; break;
            case chow::StatementType::SUBGRAPH:   return os << "SUBGRAPH"; break;
            case chow::StatementType::NONE:       return os << "NONE"; break;
        }
        return os;
    }

    class Statement {
        public:
            Statement(chow::StatementType type = chow::StatementType::NONE) : mType(type) {};
            virtual ~Statement() {};

            virtual std::string ToString() {
                std::ostringstream stream;
                stream << "<" << this->mType << ">";
                return stream.str();
            };

            chow::StatementType GetType() {
                return this->mType;
            };

            chow::StatementType mType;
    };

    class AList {
        public:
            //AList() : mLhs(""), mRhs("") {};
            AList(chow::Identifier lhs, chow::Identifier rhs, chow::AList* aList) : mLhs(lhs), mRhs(rhs), mAList(aList) {};
            AList(const AList& cp);

            std::string ToString();

            chow::Identifier mLhs;
            chow::Identifier mRhs;
            AList* mAList;
    };

    class AttrList {
        public:
            //AttrList() : mAList(), mAttrList(nullptr) {}
            AttrList(chow::AList* aList, chow::AttrList* attrList) : mAList(aList), mAttrList(attrList) {}
            
            std::string ToString();

            chow::AList* mAList;
            chow::AttrList* mAttrList;
    };

    class NodeStatement : public Statement {
        public:
            NodeStatement(chow::Identifier* id, chow::AttrList* attrList) : chow::Statement(chow::StatementType::NODE), mId(id), mAttrList(attrList) {};

            std::string ToString();

            chow::Identifier* mId;
            chow::AttrList* mAttrList;
    };

    enum class EdgeType {
        UEDGE,
        DEDGE,
    };

    class EdgeRhs {
        public:
            EdgeRhs(chow::EdgeType type, chow::Identifier* id, EdgeRhs* edgeRhs) : mType(type), mId(id), mEdgeRhs(edgeRhs) {}

            chow::EdgeType mType;
            chow::Identifier* mId;
            chow::EdgeRhs* mEdgeRhs;
    };

    class EdgeStatement : public Statement {
        public:
            EdgeStatement(chow::Identifier* id, chow::EdgeRhs* edgeRhs, chow::AttrList* attrList) : chow::Statement(chow::StatementType::EDGE), mId(id), mEdgeRhs(edgeRhs), mAttrList(attrList) {};

            std::string ToString();
            chow::Identifier* mId;
            chow::EdgeRhs* mEdgeRhs;
            chow::AttrList* mAttrList;
    };

    class AttrStatement : public Statement {
        public:
            AttrStatement() : chow::Statement(chow::StatementType::ATTR) {};
            std::string ToString();
    };

    class IdAssignId : public Statement {
        public:
            IdAssignId(chow::Identifier lhs, chow::Identifier rhs) : chow::Statement(chow::StatementType::IDASSIGNID), mLhs(lhs), mRhs(rhs) {};
            std::string ToString();

            chow::Identifier mLhs;
            chow::Identifier mRhs;
    };

    class Subgraph : public Statement {
        public:
            Subgraph() : chow::Statement(chow::StatementType::SUBGRAPH) {};
            std::string ToString();
    };

    class StatementList {
        public:
            StatementList(chow::Statement* statement, StatementList* statementList) : mStatement(statement), mStatementList(statementList) {};

            StatementList* GetStatementList() const;
            chow::Statement* GetStatement() const;

            StatementList* mStatementList;
            chow::Statement* mStatement;
    };

    inline std::ostream& operator<<(std::ostream& os, const chow::StatementList& statementList) {
        chow::Statement* stmt = statementList.GetStatement();

        if(stmt->GetType() == chow::StatementType::NODE) {
            chow::NodeStatement* temp =  dynamic_cast<chow::NodeStatement*>(stmt);
            os << temp->ToString();
        }else if(stmt->GetType() == chow::StatementType::EDGE) {
            chow::EdgeStatement* temp =  dynamic_cast<chow::EdgeStatement*>(stmt);
            os << temp->ToString();
        }else if(stmt->GetType() == chow::StatementType::ATTR) {
            chow::AttrStatement* temp =  dynamic_cast<chow::AttrStatement*>(stmt);
            os << temp->ToString();
        }else if(stmt->GetType() == chow::StatementType::IDASSIGNID) {
            chow::IdAssignId* temp =  dynamic_cast<chow::IdAssignId*>(stmt);
            os << temp->ToString();
        }else if(stmt->GetType() == chow::StatementType::SUBGRAPH) {
            chow::Subgraph* temp =  dynamic_cast<chow::Subgraph*>(stmt);
            os << temp->ToString();
        }else{
            os << stmt->ToString();
        }

        if(statementList.GetStatementList()) {
            chow::StatementList* stmtList = statementList.GetStatementList();
            os << *stmtList;
        }

        return os;
    }

    class Graph : Node {
        public:
            Graph(bool strict, chow::Type type, chow::Identifier id, chow::StatementList statementList) : mStrict(strict), mType(type), mId(id), mStatementList(statementList) {}

            chow::Identifier GetId() const;
            chow::Type GetType() const;
            bool IsStrict() const;
            void PrintTree();

            bool mStrict;
            chow::Type mType;
            chow::Identifier mId;
            chow::StatementList mStatementList;
    };

    inline std::ostream& operator<<(std::ostream& os, const chow::Graph& graph) {
        os << "<" << ((graph.IsStrict()) ? "Strict," : "");
        os << graph.GetType() << ",";
        os << ((graph.GetId().GetName() == "") ? "" : graph.GetId().GetName());
        return os;
    }
}
#endif
