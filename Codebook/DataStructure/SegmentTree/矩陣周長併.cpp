struct Event{
    int pos, a, b;
    int v;
};

bool cmp(const Event& e1, const Event& e2){
    return e1.pos < e2.pos;
}

struct Rect{
    int x1, y1, x2, y2;
};

struct Node{
    int cover;
    int segCnt;
    bool leftCovered;
    bool rightCovered;
    Node(): cover(0), segCnt(0), leftCovered(false), rightCovered(false) {}
};

class SegTree{
  public:
    SegTree(const vector<int>& coords) : coords_(coords) {
        n_ = (int)coords_.size() - 1;
        tree_.assign(4 * n_, Node());
    }

    void modify(int x1, int x2, int v) {
        int L = lower_bound(coords_.begin(), coords_.end(), x1) - coords_.begin();
        int R = lower_bound(coords_.begin(), coords_.end(), x2) - coords_.begin();
        update(1, 0, n_, L, R, v);
    }

    int segments() const {
        return tree_[1].segCnt;
    }

private:
    int n_;
    vector<int> coords_;
    vector<Node> tree_;

    void puint(int p, int l, int r) {
        Node &nd = tree_[p];
        if (nd.cover > 0) {
            nd.segCnt = 1;
            nd.leftCovered = nd.rightCovered = true;
        }
        else if (l + 1 == r) {
            nd.segCnt = 0;
            nd.leftCovered = nd.rightCovered = false;
        }
        else {
            Node &L = tree_[p << 1];
            Node &R = tree_[p << 1 | 1];
            nd.segCnt = L.segCnt + R.segCnt - (L.rightCovered && R.leftCovered ? 1 : 0);
            nd.leftCovered = L.leftCovered;
            nd.rightCovered = R.rightCovered;
        }
    }

    void update(int p, int l, int r, int L, int R, int v) {
        if (R <= l || r <= L) return;
        if (L <= l && r <= R) {
            tree_[p].cover += v;
            puint(p, l, r);
            return;
        }
        int m = (l + r) >> 1;
        update(p << 1, l, m, L, R, v);
        update(p << 1 | 1, m, r, L, R, v);
        puint(p, l, r);
    }
};

int sweepPerimeter(vector<Event>& ev, vector<int>& coords) {
    sort(coords.begin(), coords.end());
    coords.erase(unique(coords.begin(), coords.end()), coords.end());
    SegTree st(coords);
    sort(ev.begin(), ev.end(), cmp);
    int per = 0;
    int prev = ev[0].pos;
    for (size_t i = 0; i < ev.size(); ++i) {
        int x = ev[i].pos;
        int d = x - prev;
        if (d > 0) per += (int)st.segments() * 2 * d;
        st.modify(ev[i].a, ev[i].b, ev[i].v);
        prev = x;
    }
    return per;
}

signed main() {
    int n;
    cin >> n;
    vector<Rect> rec(n);
    for (int i = 0; i < n; ++i) {
        cin >> rec[i].x1 >> rec[i].y1 >> rec[i].x2 >> rec[i].y2;
    }

    vector<Event> evX, evY;
    vector<int> Ys, Xs;
    evX.reserve(2 * n);
    evY.reserve(2 * n);
    Ys.reserve(2 * n);
    Xs.reserve(2 * n);

    for (int i = 0; i < n; ++i) {
        Rect &r = rec[i];
        evX.push_back((Event){r.x1, r.y1, r.y2, +1});
        evX.push_back((Event){r.x2, r.y1, r.y2, -1});
        Ys.push_back(r.y1);
        Ys.push_back(r.y2);

        evY.push_back((Event){r.y1, r.x1, r.x2, +1});
        evY.push_back((Event){r.y2, r.x1, r.x2, -1});
        Xs.push_back(r.x1);
        Xs.push_back(r.x2);
    }

    int perH = sweepPerimeter(evX, Ys);
    int perV = sweepPerimeter(evY, Xs);
    cout << (perH + perV) << '\n';
    return 0;
}
