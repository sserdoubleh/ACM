struct TwoSAT
{
	static const int N=33;
	int n;
	vector<int> G[N<<1];
	bool mark[N<<1];
	int S[N<<1],c;

	bool dfs(int x)
	{
		if(mark[x^1]) return 0;
		if(mark[x]) return 1;
		mark[x]=1;
		S[c++]=x;
		For0(i,G[x].size())
			if(!dfs(G[x][i])) return 0;
		return 1;
	}

	void init(int n)
	{
		this->n=n;
		For0(i,2*n) G[i].clear();
		memset(mark,0,sizeof(mark));
	}

	void add_clause(int x,int xval,int y,int yval)
	{
		x=(x<<1)+xval;
		y=(y<<1)+yval;
		G[x^1].push_back(y);
		G[y^1].push_back(x);
	}

	bool solve()
	{
		ForStep(i,2*n,2) if(!mark[i]&&!mark[i+1])
		{
			c=0;
			if(!dfs(i))
			{
				while(c) mark[S[--c]]=0;
				if(!dfs(i+1)) return 0;
			}
		}
		return 1;
	}

	void print()
	{
		ForStep(i,2*n,2)
			printf("%d:%d%c",i/2+1,mark[i]?0:1,i==2*n-2?'\n':' ');
	}
};
