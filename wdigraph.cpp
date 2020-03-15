#include "wdigraph.h"
#include <unordered_map>
#include "digraph.h"
long long WDigraph::getCost(int u, int v) const {
    // uses .at because there is no const operator[]
    // for unordered maps
    return cost.at(u).at(v);
 }
void WDigraph::addEdge(int u, int v, long long w){
    // use Digraph's addEdge method
    Digraph::addEdge(u, v);
    cost[u][v] = w;
}