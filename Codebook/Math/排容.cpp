int p[] = {2, 3, 5, 7};
 
int get(int n) {
    int ans = 0;
    for (int mask = 0; mask < (1 << 4); mask++) {
        int m = n;
        for (int i = 0; i < 4; i++) {
            if (mask >> i & 1) {
                m /= p[i];
            }
        }
        ans += m * (__builtin_parity(mask) ? -1 : 1);
    }
    return ans;
}

[l,r] 被 任意p[]整除 = get(r) - get(l-1);
