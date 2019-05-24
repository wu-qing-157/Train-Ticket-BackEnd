#ifndef OPERATION
#define OPERATION

#include <cstring>
#include "bptree.hpp"
#include "return.hpp"
#include "structs.hpp"
#include "info.hpp"
#include "exceptions.hpp"
#include "container.hpp"
#include "operation_user.hpp"
#include "./vector/vector.hpp"

class train {
	friend class ticket;
private:
	sjtu::bptree<str<char, 20>, info_train, 1000> data{ "trainA", "trainB" };  //These two 1000 are written without any thoughts.

public:
	sjtu::vector<wchar_t[20]> loclist;

	train() {}
	bool add(const info_train& t) {   //Maybe there needs to be some changes here.
		data.insert(t.train_id, t);
		return true;	//I need jyq to change bptree.hpp to enable return false(try{} is too slow)
	}
	info_train query_train(char train_id[]) const {
		str<char, 20> queryId(train_id);
		info_train x = data.at(queryId);
		if (x.num_station == -1) throw 1;    //如果查不到信息，num_station我会设为-1
		return data.at(queryId);
	}
	bool delete_train(char train_id[]) {
		str<char, 20> queryId(train_id);
		info_train x = data.at(queryId);
		if (x.num_station == -1) throw 1;
		data.erase(queryId);
		return true;
	}
	bool modify_train(char train_id[], info_train& t) {
		str<char, 20> queryId(train_id);
		info_train x = data.at(queryId);
		if (x.num_station == -1) return false;
		if (x.on_sale == true) return false;
		data.modify(queryId, t);
		return true;
	}
	bool sale_train(char train_id[]) {
		str<char, 20> queryId(train_id);
		info_train x = data.at(queryId);
		if (x.num_station == -1) return false;
		
		for (short i = 0; i < x.num_station; ++i) {
			loc_t a;
			catalog_t catalog;
			str<char, 20> loc(x.train_id);
			pair<str<char, 20>, short> pr;
			pr = std::make_pair(loc, i);
			memcpy(a, x.data[i].name, 20 * sizeof (wchar_t));
			memcpy(catalog, x.catalog, 10 * sizeof(char));
			sjtu::bptree<str<char, 20>, pair<str<char, 20>, short>>
				tree(a + catalog, a + "123" + catalog);
			tree.insert(loc, pr);
		}

		//遍历一遍车站
		//加到b+树里

		return data.at(queryId).sale();    // huge problems may have happened when return wrong.
	}
};

typedef sjtu::vector<info_ticket> ctn_ticket;

class ticket{
private:
	//sjtu::bptree<str<wchar_t, 20>, ctn_train, 1000, 1000> data;   //These two 1000 are written without any thoughts
	//I have not considered catalog yet.
	train* tra;
	user* use;
	
	/*ctn_train mergeAndFind(loc_t a, loc_t b, int day, catalog_t catalog) {
		ctn_train ans;
		sjtu::bptree<str<char, 20>, pair<str<char, 20>, short>>(a + catalog, a + "123" + catalog);
		sjtu::bptree<str<char, 20>, pair<str<char, 20>, short>>(b + catalog, b + "123" + catalog);

		size_t cnta = 0, cntb = 0;

		//以下部分得等贾雨祺遍历函数写完了
		size_t maxa = a.size, maxb = b.size;
		while (cnta < maxa && cntb < maxb) {
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
			else cntb++;
		}
		return ans;
	}*/

public:
	ticket() = default;
	query_train_return query_ticket(loc_t a, loc_t b, int day, catalog_t catalog) {
		ctn_train ans;
		sjtu::bptree<str<char, 20>, pair<str<char, 20>, short>>(a + catalog, a + "123" + catalog);
		sjtu::bptree<str<char, 20>, pair<str<char, 20>, short>>(b + catalog, b + "123" + catalog);

		size_t cnta = 0, cntb = 0;

		//以下部分得等贾雨祺遍历函数写完了
		size_t maxa = a.size, maxb = b.size;
		while (cnta < maxa && cntb < maxb) {
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
				cnta++;
				cntb++;
			}
			if (a[cnta].train_id < b[cntb].train_id) cnta++;
			else cntb++;
		}
		
		
		/*
		str<wchar_t, 20> loc1(_loc1);
		str<wchar_t, 20> loc2(_loc2);
		ctn_train t1 = data.at(loc1);
		ctn_train t2 = data.at(loc2);
		ctn_train t = mergeAndFind(t1, t2, 1);  // How can date be converted to int is to be considered.
		*/

		// to be done:how to return

	}
	query_ticket_return query_transfer(wchar_t _loc1[], wchar_t _loc2[], my_date date, char catalog[]) {
		for (size_t i = 0; i < tra->loclist.size; ++i) {
			query_ticket(_loc1, tra->loclist[i], date, catalog);
			query_ticket(tra->loclist[i], _loc2, date, catalog);
		}
		//to be finished
	}

	bool buy_ticket(int id, short num, str<char, 20> train_id, wchar_t _loc1[],
		wchar_t _loc2[], my_date date, char ticket_kind[]) {  //注意判定ticket_kind是否存在
		int day;  //to do: add a convertion rule
		if (id > use->cur || id < 2019) return false;
		if (str<wchar_t, 40>(_loc1) == str<wchar_t, 40>(_loc2)) return false;
		info_train info = tra->data.at(train_id);
		int i, a, b;
		for (i = 0; i < info.num_station; ++i) {
			if (str<wchar_t, 40>(info.data->name) == str<wchar_t, 40>(_loc1)) {
				a = i;
				break;
			}	//I'm not sure whether str(pointer) is correct
		};
		for (; i < info.num_station; ++i) {
			if (str<wchar_t, 40>(info.data->name) == str<wchar_t, 40>(_loc2)) {
				b = i;
				break;
			}
			if (info.num_ticket[day][i] < num) return false;
		}
		if (i == info.num_station - 1) return false;

		for (i = a; i < b; ++i) {
			tra->data.at(train_id).num_ticket[day][i] -= num;
		}

		//below is how I can stroage user's ticket_ordered data.

	}

	query_order_return query_order(int id, my_date date, char catalog[]) {
		if (id > use->cur || id < 2019) return query_order_return(false);
		ctn_ticket ans = use->data[id - 2019].user_ticket[date];   //date should be transferred into int
		info_ticket* ticket_return = new info_ticket [ans.size()];
		return query_order_return(true, ans.size(), ticket_return);
	}
	bool refund_ticket(int id, short num, my_date date, char loc_from[], char loc_to[], char catalog[]) {
		if (id > use->cur || id < 2019) return false;
		int cnt = 0;
		for (int i = use->data[id - 2019].user_ticket[date].size() - 1; i > 0; --i) {
			if (use->data[id - 2019].user_ticket[date][i].loc_from == loc_from &&
				use->data[id - 2019].user_ticket[date][i].loc_to == loc_to) {
				//I want to change ALL the char[] and wchar_t into str<> (There's a huge bug above)
				cnt++;
				if (cnt >= num) break;
				//Is the counting process is too time-consuming?
			}		
		}
		if (cnt < num) return false;
		cnt = 0;
		for (int i = use->data[id - 2019].user_ticket[date].size() - 1; i > 0; --i) {
			if (use->data[id - 2019].user_ticket[date][i].loc_from == loc_from &&
				use->data[id - 2019].user_ticket[date][i].loc_to == loc_to) {
				//(There's a huge bug above)
				cnt++;
				use->data[id - 2019].user_ticket[date].erase((size_t)i);
				if (cnt >= num) break;
			}
		}
		
	}

};

#endif   //OPERATION