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
		if (x.num_station == -1) return false;
		data.erase(queryId);
		return true;
	}
	bool modify_train(char train_id[], const info_train& t) {
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
			str<char, 20> tid(x.train_id);
			pair<str<char, 20>, short> pr;
			pr = std::make_pair(tid, i);
			memcpy(a, x.data[i].name, 20 * sizeof(wchar_t));
			memcpy(catalog, x.catalog, 10 * sizeof(char));
			sjtu::bptree<str<char, 20>, pair<str<char, 20>, short>>
				tree(a + catalog, a + "123" + catalog);
			tree.insert(tid, pr);
		}

		//遍历一遍车站
		//加到b+树里

		return data.at(queryId).sell();    // huge problems may have happened when return wrong.
	}
};

typedef sjtu::vector<info_ticket> ctn_ticket;

class ticket {
private:
	train* tra;
	user* use;

public:
	ticket() = default;
	sjtu::vector<info_ticket> query_ticket(loc_t a, loc_t b, int day, catalog_t catalog) {
		
		typedef pair<str<char, 20>, short> value_t;
		typedef sjtu::bptree<str<char, 20>, pair<str<char, 20>, short>>::iterator bpt_itr;

		sjtu::vector<info_ticket> ans;
		sjtu::bptree<str<char, 20>, value_t> treeA(a + catalog, a + "123" + catalog);
		sjtu::bptree<str<char, 20>, value_t> treeB(b + catalog, b + "123" + catalog);

		typedef pair<str<char, 20>, short> value_t;
		typedef sjtu::bptree<str<char, 20>, pair<str<char, 20>, short>>::iterator bpt_itr;

		bpt_itr iA = treeA.begin();
		bpt_itr iB = treeB.begin();
		value_t cA = *iA;
		value_t cB = *iB;

		while (iA != treeA.end() && iB != treeB.end()) {
			if (cA.first == cB.first) {
				if (cA.second >= cB.second) {
					info_train info = tra->data.at(cA.first);
					info_ticket tic;
					tic.date = my_date(day);
					tic.time_from = info.data[cA.second].arriv;
					tic.time_to = info.data[cB.second].start;
					memcpy(tic.loc_from, info.data[cA.second].name, 20 * sizeof(wchar_t));
					memcpy(tic.loc_to, info.data[cB.second].name, 20 * sizeof(wchar_t));
					//I'm not sure whether above two sentencces are what's suppposed to be.
					tic.num_price = info.num_price;
					for (int i = 0; i < info.num_price; ++i) {
						memcpy(tic.ticket_kind[i], info.name_price[i], 20 * sizeof(wchar_t));
						int cnt = 2000;
						for (int j = cA.second; j < cB.second; ++j) {
							if (cnt > info.quan_ticket[i][j][day]) cnt = info.quan_ticket[i][j][day];
						}
						tic.ticket_quantity[i] = cnt;
					}
					bool jdg = false;
					for (int i = 0; i < tic.num_price; ++i) {
						if (tic.ticket_quantity[i] != 0) jdg = true;
						break;
					}
					if (jdg) ans.push_back(tic);
				}
				iA++; iB++;
				cA = *iA; cB = *iB;

			}
			else if ((*iA).first < (*iB).first) { iA++; cA = *iA; }
			else { iB++; cB = *iB; }
		}

		return ans;

		/*size_t maxa = a.size, maxb = b.size;
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
		}*/
		
		
		/*
		str<wchar_t, 20> loc1(_loc1);
		str<wchar_t, 20> loc2(_loc2);
		ctn_train t1 = data.at(loc1);
		ctn_train t2 = data.at(loc2);
		ctn_train t = mergeAndFind(t1, t2, 1);  // How can date be converted to int is to be considered.
		*/
	}
	
	pair<info_ticket, info_ticket> query_transfer(wchar_t _loc1[], wchar_t _loc2[], int day, char catalog[]) {
		
		pair<info_ticket, info_ticket> ans;

		//QUESTION: HOW CAN loclist BE CREATED / STORED
		for (size_t k = 0; k < tra->loclist.size(); ++k) {
			sjtu::vector<info_ticket> bfr = query_ticket(_loc1, tra->loclist[k], day, catalog);
			sjtu::vector<info_ticket> aft = query_ticket(tra->loclist[k], _loc2, day, catalog);
			
			for (int i = 0; i < bfr.size(); ++i) {
				for (int j = 0; i < aft.size(); ++j) {
					if (bfr[i].time_to > aft[j].time_from) continue;
					if (ans.first.num_price = -1 ||
						((aft[j].time_to - bfr[i].time_from) < (ans.second.time_to - ans.first.time_from))) {
						ans.first = bfr[i];
						ans.first = aft[j];
					}
				}
			}
		}
		
		return ans;
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
		vector<buy_info>
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