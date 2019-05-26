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

struct info_query_profile {  //It's very similar with info_user, except that it does not contain passward.
	char name[40];
	char email[20], phone[20];
	short privilege;

	info_query_profile(info_user data) {
		memcpy(name, data.name, 40 * sizeof(char));
		memcpy(email, data.email, 20);
		memcpy(phone, data.phone, 20);
		privilege = data.privilege;
	}
	info_query_profile(char _name[], char _email[], char _phone[20],
		short _privilege) {
		memcpy(name, _name, 40 * sizeof(char));
		memcpy(email, _email, 20);
		memcpy(phone, _phone, 20);
		privilege = _privilege;
	}
};

#ifndef CT

struct info_station {
	char name[20];
	my_time arriv, start, stopover;
	float price[5];

	info_station() = default;
	info_station(char _name[20], my_time _arriv, my_time _start, my_time _stopover, float _price[5]) :
		name(_name), arriv(_arriv), start(_start), stopover(_stopover), price(_price) {}

	void checkday() {
		if (arriv > start) start.modify_hour(start.hour() + 24);
	}
	void dayplus() {
		arriv.modify_hour(arriv.hour() + 24);
		start.modify_hour(start.hour() + 24);
	}
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
		short _num_price, info_station _data[60]) {
		train_id.cpy(_train_id);  //NOTE HERE: I'm using char[] to construct train_id, but it's stored as str.
		num_price = _num_price;
		num_station = _num_station;
		memcpy(name, _name, 20 * sizeof (char));
		for (int i = 0; i < num_price; ++i) {
			memcpy(name_price[i], _name_price[i], 20 * sizeof (char));
		}
		memcpy(catalog, _catalog, 10);
		memcpy(data, _data, num_station * sizeof (info_station));
		settime();   //NOTE HERE: I have changed trains that cross the days with new time(eg. 25:12).
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
	void settime() {
		data[0].checkday();
		for (int i = 1; i < num_station; ++i) {
			data[i].checkday();
			if (data[i].arriv < data[i - 1].start) data[i].dayplus();
		}
	}
};

struct info_ticket {
	char train_id[20];
	my_date date;
	my_date date_from, date_to;
	my_time time_from, time_to;
	char loc_from[20], loc_to[20], ticket_kind[5][20];
	short num_price, ticket_quantity[5];
	float price[5];

	info_ticket() {
		num_price = -1;  //for convenience of judging wrong info_ticket
	}
	info_ticket(const info_ticket& other) = default;
	info_ticket(char _train_id[], my_date _date, my_date _date_from, 
		my_date _date_to, my_time _time_from, my_time _time_to,
		char _loc_from[], char _loc_to[], char _ticket_kind[][20],
		short _num_price, short _ticket_quantity[], float _price[5]) {
		memcpy(train_id, _train_id, 20);
		date = _date;
		date_from = _date_from;
		date_to = _date_to;
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
		memcpy(train_id, other.train_id, 20);
		date = other.date;
		date_from = other.date_from;
		date_to = other.date_to;
		time_from = other.time_from;
		time_to = other.time_to;
		memcpy(loc_from, other.loc_from, 20 * sizeof(char));
		memcpy(loc_to, other.loc_to, 20 * sizeof(char));
		for (int i = 0; i < num_price; ++i) {
			memcpy(ticket_kind[i], other.ticket_kind[i], 20 * sizeof(char));
		}
		num_price = other.num_price;
		memcpy(ticket_quantity, other.ticket_quantity, sizeof(short));
		memcpy(price, other.price, 5 * sizeof(short));
		return *this;
	}
	void setnormal() {
		date_from = date.plus(time_from.day_added());
		date_to = date.plus(time_to.day_added());
		time_from.normalize();
		time_to.normalize();
	}
	friend ostream& operator << (ostream &os, const info_ticket &obj);
};
ostream &operator << (ostream &os, const info_ticket &obj)
{
	os << obj.train_id << ' ' << obj.loc_from << ' ' << obj.date_from << ' ' << obj.time_from << ' '
									<< obj.loc_to << ' ' << obj.date_to << ' ' << obj.time_to;
		for (int j = 0; j < obj.num_price; ++j)
			os << ' ' << obj.ticket_kind[j] << ' ' << obj.ticket_quantity[j] << ' ' << obj.price[j];
		os << '\n';
	return os;
}

