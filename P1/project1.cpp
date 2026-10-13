#include <iostream>
#include <vector>

using namespace std;

const int affinity_table[5][5] = {
    //P  N  A  B  T
    { 1, 3, 1, 3, 1}, // P
    { 5, 1, 0, 1, 1}, // N
    { 0, 1, 0, 4, 1}, // A
    { 1, 3, 2, 3, 1}, // B
    { 1, 1, 1, 1, 1}, // T
};

inline int char_to_index(char c) {
    switch (c) {
        case 'P': return 0;
        case 'N': return 1;
        case 'A': return 2;
        case 'B': return 3;
        case 'T': return 4;
    }
    return 4;
}

inline int affinity(char a, char b) {
    return affinity_table[char_to_index(a)][char_to_index(b)];
}

void backtrack(int i, int j, const vector<vector<int>>& choice, vector<int>& result) {
    if (i > j) return;
    
    int k = choice[i][j];
    
    // Process left part first
    if (k > i) {
        backtrack(i, k - 1, choice, result);
    }
    
    // Process right part
    if (k < j) {
        backtrack(k + 1, j, choice, result);
    }
    
    // Add the chosen element (postorder)
    result.push_back(k);
}

unsigned long long solve(int n, const string& chain, vector<unsigned long long>& potentials, vector<int>& sequence) {
    vector<vector<unsigned long long>> dp(n, vector<unsigned long long>(n, 0));
    vector<vector<int>> choice(n, vector<int>(n, -1)); // Track which k was chosen

    // l is the length of the substring
    // i is the starting index of the substring
    // j is the ending index of the substring
    
    for (int l = 1; l <= n; l++) {
        for (int i = 0; i <= n - l; i++) {
            int j = i + l - 1;
            
            // Precompute values that are constant for all k in this (i,j) pair
            unsigned long long left_potential = (i == 0) ? 1 : potentials[i - 1];
            unsigned long long right_potential = (j == n - 1) ? 1 : potentials[j + 1];
            int left_aff_idx = char_to_index((i == 0) ? 'T' : chain[i - 1]);
            int right_aff_idx = char_to_index((j == n - 1) ? 'T' : chain[j + 1]);
            
            for (int k = i; k <= j; k++) {
                unsigned long long left = (k == i) ? 0 : dp[i][k - 1];
                unsigned long long right = (k == j) ? 0 : dp[k + 1][j];

                int k_idx = char_to_index(chain[k]);
                unsigned long long k_potential = potentials[k];
                unsigned long long energy = left_potential * affinity_table[left_aff_idx][k_idx] * k_potential +
                                   k_potential * affinity_table[k_idx][right_aff_idx] * right_potential;
                
                unsigned long long total = left + energy + right;

                if (total >= dp[i][j]) {
                    dp[i][j] = total;
                    choice[i][j] = k;
                }
            }
        }
    }
    
    sequence.reserve(n);  // Reserve space to avoid reallocations
    backtrack(0, n - 1, choice, sequence);

    return dp[0][n - 1]; 
}
        

int main() {
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);

    int n;
    cin >> n;
    vector<unsigned long long> potentials(n);
    for (int i = 0; i < n; i++) {
        cin >> potentials[i];
    }
    string chain;
    cin >> chain;

    vector<int> sequence; // To store the removal sequence
    unsigned long long result = solve(n, chain, potentials, sequence);
    
    cout << result << endl;
    
    // Print the indices in the order they were removed (1-indexed)
    for (int i = 0; i < (int)sequence.size(); i++) {
        if (i > 0) cout << " ";
        cout << sequence[i] + 1;  // +1 to convert from 0-indexed to 1-indexed
    }
    cout << endl;

    return 0;
}