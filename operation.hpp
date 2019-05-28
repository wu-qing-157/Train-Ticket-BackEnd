#ifndef OPERATION
#define OPERATION

#include <cstring>
#include "bptree.hpp"
#include "return.hpp"
#include "structs.hpp"
#include "info.hpp"
#include "container.hpp"
#include "operation_user.hpp"
#include "ct_vector.hpp"
#include "./vector/vector.hpp"

typedef char name_t[40];
typedef char password_t[20];
typedef char email_t[20];
typedef char phone_t[20];
typedef char loc_t[20];
typedef char date_t[20];
typedef char catalog_t[10];
typedef char ticket_kind_t[20];
typedef char train_id_t[20];

class train {
	friend class ticket;
private:
	sjtu::bptree<str<char, 20>, info_train> data{ "trainData", "trainIndex" };  

public:
	sjtu::bptree<str<char, 20>, str<char, 20>> llist{"llistData", "llistIndex"};
	sjtu::bptree<str<char, 31>, str<char, 31>> lclist{"lclistData", "lclistIndex"};

	train() {}
	bool add(info_train& t) {   //Maybe there needs to be some changes here.
		if (data.count(t.train_id)) return false;
//		cout << t.train_id << ' add_train_id\n';
		data.insert(t.train_id, t);
		return true;	//I need jyq to change bptree.hpp to enable return false(try{} is too slow)
	}
	info_train query_train(char train_id[]) const {
		str<char, 20> queryId(train_id);
		// cout << train_id << " query_train_id\n";
		// if (!data.count(queryId)) return info_train();
		info_train x = data.at(queryId);
		return data.at(queryId);
	}
	bool delete_train(char train_id[]) {
		str<char, 20> queryId(train_id);
		if (!data.count(queryId)) return false;
		info_train x = data.at(queryId);
		data.erase(queryId);
		return true;
	}
	bool modify_train(char train_id[], const info_train& t) {
		str<char, 20> queryId(train_id);
		if (!data.count(queryId)) return false;
		info_train x = data.at(queryId);
		if (x.on_sale >= 0) return false;
		data.modify(queryId, t);
		return true;
	}
	bool sale_train(char train_id[]) {
		str<char, 20> queryId(train_id);
		if (!data.count(queryId)) return false;
		info_train x = data.at(queryId);

		for (int i = 0; i < x.num_station; ++i) {
			llist.insert(x.data[i].name, x.data[i].name);
			char s[31];
			sprintf(s, "%s_%s", x.data[i].name, x.catalog);
			lclist.insert(s, s);
		}

		for (short i = 0; i < x.num_station; ++i) {
			loc_t a;
			catalog_t catalog;
			str<char, 20> tid(x.train_id);
			pair<str<char, 20>, short> pr;
			pr = std::make_pair(tid, i);
			memcpy(a, x.data[i].name, 20 * sizeof(char));
			memcpy(catalog, x.catalog, 10 * sizeof(char));
			char filename[36], ffname[37];
			sprintf(filename, "data_%s_%s", a, catalog);
			sprintf(ffname, "index_%s_%s", a, catalog);

			sjtu::bptree<str<char, 20>, pair<str<char, 20>, short>>
				tree(filename, ffname);
			tree.insert(tid, pr);
		}
		//above: traverse all the station and push them into bptree

		return data.at(queryId).sell();    // huge problems may have happened when return wrong.
	}
	void clean() {
		data.clear();
		llist.clear();
		lclist.clear();
	}
};

typedef sjtu::vector<info_ticket> ctn_ticket;

const char fnametic[] = "ticket_bought_data";
//const char fname_ticket_number[] = "ticket_quantity_data";

class ticket {
private:
	train* tra;
	user* use;
	ct::vector<info_ticket_user, fnametic> data;

public:
	ticket() = default;
	ticket(train *_tr, user *_us) : tra(_tr), use(_us) {}

