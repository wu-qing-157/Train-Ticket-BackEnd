#ifndef STRUCTS
#define STRUCTS
#include <cstring>
#include <iostream>
#include <iomanip>

using std::ostream;
using std::setfill;
using std::setw;

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
	bool operator<(const str& other) const {
		for (int i = 0; i < N; ++i) {
			if (data[i] < other.data[i]) return true;
			if (data[i] > other.data[i]) return false;
		}
		return false;
	}
	bool operator>(const str& other) const {
		for (int i = 0; i < N; ++i) {
			if (data[i] > other.data[i]) return true;
			if (data[i] < other.data[i]) return false;
		}
		return false;
	}
	bool operator==(const str& other) const {
		for (int i = 0; i < N; ++i) {
			if (data[i] != other.data[i]) return false;
		}
		return true;
	}
	template<T, int M>
	str<T, M + N> operator+(const str<T, M> other) {
		str <T, M + N> ans;
		for (int i = 0; i < N; ++i) {
			ans.data[i] = data[i];
		}
		for (int i = N; i < M + N; ++i) {
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
		if (data == -1 || other.data == -1) return false;
		return data < other.data;
	}
	bool operator== (const my_time& other) const {
		return data == other.data;
	}
	bool operator> (const my_time& other) const {
		if (data == -1 || other.data == -1) return false;
		return data > other.data;
	}
	my_time operator- (const my_time& other) const {
		if (data == -1 || other.data == -1) throw 2;
		my_time ans;
		if (*this < other) throw 1;
		if (this->minute() >= other.minute()) {
			ans.modify_minute(this->minute() - other.minute());
			ans.modify_hour(this->hour() - other.hour());
		}
		else {
			ans.modify_minute(this->minute() + 60 - other.minute());
			ans.modify_hour(this->hour() - 1 - other.hour());
		}
		return ans;
	}
	my_time operator= (const my_time& other) {
		data = other.data;
		return *this;
	}
	short day_added() const {
		return (hour() / 24);
	}
	void normalize() {
		modify_hour(hour() % 24);
	}
	friend ostream& operator << (ostream &os, const my_time &obj);
};
ostream& operator << (ostream &os, const my_time &obj)
{
	if (obj.data == -1) os << "xx:xx";
	else os << setw(2) << setfill('0') << obj.hour() << ':' << setw(2) << setfill('0') << obj.minute();
	return os;
}

struct my_date {
	short day;

	my_date() = default;
	my_date(const my_date& other) = default;
	my_date(char s[]) {  //this function read in string like "2019-06-21"
		day = (s[8] - '0') * 10 + s[9] - '0';
	}
	operator short() const {
		return day - 1;
	}
	my_date plus(short i) {
		day += i;
		return *this;
	}
	friend ostream& operator << (ostream &os, const my_date &obj);
};
ostream& operator << (ostream &os, const my_date &obj)
{
	os << "2019-06-" << setw(2) << setfill('0') << obj.day;
	return os;
}

#endif  //STRUCTS