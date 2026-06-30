#include <iostream>
#include <map>
#include <vector>
#include <set>

#include "node.hpp"

extern int yyparse();
extern "C" FILE* yyin;
extern chow::Graph* programBlock;

bool isConnected(chow::Graph* programBlock);
struct graph {
    bool isRoot;
    bool visited;
    std::string id;
    std::set<graph*> parents;
    std::set<graph*> children;
};
std::map<std::string, std::vector<std::string>> GetConnections(chow::StatementList stmt_list);

int main(const int argc, const char* argv[]) {
    if(argc != 2) {
        return EXIT_FAILURE;
    }

    //std::FILE* myfile = std::fopen(argv[1], "r");
    std::FILE* myfile = std::fopen(argv[1], "r");
    if(!myfile) {
        std::cout << "I can't open" << argv[1]  << std::endl;
        return EXIT_FAILURE;
    }

    yyin = myfile;

    do {
        yyparse();
    } while (!std::feof(yyin));

    programBlock->PrintTree();

    bool connectedGraph(isConnected(programBlock));

    std::cout << "This file is ";
    if(connectedGraph){
        std::cout << "a connected graph" << std::endl;
    }else{
        std::cout << "not a connected graph" << std::endl;
    }


    return EXIT_SUCCESS;
}

void yyerror(const char *s) {
    std::cerr << "Parse error!  Message: " << s << std::endl;
    exit(EXIT_FAILURE);
}

void TraverseHelper(std::set<std::string>* node_id_set, graph* root) {
    if(root->visited) {
        return;
    }
    node_id_set->erase(root->id);
    root->visited = true;

    for(const auto& child : root->children) {
        TraverseHelper(node_id_set, child);
    }
    for(const auto& parent : root->parents) {
        TraverseHelper(node_id_set, parent);
    }
}

bool Traverse(std::set<std::string> node_id_set, graph* root) {
    TraverseHelper(&node_id_set, root);

    return node_id_set.size() == 0;
}
bool isConnected(chow::Graph* programBlock) {
    if(!programBlock) {
        return false;
    }

    chow::StatementList stmt_list(programBlock->mStatementList);
    std::map<std::string, std::vector<std::string>> nodes = GetConnections(stmt_list);
    std::vector<graph*> graph_nodes;
    std::set<std::string> node_id_set;

    for(const auto& node : nodes) {
        node_id_set.insert(node.first);
        for(const auto& edge : node.second) {
            node_id_set.insert(edge);
        }
    }

    for(const auto& elm : node_id_set) {
        graph* temp = new graph();
        temp->isRoot = true;
        temp->visited = false;
        temp->id = elm;
        graph_nodes.push_back(temp);
    }

    for(const auto& elm : node_id_set) {
        graph* current_node;
        for(auto n : graph_nodes) {
            if(n->id == elm) {
                current_node = n;
            }
        }
        if(!current_node) {
            return false;
        }

        for(const auto& n : nodes) {
            if(n.first == current_node->id) {
                for(const auto& child_name : n.second) {
                    graph* child_node;
                    for(auto cn : graph_nodes) {
                        if(cn->id == child_name) {
                            child_node = cn;
                        }
                    }
                    if(!child_node) {
                        return false;
                    }

                    child_node->isRoot = false;
                    child_node->parents.insert(current_node);
                    current_node->children.insert(child_node);
                }
                break;
            }
        }
    }
    std::cout << std::endl;
    for(const auto& node : graph_nodes) {
        std::cout << node->id << ":" << std::endl;
        std::cout << "   Parents: " << node->parents.size() << std::endl;
        std::cout << "  Children: " << node->children.size() << std::endl;
        std::cout << "      Root: " << ((node->isRoot) ? "TRUE" : "FALSE") << std::endl;
    }
    graph* root;
    for(const auto& node : graph_nodes) {
        if(node->isRoot) {
            root = node;
            break;
        }
    }

    std::cout << std::endl;
    return Traverse(node_id_set, root);
}

std::map<std::string, std::vector<std::string>> GetConnections(chow::StatementList stmt_list) {
    std::map<std::string, std::vector<std::string>> nodes;

    chow::Statement* stmt1 = stmt_list.GetStatement();
    if(stmt1->GetType() == chow::StatementType::EDGE) {
        chow::EdgeStatement* temp1 =  dynamic_cast<chow::EdgeStatement*>(stmt1);
        chow::EdgeRhs* edgeRhs1 = temp1->mEdgeRhs;
        while(edgeRhs1) {
            nodes[temp1->mId->mName].push_back(edgeRhs1->mId->mName);
            edgeRhs1 = edgeRhs1->mEdgeRhs;
        }
    }

    chow::StatementList* new_stmt_list = stmt_list.GetStatementList();
    while(new_stmt_list) {
        chow::Statement* stmt = new_stmt_list->GetStatement();
        if(stmt->GetType() == chow::StatementType::NODE) {
            chow::NodeStatement* temp =  dynamic_cast<chow::NodeStatement*>(stmt);
        }else if(stmt->GetType() == chow::StatementType::EDGE) {
            chow::EdgeStatement* temp =  dynamic_cast<chow::EdgeStatement*>(stmt);
            chow::EdgeRhs* edgeRhs = temp->mEdgeRhs;
            while(edgeRhs) {
                nodes[temp->mId->mName].push_back(edgeRhs->mId->mName);
                edgeRhs = edgeRhs->mEdgeRhs;
            }
        }else if(stmt->GetType() == chow::StatementType::ATTR) {
            chow::AttrStatement* temp =  dynamic_cast<chow::AttrStatement*>(stmt);
        }else if(stmt->GetType() == chow::StatementType::IDASSIGNID) {
            chow::IdAssignId* temp =  dynamic_cast<chow::IdAssignId*>(stmt);
        }else if(stmt->GetType() == chow::StatementType::SUBGRAPH) {
            chow::Subgraph* temp =  dynamic_cast<chow::Subgraph*>(stmt);
        }else{
        }
        new_stmt_list = new_stmt_list->GetStatementList();
    }

    return nodes;
}
