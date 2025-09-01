//Minimum Cost Maximum Flow (SPFA) 可處理負邊
//求s到t的最大流量最小費用

struct MinCostMaxFlowSPFA {
    struct Edge {
        int to;
        int rev;
        int cap;
        int cost;
    };
    int n;
    vector<vector<Edge>> g;
    MinCostMaxFlowSPFA(int n=0){ init(n); }
    void init(int _n){
        n = _n;
        g.assign(n+1, {});
    }

    void addEdge(int u, int v, int cap, int cost){
        Edge a = {v, (int)g[v].size(), cap, cost};
        Edge b = {u, (int)g[u].size(), 0, -cost};
        g[u].push_back(a);
        g[v].push_back(b);
    }

    pair<int,int> minCostMaxFlow(int s, int t){
        const int INFint = (1LL<<60);
        int flow = 0;
        int flowCost = 0;

        vector<int> dis(n+1);
        vector<int> pv(n+1), pe(n+1);
        vector<char> inq(n+1);

        while(1){
            fill(dis.begin(), dis.end(), INFint);
            fill(inq.begin(), inq.end(), 0);
            queue<int> q;
            dis[s] = 0;
            q.push(s); inq[s] = 1;
            while (!q.empty()) {
                int u = q.front(); q.pop();
                inq[u] = 0;
                for (int ei = 0; ei < g[u].size(); ++ei) {
                    const Edge &e = g[u][ei];
                    if (e.cap <= 0) continue;
                    int v = e.to;
                    int nd = dis[u] + e.cost;
                    if (nd < dis[v]) {
                        dis[v] = nd;
                        pv[v] = u;
                        pe[v] = ei;
                        if (!inq[v]) { q.push(v); inq[v] = 1; }
                    }
                }
            }

            if (dis[t] == INFint) break;

            int addf = LLONG_MAX;
            for (int v = t; v != s; v = pv[v]) {
                const Edge &e = g[pv[v]][pe[v]];
                addf = min(addf, e.cap);
            }
            if (addf == 0 || addf == LLONG_MAX) break;

            for (int v = t; v != s; v = pv[v]) {
                Edge &e = g[pv[v]][pe[v]];
                e.cap -= addf;
                g[v][e.rev].cap += addf;
            }

            flow += addf;
            flowCost += addf * dis[t];
        }

        return {flow, flowCost};
    }
};

void sol(){
    int n, m, s, t;
    cin >> n >> m >> s >> t;
    MinCostMaxFlowSPFA mcmf(n);
    for (int i = 0; i < m; ++i) {
        int u,v;
        int w,c;
        cin >> u >> v >> w >> c;
        mcmf.addEdge(u, v, w, c);
    }
    auto res = mcmf.minCostMaxFlow(s, t);
    cout << res.first << " " << res.second << '\n';
}
