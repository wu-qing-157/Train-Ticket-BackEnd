#ifndef STRUCTS
#define STRUCTS
#include <cstring>
#include <iostream>

template<class T, int N>
struct str {
	T data[N];

	str() = default;
	str(T _data[]) {
		memcpy(data, _data, N * sizeof(T));
	}
	void cpy(T _data[]) {
		memcpy(data, _data, N * sizeof(T));
	}
	bool operator<(const str& other) {
		for (int i = 0; i < N; ++i) {
			if (data[i] < other.data[i]) return true;
			if (data[i] > other.data[i]) return false;
		}
		return false;
	}
	bool operator>(const str& other) {
		for (int i = 0; i < N; ++i) {
			if (data[i] > other.data[i]) return true;
			if (data[i] < other.data[i]) return false;
		}
		return false;
	}
	bool operator==(const str& other) {
		for (int i = 0; i < N; ++i) {
			if (data[i] != other.data[i]) return false;
		}
		return true;
	}
};

struct time {
	short data;

	time() :data(0) {}
	time(char s[]) {
		if (s[0] == 'x') data = -1;
		else data = (((s[0] - '0') * 10 + s[1] - '0') * 10 + s[3] - '0') * 10 + s[4] - '0';
	}

	short hour() { return (data / 100); }
	short minute() { return (data % 100); }
};

struct date {
	short year;
	short month;
	short day;

	date() = default;
	date(char s[]) {
		year = (((s[0] - '0') * 10 + s[1] - '0') * 10 + s[3] - '0') * 10 + s[4] - '0';
		month = (s[5] - '0') * 10 + s[6] - '0';
		day = (s[8] - '0') * 10 + s[9] - '0';
	}
};

#endif  //STRUCTS