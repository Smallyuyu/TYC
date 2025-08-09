// 完全背包 (每物無限次)
// 2d
for(int i=1; i<=n; i++){  // 物品
    for(int j=1; j<=V; j++){  // 容量
        dp[i][j] = dp[i-1][j]; // 繼承前1輪
        if(j - w[i] >= 0){
            dp[i][j] = max(dp[i][j], dp[i][j - w[i]] + v[i]); // 選 (允許重複取同物品,從本輪轉移)
        }
    }
}

// 1d
for(int i=1; i<=n; i++){
    for(int j=w[i]; j<=V; j++){ // 正序允許多次選
        dp[j] = max(dp[j], dp[j - w[i]] + v[i]);
    }
}