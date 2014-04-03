struct Node
{
	Node *ch[2];
	int r;
	int v;
	int s;
	Node(int v):v(v)
	{
		ch[0]=ch[1]=NULL;
		r=rand();
		s=1;
	}
	bool operator < (const Node &rhs) const
	{ return r<rhs.r; }
	int cmp(int x) const
	{
		if(x==v) return -1;
		return x<v?0:1;
	}
	void maintain()
	{
		s=1;
		if(ch[0]!=NULL) s+=ch[0]->s;
		if(ch[1]!=NULL) s+=ch[1]->s;
	}
};

void rotate(Node* &o,int d)
{
	Node* k=o->ch[d^1];
	o->ch[d^1]=k->ch[d];
	k->ch[d]=o;
	o->maintain();
	k->maintain();
	o=k;
}

void insert(Node* &o,int x)
{
	if(o==NULL) o=new Node(x);
	else
	{
		int d=(x<o->v?0:1);
		insert(o->ch[d],x);
		if(o->ch[d]>o) rotate(o,d^1);
	}
	o->maintain();
}

void remove(Node* &o,int x)
{
	int d=o->cmp(x);
	if(d==-1)
	{
		Node *u=o;
		if(o->ch[0]!=NULL&&o->ch[1]!=NULL)
		{
			int d2=(o->ch[0]>o->ch[1]?1:0);
			rotate(o,d2); remove(o->ch[d2],x);
		}
		else
		{
			if(o->ch[0]==NULL) o=o->ch[1];
			else o=o->ch[0];
			delete u;
		}
	}
	else remove(o->ch[d],x);
	if(o!=NULL) o->maintain();
}

bool find(Node* o,int x)
{
	while(o!=NULL)
	{
		int d=o->cmp(x);
		if(d==-1) return 1;
		o=o->ch[d];
	}
	return 0;
}

int Kth(Node *o,int k)
{
	if(o==NULL||k<=0||k>o->s) return -1;
	int s=(o->ch[0]==NULL?0:o->ch[0]->s);
	if(k==s+1) return o->v;
	else if(k<=s) return Kth(o->ch[0],k);
	else return Kth(o->ch[1],k-s-1);
}

int rank(Node *o,int v)
{
	int d=o->cmp(v);
	int s=(o->ch[0]==NULL?0:o->ch[0]->s);
	if(d==-1) return s+1;
	else if(d==0) return rank(o->ch[0],v);
	else return s+1+rank(o->ch[1],v);
}
