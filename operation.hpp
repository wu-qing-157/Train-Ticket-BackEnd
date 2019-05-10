#ifndef OPERATION
#define OPERATION

#include <cstring>
#include "bptree.hpp"
#include "return.hpp"
#include "structs.hpp"
#include "info.hpp"
#include "./vector/vector.hpp"

class user {
private:
	sjtu::vector<info_user> data;
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
	info_query_profile query_profile(long long id){
		return info_query_profile(data[id - 2019]);
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
		if (data[id2 - 2019].previlege > previlege) return false;
		data[id2 - 2019].previlege = previlege;
		return true;
	}
};

class train {
private:
	sjtu::bptree<str<char, 20>, info_train, 1000, 1000> data;   //These two 1000 are written without any thoughts.

public:
	train() {}
	bool add(const info_train& t) {   //Maybe there needs to be some changes here.
		data.insert(t.train_id, t);
		return true;	//I need jyq to change bptree.hpp to enable return false(try{} is too slow)
	}
	info_train query_train(char train_id[]) {
		str<char, 20> queryId(train_id);
		return data.at(queryId);
	}
	bool delete_train(char train_id[]) {
		str<char, 20> queryId(train_id);
		data.erase(queryId);
		return true;    //I need jyq to change bptree.hpp to enable return false(try{} is too slow)
	}
	bool modify_train(char train_id[], const info_train& t) {
		str<char, 20> queryId(train_id);
		if (data.at(queryId).on_sale == true) return false;
		if (data.modify(queryId, t) == false) return false;			//There is a minor problem here.
		return true;
	}
};



struct info_ticket{
    date date_from, date_to;
    time time_from, time_to;
    wchar_t loc_from[20], loc_to[20], ticket_kind[5][20];
    short num_price, ticket_quantity[5];
    float price[5];

	info_ticket() = default;
	info_ticket(const info_ticket& other) = default;
};

class ticket {
private:
	train* tr;
	str<char, 20> 

public:
	ticket() {}
	ticket(train* _tr):tr(_tr){}
	info_ticket query_ticket(wchar_t loc1[], wchar_t loc2[], date day, char catalog[]) {

	}

};


#endif 