#include <iostream>

#include "node.hpp"

namespace chow {
    Identifier::Identifier(const Identifier& nId) {
        this->mName = nId.mName;
    }

    std::string Identifier::GetName() const {
        return this->mName;
    }

    chow::GraphType Type::GetType() const {
        return this->mType;
    }

    chow::StatementList* StatementList::GetStatementList() const {
        return this->mStatementList;
    }

    chow::Statement* StatementList::GetStatement() const {
        return this->mStatement;
    }

    std::string chow::NodeStatement::ToString() {
        std::ostringstream os;
        os << "<Node_Stmt,";

        os << this->mType;
        os << ",";
        os << this->mId->GetName();
        os << ",";
        os << this->mAttrList->ToString();

        os << ">";
        return os.str();
    }

    std::string chow::EdgeStatement::ToString() {
        std::ostringstream os;
        os << "<Edge_Stmt";

        os << ">";
        return os.str();
    }

    std::string chow::AttrStatement::ToString() {
        std::ostringstream os;
        os << "<Attr_List";

        os << ">";
        return os.str();
    }

    std::string chow::IdAssignId::ToString() {
        std::ostringstream os;
        os << "<IdAssignID";

        os << ">";
        return os.str();
    }

    std::string chow::Subgraph::ToString() {
        std::ostringstream os;
        os << "<Subgraph";

        os << ">";
        return os.str();
    }

    std::string chow::AList::ToString() {
        std::ostringstream os;
        os << "<A_List";

        os << ">";
        return os.str();
    }

    std::string chow::AttrList::ToString() {
        std::ostringstream os;
        os << "<Attr_List";

        if(this->mAList) {
            os << "," << this->mAList->ToString();
        }

        if(this->mAttrList) {
            os << "," << this->mAttrList->ToString();
        }

        os << ">";
        return os.str();
    }

    chow::Identifier Graph::GetId() const {
        return this->mId;
    }

    bool Graph::IsStrict() const {
        return this->mStrict;
    }

    chow::Type Graph::GetType() const {
        return this->mType;
    }

    void Graph::PrintTree() {
        std::cout << std::endl;
        std::cout << *this;
        std::cout << ",Stmt_List:" << this->mStatementList;
        std::cout << ">" << std::endl;
    }
}
