#include <iostream>
#include <cstdio>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <cstring>
#include <map>
#include <bitset>
#include <string>
#include <cmath>
#include <set>

using namespace std;

const int INF = 1000000009;
const int MOD = 1000000007;

typedef long long LL;
typedef unsigned long long ULL;
typedef priority_queue <int> PQ;
typedef pair <int, int> II;
typedef priority_queue <II> PQII;
typedef map <int, int> MI;
typedef map <string, int> MSI;
typedef vector <int> VI;
typedef vector <VI> VVI;
typedef vector <LL> VL;
typedef vector <VL> VVL;
typedef vector <string> VS;
typedef vector < II > VII;

struct Edge {
   int u, v;
   int rev;  // index of reverse edge
   int cap, flow;
   Edge(int _u, int _v, int _rev = 0, int _cap = 1) :
     u(_u), v(_v), rev(_rev), cap(_cap), flow(0) {}
};

class MaxFlowDinic {
   int N;
   VI sp_par, sp_D;

   bool augmenting_path_bfs(int src, int dst, int mincap = 0) {
      sp_par.assign(N, -1);
      sp_D.assign(N, -1);

      sp_D[src] = 0;
      queue<int> q;
      q.push(src);
      while (!q.empty()) {
         int cur = q.front();
         q.pop();
         for (int j = 0; j < (int) adj[cur].size(); ++j) {
            int e = adj[cur][j];
            int v = edges[e].v;
            if (sp_D[v] < 0 && edges[e].cap - edges[e].flow > mincap) {
               sp_D[v] = sp_D[cur] + 1;
               sp_par[v] = e;
               q.push(v);
            }
         }
      }
      return sp_D[dst] >= 0;
   }

   VI last_edge;
   int update_flow_dfs(int u, int dst, int cfp = INF) {
      if (u == dst)
         return cfp;
      for (; last_edge[u] < adj[u].size(); ++last_edge[u]) {
         int e = adj[u][ last_edge[u] ];
         int u = edges[e].u;
         int v = edges[e].v;

         if (sp_D[v] == sp_D[u] + 1 && edges[e].cap - edges[e].flow > 0) {
            int res = update_flow_dfs(v, dst, min( cfp, edges[e].cap - edges[e].flow ));
            if (res > 0) {
               edges[e].flow += res;
               edges[ edges[e].rev ].flow = -edges[e].flow;  // -= cfp;
               return res;
            }
         }
      }
      return 0;
   }

public:
   vector<Edge> edges;
   VVI adj;  // adjacency lists of edge indices

   MaxFlowDinic(int _N) : N(_N), adj(VVI(_N)) {}
   
   void add_edge(int u, int v, int _cap=1) {
      int num_edges = edges.size();
      // forward edge
      edges.push_back( Edge(u, v, num_edges+1, _cap) );
      adj[u].push_back(num_edges++);
      // reverse edge
      edges.push_back( Edge(v, u, num_edges-1, 0) );
      adj[v].push_back(num_edges++);
   }

   int dinic(int s, int t) {
      N = int(adj.size());
      for (int j = 0; j < (int) edges.size(); ++j)
         edges[j].flow = 0;

      VI path;
      int res = 0;

      while ( augmenting_path_bfs(s, t) ) {
   		last_edge.assign(N, 0);
   		while (true) {
   		   int cfp = update_flow_dfs(s, t);
   		   if (cfp <= 0) break;
   		   res += cfp;
   		}
      }
   return res;
   }

   vector<Edge> mincut(int s, int t) {
      dinic(s, t);  
      vector<Edge> res;
      for (int k = 0; k < edges.size(); ++k) {
         const Edge& e = edges[k];
         if (e.cap > 0 && sp_D[e.u] >= 0 && sp_D[e.v] < 0) 
            res.push_back(e);
      }
      return res;
   }
};

map<string, int> MP;

int main() {
	ios_base::sync_with_stdio(false);
	int T;
	cin >> T;
	while(T--){
		int c = 1;
		int n, m;
		cin >> n >> m;
		MaxFlowDinic MD(m*n*4 +10);
		int arr[n][m];
		set<int> S;
		for(int i=0; i<n; i++){
			int k;
			cin >> k;
			for(int j=0; j<k; j++){
				int st;;
				cin >> st;
				arr[i][st]++;
				if(i == 0){
					S.insert(st);
				}
			}
			if(i > 0){
				int g = c+1;
				for(int j=0; j<m; j++){
					MD.add_edge(c, c+1, max(arr[i][j]-1, 0));
					c+=2;
				}
				for(int j=0; j<m; j++){
					int d = c;
					for(int z=0; z<m; z++){
						MD.add_edge(g, d, INF);
						d++;
					}
					g+=2;
				}
				int d = c;
				for(int j=0; j<m; j++){
					MD.add_edge(c, d+m+j, arr[i][j]>0? 0: 1);
					c++;
				}
				d = c;
				for(int j=0; j<m; j++){
					for(int z=0; z<m; z++){
						MD.add_edge(c, d+m+z, 1);
					}
					c++;
				}
			}else{
				for(int j=0; j<m; j++){
					MD.add_edge(0, c, arr[i][j]-1);
					c++;
				}
				for(int j=0; j<m; j++){
					for(int z=0; z<m; z++){
						MD.add_edge(j+1, m+1+z, 1);
					}
				}
			}
		}
		int d = c;
		for(int j=0; j<m; j++){
			MD.add_edge(c, d+m, 1);
			c++;
		}
		int  r = MD.dinic(0, c);
		cout << r+S.size() << endl;
	}
    return 0;
}

