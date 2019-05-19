#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifndef CT
	#include "return.hpp"
	#include "operation.hpp"
	#include "info.hpp"
	#include "structs.hpp"
#endif

typedef wchar_t name[40];
typedef char password[32];
typedef char email[20];
typedef char phone[20];
typedef wchar_t log[20];
typedef char date[20];
typedef char catalog[10];
typedef wchar_t ticket_kind[20];
typedef char train_id[20];
typedef char time[10];

// ======= User BEGIN =======
user user_system;
void user_register()
{
	name _name;
	password pwd;
	email _email;
	phone _phone;
	scanf("%s%s%s%lld", _name, pwd, _email, _phone);
	printf("%d\n", user_system.regist(_name, pwd, _email, _phone));
}
void user_login()
{
	int id;
	password pwd;
	scanf("%d%s", &id, pwd);
	printf("%d\n", user_system.login(id, pwd));
}
void user_query_profile()
{
	int id; scanf("%d", &id);
	if (id < 2019 || id > user_system.counter()) {puts("0"); return ;}
	info_query_profile ans = user_system.query_profile(id);
	printf("%s %s %s %hd\n", ans.name, ans.email, ans.phone, ans.privilege);
}
void user_modify_profile()
{
	int id; scanf("%d", &id);
	name _name; password pwd; email _email; phone _phone;
	scanf("%s%s%s%s", _name, pwd, _email, _phone);
	printf("%d\n", user_system.modify_profile(id, _name, pwd, _email, _phone));
}
void user_modify_privilege()
{
	int id1, id2; short privilege;
	printf("%d\n", user_system.modify_privilege(id1, id2, privilege));
}
// ======= User END =======

// ======= Ticket BEGIN =======
ticket ticket_system;
void ticket_query_ticket()
{
	loc loc1, loc2;
	date _date; catalog _catalog;
	scanf("%s%s%s%s", loc1, loc2, _data, _catalog);
	// ...
}
void ticket_query_transfer()
{
	loc loc1, loc2;
	date _date;
	catalog _catalog;
	scanf("%s%s%s%s", loc1, loc2, _data, _catalog);
	// ...
}
void ticket_buy_ticket()
{
	int uid, num;
	train_id t_id;
	loc loc1, loc2;
	date _date;
	ticket_kind _kind;
	scanf("%d%d%s%s%s%s%s", &uid, &num, tid, loc1, loc2, _date, _kind);
	// ...
}
void ticket_query_order()
{
	int uid; date _date; catalog _catalog;
	scanf("%d%s%s", &uid, _data, _catalog);
	// ...
}
void ticket_refund_ticket()
{
	int uid, num; train_id tid; loc loc1, loc2; date _date; ticket_kind _kind;
	scanf("%d%d%s%s%s%s%s", &uid, &num, tid, loc1, loc2, _date, _kind);
	// ...
}
// ======= Ticket END =======

// ======= Train BEGIN =======
train train_system;
void train_add_train()
{
	train_id tid; name _name; catalog _cata; int num_station, num_price;
	for (int i = 0; i < num_price; ++i);
	// ...
}
void train_sale_train()
{
	train_id tid; scanf("%s", tid);
	// ...
}
void train_query_train()
{
	try {
		char train_id[20];
		info_train ans = train_system.query_train(train_id);
		printf("%s %s %s %hd %hd", train_id, ans._name, ans.catalog, ans.num_station, ans.num_price);
		for (int i = 0; i < ans.num_price; ++i) printf(" %s", ans.name_price[i]);
		puts("");
		for (int i = 0; i < ans.num_station; ++i)
		{
			printf("%s ", ans.data[i]._name);
			ans.data[i].arriv.print(" ");
			ans.data[i].start.print(" ");
			ans.data[i].stopover.print();
			for (int j = 0; j < ans.num_price; ++j)
				printf(" ￥%f", ans.data[i].price[j]);
			puts("");
		}
	} catch(index_out_of_bound) {puts("0");}
}
void train_delete_train()
{
	train_id tid; scanf("%s", tid);
	printf("%d\n", train_system.erase(tid));
}
void train_modify_train()
{
	// ...
}
// ======= Train END =======

// ======= System BEGIN =======
void system_clean();
void system_exit() {return ;}
// ======= System END =======
int main()
{
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
				if (i == command_number - 1) return 0;
				else command[i]();
	}
	return 0;
}