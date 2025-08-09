// 01背包
// 2d
for(int i=1; i<=n; i++){  // 物品
    for(int j=1; j<=V; j++){  // 容量
        dp[i][j] = dp[i-1][j];  // 繼承前1輪
        if(j >= w[i]){
            dp[i][j] = max(dp[i][j], dp[i-1][j - w[i]] + v[i]);  // 選
        }
    }
    /*
    滾動 dp[n][m] -> dp[2][m]
    */
}
// 1d
for(int i=1; i<=n; i++){
    for(int j=V; j>=w[i]; j--){  // 倒序確保限取1次
        dp[j] = max(dp[j], dp[j - w[i]] + v[i]);
    }
}