void manacher(char *t, int *p, int n)
{
	char *s = new char[2*n];
	for (int i = 0; i <= n; i++)
		s[i<<1] = '#', s[i<<1|1] = t[i];
	n <<= 1;
	int mx = 0, id = -1;
	for (int i = 0; i < n; i++)
	{
		if (mx > i)
			p[i] = min(p[2*id-i], mx-i);
		else
			p[i] = 1;
		while (i - p[i] >= 0 && i + p[i] < n && s[i-p[i]] == s[i+p[i]]) p[i]++;
		if (i + p[i] > mx)
		{
			mx = i+p[i];
			id = i;
		}
	}
}