#include <map>
#include <algorithm>
#include <utility>
#include <iostream>
#include "heap.h"
#include "wdigraph.h"

using namespace std;

typedef pair<int, long long> PIL;

void dijkstra(const WDigraph& graph, int startVertex, unordered_map<int, PIL>& tree){

	BinaryHeap<PIL, int> events;
	int temp = 0;
	PIL pair1(startVertex, startVertex);
	PIL edge;
	int time;
	HeapItem<PIL, int> item;
	events.insert(pair1,temp);
	while(events.size() != 0){
		item = events.min();
		time = item.key;
		edge = item.item;
		int u = edge.first;
		int v = edge.second;
		events.popMin();
		if(tree.find(v) == tree.end()){
			tree[v] = edge; // reached[v] <- u
			for(auto iter = graph.neighbours(v); iter != graph.endIterator(v); iter++)
			{
				PIL pair2(v, *iter);
				events.insert(pair2, time+ graph.getCost(v, *iter));
			}
		}
	}

}