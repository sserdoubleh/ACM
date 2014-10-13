const int N=1<<17;

struct Node
{
	int lc,rc;
	int sz;
};
Node buf[N<<5];

int no;
int num[N];
inline int newnode(int lc,int rc,int sz)
{
	buf[no].lc = lc, buf[no].rc = rc;
	buf[no].sz = sz;
	return no++;
}

int build(int l,int r)
{
	if(l==r) return newnode(0,0,0);
	int m=(l+r)>>1;
	return newnode( build(l,m), build(m+1,r), 0);
}

//p为上次的节点编号，pos表示要更新的位置
int insert(int l,int r,int p,int pos)
{
	if(l==r) return newnode( 0, 0, buf[p].sz+1);
	int m=(l+r)>>1;
	if(pos<=m)
		return newnode( insert(l,m,buf[p].lc,pos), buf[p].rc, buf[p].sz+1);
	else
		return newnode( buf[p].lc, insert(m+1,r, buf[p].rc,pos), buf[p].sz+1);
}
#define bit(z,k) (z&(1<<k))
int query(int l,int r,int u,int v,int x,int y, int z,int dep)
{
	if(l==r) return l^z;
	int m=(l+r)>>1;
	int havel = buf[buf[u].lc].sz + buf[buf[v].lc].sz - buf[buf[x].lc].sz - buf[buf[y].lc].sz;
	int haver = buf[buf[u].rc].sz + buf[buf[v].rc].sz - buf[buf[x].rc].sz - buf[buf[y].rc].sz;
	if( !haver || (havel&&bit(z,dep)) )
		return query( l, m, buf[u].lc, buf[v].lc, buf[x].lc, buf[y].lc, z, dep-1);
	else if( !havel || (haver&&!bit(z,dep)) )
		return query( m+1, r, buf[u].rc, buf[v].rc, buf[x].rc, buf[y].rc, z, dep-1);
}
