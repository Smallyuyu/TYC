//Gomory-Hu Tree 最小割樹，求任兩點的最小割

struct Dinic {
    struct Edge { int to; int rev; int cap; int orig; };
    int n;
    vector<vector<Edge>> g;
    vector<int> level, ptr;
    const int INF = INT_MAX;

    Dinic(int n=0) { init(n); }
    void init(int _n) {
        n = _n;
        g.assign(n, {});
        level.assign(n, -1);
        ptr.assign(n, 0);
    }

    void add_edge(int u, int v, int c) {
        Edge a = {v, (int)g[v].size(), c, c};
        Edge b = {u, (int)g[u].size(), 0, 0};
        g[u].push_back(a);
        g[v].push_back(b);
    }

    void reset() {
        for (int u = 0; u < n; ++u)
            for (auto &e : g[u])
                e.cap = e.orig;
        fill(level.begin(), level.end(), -1);
        fill(ptr.begin(), ptr.end(), 0);
    }

    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        level[s] = 0;
        q.push(s);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (const Edge &e : g[u]) {
                if (e.cap > 0 && level[e.to] == -1) {
                    level[e.to] = level[u] + 1;
                    q.push(e.to);
                }
            }
        }
        return level[t] != -1;
    }

    int dfs(int u, int t, int pushed) {
        if (u == t || pushed == 0) return pushed;
        for (int &cid = ptr[u]; cid < (int)g[u].size(); ++cid) {
            Edge &e = g[u][cid];
            if (e.cap > 0 && level[e.to] == level[u] + 1) {
                int tr = dfs(e.to, t, min(pushed, e.cap));
                if (tr > 0) {
                    e.cap -= tr;
                    g[e.to][e.rev].cap += tr;
                    return tr;
                }
            }
        }
        return 0;
    }

    int maxflow(int s, int t) {
        int flow = 0;
        while (bfs(s, t)) {
            fill(ptr.begin(), ptr.end(), 0);
            while (true) {
                int pushed = dfs(s, t, INF);
                if (pushed == 0) break;
                flow += pushed;
            }
        }
        return flow;
    }
};

struct GomoryHu {
    int n;
    vector<tuple<int,int,int>> edges;
    vector<vector<pair<int,int>>> tree;
    vector<int> parent;
    vector<int> capToParent;

    int LOG;
    vector<vector<int>> up;
    vector<vector<int>> minUp;
    vector<int> depth;

    GomoryHu() : n(0) {}
    GomoryHu(int _n, const vector<tuple<int,int,int>>& _edges) {
        init(_n, _edges);
    }

    void init(int _n, const vector<tuple<int,int,int>>& _edges) {
        n = _n;
        edges = _edges;
        tree.assign(n, {});
        parent.assign(n, 0);
        capToParent.assign(n, 0);
    }

    void build() {
        tree.assign(n, {});
        Dinic flow(n);
        for (auto &e: edges) {
            int u,v,w; tie(u,v,w) = e;
            flow.add_edge(u, v, w);
            flow.add_edge(v, u, w);
        }

        parent.assign(n, 0);
        capToParent.assign(n, 0);

        for (int i = 1; i < n; ++i) {
            flow.reset();
            int f = flow.maxflow(i, parent[i]);
            capToParent[i] = f;
            tree[i].push_back({parent[i], f});
            tree[parent[i]].push_back({i, f});
            for (int j = i + 1; j < n; ++j) {
                if (parent[j] == parent[i] && flow.level[j] != -1) {
                    parent[j] = i;
                }
            }
        }
        preprocess_lca();
    }

    void preprocess_lca() {
        LOG = 1;
        while ((1<<LOG) <= n) ++LOG;
        up.assign(LOG, vector<int>(n, -1));
        minUp.assign(LOG, vector<int>(n, INT_MAX));
        depth.assign(n, -1);

        function<void(int,int)> dfs = [&](int u, int p) {
            for (auto [v,w] : tree[u]) {
                if (v == p) continue;
                depth[v] = depth[u] + 1;
                up[0][v] = u;
                minUp[0][v] = w;
                dfs(v, u);
            }
        };

        depth[0] = 0;
        up[0][0] = -1;
        minUp[0][0] = INT_MAX;
        dfs(0, -1);

        for (int k = 1; k < LOG; ++k) {
            for (int v = 0; v < n; ++v) {
                if (up[k-1][v] == -1) {
                    up[k][v] = -1;
                    minUp[k][v] = INT_MAX;
                } else {
                    up[k][v] = up[k-1][ up[k-1][v] ];
                    if (up[k][v] == -1) minUp[k][v] = minUp[k-1][v];
                    else minUp[k][v] = min(minUp[k-1][v], minUp[k-1][ up[k-1][v] ]);
                }
            }
        }
    }

    int mincut(int u, int v) {
        if (u == v) return INT_MAX;
        int ans = INT_MAX;
        if (depth[u] < depth[v]) swap(u, v);
        int diff = depth[u] - depth[v];
        for (int k = 0; k < LOG; ++k) if (diff & (1<<k)) {
            ans = min(ans, minUp[k][u]);
            u = up[k][u];
        }
        if (u == v) return ans;
        for (int k = LOG-1; k >= 0; --k) {
            if (up[k][u] != -1 && up[k][u] != up[k][v]) {
                ans = min(ans, minUp[k][u]);
                ans = min(ans, minUp[k][v]);
                u = up[k][u];
                v = up[k][v];
            }
        }
        ans = min(ans, minUp[0][u]);
        ans = min(ans, minUp[0][v]);
        return ans;
    }

    const vector<vector<pair<int,int>>>& get_tree() const { return tree; }
};

int main() {
    int n_input, m;
    cin >> n_input >> m;
    int N = n_input + 1;

    vector<tuple<int,int,int>> edges;
    edges.reserve(m);
    for (int i = 0; i < m; ++i) {
        int u,v,w;
        cin >> u >> v >> w;
        edges.emplace_back(u, v, w);
    }

    GomoryHu gh;
    gh.init(N, edges);
    gh.build();

    int q; cin >> q;
    while (q--) {
        int u,v;
        cin >> u >> v;
        cout << gh.mincut(u, v) << '\n';
    }
    return 0;
}
