struct DSU {
    vector<int> f; //x的父節點
    vector<int> s; //以x為根的子樹大小
    
    DSU() {}
    DSU(int n) {
        init(n);
    }
    
    void init(int n) {
        f.resize(n);
        iota(f.begin(), f.end(), 0); //0,1,2,3...
        s.assign(n, 1);
    }
    
    int find(int x) {
        while (x != f[x]) {
            x = f[x] = f[f[x]];
        }
        return x;
    }
    
    bool same(int x, int y) {
        return find(x) == find(y);
    }
    
    bool merge(int x, int y) {
        x = find(x);
        y = find(y);
        if (x == y) return false;
        s[x] += s[y];
        f[y] = x;
        return true;
    }
    
    int size(int x) {
        return s[find(x)];
    }
};
signed main(){
    int n = 1;
    DSU DSU;
    DSU.init(n);
    return 0;
}
