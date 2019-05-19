#ifndef OPERATION
#define OPERATION

#include <cstring>
#include "bptree.hpp"
#include "return.hpp"
#include "structs.hpp"
#include "info.hpp"
#include "exceptions.hpp"
#include "container.hpp"
#include "./vector/vector.hpp"

class user {
private:
	sjtu::vector<info_user> data;
	int cur;
public:
	user() {
		cur = 2018;
	}
	int counter() const { return cur; }
	int regist(wchar_t name[], char passward[], char email[], char phone[]) {
		info_user tmp(name, passward, email, phone, ++cur, 0);
		if (cur == 2019) tmp.privilege = 1;
		data.push_back(tmp);
		return cur;  // I don't quite know when I should return false.
		// You should return an int instead bool... --ct
	}
	bool login(int id, char passward[]) const {
		if (strncmp(data[id - 2019].passward, passward, 20)) return true;
		return false;
	}
	info_query_profile query_profile(int id) const {
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
		//if (data[id1 - 2019].privilege < privilege) return false;
		data[id2 - 2019].privilege = privilege;
		return true;
	}
};

class train {
	friend class ticket;
private:
	sjtu::bptree<str<char, 20>, info_train, 1000, 1000> data;   //These two 1000 are written without any thoughts.
	ticket* tik;

public:
	sjtu::vector<wchar_t[20]> loclist;

	train() {}
	bool add(const info_train& t) {   //Maybe there needs to be some changes here.
		data.insert(t.train_id, t);
		return true;	//I need jyq to change bptree.hpp to enable return false(try{} is too slow)
	}
	info_train query_train(char train_id[]) const {
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

typedef sjtu::vector<info_ticket> ctn_ticket;

class ticket{
	friend class train;
private:
	sjtu::bptree<str<wchar_t, 20>, ctn_train, 1000, 1000> data;   //These two 1000 are written without any thoughts
	//I have not considered catalog yet.
	train* tra;
	
	ctn_train mergeAndFind(ctn_train a, ctn_train b, int day) {
		ctn_train ans;
		size_t cnta = 0, cntb = 0;
		size_t maxa = a.size, maxb = b.size;
		while (cnta <= maxa && cntb <= maxb) {
			if (a[cnta].train_id == b[cntb].train_id) {
				bool jdg = true;
				if (a[cnta].num >= b[cntb].num) jdg = false;
				if (jdg) {
					for (int i = a[cnta].num; i < b[cntb].num; ++i){
						if (tra->data.at(a[cnta].train_id).num_ticket[day][i] == 0) {
							jdg = false;
							break;
						}
					}
				}
				if (jdg) ans.push_back(a[cnta]);
				cnta++;
				cntb++;
			}
			if (a[cnta].train_id < b[cntb].train_id) cnta++;
			if (a[cnta].train_id > b[cntb].train_id) cntb++;
		}
		while (cnta <= maxa) {
			if (a[cnta].train_id > b[cntb].train_id) break;
			if (a[cnta].train_id == b[cntb].train_id) {
				bool jdg = true;
				if (a[cnta].num >= b[cntb].num) jdg = false;
				if (jdg) {
					for (int i = a[cnta].num; i < b[cntb].num; ++i) {
						if (tra->data.at(a[cnta].train_id).num_ticket[day][i] == 0) {
							jdg = false;
							break;
						}
					}
				}
				if (jdg) ans.push_back(a[cnta]);
				break;
			}
			if (a[cnta].train_id < b[cntb].train_id) cnta++;
		}
		while (cntb >= maxb) {
			if (a[cnta].train_id < b[cntb].train_id) break;
			if (a[cnta].train_id == b[cntb].train_id) {
				bool jdg = true;
				if (a[cnta].num >= b[cntb].num) jdg = false;
				if (jdg) {
					for (int i = a[cnta].num; i < b[cntb].num; ++i) {
						if (tra->data.at(a[cnta].train_id).num_ticket[day][i] == 0) {
							jdg = false;
							break;
						}
					}
				}
				if (jdg) ans.push_back(a[cnta]);
				break;
			}
			if (a[cnta].train_id > b[cntb].train_id) cntb++;
		}
		return ans;
	}

public:
	ticket() = default;
	query_ticket_return query_ticket(wchar_t _loc1[], wchar_t _loc2[], date dat, char catalog[]) {
		str<wchar_t, 20> loc1(_loc1);
		str<wchar_t, 20> loc2(_loc2);
		ctn_train t1 = data.at(loc1);
		ctn_train t2 = data.at(loc2);
		ctn_train t = mergeAndFind(t1, t2, 1);  // How can date be converted to int is to be considered.

		// to be done: return function

	}
	query_ticket_return query_transfer(wchar_t _loc1[], wchar_t _loc2[], date dat, char catalog[]) {
		for (size_t i = 0; i < tra->loclist.size; ++i) {
			query_ticket(_loc1, tra->loclist[i], dat, catalog);
			query_ticket(_loc2, tra->loclist[i], dat, catalog);
		}
		//to be finished
	}

	bool buy_ticket(long long id, short num, str<char, 20> train_id, wchar_t _loc1[],
		wchar_t _loc2[], date dat, char ticket_kind[]) {
		int day;  //to do: add a convertion rule
		if (str<wchar_t, 40>(_loc1) == str<wchar_t, 40>(_loc2)) return false;
		info_train info = tra->data.at(train_id);  //to be added: a copy constructer
		int i, a, b;
		for (i = 0; i < info.num_station; ++i) {
			if (str<wchar_t, 40>(info.data->name) == str<wchar_t, 40>(_loc1)) {
				a = i;
				break;
			}	//I'm not sure whether str(pointer) is correct
		};
		for (; i < info.num_station; ++i) {
			if (str<wchar_t, 40>(info.data->name) == str<wchar_t, 40>(_loc1)) {
				b = i;
				break;
			}
			if (info.data.num_ticket[day][i] < num) return false;   // why is there a bug??
		}
		if (i == info.num_station - 1) return false;

		for (int i = a; i < b; ++i) {
			tra->data.at(train_id).num_ticket[day][i] -= num;
		}
		return true;
	}


};


#endif   //OPERATION