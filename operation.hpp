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

constexpr char train_info_name[] = "train_info_name";
class train {
	friend class ticket;
private:
	sjtu::bptree<str<char, 20>, int> data{ "trainData", "trainIndex" };  
	ct::vector<info_train, train_info_name> tr_info;
const info_train IDquery(str<char, 20> queryId) const
{
	int x = data.at(queryId);
	return tr_info[x];
}

public:
	sjtu::bptree<str<char, 20>, str<char, 20>> llist{"llistData", "llistIndex"};

	train() {}
	bool add(info_train& t) {
		if (data.count(t.train_id)) return false;
		data.insert(t.train_id, tr_info.size());
		tr_info.push_back(t);
		return true;
	}
	const info_train query_train(const char *train_id) const {
		str<char, 20> queryId(train_id);
		if (!data.count(queryId)) return info_train();
		info_train x = IDquery(queryId);
		return x;
	}
	bool delete_train(char train_id[]) {
		str<char, 20> queryId(train_id);
		if (!data.count(queryId)) return false;
		data.erase(queryId);
		return true;
	}
	bool modify_train(char train_id[], const info_train& t) {
		str<char, 20> queryId(train_id);
		if (!data.count(queryId)) return false;
		int pos = data.at(queryId);
		info_train x = tr_info[pos];
		if (x.on_sale >= 0) return false;
		tr_info.modify(pos, t);
		return true;
	}
	bool sale_train(char train_id[]) {
		str<char, 20> queryId(train_id);
		if (!data.count(queryId)) return false;
		int pos = data.at(queryId);
		info_train x = tr_info[pos];
		if (x.on_sale >= 0) return false;

		for (int i = 0; i < x.num_station; ++i)
			llist.insert(x.data[i].name, x.data[i].name);

		for (short i = 0; i < x.num_station; ++i) {

            typedef sjtu::pair<str<char, 20>, str<char, 20>> index_t;
            typedef short value_t;

			index_t ind(x.data[i].name, x.train_id);
			value_t val = i;
			char filename[25] = {0}, ffname[26] = {0};
			sprintf(filename, "data_loctid");
			sprintf(ffname, "index_loctid");

			sjtu::bptree<index_t, value_t>
				tree(filename, ffname);
			tree.insert(ind, val);
		}
		x.sell(); tr_info.modify(pos, x);
		return true;
	}
	void clean() {
		data.clear();
		llist.clear();
		tr_info.clean();
	}
};

typedef sjtu::vector<info_ticket> ctn_ticket;

