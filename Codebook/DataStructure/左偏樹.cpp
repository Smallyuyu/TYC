template <typename T, typename Compare = less<T>>
struct LeftistHeap {
    struct Node {
        T        val;
        int      dist;
        Node*    l;
        Node*    r;
        Node(const T& v): val(v), dist(0), l(nullptr), r(nullptr) {}
    };

    Node* root;
    Compare cmp;
    LeftistHeap(): root(nullptr), cmp(Compare()) {}
    ~LeftistHeap() { clear(root); }

    static Node* merge(Node* a, Node* b, Compare cmp) {
        if (!a) return b;
        if (!b) return a;
        if (cmp(b->val, a->val)) swap(a, b);
        a->r = merge(a->r, b, cmp);
        if (!a->l || (a->l->dist < a->r->dist)) swap(a->l, a->r);
        a->dist = (a->r ? a->r->dist : -1) + 1;
        return a;
    }

    void push(const T& v) {
        Node* x = new Node(v);
        root = merge(root, x, cmp);
    }
    const T& top() const {
        return root->val;
    }
    void pop() {
        Node* old = root;
        root = merge(root->l, root->r, cmp);
        delete old;
    }
    bool empty() const {
        return root == nullptr;
    }
    
    void merge(LeftistHeap& other) {
        root = merge(root, other.root, cmp);
        other.root = nullptr;
    }

    void clear(Node* t) {
        if (!t) return;
        clear(t->l);
        clear(t->r);
        delete t;
    }
};

//左偏樹，key = (value, original_index)
vector<LeftistHeap<pair<int,int>>> heaps(n+1);
for(int i = 1; i <= n; i++){
    int v; 
    cin >> v;
    heaps[i].push({v, i});
}
heaps[fx].merge(heaps[fy]);
auto pr = heaps[fx].top();
int val = pr.first, idx = pr.second;

