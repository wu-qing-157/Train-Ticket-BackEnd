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
	template<class T, int M>
	str<T, M + N> operator+(const str<T, M> other) {
		str <T, M + N> ans;
		for (int i = 0; i < N; ++i) {
			ans.data[i] = data[i];
		}
		for (int i = n; i < M + N; ++i) {
			ans.data[i] = data[i];
		}
		return ans;
	}
};

struct my_time {
	short data;

	my_time() :data(0) {}
	my_time(char s[]) {
		if (s[0] == 'x') data = -1;
		else data = (((s[0] - '0') * 10 + s[1] - '0') * 100 + s[3] - '0') * 10 + s[4] - '0';
	}

	short hour() const { return (data / 100); }
	short minute() const { return (data % 100); }
	void print(char end = 0) const
	{
		if (data == -1) printf("xx:xx");
		else printf("%hd:%hd", hour(), minute());
		if (end) printf("%c", end);
	}
	void modify_hour(short new_hour) {
		data = new_hour * 100 + minute();
	}
	void modify_minute(short new_minute) {
		data = hour() * 100 + new_minute;
	}
	bool operator< (const my_time& other) const{
		return data < other.data;
	}
	bool operator== (const my_time& other) const {
		return data == other.data;
	}
	bool operator> (const my_time& other) const {
		return data > other.data;
	}
	my_time operator- (const my_time& other) const {
		my_time ans;
		if (*this < other) throw 1;
		if (this->minute() >= other.minute()) {
			ans.modify_minute(this->minute() - other.minute());
			ans.modify_hour(this->hour - other.hour());
		}
		else {
			ans.modify_minute(this->minute() + 60 - other.minute());
			ans.modify_hour(this->hour - 1 - other.hour());
		}
		return ans;
	}
	my_time operator= (const my_time& other) {
		data = other.data;
		return *this;
	}

};

struct my_date {
	short year;
	short month;
	short day;

	my_date() = default;
	my_date(char s[]) {
		year = (((s[0] - '0') * 10 + s[1] - '0') * 10 + s[3] - '0') * 10 + s[4] - '0';
		month = (s[5] - '0') * 10 + s[6] - '0';
		day = (s[8] - '0') * 10 + s[9] - '0';
	}
};

#endif  //STRUCTS