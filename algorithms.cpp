#include "algorithms.h"
#include <map>
#include <queue>

#include <iostream>
#include <algorithm>

using namespace std;

vector<Graph::Node> BFSTraversal(Graph::Node start, map<Graph::Node, vector<Graph::Edge>> & adj_list, map<Graph::Node, bool> & visited) {
    vector<Graph::Node> result;
    queue<Graph::Node> q;
    q.push(start);
    visited.at(start) = true;
    
    while (!q.empty()) {
        Graph::Node curr = q.front();
        q.pop();
        result.push_back(curr);
        for (size_t i = 0; i < adj_list.at(curr).size(); i++) {
            if (!visited.at(adj_list.at(curr)[i].end_)) {
                q.push(adj_list.at(curr)[i].end_);
                visited.at(adj_list.at(curr)[i].end_) = true;
            }
        }
    }
    return result;
}

vector<vector<Graph::Node>> BFSTraversal(Graph::Node start, Graph * graph) {
    map<Graph::Node, bool> visited;
    map<Graph::Node, vector<Graph::Edge>> adj_list = graph->getAdjacencyList();
    for (auto it = adj_list.begin(); it != adj_list.end(); ++it) {
        visited.insert({it->first, false});
    }

    vector<vector<Graph::Node>> result;
    result.push_back(BFSTraversal(start, adj_list, visited));

    for (auto it = visited.begin(); it != visited.end(); ++it) {
        if (!(it->second)) {
            result.push_back(BFSTraversal(it->first, adj_list, visited));
        }
    }
    
    return result;
 }

 vector<vector<Graph::Node>> kosaraju(Graph * graph) {
    // setup
    vector<vector<Graph::Node>> result;
    stack<Graph::Node> s;

    map<Graph::Node, bool> visited;
    map<Graph::Node, vector<Graph::Edge>> adj_list = graph->getAdjacencyList();
    for (auto it = adj_list.begin(); it != adj_list.end(); ++it) {
        visited.insert({it->first, false});
    }

    map<Graph::Node, bool> reverse_visited;
    map<Graph::Node, vector<Graph::Edge>> reverse_adj_list = reverseAdjacencyList(adj_list);
    // for (auto it = reverse_adj_list.begin(); it != reverse_adj_list.end(); ++it) {
    //     cout << "key: " << it->first << " edges: ";
    //     for (Graph::Edge edge : it->second) {
    //         cout << edge.end_ << " ";
    //     }
    //     cout << endl;
    // }
    for (auto it = reverse_adj_list.begin(); it != reverse_adj_list.end(); ++it) {
        reverse_visited.insert({it->first, false});
    }

    // Start of Kosaraju's algorithm
    for (auto it = visited.begin(); it != visited.end(); ++it) {
        if (!it->second) {
            vector<Graph::Node> placeholder;
            // cout << it->first << "node" << endl;
            DFS(it->first, s, adj_list, visited, true, placeholder);
        }
        
    }
    // cout << endl;

    while (!s.empty()) {
        Graph::Node currNode = s.top();
        s.pop();

        // cout << "Reverse Visited: " << endl;
        // for (auto it = reverse_visited.begin(); it != reverse_visited.end(); ++it) {
        //     cout << it->first << ": " << it->second << endl;;
        // }
        // cout << "rev currNode: " << currNode << endl;
        if (!reverse_visited.at(currNode)) {
            vector<Graph::Node> scc;
            DFS(currNode, s, reverse_adj_list, reverse_visited, false, scc);
            sort(scc.begin(), scc.end());
            result.push_back(scc);
        }
    }

    return result;
 }

map<Graph::Node, vector<Graph::Edge>> reverseAdjacencyList(map<Graph::Node, vector<Graph::Edge>> & adj_list) {
    map<Graph::Node, vector<Graph::Edge>> reversed;
    for (auto it = adj_list.begin(); it != adj_list.end(); ++it) {
        reversed.insert({it->first, vector<Graph::Edge>(0)});
    }
    for (auto it = adj_list.begin(); it != adj_list.end(); ++it) {
        for (Graph::Edge edge : it->second) {
            Graph::Node start = edge.start_;
            Graph::Node end = edge.end_;
            if (!(reversed.count(end))) { // key doesnt exists
                reversed.insert({end, vector<Graph::Edge>(0)});
            }
            Graph::Edge e;
            e.start_ = end;
            e.end_ = start;
            reversed.at(end).push_back(e);
        }
    }

    return reversed;
}

