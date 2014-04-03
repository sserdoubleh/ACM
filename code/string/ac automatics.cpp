vector<int> word;							// the end node of word
struct Trie
{
	static const int N=600001,sigma_size=26;
	int ch[N][sigma_size];
	bool val[N];
	int fa[N];
	int last[N];
	int sz;
	Trie()
	{
		sz=1;
		val[0]=0;
	}
	inline int ind(char c) { return c-'a'; }
	void insert(char *s,int n)
	{
		int u=0;
		for(int i=0;i<n;++i)
		{
			int c=ind(s[N]);
			if(!ch[u][c])
			{
				val[sz]=0;
				ch[u][c]=sz++;
			}
			u=ch[u][c];
		}
		val[u]=1;
		word.push_back(u);
	}
	void getfail()
	{
		queue<int> q;
		fa[0]=0;
		for(int i=0;i<sigma_size;++i) if(ch[0][i])
		{
			int u=ch[0][i];
			fa[u]=last[u]=0;
			q.push(u);
		}
		while(!q.empty())
		{
			int r=q.front();q.pop();
			for(int c=0;c<sigma_size;++c) if(ch[r][c])
			{
				int u=ch[r][c];
				q.push(u);
				int v=fa[r];
				while(v&&!ch[v][c]) v=fa[v];
				fa[u]=ch[v][c];
				last[u]=val[fa[u]]?fa[u]:last[fa[u]];
			}
		}
	}
	void print(int j,int &ans)
	{
		if(j)
		{
			++ans;
			val[j]=0;
		}
	}
	void find(char *s,int n,int &ans)
	{
		for(int i=0,j=0;i<n;++i)
		{
			int c=ind(s[i]);
			while(j&&!ch[j][c]) j=fa[j];
			j=ch[j][c];
			if(val[j]) print(j,ans);
			else if(last[j]) print(last[j],ans);
		}
	}
};
