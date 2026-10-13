#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

vector<unsigned long long> topoSort(unsigned long long N, const vector<vector<unsigned long long>>& adj, vector<unsigned long long>& in_degree) {
    vector<unsigned long long> sorted_nodes;
    stack<unsigned long long> zero_in_degree; // Stack to hold nodes with zero in-degree

    for (unsigned long long i = 1; i <= N; i++) {
        if (in_degree[i] == 0) {
            zero_in_degree.push(i);
        }
    }

    while (!zero_in_degree.empty()) {
        unsigned long long u = zero_in_degree.top();
        zero_in_degree.pop();
        sorted_nodes.push_back(u);

        for (unsigned long long v : adj[u]) {
            if (--in_degree[v] == 0) {
                zero_in_degree.push(v);
            }
        }
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
    vector<unsigned long long> in_degree(N + 1, 0);
    for (unsigned long long i = 0; i < K; i++) {
        unsigned long long u, v;
        cin >> u >> v;
        in_degree[v]++;
        adj[u].push_back(v);
    }

    vector<unsigned long long> sorted_nodes = topoSort(N, adj, in_degree);
    vector<long long> dp(N + 1);
    vector<char> reachable(N + 1);

    for (unsigned long long node = 0; node < sorted_nodes.size(); node++) {
        fill(dp.begin(), dp.end(), 0);
        fill(reachable.begin(), reachable.end(), 0);
        unsigned long long start = sorted_nodes[node];
        dp[start] = 1;
        reachable[start] = 1;

        for (unsigned long long A = node; A < sorted_nodes.size(); A++) {
            unsigned long long u = sorted_nodes[A];
            if (!reachable[u]) 
                continue;

            for (unsigned long long it : adj[u]) {
                reachable[it] = 1;

                unsigned long long sum = dp[u] + dp[it];

                if (sum >= M) {
                    sum -= M;
                }
                dp[it] = sum;
            }
        }

        for (unsigned long long i = 1; i <= N; i++) {
            if (reachable[i] && i != start) {
                unsigned long long truck_number = 1 + dp[i];
                if (truck_number >= m1 && truck_number <= m2) {
                    trucks[truck_number - m1].push_back({start, i});
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