void DFS(Graph::Node node, stack<Graph::Node> & s, map<Graph::Node, vector<Graph::Edge>> & adj_list, map<Graph::Node, bool> & visited, bool isFirst, vector<Graph::Node> & scc) {
    // cout << "dfs visited for node: " << node << endl;
    visited.at(node) = true;
    // cout << "DFS Node: " << node << endl;

    if (!isFirst) {
        scc.push_back(node);
    }

    vector<Graph::Edge> edges = adj_list.at(node);
    // cout << "Edges: ";
    // for (Graph::Edge edge : edges) {
    //     cout << edge.end_ << " ";
    // }
    // cout << endl;
    // cout << "Visited: " << endl;
    // for (auto it = visited.begin(); it != visited.end(); ++it) {
    //     cout << it->first << ": " << it->second << endl;;
    // }
    for (size_t i = 0; i < edges.size(); i++) {
        Graph::Node currNeighbor = edges[i].end_;
        // cout << "currNeighbor: " << currNeighbor << endl;
        if (!visited.at(currNeighbor)) {
            // cout << "i: " << i << endl;
            DFS(currNeighbor, s, adj_list, visited, isFirst, scc);
        }
    }
    
    if (isFirst) {
        s.push(node);
    }
    // cout << "end curr dfs cycle" << endl;
}

map<Graph::Node, float> betweennessCentrality(Graph * graph) {
    map<Graph::Node, float> betweenness;
    map<Graph::Node, vector<Graph::Edge>> adj_list = graph->getAdjacencyList();
    for (auto it = adj_list.begin(); it != adj_list.end(); ++it) {
        betweenness.insert({it->first, 0.0});
    }
    int num_paths = 0;
    int count = 0;
    for (auto start = betweenness.begin(); start != betweenness.end(); ++start) {
        map<Graph::Node, vector<Graph::Node>> paths_from = shortestPath(start->first, graph);
        if (count == 0) {
            for (auto it = paths_from.begin(); it != paths_from.end(); ++it) {
                std::cout << it->first;
                for (Graph::Node node : it->second) {
                    std::cout << node;
                }
                std::cout << std::endl;
            }
        }
        for (auto it = paths_from.begin(); it != paths_from.end(); ++it) {
            if (it->second.size() == 2) {
                num_paths++;
            } else if (it->second.size() > 2) {
                num_paths++;
                for (unsigned index = 1; index < it->second.size() - 1; index++) {
                    betweenness.at(it->second[index])++;
                }
            }
        }
        count++;
    }

    for (auto start = betweenness.begin(); start != betweenness.end(); ++start) {
        start->second /= num_paths;
    }

    return betweenness;
}   

map<Graph::Node, vector<Graph::Node>> shortestPath(Graph::Node start, Graph * graph){
    map<Graph::Node, bool> visited;
    map<Graph::Node, vector<Graph::Edge>> adj_list = graph->getAdjacencyList();
    vector<Graph::Node> path;
    map<Graph::Node, vector<Graph::Node>> all_paths;
    for (auto it = adj_list.begin(); it != adj_list.end(); ++it) {
        visited.insert({it->first, false});
        all_paths.insert({it->first, vector<Graph::Node>()});
    }
    
    queue<Graph::Node> q;
    q.push(start);
    visited.at(start) = true;
    
    while (!q.empty()) {
        Graph::Node curr = q.front();
        q.pop();
        for (size_t i = 0; i < adj_list.at(curr).size(); i++) {
            if (!visited.at(adj_list.at(curr)[i].end_)) {
                visited.at(adj_list.at(curr)[i].end_) = true;
                q.push(adj_list.at(curr)[i].end_);
                for (unsigned j = 0 ; j < all_paths.at(curr).size(); j++) {
                    all_paths.at(adj_list.at(curr)[i].end_).push_back(all_paths.at(curr)[j]);
                }
                all_paths.at(adj_list.at(curr)[i].end_).push_back(curr);
            }
        }
    }

    for (auto it = all_paths.begin(); it != all_paths.end(); ++it) {
        if (it->second.size() > 0) {
            it->second.push_back(it->first);
        }
    }
    
    return all_paths;
}
