struct Node
{
	Node *ch[2];
	int r;
	int v;
	int s;
	Node (int v): v(v)
	{
		ch[0] = ch[1] = NULL;
		r = rand();
		s = 1;
	}
	bool operator < (const Node &rhs) const
	{ return r < rhs. r; }
	int cmp(int x) const
	{
		if (x == v) return -1;
		return x < v ? 0 : 1;
	}
	void maintain()
	{
		s = 1;
		if (ch[0]) s += ch[0]->s;
		if (ch[1]) s += ch[1]->s;
	}
};
// d = 0 left rotate, d = 1 right rotate
void rotate(Node* &o, int d)
{
	Node *k = o->ch[d^1]; o->ch[d^1] = k->ch[d]; k->ch[d] = o;
	o->maintain(); k->maintain(); o = k;
}
void insert(Node* &o, int x)
{
	if (!o) o = new Node(x);
	else
	{
		int d = o->cmp(x);
		// in the treap only one unique value for any node
		if (d != -1)
		{
			insert(o->ch[d], x);
			if (o->ch[d] > o) rotate(o, d^1);
		}
	}
	o->maintain();
}
void remove(Node* &o, int x)
{
	// the value does not exist
	if (!o) return;
	int d = o->cmp(x);
	if (d == -1)
	{
		if (!o->ch[0]) o = o->ch[1];
		else if (!o->ch[1]) o = o->ch[0];
		else
		{
			int d2 = (o->ch[0] > o->ch[1] ? 1: 0);
			rotate(o, d2); remove(o->ch[d2], x);
		}
	}
	else remove(o->ch[d], x);
	if (o) o->maintain();
}
int kth(Node *o, int k)
{
	// the kth value does not exist
	if (!o || k <= 0 || k > o->s) return -1;
	int s = o->ch[0]? o->ch[0]->s: 0;
	if (k == s+1) return o->v;
	else if (k <= s) return kth(o->ch[0], k);
	else return kth(o->ch[1], k-s-1);
}
int rank(Node *o, int x)
{
	// the value does not exist
	if (!o) return -1;
	int d = o->cmp(x);
	int s = o->ch[0]? o->ch[0]->s: 0;
	if (d == -1) return s+1;
	else if (d == 0) return rank(o->ch[0], x);
	else
	{
		int ret = rank(o->ch[1], v);
		if (~ret) return ret + s + 1;
		else return -1;
	}
}
