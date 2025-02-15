// OpenMP Implementation of Topological Sort

#include <bits/stdc++.h>
#include <omp.h>

using namespace std;

void topologicalSort( int V, vector<int> adj[] ){
    vector<int> inDegree(V, 0);
    for( int u = 0 ; u < V ; u++ ){
        for( auto v : adj[u] )
            inDegree[v]++;
    }

    queue<int> q;
    #pragma omp parallel for
    for( int i = 0 ; i < V ; i++ )
        if( inDegree[i] == 0 )
            q.push(i);

    vector<int> result;
    while( !q.empty() ){
        int u = q.front(); q.pop();
        result.push_back( u );

        #pragma omp parallel for
        for( auto v : adj[u] ){
            if( --inDegree[v] == 0 )
                q.push(v);
        }
    }

    for( auto node : result )
        cout << node << " ";
}

int main(){
    int V = 6;
    vector<int> adj[V];
    adj[5].push_back( 2 );
    adj[5].push_back( 0 );
    adj[4].push_back( 0 );
    adj[4].push_back( 1 );
    adj[2].push_back( 3 );
    adj[3].push_back( 1 );

    cout << "Topological Sort:\n";
    topologicalSort( V, adj );
    return 0;
}
