#include <cstdio>
#include "bptree.hpp"

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
	sjtu::bptree<KKey, VVal> tr("fuck", "fuckk");
	for (int i = 1; i < 7; ++i)
		tr.insert(KKey(i * i % 7, i), VVal(i * i * i));
	for (auto it = tr.cbegin(); it != tr.cend(); ++it) printf("%d\n", (*it).p );
	tr.clear();
	return 0;
}