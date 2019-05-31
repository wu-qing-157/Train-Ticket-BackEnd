#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifndef CT
	#include "return.hpp"
	#include "operation.hpp"
	#include "operation_user.hpp"
	#include "info.hpp"
	#include "structs.hpp"
#else
	#include "operation_user.hpp"
#endif

using std::cin;
using std::cout;

typedef char name_t[40];
typedef char password_t[20];
typedef char email_t[20];
typedef char phone_t[20];
typedef char loc_t[20];
typedef char date_t[20];
typedef char catalog_t[10];
typedef char ticket_kind_t[20];
typedef char train_id_t[20];
typedef char my_timer_t[10];

#define cls(_arr) memset(_arr, 0, sizeof (_arr))

// ======= User BEGIN =======
user user_system;
void user_register()
{
	name_t _name;
	password_t pwd;
	email_t _email;
	phone_t _phone;
	cls(pwd);
	cin >> _name >> pwd >> _email >> _phone;
	cout << user_system.regist(_name, pwd, _email, _phone) << "\n";
}
void user_login()
{
	int id; password_t pwd; cls(pwd);
	cin >> id >> pwd;
	cout << user_system.login(id, pwd) << "\n";
}
void user_query_profile()
{
	int id; cin >> id;
	if (id < 2019 || id > user_system.counter()) {cout << "0\n"; return ;}
	info_query_profile ans = user_system.query_profile(id);
	cout << ans.name << ' ' << ans.email << ' ' << ans.phone << ' ' << ans.privilege + 1 << '\n';
}
void user_modify_profile()
{
	int id; cin >> id;
	name_t _name; password_t pwd; email_t _email; phone_t _phone;
	cin >> _name >> pwd >> _email >> _phone;
	cout << user_system.modify_profile(id, _name, pwd, _email, _phone) << '\n';
}
void user_modify_privilege()
{
	int id1, id2; short privilege;
	cin >> id1 >> id2 >> privilege; --privilege;
	cout << user_system.modify_privilege(id1, id2, privilege) << '\n';
}
// ======= User END =======

// ======= Ticket BEGIN =======
train train_system;
ticket ticket_system{&train_system, &user_system};
void ticket_query_ticket()
{
	loc_t loc1, loc2; date_t _date; catalog_t _catalog;
	cls(loc1); cls(loc2); cls(_date); cls(_catalog);
	cin >> loc1 >> loc2 >> _date >> _catalog;
	auto ret = ticket_system.query_ticket(loc1, loc2, my_date(_date), _catalog);
	cout << ret.size() << "\n";
	for (int i = 0; i < ret.size(); ++i) cout << ret[i];
}
void ticket_query_transfer()
{
	loc_t loc1, loc2; date_t _date; catalog_t _catalog;
	cls(loc1); cls(loc2); cls(_date); cls(_catalog);
	cin >> loc1 >> loc2 >> _date >> _catalog;
	auto ret = ticket_system.query_transfer(loc1, loc2, my_date(_date), _catalog);
	cout << ret.first << ret.second;
}
void ticket_buy_ticket()
{
	int uid, num; train_id_t t_id; loc_t loc1, loc2; date_t _date; ticket_kind_t _kind;
	cls(t_id); cls(loc1); cls(loc2); cls(_date); cls(_kind);
	cin >> uid >> num >> t_id >> loc1 >> loc2 >> _date >> _kind;
	cout << ticket_system.buy_ticket(uid, num, t_id, loc1, loc2, my_date(_date), _kind) << '\n';
}
void ticket_query_order()
{
	int uid; date_t _date; catalog_t _catalog;
	cls(_date); cls(_catalog);
	cin >> uid >> _date >> _catalog;
	auto ret = ticket_system.query_order(uid, my_date(_date), _catalog);
	if (ret.success)
	{
		cout << ret.data.size() << '\n';
		for (int i = 0; i < ret.data.size(); ++i)
			cout << ret.data[i];
	}
	else cout << "-1\n";
	// ...
}
void ticket_refund_ticket()
{
	int uid, num; train_id_t tid; loc_t loc1, loc2; date_t _date; ticket_kind_t _kind;
	cls(tid); cls(loc1); cls(loc2); cls(_date); cls(_kind);
	cin >> uid >> num >> tid >> loc1 >> loc2 >> _date >> _kind;
	cout << ticket_system.refund_ticket(uid, num, tid, loc1, loc2, my_date(_date), _kind) << '\n';
	// ...
}
// ======= Ticket END =======

