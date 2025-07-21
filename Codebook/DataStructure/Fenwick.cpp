template <typename T>
struct BitT {
    int n;
    vector<T> tree;
    
    BitT(int n_ = 0) {
        init(n_);
    }
    void init(int n_) {
        n = n_;
        tree.assign(n, T{});
    }
    
    void add(int x, const T &v) {
        for (int i = x; i <= n; i += i & -i) {
            tree[i - 1] += v;
        }
    }

    void update(int x,const T &v){
        T diff = v - rangeSum(x,x);
        for (int i = x; i <= n; i += i & -i) {
            tree[i - 1] += diff;
        }
    }

    T sum(int x) {
        T ans{};
        for (int i = x; i > 0; i -= i & -i) {
            ans += tree[i - 1];
        }
        return ans;
    }
    
    T rangeSum(int l, int r){
        return sum(r) - sum(l - 1);
    }

    void Debug(){
        for (int i = 1; i <= n; i++) {
            cout << rangeSum(i, i) << " ";
        }
        cout << '\n';
    }
};

int main(){
    int n,q,num;
    cin>>n>>q;
    BitT<long long int> bitT(n + 1);
    for(int i = 1 ; i <= n ; i++){
        cin>>num;
        bitT.add(i,num);
    }
    long long int op,a,b;
    while(q--){
        cin>>op>>a>>b;
        if(op == 1){
            bitT.add(a,b);
        }
        else{
            cout<<bitT.rangeSum(a,b)<<'\n';
        }
    }
    return 0;
}
