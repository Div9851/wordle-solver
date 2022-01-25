#include <bits/stdc++.h>
using namespace std;

using ll = long long;

vector<string> words;
map<string, int> word_to_idx;
int num_words;
vector<vector<int>> results;
vector<vector<int>> cnt;
vector<ll> score;
int num_candidates;
vector<bool> is_candidate;
const int pow3[] = {1, 3, 9, 27, 81, 243};

int check(int guess, int answer) {
    bool used1[5] = {}, used2[5] = {};
    int result = 0;
    for (int i = 0; i < 5; i++) {
        if (words[guess][i] == words[answer][i]) {
            used1[i] = used2[i] = 1;
            result += 2 * pow3[i];
        }
    }
    for (int i = 0; i < 5; i++) {
        if (used1[i])
            continue;
        for (int j = 0; j < 5; j++) {
            if (used2[j] || words[guess][i] != words[answer][j])
                continue;
            used1[i] = used2[j] = 1;
            result += pow3[i];
            break;
        }
    }
    return result;
}

int main() {
    ifstream in("len5.txt");
    while (!in.eof()) {
        string word;
        in >> word;
        word_to_idx[word] = words.size();
        words.push_back(word);
    }
    num_words = words.size();
    results = vector<vector<int>>(num_words, vector<int>(num_words, -1));
    cnt = vector<vector<int>>(num_words, vector<int>(pow3[5], 0));
    score = vector<ll>(num_words, 0);
    for (int i = 0; i < num_words; i++) {
        for (int j = 0; j < num_words; j++) {
            int result = check(i, j);
            results[i][j] = result;
            score[i] -= cnt[i][result] * cnt[i][result];
            cnt[i][result]++;
            score[i] += cnt[i][result] * cnt[i][result];
        }
    }
    num_candidates = num_words;
    is_candidate = vector<bool>(num_candidates, true);
    for (int i = 0; i < 6; i++) {
        int mi = 0;
        vector<string> candidates;
        for (int i = 0; i < num_words; i++) {
            if (score[mi] > score[i]) {
                mi = i;
            }
            if (is_candidate[i] && candidates.size() < 3)
                candidates.push_back(words[i]);
        }
        cout << "number of candidates:" << num_candidates << endl;
        cout << "candidates:";
        for (const string cand : candidates)
            cout << cand << " ";
        if (num_candidates > 3)
            cout << "...";
        cout << endl;
        if (num_candidates > 1) {
            cout << "suggest:" << words[mi] << "("
                 << 1.0 * score[mi] / num_candidates << ")" << endl;
        }
        cout << "guess:";
        string str;
        cin >> str;
        int guess = word_to_idx[str];
        cout << "result:";
        cin >> str;
        int result = 0;
        for (int i = 0; i < 5; i++) {
            result += (str[i] - '0') * pow3[i];
        }
        if (result == 242) {
            cout << "clear!" << endl;
            return 0;
        }
        for (int i = 0; i < num_words; i++) {
            if (!is_candidate[i])
                continue;
            if (results[guess][i] != result) {
                is_candidate[i] = 0;
                num_candidates--;
                for (int j = 0; j < num_words; j++) {
                    int tmp = results[j][i];
                    score[j] -= cnt[j][tmp] * cnt[j][tmp];
                    cnt[j][tmp]--;
                    score[j] += cnt[j][tmp] * cnt[j][tmp];
                }
            }
        }
    }
    cout << "failed..." << endl;
}