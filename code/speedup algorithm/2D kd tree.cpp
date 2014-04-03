typedef long long LL;
typedef pair<LL,LL> pii;
#define X first
#define Y second
bool cmpX(const pii &a,const pii &b)
{
	if(a.X!=b.X) return a.X<b.X;
	return a.Y<b.Y;
}
bool cmpY(const pii &a,const pii &b)
{
	if(a.Y!=b.Y) return a.Y<b.Y;
	return a.X<b.X;
}
LL dist2(const pii &a,const pii &b)
{ return (a.X-b.X)*(a.X-b.X)+(a.Y-b.Y)*(a.Y-b.Y); }
struct KDTree
{
	static const int N=17;
	pii p[2<<N];
	pii P;
	LL res;
	void build(int l,int r,int split)
	{
		if(r<l) return;
		int m=(l+r)>>1;
		nth_element(p+l,p+m,p+r+1,split?cmpX:cmpY);
		build(l,m-1,split^1);
		build(m+1,r,split^1);
	}
	void find(int l,int r,int split)
	{
		if(r<l) return;
		int m=(l+r)>>1;
		LL dist=dist2(P,p[m]);
		if(dist>0) res=min(res,dist);
		LL d=split?(P.X-p[m].X):(P.Y-p[m].Y);
		int l1=l,r1=m-1,l2=m+1,r2=r;
		if(d>0) swap(l1,l2),swap(r1,r2);
		find(l1,r1,split^1);
		if(d*d<res) find(l2,r2,split^1);
	}
};
