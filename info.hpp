#ifndef INFO
#define INFO
#include <cstring>
#include <iostream>
#include "structs.hpp"

struct info_user {
	wchar_t name[40];
	char email[20], passward[20];
	long long id, phone;
	short previlege;

	info_user() = default;
	info_user(const info_user& other) = default;
	info_user(wchar_t _name[], char _passward[], char _email[],
		long long _phone, long long _id, short _previlege) {
		memcpy(name, _name, 80);
		memcpy(passward, _passward, 20);
		memcpy(email, _email, 20);
		phone = _phone;
		id = _id;
		previlege = _previlege;
	}
};

struct info_station {
	wchar_t name[20];
	time arriv, start, stopover;
	float price[5];
};

struct info_train {
	str<char, 20> train_id;
	short num_station, num_price;
	wchar_t name[20], name_price[5][20];
	char catalog[10];
	info_station* data;
	bool on_sale;

	info_train() = default;
	info_train(wchar_t _name[], wchar_t _name_price[][20],
		char _train_id[], char _catalog[], short _num_station,
		short _num_price, info_station* _data) {
		train_id.cpy(_train_id);  //NOTE HERE: I'm using char[] to construct train_id, but it's stored as str.
		num_price = _num_price;
		num_station = _num_station;
		memcpy(name, _name, 20 * sizeof(wchar_t));
		//memcpy(name_price, _name_price, num_price * 4); It seems that this sentence should be deleted, but I'm not sure.
		for (int i = 0; i < num_price; ++i) {
			memcpy(name_price[i], _name_price[i], 20 * sizeof(wchar_t));
		}
		memcpy(catalog, _catalog, 10);
		data = new info_station[num_station];
		memcpy(data, _data, num_station * sizeof(info_station));
		on_sale = false;
	}
	info_train(const info_train& other) = delete;
};

#endif  //INFO