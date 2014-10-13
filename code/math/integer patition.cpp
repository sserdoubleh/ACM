const int maxn=110000;
const int mod=1000000007;
int n,m;
int cnt;
int p[maxn],q[maxn];
void init()
{
	cnt=0;
	for(int i=1;;++i)
	{
		int tm1=(3*i*i-i)/2,tm2=(3*i*i+i)/2;
		if(tm2>maxn) break;
		q[cnt++]=tm1;
		q[cnt++]=tm2;
	}
	p[0]=1;
	for(int i=1;i<maxn;++i)
	{
		p[i]=0;
		for(int j=0;j<cnt&&q[j]<=i;++j)
			(p[i]+=(j&2?-1:1)*p[i-q[j]])%=mod;
	}
}
