int pre[MAXN],cnt[MAXN],bccno[MAXN];
stack<pii> S;
int dfs2(int u,int fa)
{
	int lowu=pre[u]=++dfs_clock;
	int child=0;
	For(i,G[u].size())
	{
		int v=G[u][i];
		if(v==fa) continue;
		if(!pre[v])
		{
			S.push(MP(u,v));
			++child;
			int lowv=dfs2(v,u);
			lowu=min(lowu,lowv);
			if(lowv>=pre[u])
			{
				++bcc_cnt;
				while(1)
				{
					pii p=S.top();S.pop();
					if(bccno[p.X]!=bcc_cnt)
					{ ++cnt[p.X];bccno[p.X]=bcc_cnt; }
					if(bccno[p.Y]!=bcc_cnt)
					{ ++cnt[p.Y];bccno[p.Y]=bcc_cnt; }
					if(p.X==u&&p.Y==v) break;
				}
			}
		}
		else if(pre[v]<pre[u])
		{
			S.push(MP(u,v));
			lowu=min(lowu,pre[v]);
		}
	}
	return lowu;
}

void find_bcc(int n)
{
	dfs_clock=bcc_cnt=0;
	memset(pre,0,sizeof(pre));
	memset(cnt,0,sizeof(cnt));
	memset(bccno,0,sizeof(bccno));
	For(i,n) if(!pre[i]) dfs2(i,-1);
}
