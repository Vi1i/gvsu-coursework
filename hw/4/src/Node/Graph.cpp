#include <chow/Node/Graph.hpp>

namespace chow {
    IDNode Node::Graph::GetIDNode() {
        return this->_id;
    }

    StmtListNode Node::Graph::GetStmtListNode() {
        return this->_list;
    }
}
