// Hopcroft-Karp 無權二分圖最大匹配 O(E*sqrt(V))
struct HopcroftKarp {
    int n, m;
    vector<vector<int>> adj;
    vector<int> a, b, dist;
    const int INF = 1e9;

    HopcroftKarp() : n(0), m(0) {}
    HopcroftKarp(int n_, int m_) { init(n_, m_); }

    void init(int n_, int m_) {
        n = n_; m = m_;
        adj.assign(n + 1, {});
        a.assign(n + 1, 0);
        b.assign(m + 1, 0);
        dist.assign(n + 1, 0);
    }

    //u in [1..n], v in [1..m]
    void addEdge(int u, int v) {
        if (u < 1 || u > n || v < 1 || v > m) return;
        adj[u].push_back(v);
    }
    
    bool bfs() {
        queue<int> q;
        for (int u = 1; u <= n; ++u) {
            if (a[u] == 0) {
                dist[u] = 0;
                q.push(u);
            }
            else dist[u] = INF;
        }
        bool ok = false;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : adj[u]) {
                int pu = b[v];
                if(pu == 0) ok = true;
                else if(dist[pu] == INF){
                    dist[pu] = dist[u] + 1;
                    q.push(pu);
                }
            }
        }
        return ok;
    }

    bool dfs(int u) {
        for (int v : adj[u]) {
            int pu = b[v];
            if (pu == 0 || (dist[pu] == dist[u] + 1 && dfs(pu))) {
                a[u] = v;
                b[v] = u;
                return true;
            }
        }
        dist[u] = INF;
        return false;
    }

    int maxMatching() {
        int matching = 0;
        while (bfs()) {
            for (int u = 1; u <= n; ++u) {
                if (a[u] == 0) {
                    if (dfs(u)) matching++;
                }
            }
        }
        return matching;
    }
    void printMatch() {
    	for (int u = 1; u <= n; ++u) {
            if (a[u] != 0) {
                cout << u << " " << a[u] << "\n";
            }
        }
    }

};

void sol() {
    int n, m, e;
    cin >> n >> m >> e;

    vector<pair<int,int>> edges;
    edges.reserve(e);
    for (int i = 0; i < e; ++i) {
        int u, v;
        cin >> u >> v;
        edges.emplace_back(u, v);
    }

    sort(edges.begin(), edges.end());
    edges.erase(unique(edges.begin(), edges.end()), edges.end());

    HopcroftKarp hk(n, m);
    for (auto &pr : edges) hk.addEdge(pr.first, pr.second);

    int ans = hk.maxMatching();
    cout << ans << '\n';
}
