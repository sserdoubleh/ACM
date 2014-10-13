/* By doublehh */
const int maxn = 250;
int n;
bool G[maxn][maxn];

const int S = 1, T = 2;
// for node tag
int tag[maxn];
// for matched (one of the two way)
int matched[maxn];
// for father (one of the two way)
int fa[maxn];
// for union-find set
int f[maxn];
// for find lca
int vis[maxn];

int find(int x) { return f[x]==x?x:f[x]=find(f[x]); }

int findLCA(int u, int v)
{
	// frame update
	static int t = 0;
	t++;
	for (;;)
	{
		if (u)
		{
			u = find(u);
			if (vis[u] == t) return u;
			vis[u] = t;
			if (matched[u]) u = fa[matched[u]];
			else u = 0;
		}
		swap(u, v);
	}
	return -1;
}

// contract u to p (one part of a blossom)
void contract(int x, int p, queue<int> &q)
{
	while (find(x) != p)
	{
		int y = matched[x], z = fa[y];
		// important
		if (find(z) != p) fa[z] = y;
		// important
		if (tag[y] == T)
		{
			tag[y] = S;
			q.push(y);
		}
		f[x] = f[y] = p;
		// important
		x = z;
	}
}

// argument
void argument(int u, int v)
{
	fa[v] = u;
	for (int x = v; x; )
	{
		int y = fa[x];
		int z = matched[y];
		matched[x] = y;
		matched[y] = x;
		x = z;
	}
}

// expand form u-v edge
void expand(int u, int v, queue<int> &q)
{
	fa[v] = u;
	tag[v] = T;
	int x = matched[v];
	tag[x] = S;
	q.push(x);
}

bool findArgument(int s)
{
	bool ret = 0;
	memset(tag, 0, sizeof(tag));
	memset(fa, 0, sizeof(fa));
	for (int i = 1; i <= n; i++) f[i] = i;
	queue<int> q;
	q.push(s);
	tag[s] = S;
	while (!q.empty())
	{
		int u = q.front(); q.pop();
		for (int v = 1; v <= n; v++) if (G[u][v] && v != matched[u] && find(u) != find(v) && tag[v] != T)
		{
			if (tag[v] == S) // meet a S node
			{
				int p = findLCA(u, v);
				// important
				if (find(u) != p) fa[u] = v;
				if (find(v) != p) fa[v] = u;
				// important order 
				contract(u, p, q);
				contract(v, p, q);
			}
			else if (!matched[v]) // find a free node
			{
				argument(u, v);
				return 1;
			}
			else // find a matched node
				expand(u, v, q);
		}
	}
	return 0;
}

void solve()
{
	int ans = 0;
	memset(matched, 0, sizeof(matched));
	for (int i = 1; i <= n; i++)
		if (!matched[i] && findArgument(i)) // find argument road
			ans++;

	printf("%d\n", 2*ans);
	for (int i = 1; i <= n; i++) if (matched[i])
		if (i < matched[i])
			printf("%d %d\n", i, matched[i]);
}

int main()
{
	scanf("%d", &n);
	int u, v;
	memset(G, 0, sizeof(G));
	while (~scanf("%d%d", &u, &v))
		G[v][u] = G[u][v] = 1;
	solve();
}
