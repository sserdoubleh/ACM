#include<cstdio>
#include<cstring>
#include<algorithm>
#include<queue>
#include<vector>
using namespace std;
typedef int LL;							//流量的数据类型
struct Edge
{
    int from,to;LL cap,flow;
    Edge(int F,int T,LL c,LL f):from(F),to(T),cap(c),flow(f){}
};
struct Dinic
{
	static const int N=10001;	
	static const LL INF=1<<30;
    int n,m,s,t;
    vector<Edge> edges;
    vector<int> G[N];
    bool vis[N];
    int d[N];
    int cur[N];
    void AddEdge(int from,int to,LL cap)
    {
        edges.push_back(Edge(from,to,cap,0));
        edges.push_back(Edge(to,from,0,0));
        m=edges.size();
        G[from].push_back(m-2);
        G[to].push_back(m-1);
    }
    bool BFS()
    {
        memset(vis,0,sizeof(vis));
        queue<int> Q;
        Q.push(s);
        d[s]=0;
        vis[s]=1;
        while(!Q.empty())
        {
            int x=Q.front();Q.pop();
            for(int i=0;i<G[x].size();++i)
            {
                Edge &e=edges[G[x][i]];
                if(!vis[e.to]&&e.cap>e.flow)
                {
                    vis[e.to]=1;
                    d[e.to]=d[x]+1;
                    Q.push(e.to);
                }
            }
        }
        return vis[t];
    }
    LL DFS(int x,LL a)
    {
        if(x==t||a==0) return a;
        LL flow=0,f;
        for(int &i=cur[x];i<G[x].size();++i)
        {
            Edge &e=edges[G[x][i]];
            if(d[x]+1==d[e.to]&&(f=DFS(e.to,min(a,e.cap-e.flow)))>0)
            {
                e.flow+=f;
                edges[G[x][i]^1].flow-=f;
                flow+=f;
                a-=f;
                if(a==0) break;
            }
        }
        return flow;
    }
    LL Maxflow(int s,int t)
    {
		this->s = s;
		this->t = t;
        LL flow=0;
        while(BFS())
        {
            memset(cur,0,sizeof(cur));
            flow+=DFS(s,INF);
        }
        return flow;
    }
};
