#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <queue>
#include <stack>
#include <map>
#include <set>

using namespace std;

ifstream fileIn;
ofstream fileOut;
string line;
int from, to;


set<int> allVertices, inVertices;
map<int, vector<int>> graph, P;
map<int, int> sigma, dist;
map<int, double> BC, delta;

void brandes(int startV) {
	stack<int> S;
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
		
		if (v != startV)
			BC[v] += delta[v];
	}
}

int main(int argc, char* argv[]) {
	fileIn.open(argv[1]);
	fileOut.open(argv[2]);
	if (fileIn.is_open() && fileOut.is_open()) {
		while (fileIn >> from >> to) {
			cout<<from<<" "<<to<<endl;
			allVertices.insert(from);
			allVertices.insert(to);
			inVertices.insert(from);
			graph[from].push_back(to);
		}
		
		for (auto v : inVertices)
			brandes(v);
			
		for (auto v : inVertices)
			fileOut << v << " " << BC[v] << "\n";
		fileIn.close();
		fileOut.close();
	} else {
		cout<<"Unable to open file(s)\n";
	}
	return 0;
}

