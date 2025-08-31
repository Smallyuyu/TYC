//找割點、Bridge

struct Tarjan {
    int n;
    vector<vector<pair<int,int>>> adj;

    vector<int> tin, low;
    vector<char> vis;
    int timer;

    vector<char> isArt;
    vector<pair<int,int>> bridges;
    int cnt;

    Tarjan(): n(0), timer(0), cnt(0) {}

    void init(int _n){
        n = _n;
        adj.assign(n+1, {});
        tin.assign(n+1, -1);
        low.assign(n+1, -1);
        vis.assign(n+1, 0);
        isArt.assign(n+1, 0);
        bridges.clear();
        timer = 0;
        cnt = 0;
    }

    void add_edge(int u, int v){
        adj[u].push_back({v, cnt});
        adj[v].push_back({u, cnt});
        ++cnt;
    }

    void dfs(int u, int par_edge){
        vis[u] = 1;
        tin[u] = low[u] = ++timer;

        int child = 0;
        for (auto x : adj[u]) {
        	int v = x.first;
        	int eid = x.second;
            if (eid == par_edge) continue;
            if (vis[v]) low[u] = min(low[u], tin[v]);
            else{
                ++child;
                dfs(v, eid);
                low[u] = min(low[u], low[v]);

                if (low[v] > tin[u]) {
                    int a = u, b = v;
                    if (a > b) swap(a,b);
                    bridges.push_back({a,b});
                }
                
                if (low[v] >= tin[u]) {
                    isArt[u] = 1;
                }
            }
        }
    }

    void run(){
        for (int i = 1; i <= n; ++i) {
            vis[i] = 0;
            tin[i] = low[i] = -1;
            isArt[i] = 0;
        }
        bridges.clear();
        timer = 0;

        for (int i = 1; i <= n; ++i) {
            if (!vis[i]) {
                vis[i] = 1;
                tin[i] = low[i] = ++timer;

                int root = i;
                int root_child = 0;
                for (auto x : adj[root]) {
                	int v = x.first;
        			int eid = x.second;
                    if (vis[v]) low[root] = min(low[root], tin[v]);
                    else{
                        ++root_child;
                        dfs(v, eid);
                        low[root] = min(low[root], low[v]);
                        if (low[v] > tin[root]) {
                            int a = root;
                            int b = v;
                            if (a > b) swap(a,b);
                            bridges.push_back({a,b});
                        }
                    }
                }
                if (root_child >= 2) isArt[root] = 1;
            }
        }
        sort(bridges.begin(), bridges.end());
        bridges.erase(unique(bridges.begin(), bridges.end()), bridges.end());
    }

    vector<int> get_points() const {
        vector<int> res;
        for (int i = 1; i <= n; ++i) if (isArt[i]) res.push_back(i);
        sort(res.begin(), res.end());
        return res;
    }

    vector<pair<int,int>> get_bridges() const {
        return bridges;
    }
};


void sol() {
    int n, m;
    cin >> n >> m;
    Tarjan tr;
    tr.init(n);
    for (int i = 0; i < m; ++i) {
        int x, y;
        cin >> x >> y;
        tr.add_edge(x, y);
    }

    tr.run();
    auto arts = tr.get_points();

    cout << arts.size() << '\n';
    for (size_t i = 0; i < arts.size(); ++i) {
        cout << arts[i] <<" ";
    }
    cout << '\n';
    /*
    auto brs  = tr.get_bridges();
    cout << "bridges: " << brs.size() << '\n';
    for (auto &e : brs) {
        cout << e.first << ' ' << e.second << '\n';
    }
    */
}
