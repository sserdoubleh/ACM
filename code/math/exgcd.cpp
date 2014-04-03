void gcd(LL a,LL b,LL &d,LL &x,LL&y)
{
	if(b==0) { d=a,x=1,y=0; }
	else { gcd(b,a%b,d,y,x); y-=x*(a/b); }
}
