#ifndef OPERATION_USER
#define OPERATION_USER

#include <cstring>
#include "return.hpp"
//#include "structs.hpp"
#include "info.hpp"
#include "ct_exceptions.hpp"
#include "ct_vector.hpp"

const char filename[] = "user_data";
class user {
	friend class ticket;
private:
	ct::vector<info_user, filename> data;
	int cur;

public:
	int counter() const { return cur; }
	void init()
	{
		data.init();
		cur = 2018 + data.size();
	}
	void clean()
	{
		data.clean();
		cur = 2018;
	}

	int regist(char name[], char passward[], char email[], char phone[]) {
		//printf("%s\n", passward );
		info_user tmp(name, passward, email, phone, ++cur, 0);
		if (cur == 2019) tmp.privilege = 1;
		data.push_back(tmp);
		return cur;  // I don't quite know when I should return false.
		// You should return an int instead bool... --ct
	}
	bool login(int id, char passward[]) const {
		if (id <= 2018 || id > cur) return false;
		//printf("%s %s %d\n", passward, data[id - 2019].passward, strcmp(data[id - 2019].passward, passward));
		if (strcmp(data[id - 2019].passward, passward) == 0) return true;
		return false;
	}
	info_query_profile query_profile(int id) const {
		return info_query_profile(data[id - 2019]);
	}
	bool modify_profile(int id, char name[], char passward[], char email[], char phone[]) {
		if (id > cur || id < 2019) return false;
		else {
			info_user tmp(name, passward, email, phone, id, data[id - 2019].privilege);
			data.modify(id - 2019, tmp);
			return true;
		}
	}
	bool modify_privilege(int id1, int id2, short privilege) {
		if (id1 > cur || id2 > cur || id1 < 2019 || id2 < 2019) return false;
		if (privilege < 0 || privilege > 2) return false;
		info_user u1 = data[id1 - 2019], u2 = data[id2 - 2019];
		if (!u1.privilege) return false;
		if (u2.privilege && !privilege) return false;
		//if (data[id1 - 2019].privilege < privilege) return false;
		u2.privilege = privilege;
		data.modify(id2 - 2019, u2);
		return true;
	}
};

#endif