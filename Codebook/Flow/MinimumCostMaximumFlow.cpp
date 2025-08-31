//Minimum Cost Maximum Flow (SSP) 無法處理負邊
//求s到t的最大流量最小費用

struct MinCostMaxFlow {
    struct Edge {
        int to;
        int rev;
        int cap;
        int cost;
    };
    int n;
    vector<vector<Edge>> g;
    MinCostMaxFlow(int n=0){ init(n); }
    void init(int _n){
        n = _n;
        g.assign(n, {});
    }
    void addEdge(int u, int v, int cap, int cost){
        Edge a = {v, g[v].size(), cap, cost};
        Edge b = {u, g[u].size(), 0, -cost};
        g[u].push_back(a);
        g[v].push_back(b);
    }

    pair<int,int> minCostMaxFlow(int s, int t){
        const int INFint = (1LL<<60);
        int flow = 0, flowCost = 0;
        vector<int> p(n, 0);
        vector<int> dis(n);
        vector<int> pv(n), pe(n);

        while(1){
            fill(dis.begin(), dis.end(), INFint);
            dis[s] = 0;
            priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
            pq.push({0, s});
            while (!pq.empty()) {
                auto [d,u] = pq.top();
                pq.pop();
                if (d != dis[u]) continue;
                for (int ei = 0; ei < g[u].size(); ++ei) {
                    const Edge &e = g[u][ei];
                    if (e.cap <= 0) continue;
                    int nd = d + e.cost + p[u] - p[e.to];
                    if (nd < dis[e.to]) {
                        dis[e.to] = nd;
                        pv[e.to] = u;
                        pe[e.to] = ei;
                        pq.push({nd, e.to});
                    }
                }
            }
            if (dis[t] == INFint) break;

            for (int v = 0; v < n; ++v) {
                if (dis[v] < INFint) p[v] += dis[v];
                else p[v] += dis[t];
            }

            int addf = INT_MAX;
            for (int v = t; v != s; v = pv[v]) {
                Edge &e = g[pv[v]][pe[v]];
                addf = min(addf, e.cap);
            }

            for (int v = t; v != s; v = pv[v]) {
                Edge &e = g[pv[v]][pe[v]];
                e.cap -= addf;
                g[v][e.rev].cap += addf;
            }

            int addCost = addf * p[t];
            flow += addf;
            flowCost += addCost;
        }
        return {flow, flowCost};
    }
};

void sol(){
    int n, m, s, t;
    cin >> n >> m >> s >> t;
    MinCostMaxFlow mcmf(n+1);
    for (int i = 0; i < m; ++i){
        int u,v,w,c;
        cin >> u >> v >> w >> c;
        mcmf.addEdge(u, v, w, c);
    }
    auto res = mcmf.minCostMaxFlow(s, t);
    cout << res.first << " " << res.second << '\n';
}
