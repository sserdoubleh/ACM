void RMQ_init(int d[maxn][maxn],int val[maxn],int n)
{
	for(int i=1;i<=n;++i) d[i][0]=val[i];
	for(int j=1;(1<<j)<=n;++j)
		for(int i=1;i+j-1<=n;++i)
			d[i][j]=min(d[i][j-1],d[i+(1<<(j-1))][j-1]);
}
int RMQ(int d[maxn][maxn],int l,int r)
{
	int k=0;
	while((1<<(k+1))<=r-l+1) ++k;
	return min(d[l][k],d[r-(1<<k)+1][k]);
}
