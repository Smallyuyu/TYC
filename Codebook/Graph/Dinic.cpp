//求點s to t的最大流

struct Dinic {
    struct Edge { int to; int rev; int cap; };
    int n;
    vector<vector<Edge>> g;
    vector<int> level, ptr;
    const int INF = 4e18;

    Dinic(int n=0) { init(n); }
    void init(int _n) {
        n = _n;
        g.assign(n, {});
        level.assign(n, -1);
        ptr.assign(n, 0);
    }

    void add_edge(int u, int v, int c) {
        Edge a = {v, g[v].size(), c};
        Edge b = {u, g[u].size(), 0};
        g[u].push_back(a);
        g[v].push_back(b);
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
        for (int &cid = ptr[u]; cid < g[u].size(); ++cid) {
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

void sol() {
    int n, m, s, t;
    cin >> n >> m >> s >> t;
    Dinic dinic(n);
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        dinic.add_edge(u-1, v-1, w);
    }
    int ans = dinic.maxflow(s-1, t-1);
    cout << ans << '\n';
}
