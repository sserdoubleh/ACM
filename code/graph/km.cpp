struct KM
{
	int n;
	int w[maxn][maxn];
	int Lx[maxn], Ly[maxn];
	int slack[maxn];
	int left[maxn];
	bool S[maxn], T[maxn];
	bool match(int u)
	{
		S[u] = 1;
		FOR1 (v, n)
		{
			if (Lx[u]+Ly[v] == w[u][v] && !T[v])
			{
				T[v] = 1;
				if (!left[v] || match(left[v]))
				{
					left[v] = u;
					return 1;
				}
			}
			else if (Lx[u]+Ly[v] != w[u][v])
				slack[v] = min(slack[v], Lx[u]+Ly[v]-w[u][v]);
		}
		return 0;
	}
	void update()
	{
		int a = INF;
		FOR1 (i, n) if (!T[i])
			a = min(a, slack[i]);
		FOR1 (i, n)
		{
			if (S[i]) Lx[i] -= a;
			if (T[i]) Ly[i] += a;
		}
	}
	int solve()
	{
		FOR1 (i, n)
		{
			left[i] = Ly[i] = 0;
			Lx[i] = -INF;
			FOR1 (j, n)
				Lx[i] = max(Lx[i], w[i][j]);
		}
		FOR1 (i, n)
		{
			for (;;)
			{
				FOR1 (j, n) S[j] = T[j] = 0, slack[j] = INF;
				if (match(i)) break; else update();
			}
		}
		int ret = 0;
		FOR1 (i, n)
			ret += w[left[i]][i];
		return ret;
	}
};