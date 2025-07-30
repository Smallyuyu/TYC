struct SegTree {
    int n;
    vector<int> cnt;
    vector<int> len;
    vector<int> xs;

    SegTree(const vector<int>& _xs): xs(_xs) {
        n = xs.size() - 1;
        cnt.assign(4*n, 0);
        len.assign(4*n, 0);
    }

    void update(int p, int l, int r, int L, int R, int v){
        if(R <= l || r <= L) return;
        if(L <= l && r <= R) cnt[p] += v;
        else{
            int m = (l + r) >> 1;
            update(p<<1, l, m, L, R, v);
            update(p<<1|1, m, r, L, R, v);
        }
        if(cnt[p] > 0) len[p] = xs[r] - xs[l];
        else if(l+1 == r) len[p] = 0;
        else len[p] = len[p<<1] + len[p<<1|1];
    }

    void update(int xL, int xR, int v) {
        int L = lower_bound(xs.begin(), xs.end(), xL) - xs.begin();
        int R = lower_bound(xs.begin(), xs.end(), xR) - xs.begin();
        update(1, 0, n, L, R, v);
    }

    int query_total_covered() const {
        return len[1];
    }
};

signed main(){
    int n;
    cin >> n;
    struct Event { int y, x1, x2; int v; };
    vector<Event> ev;
    ev.reserve(2*n);

    vector<int> xs;
    xs.reserve(2*n);

    for(int i = 0; i < n; i++){
        int x1,y1,x2,y2;
        cin >> x1 >> y1 >> x2 >> y2;
        ev.push_back({y1, x1, x2, +1});
        ev.push_back({y2, x1, x2, -1});
        xs.push_back(x1);
        xs.push_back(x2);
    }

    sort(xs.begin(), xs.end());
    xs.erase(unique(xs.begin(), xs.end()), xs.end());

    SegTree st(xs);

    sort(ev.begin(), ev.end(), [](auto &a, auto &b){
        return a.y < b.y;
    });

    int area = 0;
    for(int i = 0; i+1 < (int)ev.size(); i++){
        st.update(ev[i].x1, ev[i].x2, ev[i].v);
        int dy = ev[i+1].y - ev[i].y;
        int coverW = st.query_total_covered();
        area += coverW * dy;
    }

    cout << area << "\n";
    return 0;
}