constexpr char fnametic[] = "ticket_bought_data";
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
		sjtu::vector<info_ticket> ans;

		char filename1[25] = {0}, filename2[25] = {0};
		sprintf(filename1, "data_loctid");

		FILE *f = fopen(filename1, "rb+");
		if (!f) return ans;
		fclose(f);

		char ffname1[26] = {0}, ffname2[26] = {0};
		sprintf(ffname1, "index_loctid");

		typedef sjtu::pair<str<char, 20>, str<char, 20>> index_t;
        typedef short value_t;
        typedef sjtu::bptree<index_t, value_t>::iterator bpt_itr;

		sjtu::bptree<index_t, value_t> tree(filename1, ffname1);

        index_t indA(a, 0);
        index_t indB(b, 0);
        bpt_itr iA = tree.lowerbound(indA);
		bpt_itr iB = tree.lowerbound(indB);
		index_t dA = iA.get_key();
		index_t dB = iB.get_key();
		value_t cA = *iA;
		value_t cB = *iB;

		int cata_len = strlen(catalog);
		while (iA != tree.end() && iB != tree.end() && dA.first == a && dB.first == b) {
			if (dA.second == dB.second) {
				if (cA < cB) {
					info_train info = tra->IDquery(dA.second);
					bool find = 0;
					for (int i = 0; i < cata_len && !find; ++i)
						if (catalog[i] == info.catalog) find = 1;
					if (find)
					{
						ct::vector<ticket_number_t, fname_ticket_number> vv;
						ticket_number_t quan = vv[info.on_sale];
						info_ticket tic;
						memcpy(tic.train_id, dA.second.data, 20);
						tic.time_from = info.data[cA].start;
						tic.time_to = info.data[cB].arriv;
						tic.date = date.plus2(-(tic.time_from.hour() / 24));
						memcpy(tic.loc_from, info.data[cA].name, 20 * sizeof(char));
						memcpy(tic.loc_to, info.data[cB].name, 20 * sizeof(char));
						tic.num_price = info.num_price;
						for (int i = 0; i < info.num_price; ++i) {
							memcpy(tic.ticket_kind[i], info.name_price[i], 20 * sizeof(char));
							int cnt = 2000;
							double price = 0;
							for (int j = cA; j < cB; ++j) {
								if (cnt > quan.at(date, i, j)) cnt = quan.at(date, i, j);
								price += info.data[j + 1].price[i];
							}
							tic.ticket_quantity[i] = cnt;
							tic.price[i] = price;
						}
						bool jdg = false;
						for (int i = 0; i < tic.num_price; ++i) {
							if (tic.ticket_quantity[i] != 0)
							{
								jdg = true;
								break;
							}
						}
						tic.setnormal();
						if (jdg) ans.push_back(tic);
					}
				}
				++iA; ++iB;
				dA = iA.get_key(); dB = iB.get_key();
				cA = *iA; cB = *iB;

			}
			else if (dA.second < dB.second) { ++iA; dA = iA.get_key(); cA = *iA; }
			else { ++iB; dB = iB.get_key(); cB = *iB; }
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
						ans.second = aft[j];
					}
				}
			}
		}
		
		return ans;
	}

	bool buy_ticket(int id, short num, char train_id[20], char _loc1[],
		char _loc2[], my_date date, char ticket_kind[]) {

		if (id > use->cur || id < 2019) return false;
		if (!strcmp(_loc1, _loc2)) return false;
		info_train info = tra->IDquery(train_id);
		if (info.on_sale == -1) return false;
		ct::vector<ticket_number_t, fname_ticket_number> vv;
		ticket_number_t quan = vv[info.on_sale];
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
			if (!strcmp(info.data[j].name, _loc1)) {
				a = j;
				break;
			}
		}
		for (; j < info.num_station; ++j) {
			if (!strcmp(info.data[j].name, _loc2)) {
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

		info_ticket tic;
		memcpy(tic.train_id, train_id, 20);
		tic.time_from = info.data[a].start;
		tic.time_to = info.data[b].arriv;
		tic.date = date.plus2(-(tic.time_from.hour() / 24));
		memcpy(tic.loc_from, info.data[a].name, 20 * sizeof(char));
		memcpy(tic.loc_to, info.data[b].name, 20 * sizeof(char));
		tic.num_price = info.num_price;
		tic.ticket_quantity[k] = num;
		for (i = 0; i < info.num_price; ++i) {
			memcpy(tic.ticket_kind[i], info.name_price[i], 20 * sizeof(char));
			double price = 0;
			for (j = a + 1; j <= b; ++j) {
				price += info.data[j].price[i];
			}
			tic.price[i] = price;
		}
		tic.setnormal();
		info_ticket_user ans(id, train_id, info.catalog, tic);
		data.push_back(ans);
		return true;
	}

	query_order_return query_order(int id) {
		query_order_return ans;
		if (id > use->cur || id < 2019) return ans;
		for (int i = 0; i < data.size(); ++i) {
			if (data[i].id == id) {
				bool jdg = true;
				for (int j = 0; jdg && j < ans.data.size(); ++j) {
					if (!strcmp(ans.data[j].train_id, data[i].train_id) && ans.data[j].date == data[i].date) {
						for (int k = 0; k < ans.data[j].num_price; ++k)
							ans.data[j].ticket_quantity[k] += data[i].ticket_quantity[k];
						jdg = false;
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
        if (!strcmp(loc_from, loc_to)) return false;
        info_train info = tra->IDquery(train_id);
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
			if (!strcmp(info.data[j].name, loc_from)) {
				a = j;
				break;
			}
		}
		for (; j < info.num_station; ++j) {
			if (!strcmp(info.data[j].name, loc_to)) {
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
		for (i = 0; i < data.size(); ++i) {   //can be written better
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

	void clean() {
		data.clean();
		if (!tra -> llist.empty())
        {
            system("rm data_*");
            system("rm index_*");
        }
		ct::vector<ticket_number_t, fname_ticket_number> vv;
		vv.clean();
	}
};

#endif   //OPERATION