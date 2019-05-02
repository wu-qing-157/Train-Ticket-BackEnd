#ifndef OPERATION
#define OPERATION

#include <cstring>
#include <iostream>
#include "bptree.hpp"
#include "return.hpp"
#include "./vector/vector.hpp"

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

struct info_user{
    wchar_t name[40];
    char email[20], passward[20];
	long long id, phone;
    short previlege;

    info_user(){
        previlege = 1;
    }
    info_user (const info_user& other) = default;
    info_user (wchar_t _name[], char _passward[], char _email[],
          long long _phone, long long _id, short _previlege){
        memcpy(name, _name, 80);
		memcpy(passward, _passward, 20);
        memcpy(email, _email, 20);
		phone = _phone;
		id = _id;
        previlege = _previlege;
    }
};

class user {
private:
	vector<info_user> data;
	long long cur;

public:
	user() {
		cur = 2018;
	}
	bool regist(wchar_t name[], char passward[], char email[], long long phone) {
		info_user tmp(name, passward, email, phone, ++cur, 0);
		if (cur == 2019) tmp.previlege = 1;
		data.push_back(tmp);
		return true;  //I don't quite know when I should return false.
	}
	bool login(long long id, char passward[]) {
		if (strncmp(data[id - 2019].passward, passward, 20)) return true;
		return false;
	}
	query_profile_info query_profile(long long id){
		return query_profile_info(data[id - 2019]);
	}
	bool modify_profile(long long id, wchar_t name[], char passward[], char email[], long long phone) {
		if (id > cur || id < 2019) return false;
		else {
			info_user tmp(name, passward, email, phone, id, data[id - 2019].previlege);
			data[id - 2019] = tmp;
			return true;
		}
	}
	bool modify_previledge(long long id1, long long id2, short previlege) {
		if (id1 > cur || id2 > cur || id1 < 2019 || id2 < 2019) return false;
		if (data[id1 - 2019].previlege < previlege) return false;
		data[id2 - 2019].previlege = previlege;
		return true;
	}
};


//These below haven't been finished yet.
struct time{
    short data;

    time ():data(0){}
    time (char s[]){
        if (s[0] == 'x') data = -1;
        else data = (((s[0]-'0')*10 + s[1]-'0')*10 + s[3]-'0')*10 + s[4]-'0';
    }

    short hour(){return (data / 100);}
    short minute(){return (data % 100);}
};

struct date{
    short year;
    short month;
    short day;

    date() = default;
    date(char s[]){
        year = (((s[0]-'0')*10 + s[1]-'0')*10 + s[3]-'0')*10 + s[4]-'0';
        month = (s[5]-'0')*10 + s[6]-'0';
        day = (s[8]-'0')*10 + s[9]-'0';
    }
};

struct info_station {
	wchar_t name[20];
	time arriv, start, stopover;
	float price[5];
};

class info_train{
private:
	short num_station, num_price;
	wchar_t name[20], name_price[5][20];
	char train_id[20], catalog[10];
	info_station* data;

public:
	info_train(wchar_t _name[], wchar_t _name_price[][20],
		char _train_id[], char _catalog[], short _num_station,
		short _num_price, info_station* _data){
		num_price = _num_price;
		num_station = _num_station;
		memcpy(name, _name, 20 * sizeof(wchar_t));
		for (int i = 0; i < num_price; ++i) {
			memcpy(name_price[i], _name_price[i], 20 * sizeof(wchar_t));
		}
		memcpy(name_price, _name_price, num_price * 4);  //sizeof a pointer is 4
		memcpy(train_id, _train_id, 20);
		memcpy(catalog, _catalog, 10);
		data = new info_station[num_station];
		memcpy(data, _data, num_station * sizeof(info_station));
	}
};

class train {
private:
	sjtu::bptree<int, info_train, 1000, 1000> data;

public:
	train() {}
	int add(const info_train& s) {
		data.insert(, s);
		return 1;
	}
	info_train()


};




/*
class ticket{
private:
    date date_from, date_to;
    time time_from, time_to;
    wchar_t loc_from[20], loc_to[20], ticket_kind[5][20];
    short num_price, ticket_quantity[5];
    float price[5];


};
*/


#endif //OPERATION
