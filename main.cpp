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

// ======= User BEGIN =======
user user_system;
void user_register()
{
	wchar_t name[40];
	char pwd[32], email[20], phone[20];
	scanf("%s%s%s%lld", name, pwd, email, phone);
	printf("%d\n", user_system.regist(name, pwd, email, phone));
}
void user_login()
{
	int id; char pwd[32];
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
	wchar_t name[40]; char pwd[32], email[20], phone[20];
	scanf("%s%s%s%s", name, pwd, email, phone);
	printf("%d\n", user_system.modify_profile(id, name, pwd, email, phone));
}
void user_modify_privilege()
{
	int id1, id2; short privilege;
	user_system.modify_privilege(id1, id2, privilege);
}
// ======= User END =======

// ======= Ticket BEGIN =======
void ticket_query_ticket();
void ticket_query_transfer();
void ticket_buy_ticket();
void ticket_query_order();
void ticket_refund_ticket();
// ======= Ticket END =======

// ======= Train BEGIN =======
void train_add_train();
void train_sale_train();
void train_query_train();
void train_delete_train();
void train_modify_train();
// ======= Train END =======

// ======= System BEGIN =======
void system_clean();
void system_exit();
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
				command[i]();
	}
	return 0;
}