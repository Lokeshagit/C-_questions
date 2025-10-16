#include <bits/stdc++.h>
using namespace std;
using pii = pair<int,int>;

int main() {
   ios::sync_with_stdio(false);
   cin.tie(nullptr);
   int N;
   
   if(!(cin >> N)) return 0;
   
   vector<int> royalty(N+1);
   
   for (int i = 1; i <= N; ++i) cin >> royalty[i];
   
   vector<int> parent(N+1, 0);
   
   // road array has N-1 entries: i-th element denotes edge between (i+1) and road[i]
   for (int i = 2; i <= N; ++i) {
       int r; cin >> r;
       parent[i] = r;
   }
   
   // capacities for edges (i connects i with parent[i]) for i=2..N
   vector<int> cap(N+1, 0);
   for (int i = 2; i <= N; ++i) {
       int c; cin >> c;
       cap[i] = c;
   }
   
   int Q; cin >> Q;
   vector<long long> queries(Q);
   long long maxQ = 0;
   for (int i = 0; i < Q; ++i) {
       cin >> queries[i];
       maxQ = max(maxQ, queries[i]);
   }
   
   // arrivals_by_day: map day -> vector of (node, king_id) arriving that day
   unordered_map<long long, vector<pii>> arrivals;
   arrivals.reserve(1024);
   // put every king initially in its city at day 0
   for (int i = 1; i <= N; ++i) {
       arrivals[0].push_back({i, i});
   }
   
   // per-node max-heap of waiting kings: (royalty, king_id)
   vector<priority_queue<pair<int,int>>> pq(N+1);
   vector<char> in_active(N+1, 0);
   // reached_count_by_day: number of kings that arrive to capital on that day
   // We'll store counts in unordered_map too (sparse), since maxQ may be large
   unordered_map<long long,int> reached_count;
   reached_count.reserve(1024);
   long long total_reached = 0;
   long long last_processed_day = 0;
   // active nodes for current day: nodes whose pq is non-empty and node != 1
   vector<int> cur_active, next_active;
   // simulate day by day up to maxQ (we only need answers up to maxQ), but stop earlier if all reached
   long long day = 0;
   for (day = 0; day <= maxQ; ++day) {
       last_processed_day = day;
       // 1) Add arrivals scheduled for this day into node pq's
       auto it = arrivals.find(day);
       if (it != arrivals.end()) {
           for (auto &pr : it->second) {
               int node = pr.first;
               int king = pr.second;
               if (node == 1) {
                   // arrived at capital
                   reached_count[day] += 1;
                   total_reached += 1;
               } else {
                   pq[node].push({royalty[king], king});
                   if (!in_active[node]) {
                       cur_active.push_back(node);
                       in_active[node] = 1;
                   }
               }
           }
           // free memory for this day, not needed anymore
           arrivals.erase(it);
       }
       if (total_reached == N) {
           // all kings reached capital; we can stop early
           break;
       }
       if (cur_active.empty()) {
           // no nodes with waiting kings today; if there are no scheduled future arrivals up to maxQ,
           // then nothing will change for later days -> break.
           // However there might be arrivals scheduled for future days > day, we need to advance day to next such day.
           // But we are iterating day sequentially up to maxQ, so allowing the loop to continue suffices.
           continue;
       }
       // 2) For each active node, send up to cap[node] kings to parent (they arrive at day+1)
       next_active.clear();
       for (int node : cur_active) {
           in_active[node] = 0; // will re-add if still has waiting kings after sending
           if (node == 1) continue;
           int c = cap[node];
           for (int t = 0; t < c; ++t) {
               if (pq[node].empty()) break;
               auto top = pq[node].top(); pq[node].pop();
               int king = top.second;
               int p = parent[node];
               // schedule arrival at parent on day+1
               if (p == 1) {
                   // arrives at capital on day+1
                   reached_count[day+1] += 1;
                   total_reached += 1;
               } else {
                   arrivals[day+1].push_back({p, king});
               }
           }
           if (!pq[node].empty()) {
               // still has waiting kings - will be active next day
               next_active.push_back(node);
               in_active[node] = 1;
           }
       }
       // swap active lists
       cur_active.swap(next_active);
       // continue to next day
       if (total_reached == N) break;
   }
   // Build prefix counts up to maxQ: number reached by end of day D
   vector<long long> prefix(maxQ+1 + 1, 0); // +1 for safe indexing
   long long running = 0;
   for (long long d = 0; d <= maxQ; ++d) {
       auto it2 = reached_count.find(d);
       if (it2 != reached_count.end()) running += it2->second;
       prefix[d] = running;
   }
   // If we broke early because all reached before maxQ, prefix after that day remain total_reached
   long long final_total = total_reached;
   // Output answers for queries in input order
   for (int i = 0; i < Q; ++i) {
       long long D = queries[i];
       long long ans;
       if (D <= maxQ) ans = prefix[D];
       else ans = final_total; // beyond simulated maxQ (which equals our requested max) -> all we have
       if (i) cout << ' ';
       cout << ans;
   }
   cout << '\n';
   return 0;
}

---------------

Input:
N=4
royalty = 4 1 3 2
edges = 1 2 2
capacity = 1 1 1
q = 4
queries = 2 1 5 3

output : 3 2 4 4


========================

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N; cin >> N;
    vector<int> royalty(N+1);
    for(int i=1;i<=N;i++) cin >> royalty[i];

    vector<int> parent(N+1);
    for(int i=2;i<=N;i++) cin >> parent[i];

    vector<int> cap(N+1);
    for(int i=2;i<=N;i++) cin >> cap[i];

    int Q; cin >> Q;
    vector<long long> queries(Q);
    for(int i=0;i<Q;i++) cin >> queries[i];

    vector<long long> arrival(N+1, -1);

    // per-node queue (FIFO)
    vector<queue<int>> pq(N+1);

    // initially, every king at its node
    for(int i=1;i<=N;i++) {
        if(i==1) arrival[i]=0; // capital
        else pq[i].push(i);
    }

    long long day=0;
    while(true){
        bool any_move=false;
        vector<queue<int>> next_pq(N+1);
        for(int node=2;node<=N;node++){
            int moves = cap[node];
            while(moves-- && !pq[node].empty()){
                int k = pq[node].front(); pq[node].pop();
                int p = parent[node];
                if(p==1) arrival[k]=day+1;
                else next_pq[p].push(k);
                any_move=true;
            }
            while(!pq[node].empty()){
                next_pq[node].push(pq[node].front()); pq[node].pop();
            }
        }
        pq = next_pq;
        if(!any_move) break;
        day++;
    }

    vector<long long> arr;
    for(int i=1;i<=N;i++) arr.push_back(arrival[i]);
    sort(arr.begin(), arr.end());
    for(auto x:arr){
		cout << x << " ";
	}
	cout << "\n";
	vector<int> ans(q);
    for(int i=0;i<Q;i++){
        if(i) cout << " ";
        long long D = queries[i];
        ans[i] = upper_bound(arr.begin(),arr.end(),D) - arr.begin();
        cout << ans[i];
    }
    cout << "\n";
    return 0;
}

