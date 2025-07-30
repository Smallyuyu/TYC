template<class Info>
struct SegmentTree {
    int n;
    vector<Info> info, lazy;

    SegmentTree() : n(0) {}

    SegmentTree(int n_, Info v_ = Info()) {
        init(n_, v_);
    }

    void init(int n_, Info v_ = Info()) {
        init(vector<Info>(n_, v_));
    }

    template<class T>
    SegmentTree(vector<T> init_) {
        init(init_);
    }

    template<class T>
    void init(vector<T> init_) {
        n = init_.size();
        info.assign(4 << __lg(n), 0);
        lazy.assign(4 << __lg(n), 0);
        function<void(int, int, int)> build = [&](int p, int l, int r) {
            if (r - l == 1) {
                info[p] = init_[l];
                return;
            }
            int m = (l + r) / 2;
            build(2 * p, l, m);
            build(2 * p + 1, m, r);
            pull(p);
        };
        build(1, 0, n);
    }

    void pull(int p) {
        info[p] = info[2 * p] + info[2 * p + 1];
    }

    void push(int p, int l, int r) {
        if (lazy[p] != 0) {
            int m = (l + r) / 2;
            // 將懶標記用到子節點
            lazy[2 * p] += lazy[p];
            lazy[2 * p + 1] += lazy[p];
            // 更新當前區間的值
            info[2 * p] += lazy[p] * (m - l);
            info[2 * p + 1] += lazy[p] * (r - m);
            lazy[p] = 0; // 清空懶標記
        }
    }

    void rangeModify(int p, int l, int r, int x, int y, Info v) {
        if (l >= y || r <= x) return;
        if (l >= x && r <= y){
            lazy[p] += v;
            info[p] += v * (r - l); // 更新當前區間的值
            return;
        }
        push(p, l, r); // 向下傳遞懶標記
        int m = (l + r) / 2;
        rangeModify(2 * p, l, m, x, y, v);
        rangeModify(2 * p + 1, m, r, x, y, v);
        pull(p); // 更新父節點
    }

    void rangeModify(int l, int r, Info v) {
        rangeModify(1, 0, n, l, r, v);
    }

    Info rangeQuery(int p, int l, int r, int x, int y) {
        if (l >= y || r <= x) return 0;
        if (l >= x && r <= y) return info[p];
        push(p, l, r); // 向下傳遞懶標記
        int m = (l + r) / 2;
        return rangeQuery(2 * p, l, m, x, y) + rangeQuery(2 * p + 1, m, r, x, y);
    }

    Info rangeQuery(int l, int r) {
        return rangeQuery(1, 0, n, l, r);
    }
};

signed main() {
    int n, q;
    cin >> n >> q;
    vector<int> arr(n, 0);
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }
    SegmentTree<int> sgT(arr);
	//l,r: index從1開始
    while (q--) {
        int op;
        cin >> op;
        if (op == 1) { // 區間加值
            int l, r, x;
            cin >> l >> r >> x;
            sgT.rangeModify(l - 1, r, x);
        } else if (op == 2) { // 區間和查詢
            int l,r;
            cin >> l >> r;
            cout << sgT.rangeQuery(l - 1, r) << '\n';
        }
    }
    return 0;
}
