struct Edge
{
	int from,to,cap,flow,cost;
	Edge(int from,int to,int cap,int flow,int cost)
		:from(from),to(to),cap(cap),flow(flow),cost(cost){}
};
struct MCMF
{
	static const int N=402;
	int n,m,s,t;
	vector<Edge> edges;
	vector<int> G[N];
	int inq[N];
	int d[N];
	int p[N];
	int a[N];
	void AddEdge(int from,int to,int cap,int cost)
	{
		edges.push_back(Edge(from,to,cap,0,cost));
		edges.push_back(Edge(to,from,0,0,-cost));
		m=edges.size();
		G[from].push_back(m-2);
		G[to].push_back(m-1);
	}
	void init(int n,int m)
	{
		int point[200];
		int c[200],q[200];
		s=n+m,t=s+1;
		this->n=t+1;
		for(int i=0;i<this->n;++i) G[i].clear();
		edges.clear();
		for(int i=0;i<n;++i) scanf("%d",&point[i]);
		for(int i=0;i<m;++i)
			scanf("%d%d",&c[i],&q[i]);
		for(int i=0;i<n;++i) for(int j=0;j<m;++j)
		{
			if(point[i]<=q[j])
				AddEdge(i,n+j,1,-point[i]);
		}
		for(int i=0;i<n;++i)
			AddEdge(s,i,1,0);
		for(int i=0;i<m;++i)
			AddEdge(i+n,t,c[i],0);
	}
	bool BellmanFord(int s,int t,int &flow,int &cost)
	{
		for(int i=0;i<n;++i) d[i]=INF;
		memset(inq,0,sizeof(inq));
		d[s]=0,inq[s]=1,p[s]=0,a[s]=INF;
		queue<int> Q;
		Q.push(s);
		while(!Q.empty())
		{
			int u=Q.front();Q.pop();
			inq[u]=0;
			for(int i=0;i<G[u].size();++i)
			{
				Edge &e=edges[G[u][i]];
				if(e.cap>e.flow&&d[e.to]>d[u]+e.cost)
				{
					d[e.to]=d[u]+e.cost;
					p[e.to]=G[u][i];
					a[e.to]=min(a[u],e.cap-e.flow);
					if(!inq[e.to]) {Q.push(e.to);inq[e.to]=1;}
				}
			}
		}
		if(d[t]==INF) return 0;
		flow+=a[t];
		cost+=d[t]*a[t];
		int u=t;
		while(u!=s)
		{
			edges[p[u]].flow+=a[t];
			edges[p[u]^1].flow-=a[t];
			u=edges[p[u]].from;
		}
		return 1;
	}
	void MinCost()
	{
		int flow=0,cost=0;
		while(BellmanFord(s,t,flow,cost));
		printf("%d %d\n",flow,-cost);
	}
};
