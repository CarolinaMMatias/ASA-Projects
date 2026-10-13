#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

void dfs(unsigned long long node, unsigned long long vis[], stack<unsigned long long> &st, const vector<vector<unsigned long long>>& adj) {
    vis[node] = 1;
    for (unsigned long long it : adj[node]) {
        if (!vis[it]) {
            dfs(it, vis, st, adj);
        }
    }
    st.push(node);
}

vector<unsigned long long> topoSort(unsigned long long N, const vector<vector<unsigned long long>>& adj) {
    unsigned long long vis[N + 1] = {0};
    stack<unsigned long long> st;
    
    for (unsigned long long i = 1; i <= N; i++) {
        if (!vis[i]) {
            dfs(i, vis, st, adj);
        }
    }

    vector<unsigned long long> sorted_nodes;
    while (!st.empty()) {
        sorted_nodes.push_back(st.top());
        st.pop();
    }

    return sorted_nodes;
}

int main() {
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);

    unsigned long long N;
    cin >> N;

    unsigned long long M;
    cin >> M;

    unsigned long long m1, m2;
    cin >> m1 >> m2;
    vector<vector<pair<unsigned long long, unsigned long long>>> trucks(m2 - m1 + 1);

    unsigned long long K;
    cin >> K;
    vector<vector<unsigned long long>> adj(N + 1);
    for (unsigned long long i = 0; i < K; i++) {
        unsigned long long u, v;
        cin >> u >> v;
        adj[u].push_back(v);
    }

    vector<unsigned long long> sorted_nodes = topoSort(N, adj);

    for (unsigned long long node = 0; node < (unsigned long long)sorted_nodes.size(); node++) {
        vector<long long> dp(N + 1, 0);
        vector<bool> reachable(N + 1, false);
        dp[sorted_nodes[node]] = 1; 
        reachable[sorted_nodes[node]] = true;
        for (unsigned long long A = node; A < (unsigned long long)sorted_nodes.size(); A++) {
           
            if (reachable[sorted_nodes[A]]) {
                for (unsigned long long it : adj[sorted_nodes[A]]) {
                    reachable[it] = true;
                    dp[it] = (dp[it] + dp[sorted_nodes[A]]) % M;
                }
            }
        }

        for (unsigned long long i = 1; i <= N; i++) {
            if (reachable[i] && i != sorted_nodes[node]) {
                unsigned long long truck_number = 1 + dp[i];
                if (truck_number >= m1 && truck_number <= m2) {
                    trucks[truck_number - m1].push_back({sorted_nodes[node], i});
                }
            }
        }
    }

    // Sort pairs within each truck
    for (unsigned long long i = 0; i < (unsigned long long)trucks.size(); i++) {
        sort(trucks[i].begin(), trucks[i].end());
    }
    
    for (unsigned long long i = 0; i < (unsigned long long)trucks.size(); i++) {
        cout << "C" << (i + m1);
        for (auto p : trucks[i]) {
            cout << " " << p.first << "," << p.second;
        }
        cout << "\n";
    }

    return 0;
}