// 分組背包 (同組取1個)

// 2D
for(int g=1; g<=groups; g++){  // 每組
    for(int j=1; j<=V; j++){  // 容量
        dp[g][j] = dp[g-1][j];  // 不選本組
        for(auto item : group[g]){  // 組內物品 (互斥)
            if(j >= w[item]){
                dp[g][j] = max(dp[g][j], dp[g-1][j - w[item]] + v[item]);
            }
        }
    }
}

// 1D
for(int g=1; g<=groups; g++){
    for(int j=V; j>=0; j--){  // 倒序
        for(auto item : group[g]){
            if(j >= w[item]){
                dp[j] = max(dp[j], dp[j - w[item]] + v[item]);
            }
        }
    }
}