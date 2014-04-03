#define For1(i,n) for(int i=1;i<=n;++i)
struct KM
{
	static const int maxn=100;
	static const int INF=1<<30;
	int n;
	int w[maxn][maxn];
	int Lx[maxn],Ly[maxn];
	int slack[maxn];
	int left[maxn];
	bool S[maxn],T[maxn];
	bool match(int u)
	{
		S[u]=1;
		For1(v,n) if(Lx[u]+Ly[v]==w[u][v]&&!T[v])
		{
			T[v]=1;
			if(!left[v]||match(left[v]))
			{
				left[v]=u;
				return 1;
			}
		}
		return 0;
	}
	void update()
	{
		int a=INF;
		For1(i,n) if(!T[i])
			a=min(a,slack[i]);
		For1(i,n)
		{
			if(S[i]) Lx[i]-=a;
			if(T[i]) Ly[i]+=a;
		}
	}
	void solve()
	{
		For1(i,n)
		{
			left[i]=Lx[i]=Ly[i]=0;
			For1(j,n)
				Lx[i]=max(Lx[i],w[i][j]);
		}
		For1(i,n)
		{
			For1(y,n)
			{
				slack[y]=INF;
				For1(x,n)
					slack[y]=min(slack[y],Lx[x]+Ly[y]-w[x][y]);
			}
			while(1)
			{
				For1(j,n) S[j]=T[j]=0;
				if(match(i)) break; else update();
			}
		}
	}
};
