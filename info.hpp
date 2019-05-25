#ifndef INFO
#define INFO
#include <cstring>
#include <iostream>
#include "structs.hpp"

struct info_user {
	char name[40];
	char email[20], passward[20], phone[20];
	long long id;
	short privilege;

	info_user() = default;
	info_user(const info_user& other) = default;
	info_user(char _name[], char _passward[], char _email[],
		char _phone[20], long long _id, short _privilege) {
		memcpy(name, _name, 40 * sizeof (char)); // Diffenrece between Win and Linux. //Just keep the parenthesis
		memcpy(passward, _passward, 20);
		memcpy(email, _email, 20);
		memcpy(phone, _phone, 20);
		id = _id;
		privilege = _privilege;
	}
};

#ifndef CT

struct info_station {
	char name[20];
	my_time arriv, start, stopover;
	float price[5];
};

struct info_train {
	str<char, 20> train_id;
	short num_station, num_price;
	char name[20], name_price[5][20];
	char catalog[10];
	info_station data[60];
	short quan_ticket[5][60][30];  //5 means type of tickets, 60 means station number, 30 means days
	bool on_sale;

	info_train() {
		num_station = -1;	//for convenience of judging wrong visit
	}
	info_train(const info_train& other) {
		train_id = other.train_id;
		num_station = other.num_station;
		num_price = other.num_price;
		memcpy(name, other.name, 20 * sizeof(char));
		memcpy(catalog, other.catalog, 10 * sizeof(char));
		memcpy(data, other.data, num_station * sizeof(info_station));
		for (int i = 0; i < num_price; ++i) {
			memcpy(name_price[i], other.name_price[i], 20 * sizeof(char));
		}
		for (int i = 0; i < 5; ++i) {
			for (int j = 0; j < 60; ++j) {
				memcpy(quan_ticket[i][j], other.quan_ticket[i][j], 30 * sizeof(short));
			}
		}
		on_sale = other.on_sale;
	}
	info_train(char _name[20], char _name_price[5][20],
		char _train_id[20], char _catalog[10], short _num_station,
		short _num_price, short _quan_ticket[5][60][30], info_station _data[60]) {
		train_id.cpy(_train_id);  //NOTE HERE: I'm using char[] to construct train_id, but it's stored as str.
		num_price = _num_price;
		num_station = _num_station;
		memcpy(name, _name, 20 * sizeof (char));
		//memcpy(name_price, _name_price, num_price * 4); It seems that this sentence should be deleted, but I'm not sure.
		for (int i = 0; i < num_price; ++i) {
			memcpy(name_price[i], _name_price[i], 20 * sizeof (char));
		}
		memcpy(catalog, _catalog, 10);
		memcpy(data, _data, num_station * sizeof (info_station));
		for (int i = 0; i < 5; ++i) {
			for (int j = 0; j < 60; ++j) {
				memcpy(quan_ticket[i][j], _quan_ticket[i][j], 30 * sizeof(short));
			}
		}
		on_sale = false;
	}
	bool sell() {
		if (on_sale) return false;
		on_sale = true;
		for (int i = 0; i < num_price; ++i) {
			for (int j = 0; i < num_station; ++j) {
				for (int k = 0; k < 30; ++k) {
					quan_ticket[i][j][k] = 2000;
				}
			}
		}
		return true;
	}

	//info_train(const info_train& other) = delete;
};

struct info_ticket {
	my_date date;
	my_time time_from, time_to;
	char loc_from[20], loc_to[20], ticket_kind[5][20];
	short num_price, ticket_quantity[5];
	float price[5];

	info_ticket() {
		num_price = -1;  //for convenience of judging wrong info_ticket
	}
	info_ticket(const info_ticket& other) = default;
	info_ticket(my_date _date, my_time _time_from, my_time _time_to,
		char _loc_from[], char _loc_to[], char _ticket_kind[][20],
		short _num_price, short _ticket_quantity[], float _price[5]) {
		date = _date;
		time_from = _time_from;
		time_to = _time_to;
		memcpy(loc_from, _loc_from, 20 * sizeof(char));
		memcpy(loc_to, _loc_to, 20 * sizeof(char));
		for (int i = 0; i < num_price; ++i) {
			memcpy(ticket_kind[i], _ticket_kind[i], 20 * sizeof(char));
		}
		num_price = _num_price;
		memcpy(ticket_quantity, _ticket_quantity, sizeof(short));
		memcpy(price, _price, 5 * sizeof(short));
	}
	info_ticket operator= (const info_ticket& other) {
		date = other.date;
		time_from = other.time_from;
		time_to = other.time_to;
		memcpy(loc_from, other.loc_from, 20 * sizeof(wchar_t));
		memcpy(loc_to, other.loc_to, 20 * sizeof(wchar_t));
		for (int i = 0; i < num_price; ++i) {
			memcpy(ticket_kind[i], other.ticket_kind[i], 20 * sizeof(wchar_t));
		}
		num_price = other.num_price;
		memcpy(ticket_quantity, other.ticket_quantity, sizeof(short));
		memcpy(price, other.price, 5 * sizeof(short));
		return *this;
	}
	
};

#endif // CT

#endif  //INFO