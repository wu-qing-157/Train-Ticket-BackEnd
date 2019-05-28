#include <cstdio>
#include "bptree.hpp"
#include "structs.hpp"

struct KKey
{
	int a, b;
	inline bool operator < (const KKey &that) const {return a < that.a || (a == that.a && b < that.b);}
	KKey() = default;
	KKey(const int &_a, const int &_b) : a(_a), b(_b) {}
	KKey(const KKey &that) {a = that.a; b = that.b;}
	void print() {printf("a %d b %d\n", a, b);}
} ;
struct VVal {
	int p;
	VVal(const int &_p) : p(_p) {}
	VVal(const VVal &that) {p = that.p;}
	void print() {printf("p = %d\n", p );}
} ;

int main()
{
	sjtu::bptree<str<char, 20>, VVal> tr("fuck", "fuckk");
	str<char, 20> pp; char mmp[20];
	cin >> mmp;

	pp = mmp;
	tr.insert(pp, 100);
	printf("%d\n", tr.count(pp) );
	//for (int i = 1; i < 7; ++i)
	//	tr.insert(KKey(i * i % 7, i), VVal(i * i * i));
//	for (int i = 0; i < 10; ++i) printf("%d\n", tr.count(KKey(i * i % 7, i)) );
	tr.clear();
	return 0;
}