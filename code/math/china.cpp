LL china()
{
	LL M=1,d,y,x=0;
	for(int i=0;i<n;++i) M*=m[i];
	for(int i=0;i<n;++i)
	{
		LL w=M/m[i];
		gcd(m[i],w,d,d,y);
		x=(x+y*w*a[i])%M;
	}
	return (x+M)%M;
}

void china_part(int i)
{
	if(R[0]==n) return;
	LL g=gcd(Mod[0],Mod[i]),C=R[i]-R[0];
	if(C%g!=0) {R[0]=n;return;}
	LL v=inv(Mod[0]/g,Mod[i]/g);
	if(v==-1) {R[0]=n;return;}
	LL K=C/g*v%(Mod[i]/g);
	LL ans=K*Mod[0]+R[0];
	LL ll=Mod[0]*Mod[i]/g;
	R[0]=(ans%ll+ll)%ll;
	Mod[0]=ll;
	return;
}

LL china(int n)
{
	for(int i=1;i<n;++i)
		china_part(i);
	return R[0];
}
