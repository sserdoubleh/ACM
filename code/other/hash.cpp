int ELFhash(char *key)
{
	unsigned long h = 0;
	while (*key)
	{
		h = (h<<4) + *key++;
		unsigned long g = h&0xf0000000L;
		if (g) h ^= g>>24;
		h &= ~g;
	}
	return h;
}
