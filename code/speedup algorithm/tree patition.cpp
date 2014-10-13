int val[maxn];
int nex[maxn];
int dep[maxn];
int maxdep[maxn];
int pos[maxn];
int top[maxn];
int f[maxn];
vector<pii> G[maxn];
void dfs(int u,int fa,int d)
{
	int p=-1;
	dep[u]=maxdep[u]=d;
	f[u]=fa;
	For0(i,G[u].size())
	{
		int v=G[u][i].X;
		if(v==fa) continue;
		dfs(v,u,d+1);
		maxdep[u]=max(maxdep[u],maxdep[v]);
		if(p==-1||maxdep[p]<maxdep[v]) p=v;
	}
	nex[u]=p;
}

int dfs_clock;
void Dfs(int u,int fa)
{
	pos[u]=++dfs_clock;
	if(nex[u]!=-1)
	{
		top[nex[u]]=top[u];
		Dfs(nex[u],u);
	}
	For0(i,G[u].size())
	{
		int v=G[u][i].X;
		if(v==fa) continue;
		if(v==nex[u])
		{
			val[pos[nex[u]]]=G[u][i].Y;
			continue;
		}
		top[v]=v;
		Dfs(v,u);
		val[pos[v]]=G[u][i].Y;
	}
}