	sjtu::vector<info_ticket> query_ticket(loc_t a, loc_t b, my_date date, catalog_t catalog) {
		
		typedef pair<str<char, 20>, short> value_t;
		typedef sjtu::bptree<str<char, 20>, pair<str<char, 20>, short>>::iterator bpt_itr;

		sjtu::vector<info_ticket> ans;

		char filename1[36], filename2[36];
		sprintf(filename1, "data_%s_%s", a, catalog);
		sprintf(filename2, "data_%s_%s", b, catalog);
		FILE *f = fopen(filename1, "rw+");
		if (!f) return ans;  //NOTE HERE: this ans cannot be used directedly
		f = fopen(filename2, "rw+");
		if (!f) return ans;  //NOTE HERE: this ans cannot be used directedly

		char ffname1[37], ffname2[37];
		sprintf(ffname1, "index_%s_%s", a, catalog);
		sprintf(ffname2, "index_%s_%s", b, catalog);

		sjtu::bptree<str<char, 20>, value_t> treeA(filename1, ffname1);
		sjtu::bptree<str<char, 20>, value_t> treeB(filename2, ffname2);


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
					ct::vector<ticket_number_t, fname_ticket_number> vv;
					ticket_number_t quan = vv[info.on_sale];
					info_ticket tic;
					memcpy(tic.train_id, cA.first.data, 20);
					tic.date = date;
					tic.time_from = info.data[cA.second].arriv;
					tic.time_to = info.data[cB.second].start;
					//I'm not sure whether above two sentencces are what's suppposed to be.
					memcpy(tic.loc_from, info.data[cA.second].name, 20 * sizeof(char));
					memcpy(tic.loc_to, info.data[cB.second].name, 20 * sizeof(char));
					tic.num_price = info.num_price;
					for (int i = 0; i < info.num_price; ++i) {
						memcpy(tic.ticket_kind[i], info.name_price[i], 20 * sizeof(char));
						int cnt = 2000;
						float price = 0;
						for (int j = cA.second; j < cB.second; ++j) {
							if (cnt > quan.at(date, i, j)) cnt = quan.at(date, i, j);
							price += info.data[j + 1].price[i];
						}
						tic.ticket_quantity[i] = cnt;
						tic.price[i] = price;
					}
					bool jdg = false;
					for (int i = 0; i < tic.num_price; ++i) {
						if (tic.ticket_quantity[i] != 0) jdg = true;
						break;
					}
					tic.setnormal();
					if (jdg) ans.push_back(tic);
				}
				iA++; iB++;
				cA = *iA; cB = *iB;

			}
			else if ((*iA).first < (*iB).first) { iA++; cA = *iA; }
			else { iB++; cB = *iB; }
		}

		return ans;
	}
	
	pair<info_ticket, info_ticket> query_transfer(char _loc1[], char _loc2[], my_date date, char catalog[]) {
		
		pair<info_ticket, info_ticket> ans;

		sjtu::bptree<str<char, 20>, str<char, 20>>::iterator it = tra->llist.begin();
		for (; it != tra->llist.end(); ++it) {
			char ct[20];
			memcpy(ct, (*it).data, 20);
			sjtu::vector<info_ticket> bfr = query_ticket(_loc1, ct, date, catalog);
			sjtu::vector<info_ticket> aft = query_ticket(ct, _loc2, date, catalog);
			
			for (size_t i = 0; i < bfr.size(); ++i) {
				for (size_t j = 0; j < aft.size(); ++j) {
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

	bool buy_ticket(int id, short num, char train_id[20], char _loc1[],
		char _loc2[], my_date date, char ticket_kind[]) {
		//the time-comsuming constant of this function can be better

		if (id > use->cur || id < 2019) return false;
		if (!strcmp(_loc1, _loc2)) return false;
		info_train info = tra->data.at(train_id);
		if (info.on_sale == -1) return false;
		ct::vector<ticket_number_t, fname_ticket_number> vv;
		ticket_number_t quan = vv[info.on_sale];
		//NOTE HERE: there is a convertion of char[20] to str<char, 20> above, and I'm not sure it's correct.
		int i = 0, j = 0;
		int a, b, k;

		for (; i < info.num_price; ++i) {
			if (!strcmp(ticket_kind, info.name_price[i])) {
				k = i;
				break;
			}
		}
		if (i == info.num_price) return false;

		for (; j < info.num_station; ++j) {
			if (!strcmp(info.data->name, _loc1)) {
				a = j;
				break;
			}
		}
		for (; j < info.num_station; ++j) {
			if (!strcmp(info.data->name, _loc2)) {
				b = j;
				break;
			}
			if (quan.at(date, i, j) < num) return false;
		}
		if (j == info.num_station) return false;

		for (j = a; j < b; ++j) {
			quan.at(date, i, j) -= num;
		}
		vv.modify(info.on_sale, ticket_number_t(quan));

		//below is how I can stroage user's ticket_ordered data.

		info_ticket tic;
		memcpy(tic.train_id, train_id, 20);
		tic.date = date;
		tic.time_from = info.data[a].arriv;
		tic.time_to = info.data[b].start;
		//I'm not sure whether above two sentencces are what's suppposed to be.
		memcpy(tic.loc_from, info.data[a].name, 20 * sizeof(char));
		memcpy(tic.loc_to, info.data[b].name, 20 * sizeof(char));
		tic.num_price = info.num_price;
		tic.ticket_quantity[k] = num;
		for (i = 0; i < info.num_price; ++i) {
			memcpy(tic.ticket_kind[i], info.name_price[i], 20 * sizeof(char));
			float price = 0;
			for (j = a + 1; j <= b; ++j) {
				price += info.data[j].price[i];
			}
			tic.price[i] = price;
		}
		tic.setnormal();
		info_ticket_user ans(id, train_id, info.catalog, tic);
		data.push_back(ans);
	}

	query_order_return query_order(int id, my_date date, char catalog[]) {
		query_order_return ans;
		
		if (id > use->cur || id < 2019) return ans;
		for (int i = 0; i < data.size(); ++i) {
			if (data[i].id == id && data[i].date == date && !strcmp(data[i].catalog, catalog)) {
				bool jdg = true;
				for (int j = 0; j < ans.data.size(); ++j) {
					if (!strcmp(ans.data[j].train_id, data[i].train_id)) {
						int k = 0;
						for (; k < ans.data[j].num_price; ++k) {
							if (!strcmp(ans.data[j].ticket_kind[k], data[i].ticket_kind[k])) {
								break;
							}
							ans.data[j].ticket_quantity[k] += data[i].ticket_quantity[k];
						}
						if (k == ans.data[j].num_price) throw 1;
					}
				}
				if (jdg) {
					ans.data.push_back(data[i].ticket());
				}
			}
		}
		ans.success = true;
		return ans;
	}

	bool refund_ticket(int id, short num, char train_id[20],char loc_from[20],
		char loc_to[20], my_date date, char ticket_kind[20]) {
		if (id > use->cur || id < 2019) return false;
		info_train info = tra->data.at(train_id);
		if (info.on_sale == -1) return false;
		short cnt = num;
		for (int i = 0; i < data.size(); ++i){
			if (data[i].id == id && data[i].date == date && !strcmp(data[i].train_id, train_id)) {
				int k = 0;
				for (; k < data[i].num_price; ++k) {
					if (!strcmp(ticket_kind, data[i].ticket_kind[k])) {
						break;
					}
				}
				if (k == data[i].num_price) return false;
				cnt -= data[i].ticket_quantity[k];
			}
		}
		if (cnt > 0) return false;

		if (!strcmp(loc_from, loc_to)) return false;
		//NOTE HERE: there is a convertion of char[20] to str<char, 20> above, and I'm not sure it's correct.
		int i = 0, j = 0;
		int a, b, k;

		for (; i < info.num_price; ++i) {
			if (!strcmp(ticket_kind, info.name_price[i])) {
				k = i;
				break;
			}
		}
		if (i == info.num_price) return false;

		for (; j < info.num_station; ++j) {
			if (!strcmp(info.data->name, loc_from)) {
				a = j;
				break;
			}
		}
		for (; j < info.num_station; ++j) {
			if (!strcmp(info.data->name, loc_to)) {
				b = j;
				break;
			}
		}
		if (j == info.num_station) return false;

		ct::vector<ticket_number_t, fname_ticket_number> vv;
		ticket_number_t quan = vv[info.on_sale];
		for (j = a; j < b; ++j) {
			quan.at(date, i, j) += num;
		}
		vv.modify(info.on_sale, ticket_number_t(quan));

		cnt = num;
		for (i = 0; i < data.size(); ++i) {
			if (data[i].id == id && data[i].date == date && !strcmp(data[i].train_id, train_id)) {
				k = 0;
				for (; k < data[i].num_price; ++k) {
					if (!strcmp(ticket_kind, data[i].ticket_kind[k])) {
						break;
					}
				}
				if (cnt > data[i].ticket_quantity[k]) {
					cnt -= data[i].ticket_quantity[k];
					info_ticket_user tmp = data[i];
					tmp.ticket_quantity[k] = 0;
					data.modify(i, tmp);
				}
				else {
					info_ticket_user tmp = data[i];
					tmp.ticket_quantity[k] -= cnt;
					data.modify(i, tmp);
					return true;
				}
			}
		}
	}
	
	/*
	query_order_return query_order(int id, my_date date, char catalog[]) {
		if (id > use->cur || id < 2019) return query_order_return(false);
		ctn_ticket ans = use->data[id - 2019].user_ticket[date];
		info_ticket* ticket_return = new info_ticket [ans.size()];
		return query_order_return(true, ans.size(), ticket_return);
	}
	bool refund_ticket(int id, short num, my_date date, char loc_from[], char loc_to[], char catalog[]) {
		if (id > use->cur || id < 2019) return false;
		int cnt = 0;
		for (int i = use->data[id - 2019].user_ticket[date].size() - 1; i > 0; --i) {
			if (use->data[id - 2019].user_ticket[date][i].loc_from == loc_from &&
				use->data[id - 2019].user_ticket[date][i].loc_to == loc_to) {
				//I want to change ALL the char[] and char[] into str<> (There's a huge bug above)
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
		
	}*/
	void clean() {
		data.clean();
		sjtu::bptree<str<char, 31>, str<char, 31>>::iterator it = tra->lclist.begin();
		for (; it != tra->lclist.end(); ++it) {
			char ct[31];
			memcpy(ct, (*it).data, 31);
			char s1[36], s2[37];
			sprintf(s1, "data_%s", ct);
			sprintf(s2, "index_%s", ct);
			typedef pair<str<char, 20>, short> value_t;
			sjtu::bptree<str<char, 20>, value_t> tree(s1, s2);
			tree.clear();
		}
	}
};

#endif   //OPERATION