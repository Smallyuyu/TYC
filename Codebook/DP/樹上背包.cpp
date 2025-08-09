// 樹上背包
// dp[node][i] : 以node為root的subtree選了i物的最大值
void Dfs(int node) // 樹形結構從root遍歷
{
    dp[node][0] = 0;  // 不選
    dp[node][1] = a[node];  // 選node自身
    if(kid[node].empty()){ // leaf
        return;
    }
    for(auto kd:kid[node]){  // 每個kid為一組
        Dfs(kd);
        for(int j=m; j>=0; j--){ // 組內01背包 倒序
            for(int k=0; k<=m; k++){ // 此subtree取k門
                if(j-k >= 0 && dp[node][j-k] >= 0 && dp[kd][k] >= 0){
                    dp[node][j] = max(dp[node][j], dp[node][j-k] + dp[kd][k]);
                }
            }
        }
    }
    for(LL i=m; i>=2; i--){
        if(dp[node][i-1] >= 0){
            dp[node][i] = dp[node][i-1] + a[node];
        }
    }
}

