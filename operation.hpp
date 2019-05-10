#ifndef OPERATION
#define OPERATION

#include <cstring>
#include "bptree.hpp"
#include "return.hpp"
#include "structs.hpp"
#include "info.hpp"
#include "exceptions.hpp"
#include "./vector/vector.hpp"

class user {
private:
	sjtu::vector<info_user> data;
	int cur;
public:
	user() {
		cur = 2018;
	}
	int counter() {return cur;}
	int regist(wchar_t name[], char passward[], char email[], char phone[]) {
		info_user tmp(name, passward, email, phone, ++cur, 0);
		if (cur == 2019) tmp.privilege = 1;
		data.push_back(tmp);
		return cur;  // I don't quite know when I should return false.
		// You should return an int instead bool... --ct
	}
	bool login(int id, char passward[]) {
		if (strncmp(data[id - 2019].passward, passward, 20)) return true;
		return false;
	}
	info_query_profile query_profile(int id){
		return info_query_profile(data[id - 2019]);
	}
	bool modify_profile(int id, wchar_t name[], char passward[], char email[], char phone[]) {
		if (id > cur || id < 2019) return false;
		else {
			info_user tmp(name, passward, email, phone, id, data[id - 2019].privilege);
			data[id - 2019] = tmp;
			return true;
		}
	}
	bool modify_priviledge(int id1, int id2, short privilege) {
		if (id1 > cur || id2 > cur || id1 < 2019 || id2 < 2019) return false;
		if (privilege < 0 || privilege > 2) return false;
		if (!data[id1 - 2019].privilege) return false;
		if (data[id2 - 2019].privilege && !privilege) return false;
//		if (data[id1 - 2019].privilege < privilege) return false;
		data[id2 - 2019].privilege = privilege;
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
	bool modify_train(char train_id[], info_train& t) {
		str<char, 20> queryId(train_id);
		if (data.at(queryId).on_sale == true) return false;
		if (data.modify(queryId, t) == false) return false;			//There is a minor problem here.
		return true;
	}

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

#endif   //OPERATION