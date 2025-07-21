struct ST{
    int n, LG;
    vector<vector<int>> st;
    ST(const vector<int>& v) {
        n = v.size();
        LG = __lg(n) + 1;
        st.assign(LG, vector<int>(n));
        for(int i = 0; i < n; i++)
            st[0][i] = v[i];
        for(int k = 1; k < LG; k++){
            int len = 1 << (k - 1);
            for(int i = 0; i + (1<<k) <= n; i++){
                st[k][i] = max(st[k-1][i], st[k-1][i+len]); //max,min,gcd...
            }
        }
    }
    int query(int l, int r){
        int len = r - l + 1;
        int k = __lg(len);
        return max(st[k][l], st[k][r - (1<<k) + 1]); //max,min,gcd...
    }
};
