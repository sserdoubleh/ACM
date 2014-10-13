#include <cstdio>
#include <complex>
#include <iostream>
#include <algorithm>
using namespace std;

#define For0(i,n) for(int i=0;i<(n);++i)
#define Forto(i,st,et) for(int i=(st);i<(et);++i)

typedef complex<double> com;
typedef long long LL;

const int maxlen=1<<17;
const double pi=acos(-1.0);

//单个位转置
int inv(int x,int n)
{
	int res=0;
	for(int i=0;i<n;++i) if(x&(1<<i))
		res|=1<<(n-1-i);
	return res;
}

//位转置
void changeinvbits(com *a,com *A,int len)
{
	int n=0;
	while((1<<n)<len) n++;
	for(int i=0;i<len;++i)
		A[inv(i,n)]=a[i];
}
void fft(com *a,com *A,int n,double flag=1)
{
	com u,t;
	changeinvbits(a,A,n);
	for(int h=2;h<=n;h<<=1)
	{
		com wn(cos(flag*2*pi/h),sin(flag*2*pi/h));
		for(int j=0;j<n;j+=h)
		{
			com w(1,0);
			for(int k=j;k<j+h/2;++k)
			{
				u=A[k];
				t=w*A[k+h/2];
				A[k]=u+t;
				A[k+h/2]=u-t;
				w*=wn;
			}
		}
	}
	if(flag==-1) for(int i=0;i<n;++i)
		A[i]/=n;
}

com buf1[maxlen],buf2[maxlen];
void mul(int n,com *a,com *b,com *c)
{
	fft(a,buf1,n);
	fft(b,buf2,n);
	for(int i=0;i<n;++i)
		c[i]=buf1[i]*buf2[i];
	fft(c,buf1,n,-1);
	For0(i,n) c[i]=buf1[i];
}

com a[maxlen],b[maxlen],c[maxlen];
char A[maxlen],B[maxlen],C[maxlen];
LL ta[maxlen],tb[maxlen],tc[maxlen];
