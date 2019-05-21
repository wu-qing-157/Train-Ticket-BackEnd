#ifndef INFO
#define INFO
#include <cstring>
#include <iostream>
#include "structs.hpp"

struct info_user {
	wchar_t name[40];
	char email[20], passward[32], phone[20];
	long long id;
	short privilege;

	info_user() = default;
	info_user(const info_user& other) = default;
	info_user(wchar_t _name[], char _passward[], char _email[],
		char _phone[20], long long _id, short _privilege) {
		memcpy(name, _name, 40 * sizeof (wchar_t)); // Diffenrece between Win and Linux. //Just keep the parenthesis
		memcpy(passward, _passward, 32);
		memcpy(email, _email, 20);
		memcpy(phone, _phone, 20);
		id = _id;
		privilege = _privilege;
	}
};

struct info_station {
	wchar_t name[40];
	my_time arriv, start, stopover;
	float price[5];
};

struct info_train {
	str<char, 20> train_id;
	short num_station, num_price;
	wchar_t name[40], name_price[5][20];
	char catalog[10];
	info_station* data;
	short num_ticket[40][60];  //40 means days, 60 means station number
	bool on_sale;

	info_train() = default;
	info_train(wchar_t _name[], wchar_t _name_price[][20],
		char _train_id[], char _catalog[], short _num_station,
		short _num_price, info_station* _data) {
		train_id.cpy(_train_id);  //NOTE HERE: I'm using char[] to construct train_id, but it's stored as str.
		num_price = _num_price;
		num_station = _num_station;
		memcpy(name, _name, 40 * sizeof (wchar_t));
		//memcpy(name_price, _name_price, num_price * 4); It seems that this sentence should be deleted, but I'm not sure.
		for (int i = 0; i < num_price; ++i) {
			memcpy(name_price[i], _name_price[i], 20 * sizeof (wchar_t));
		}
		memcpy(catalog, _catalog, 10);
		data = new info_station[num_station];
		memcpy(data, _data, num_station * sizeof (info_station));
		on_sale = false;
	}
	info_train(const info_train& other) = delete;
};

struct info_ticket {
	date date_from;
	my_time time_from, time_to;
	wchar_t loc_from[20], loc_to[20], ticket_kind[5][20];
	short num_price, ticket_quantity[5];
	float price[5];

	info_ticket() = default;
	info_ticket(date _date_from, my_time _time_from, my_time _time_to,
		wchar_t _loc_from[], wchar_t _loc_to[], wchar_t _ticket_kind[][20],
		short _num_price, short _ticket_quantity[], float _price[5]) {
		date_from = _date_from;
		time_from = _time_from;
		time_to = _time_to;
		memcpy(loc_from, _loc_from, 20 * sizeof(wchar_t));
		memcpy(loc_to, _loc_to, 20 * sizeof(wchar_t));
		for (int i = 0; i < num_price; ++i) {
			memcpy(ticket_kind[i], _ticket_kind[i], 20 * sizeof(wchar_t));
		}
		num_price = _num_price;
		memcpy(ticket_quantity, _ticket_quantity, sizeof(short));
		memcpy(price, _price, 5 * sizeof(short));
	}
};

#endif  //INFO