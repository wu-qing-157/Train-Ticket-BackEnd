#ifndef DEBUGER_COUNTER
#define DEBUGER_COUNTER

struct debuger {
	int cntera, cnterb;
	void adda() {++cntera;}
	void addb() {++cnterb;}
};

#endif