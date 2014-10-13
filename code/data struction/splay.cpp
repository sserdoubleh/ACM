struct Node
{
	Node *ch[2];
	bool flip;
	int v;
	int s;
	Node(int v):v(v), s(1),flip(0) {}
	int cmp(int k) const
	{
		int ts = (ch[0] ? ch[0]->s: 0) + 1;
		if (k == ts) return -1;
		return k < ts ? 0 : 1;
	}
	void maintain()
	{
		s = 1;
		if (ch[0]) s += ch[0]->s;
		if (ch[1]) s += ch[1]->s;
	}
	void pushdown()
	{
		if (flip)
		{
			flip = 0;
			swap(ch[0], ch[1]);
			if (ch[0]) ch[0]->flip ^= 1;
			if (ch[1]) ch[1]->flip ^= 1;
		}
	}
};
Node* build(int *a, int sz)
{
	if (!sz) return NULL;
	Node* o = new Node(*(a+sz/2+1));
	o->ch[0] = build(a, sz/2);
	o->ch[1] = build(a+sz/2+1, sz-sz/2-1);
	o->maintain();
	return o;
}

// d = 0 left rotate, d = 1 right rotate
void rotate(Node* &o, int d)
{
	Node *k = o->ch[d^1]; o->ch[d^1] = k->ch[d]; k->ch[d] = o;
	o->maintain(); k->maintain(); o = k;
}

void splay(Node* &o, int k)
{
	o->pushdown();
	int d = o->cmp(k);
	if (d == 1) k -= (o->ch[0]? o->ch[0]->s: 0) + 1;
	if (d != -1)
	{
		Node *p = o->ch[d];
		p->pushdown();
		int d2 = p->cmp(k);
		if (d2 == 1) k -= (p->ch[0]? p->ch[0]->s: 0) + 1;
		if (d2 != -1)
		{
			splay(p->ch[d2], k);
			if (d == d2) rotate(o, d^1); else rotate(o->ch[d], d);
		}
		rotate(o, d^1);
	}
}
Node *merge(Node* left, Node* right)
{
	splay(left, left->s);
	left->ch[1] = right;
	left->maintain();
	return left;
}
void split(Node* o, int k, Node* &left, Node* &right)
{
	splay(o, k);
	left = o;
	right = o->ch[1];
	left->ch[1] = NULL;
	left->maintain();
}
