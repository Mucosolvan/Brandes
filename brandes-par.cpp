#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <queue>
#include <stack>
#include <map>
#include <atomic>
#include <thread>

using namespace std;

ifstream fileIn;
ofstream fileOut;
string line;
int from, to, threadCounter;

vector<int> allVertices, inVertices, reverseIds;
map<int, int> ids;
vector<vector<int>> graph;
atomic<double> *BC;

atomic<int> counter;
int vertexCounter = 1, inVertexCounter = 0;

void addTo(atomic<double> &first, double second) {
	auto cur = first.load();
	while (!first.compare_exchange_weak(cur, cur + second));
}

void brandes() {
	vector<int> sigma(vertexCounter);
	vector<int> dist(vertexCounter);
	vector<double> delta(vertexCounter);
	vector<vector<int>> P(vertexCounter);
	stack<int> S;
		
	while (true) {
		int startV = counter++;
		if (startV >= inVertexCounter)
			return;
			
		startV = inVertices[startV];
		
		for (auto v : allVertices) {
			P[v] = vector<int>();
			sigma[v] = 0;
			dist[v] = -1;
			delta[v] = 0;
		}
		
		sigma[startV] = 1;
		dist[startV] = 0;
		queue<int> Q;
		Q.push(startV);
		
		while (!Q.empty()) {
			int v = Q.front();
			S.push(v);
			Q.pop();
			
			for (auto w : graph[v]) {
				if (dist[w] < 0) {
					Q.push(w);
					dist[w] = dist[v] + 1;
				}
				
				if (dist[w] == dist[v] + 1) {
					sigma[w] += sigma[v];
					P[w].push_back(v);
				}
			}
		}
		
		while (!S.empty()) {
			int v = S.top();
			S.pop();
			
			for (auto w : P[v]) {
				delta[w] += ((double)sigma[w] / sigma[v]) * (1 + delta[v]);
			}
			
			if (v != startV) {
				addTo(BC[v], delta[v]);
			}
		}
	}
} 


int main(int argc, char* argv[]) {
	
	if (argc != 4) {
		cout<<"Wrong number of arguments!\n";
		return 0;
	}
	
	threadCounter = atoi(argv[1]);
	fileIn.open(argv[2]);
	fileOut.open(argv[3]);
	
	if (fileIn.is_open() && fileOut.is_open()) {
		int prevFrom = -1;
		graph.push_back(vector<int>());
		reverseIds.push_back(0);
		
		while (fileIn >> from >> to) {
			if (ids[from] == 0) {
				ids[from] = vertexCounter;
				allVertices.push_back(vertexCounter);
				vertexCounter++;
				graph.push_back(vector<int>());
			}
			
			if (from != prevFrom) {
				inVertexCounter++;
				inVertices.push_back(ids[from]);
			}
					
			if (ids[to] == 0) {
				ids[to] = vertexCounter;
				allVertices.push_back(vertexCounter);
				vertexCounter++;
				graph.push_back(vector<int>());
			}
			
			graph[ids[from]].push_back(ids[to]);
			prevFrom = from;
		}
		
		BC = new atomic<double>[vertexCounter];
		reverseIds.reserve(ids.size() + 1);
		
		for (auto p : ids) {
			reverseIds[p.second] = p.first;
		}
		
		for (int i = 0; i < vertexCounter; i++) {
			BC[i] = 0;
		}
		
		vector<thread> threads;
		
		for (int i = 0; i < threadCounter; i++) {
			threads.push_back(thread {[] {brandes();}});
		}

		for (auto &t : threads) {
			t.join();
		}
			
		for (auto v : inVertices) {
			fileOut << reverseIds[v] << " " << BC[v] << "\n";
		}
		delete[] BC;
		fileIn.close();
		fileOut.close();
	} else {
		cout<<"Unable to open file(s)\n";
	}
	return 0;
}