// ======= Train BEGIN =======
void train_add_train()
{
	train_id_t tid; name_t _name; catalog_t _cata; int num_station, num_price;
	ticket_kind_t price_name[5];
	cls(tid); cls(_name); cls(_cata); cls(price_name);
	cin >> tid >> _name >> _cata >> num_station >> num_price;
	for (int i = 0; i < num_price; ++i) cin >> price_name[i];
	info_station st_info[60];
	for (int i = 0; i < num_station; ++i)
	{
		loc_t loc;
		my_timer_t arr, start, stopover; float price[5];
		memset(price, 0, sizeof (price));
		char price_s[20];
		cls(loc); cls(arr); cls(start); cls(stopover);
		cin >> loc >> arr >> start >> stopover;
		for (int j = 0; j < num_price; ++j)
		{
			cin >> price_s; sscanf(price_s + 3, "%f", price + j);
		}
		st_info[i] = info_station(loc, my_time(arr), my_time(start), my_time(stopover), price);
	}
	info_train now = info_train(_name, price_name, tid, _cata[0], num_station, num_price, st_info);
	cout << train_system.add(now) << '\n';
}
void train_sale_train()
{
	train_id_t tid; cls(tid); cin >> tid;
	cout << train_system.sale_train(tid) << '\n';
}
void train_query_train()
{
	train_id_t tid; cls(tid); cin >> tid;
	info_train ans = train_system.query_train(tid);
	if (ans.num_station == 0) {cout << "0\n"; return ;}
	cout << tid << ' ' << ans.name << ' ' << ans.catalog << ' ' << ans.num_station << ' ' << ans.num_price;
	for (int i = 0; i < ans.num_price; ++i) cout << ' ' << ans.name_price[i];
	cout << '\n';
	for (int i = 0; i < ans.num_station; ++i)
	{
		cout << ans.data[i].name << ' ' << ans.data[i].arriv << ' ' << ans.data[i].start << ' ' << ans.data[i].stopover;
		for (int j = 0; j < ans.num_price; ++j)
			cout << " ￥" << ans.data[i].price[j];
		cout << '\n';
	}
}
void train_delete_train()
{
	train_id_t tid; cls(tid); cin >> tid;
	cout << train_system.delete_train(tid) << '\n';
}
void train_modify_train()
{
    ticket_kind_t price_name[5];
    train_id_t tid; name_t _name; catalog_t _cata; int num_station, num_price;
	cls(tid); cls(_name); cls(_cata); cls(price_name);
	cin >> tid >> _name >> _cata >> num_station >> num_price;
	for (int i = 0; i < num_price; ++i) cin >> price_name[i];
	info_station st_info[60];
	for (int i = 0; i < num_station; ++i)
	{
		loc_t loc;
		my_timer_t arr, start, stopover; float price[5];
		char price_s[10];
		cls(loc); cls(arr); cls(start); cls(stopover);
		cin >> loc >> arr >> start >> stopover;
		for (int j = 0; j < num_price; ++j)
		{
			cin >> price_s; sscanf(price_s + 1, "%f", price + j);
		}
		st_info[i] = info_station(loc, my_time(arr), my_time(start), my_time(stopover), price);
	}
	info_train now = info_train(_name, price_name, tid, _cata[0], num_station, num_price, st_info);
	cout << train_system.modify_train(tid, now) << '\n';
	return ;
}
// ======= Train END =======

// ======= System BEGIN =======
bool is_it_clean;
void system_clean()
{
	user_system.clean();
	ticket_system.clean();
	train_system.clean();
	is_it_clean = 1;
	cout << "1\n";
}
void system_exit() {return ;}
// ======= System END =======
int main()
{
	user_system.init();
		// system_clean();
	while (1)
	{
		const int command_number = 17;
		const char *command_name[command_number] =
		{"register", "login", "query_profile", "modify_profile", "modify_privilege",
		"query_ticket", "query_transfer", "buy_ticket", "query_order", "refund_ticket",
		"add_train", "sale_train", "query_train", "delete_train", "modify_train",
		"clean", "exit"};
		void (*command[command_number])(void) =
		{
			user_register,
			user_login,
			user_query_profile,
			user_modify_profile,
			user_modify_privilege,
			ticket_query_ticket,
			ticket_query_transfer,
			ticket_buy_ticket,
			ticket_query_order,
			ticket_refund_ticket,
			train_add_train,
			train_sale_train,
			train_query_train,
			train_delete_train,
			train_modify_train,
			system_clean,
			system_exit
		};
		char str[30]; scanf("%s", str);
		for (int i = 0; i < command_number; ++i)
			if (!strcmp(str, command_name[i]))
				if (i == command_number - 1) {cout << "BYE\n"; return 0;}
				else command[i]();
	}
	return 0;
}