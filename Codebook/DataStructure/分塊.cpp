template<typename T>
struct SqrtDecomposition {
    int n, B, numB;
    vector<T> a;
    vector<int> block_sum;
    vector<int> block_flip;

    SqrtDecomposition(const vector<T>& data) {
        a = data;
        n = a.size();
        B = int(sqrt(n)) + 1;
        numB = (n + B - 1) / B;
        block_sum.assign(numB, 0);
        block_flip.assign(numB, 0);
        for (int i = 0; i < n; i++) {
            block_sum[i/B] += a[i];
        }
    }

    void apply_single(int i) {
        int b = i / B;
        int real = a[i] ^ block_flip[b];
        int now  = real ^ 1;
        block_sum[b] += (now - real);
        a[i] ^= 1;
    }

    void range_flip(int l, int r) {
        int bl = l / B, br = r / B;
        if (bl == br){
            for (int i = l; i <= r; i++) apply_single(i);
        }
        else {
            // 左
            int endL = min(n-1, (bl+1)*B - 1);
            for (int i = l; i <= endL; i++) apply_single(i);
            // 中
            for (int b = bl+1; b <= br-1; b++) {
                block_flip[b] ^= 1;
                int len = min(n-1, (b+1)*B - 1) - b*B + 1;
                block_sum[b] = len - block_sum[b];
            }
            // 右
            int startR = br * B;
            for (int i = startR; i <= r; i++) apply_single(i);
        }
    }

    int range_query(int l, int r) {
        int ans = 0;
        int bl = l / B, br = r / B;
        if (bl == br) {
            for (int i = l; i <= r; i++) {
                ans += (a[i] ^ block_flip[bl]);
            }
        } else {
            // 左
            int endL = min(n-1, (bl+1)*B - 1);
            for (int i = l; i <= endL; i++) {
                ans += (a[i] ^ block_flip[bl]);
            }
            // 中
            for (int b = bl+1; b <= br-1; b++) {
                ans += block_sum[b];
            }
            // 右
            int startR = br * B;
            for (int i = startR; i <= r; i++) {
                ans += (a[i] ^ block_flip[br]);
            }
        }
        return ans;
    }
};

int main(){
    int n, m;
    cin >> n >> m;
    vector<int> data(n, 0);
    SqrtDecomposition<int> sd(data);

    while (m--) {
        int c,l,r;
        cin>>c>>l>>r;
        l--,r--;
        if (l > r) swap(l, r);
        if (c == 0) sd.range_flip(l, r);
        else cout << sd.range_query(l, r) << "\n";
    }
    return 0;
}
