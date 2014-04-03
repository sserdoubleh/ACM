struct SplayTree
{
	struct Node
	{
		Node *ch[2];
		int v;
		int s;
		Node(int v):v(v)
		{
			ch[0]=ch[1]=NULL;
			s=1;
		}
		int cmp(int x) const
		{
			int sz=ch[0]==NULL?0:ch[0]->s;
			if(sz==x) return -1;
			return x<=sz?0:1;
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

	void splay(Node* &o,int k)
	{
		int d=o->cmp(k);
		if(d==1) k-=o->ch[0]->s+1;
		if(d!=-1)
		{
			Node* p=o->ch[d];
			int d2=p->cmp(k);
			if(d2==1) k-=p->ch[0]->s+1;
			if(d2==-1)
			{
				splay(p->ch[d2],k);
				if(d==d2) rotate(o,d^1);
				else rotate(o->ch[d],d);
			}
			rotate(o,d^1);
		}
	}

	Node* merge(Node *left,Node *right)
	{
		splay(left,left->s);
		left->ch[1]=right;
		left->maintain();
		return left;
	}

	void split(Node *o,int k,Node* &left,Node* &right)
	{
		splay(o,k);
		left=o;
		right=o->ch[1];
		o->ch[1]=NULL;
		left->maintain();
	}

	Node *build(int *a,int sz)
	{
		if(!sz) return NULL;
		Node *o=new Node(*a);
		o->ch[0]=build(a,sz/2);
		o->ch[1]=build(a+sz/2+1,sz-sz/2-1);
		o->maintain();
		return o;
	}

	Node *root;
	void init(int *a,int sz)
	{ root=build(a,sz); }
};
