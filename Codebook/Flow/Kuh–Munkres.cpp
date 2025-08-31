// Kuh–Munkres 帶權二分圖最大完美匹配
const int INF = 9e15;

struct KM {
    int n;
    vector<vector<int>> a;
    KM(int n=0): n(n), a(n+1, vector<int>(n+1, INF)) {}
    void reset(int nn){
        n = nn;
        a.assign(n+1, vector<int>(n+1, INF));
    }
    void set_cost(int i, int j, int cost){
        a[i][j] = cost;
    }

    // 回傳 pair<min_cost, matchR>, matchR[1..n]: 右 j 被配到的左節點 index
    pair<int, vector<int>> solve(){
        vector<int> u(n+1,0), v(n+1,0), minv(n+1);
        vector<int> p(n+1, 0), way(n+1, 0);
        for(int i=1;i<=n;i++){
            p[0] = i;
            int j0 = 0;
            fill(minv.begin(), minv.end(), INF);
            vector<char> used(n+1, false);
            do{
                used[j0] = true;
                int i0 = p[j0];
                int delta = INF;
                int j1 = 0;
                for(int j=1;j<=n;j++){
                    if(used[j]) continue;
                    int cur;
                    if(a[i0][j] >= INF/2) cur = INF;
                    else cur = a[i0][j] - u[i0] - v[j];
                    if(cur < minv[j]){
                        minv[j] = cur;
                        way[j] = j0;
                    }
                    if(minv[j] < delta){
                        delta = minv[j];
                        j1 = j;
                    }
                }
                for(int j=0;j<=n;j++){
                    if(used[j]){
                        u[p[j]] += delta;
                        v[j] -= delta;
                    }
                    else {
                        if(minv[j] < INF) minv[j] -= delta;
                    }
                }
                j0 = j1;
            } while(p[j0] != 0);
            do{
                int j1 = way[j0];
                p[j0] = p[j1];
                j0 = j1;
            } while(j0 != 0);
        }

        vector<int> matchR(n+1, 0);
        for(int j=1;j<=n;j++) matchR[j] = p[j];

        int total = 0;
        for(int j=1;j<=n;j++){
            int L = matchR[j];
            if(L == 0 || a[L][j] >= INF/2){
                return {INF, matchR}; //無解
            }
            total += a[L][j];
        }
        return {total, matchR};
    }
};

void sol(){
    int n;
    int m;
    cin >> n >> m;
    KM km(n);
    for(int k=0;k<m;k++){
        int y,c;
        int h;
        cin >> y >> c >> h;
        km.a[y][c] = min(km.a[y][c], -h);
    }

    auto res = km.solve();
    int min_cost = res.first;
    vector<int> matchR = res.second;
    int ans = -min_cost;
    cout << ans << "\n";
    for(int i=1;i<=n;i++){
        cout << matchR[i] << (i==n?'\n':' ');
    }
}
