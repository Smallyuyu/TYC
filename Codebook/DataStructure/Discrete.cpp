//離散化
struct Discrete {
    unordered_map<int,int> mp;
    int sz = 0;
    void add(int x) {
        if (!mp.count(x)) mp[x] = sz++;
    }
    int find(int x) const {
        auto it = mp.find(x);
        return it == mp.end() ? -1 : it->second;
    }
    int size() const { return sz; }
};
