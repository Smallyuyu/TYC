struct SegmentTree {
    int n;
    int mod;
    vector<int> sum, lazyMul, lazyAdd;

    SegmentTree(const vector<int>& a, int m) : n(a.size()), mod(m) {
        sum.assign(4*n, 0);
        lazyMul.assign(4*n, 1);
        lazyAdd.assign(4*n, 0);
        build(1, 0, n, a);
    }

    void build(int p, int l, int r, const vector<int>& a) {
        if (r - l == 1) {
            sum[p] = a[l] % mod;
            return;
        }
        int m = (l + r) / 2;
        build(p*2, l, m, a);
        build(p*2+1, m, r, a);
        pull(p);
    }

    void pull(int p) {
        sum[p] = (sum[p*2] + sum[p*2+1]) % mod;
    }

    void applyMul(int p, int v, int l, int r) {
        sum[p] = sum[p] * v % mod;
        lazyMul[p] = lazyMul[p] * v % mod;
        lazyAdd[p] = lazyAdd[p] * v % mod;
    }

    void applyAdd(int p, int v, int l, int r) {
        int len = r - l;
        sum[p] = (sum[p] + v * len) % mod;
        lazyAdd[p] = (lazyAdd[p] + v) % mod;
    }

    void push(int p, int l, int r) {
        if (r - l <= 1) return;
        int m = (l + r) / 2;
        applyMul(p*2, lazyMul[p], l, m);
        applyMul(p*2+1, lazyMul[p], m, r);
        applyAdd(p*2, lazyAdd[p], l, m);
        applyAdd(p*2+1, lazyAdd[p], m, r);
        lazyMul[p] = 1;
        lazyAdd[p] = 0;
    }

    void rangeMul(int p, int l, int r, int ql, int qr, int v) {
        if (qr <= l || r <= ql) return;
        if (ql <= l && r <= qr) {
            applyMul(p, v, l, r);
            return;
        }
        push(p, l, r);
        int m = (l + r) / 2;
        rangeMul(p*2, l, m, ql, qr, v);
        rangeMul(p*2+1, m, r, ql, qr, v);
        pull(p);
    }

    void rangeAdd(int p, int l, int r, int ql, int qr, int v) {
        if (qr <= l || r <= ql) return;
        if (ql <= l && r <= qr) {
            applyAdd(p, v, l, r);
            return;
        }
        push(p, l, r);
        int m = (l + r) / 2;
        rangeAdd(p*2, l, m, ql, qr, v);
        rangeAdd(p*2+1, m, r, ql, qr, v);
        pull(p);
    }

    int rangeSum(int p, int l, int r, int ql, int qr) {
        if (qr <= l || r <= ql) return 0;
        if (ql <= l && r <= qr) return sum[p];
        push(p, l, r);
        int m = (l + r) / 2;
        return (rangeSum(p*2, l, m, ql, qr) +
                rangeSum(p*2+1, m, r, ql, qr)) % mod;
    }

    // wrappers
    void rangeMultiply(int l, int r, int v) { rangeMul(1, 0, n, l, r, v); }
    void rangeAdd(int l, int r, int v)      { rangeAdd(1, 0, n, l, r, v); }
    int rangeSum(int l, int r)              { return rangeSum(1, 0, n, l, r); }
};

signed main() {
    int n, q;
    int m;
    cin >> n >> q >> m;
    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
    SegmentTree st(a, m);
    while (q--) {
        int op;
        cin >> op;
        if (op == 1) {
            int l, r;
            int k;
            cin >> l >> r >> k;
            st.rangeMultiply(l-1, r, k);
        }
        else if (op == 2) {
            int l, r;
            int k;
            cin >> l >> r >> k;
            st.rangeAdd(l-1, r, k);
        }
        else if (op == 3) {
            int l, r;
            cin >> l >> r;
            cout << st.rangeSum(l-1, r) << '\n';
        }
    }
    return 0;
}
