A building has L floor and each floor has RxC units of places. There are mines are placed in all over the building and there was an accident happened. 
Due to the accident, some places are damaged and it is represented by '1', the non damaged places where mines are placed represented by '0'. 
There is an entry to point to the building which is represented by '2'.
We can traverse 6 directions (East, West, North, South, Up and Down) from any of the places.
Identify the number of isolated non damaged areas and provide the top 3 isolated areas (if no provide 0).
Note: we should not count '0's which around the entry point (2).
Input Format:
First line contains 3 values (L, R, C)
The next L*R*C lines contains the mine values of grids in each layer.
Sample Input:

------------------------

#include <bits/stdc++.h>
using namespace std;

struct Pos {
    int l, r, c;
};

int L, R, C;
vector<vector<vector<char>>> grid;
vector<vector<vector<bool>>> visited;

int dl[] = {1, -1, 0, 0, 0, 0};
int dr[] = {0, 0, 1, -1, 0, 0};
int dc[] = {0, 0, 0, 0, 1, -1};

bool isValid(int l, int r, int c) {
    return l >= 0 && l < L && r >= 0 && r < R && c >= 0 && c < C;
}

int bfs(Pos start) {
    queue<Pos> q;
    q.push(start);
    visited[start.l][start.r][start.c] = true;
    int count = 1;

    while (!q.empty()) {
        Pos curr = q.front(); q.pop();
        for (int i = 0; i < 6; i++) {
            int nl = curr.l + dl[i];
            int nr = curr.r + dr[i];
            int nc = curr.c + dc[i];

            if (isValid(nl, nr, nc) && !visited[nl][nr][nc] && grid[nl][nr][nc] == '0') {
                visited[nl][nr][nc] = true;
                q.push({nl, nr, nc});
                count++;
            }
        }
    }
    return count;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> L >> R >> C;
    grid.assign(L, vector<vector<char>>(R, vector<char>(C)));
    visited.assign(L, vector<vector<bool>>(R, vector<bool>(C, false)));

    vector<Pos> entryPoints;

    // Input reading
    for (int l = 0; l < L; l++) {
        for (int r = 0; r < R; r++) {
            for (int c = 0; c < C; c++) {
                cin >> grid[l][r][c];
                if (grid[l][r][c] == '2') {
                    entryPoints.push_back({l, r, c});
                }
            }
        }
    }

    // Mark all 0 connected to entry points as visited
    for (auto &ep : entryPoints) {
        visited[ep.l][ep.r][ep.c] = true; // Mark entry point itself visited
        for (int i = 0; i < 6; i++) {
            int nl = ep.l + dl[i];
            int nr = ep.r + dr[i];
            int nc = ep.c + dc[i];
            if (isValid(nl, nr, nc) && grid[nl][nr][nc] == '0' && !visited[nl][nr][nc]) {
                bfs({nl, nr, nc});
            }
        }
    }

    // Find isolated areas of 0
    vector<int> isolatedAreas;
    for (int l = 0; l < L; l++) {
        for (int r = 0; r < R; r++) {
            for (int c = 0; c < C; c++) {
                if (grid[l][r][c] == '0' && !visited[l][r][c]) {
                    int size = bfs({l, r, c});
                    isolatedAreas.push_back(size);
                }
            }
        }
    }

    cout << (int)isolatedAreas.size() << "\n";
    if (!isolatedAreas.empty()) {
        sort(isolatedAreas.rbegin(), isolatedAreas.rend());
        for (int i = 0; i < min(3, (int)isolatedAreas.size()); i++) {
            cout << isolatedAreas[i];
            if (i != min(3, (int)isolatedAreas.size()) - 1) cout << " ";
        }
        cout << "\n";
    } else {
        cout << 0 << "\n";
    }

    return 0;
}

Input:
3 3 3
1 2 0
0 0 0 
1 0 1
1 1 0
1 0 1
0 1 0
1 1 1
1 1 0
1 0 0 

Output:
2
4 1

=============================