struct info_ticket_user{
	int id;
	char train_id[20];
	char catalog[10];
	my_date date;
	my_date date_from, date_to;
	my_time time_from, time_to;
	char loc_from[20], loc_to[20], ticket_kind[5][20];
	short num_price, ticket_quantity[5];
	float price[5];

	info_ticket_user() = default;
	info_ticket_user(const info_ticket_user& other) {
		id = other.id;
		memcpy(train_id, other.train_id, 20);
		memcpy(catalog, other.catalog, 10);
		date = other.date;
		date_from = other.date_from;
		date_to = other.date_to;
		time_from = other.time_from;
		time_to = other.time_to;
		memcpy(loc_from, other.loc_from, 20 * sizeof(char));
		memcpy(loc_to, other.loc_to, 20 * sizeof(char));
		for (int i = 0; i < num_price; ++i) {
			memcpy(ticket_kind[i], other.ticket_kind[i], 20 * sizeof(char));
		}
		num_price = other.num_price;
		memcpy(ticket_quantity, other.ticket_quantity, sizeof(short));
		memcpy(price, other.price, 5 * sizeof(short));
	}
	info_ticket_user(int _id, char _train_id[20], char _catalog[10], const info_ticket other) {
		id = _id;
		memcpy(train_id, _train_id, 20);
		memcpy(catalog, _catalog, 10);
		date = other.date;
		date_from = other.date_from;
		date_to = other.date_to;
		time_from = other.time_from;
		time_to = other.time_to;
		memcpy(loc_from, other.loc_from, 20 * sizeof(char));
		memcpy(loc_to, other.loc_to, 20 * sizeof(char));
		for (int i = 0; i < num_price; ++i) {
			memcpy(ticket_kind[i], other.ticket_kind[i], 20 * sizeof(char));
		}
		num_price = other.num_price;
		memcpy(ticket_quantity, other.ticket_quantity, sizeof(short));
		memcpy(price, other.price, 5 * sizeof(short));
	}
	info_ticket_user operator= (const info_ticket_user& other) {
		id = other.id;
		memcpy(train_id, other.train_id, 20);
		memcpy(catalog, other.catalog, 10);
		date = other.date;
		time_from = other.time_from;
		time_to = other.time_to;
		memcpy(loc_from, other.loc_from, 20 * sizeof(char));
		memcpy(loc_to, other.loc_to, 20 * sizeof(char));
		for (int i = 0; i < num_price; ++i) {
			memcpy(ticket_kind[i], other.ticket_kind[i], 20 * sizeof(char));
		}
		num_price = other.num_price;
		memcpy(ticket_quantity, other.ticket_quantity, sizeof(short));
		memcpy(price, other.price, 5 * sizeof(short));
		return *this;
	}

	info_ticket ticket() const {
		info_ticket ans;
		ans.date = date;
		ans.time_from = time_from;
		ans.time_to = time_to;
		memcpy(ans.loc_from, loc_from, 20 * sizeof(char));
		memcpy(ans.loc_to, loc_to, 20 * sizeof(char));
		for (int i = 0; i < num_price; ++i) {
			memcpy(ans.ticket_kind[i], ticket_kind[i], 20 * sizeof(char));
		}
		ans.num_price = num_price;
		memcpy(ans.ticket_quantity, ticket_quantity, sizeof(short));
		memcpy(ans.price, price, 5 * sizeof(short));
		return ans;
	}
};

#endif // CT

#endif  //INFO