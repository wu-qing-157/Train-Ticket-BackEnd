#ifndef RETURN
#define RETURN

#include <iostream>
#include <cstring>
#include "structs.hpp"
#include "info.hpp"

struct info_query_profile {  //It's very similar with info_user, except that it does not contain passward.
    wchar_t name[40];
    char email[20], phone[20];
    short privilege;

	info_query_profile(info_user data) {
		memcpy(name, data.name, 40 * sizeof (wchar_t));
		memcpy(email, data.email, 20);
		memcpy(phone, data.phone, 20);
		privilege = data.privilege;
	}
	info_query_profile (wchar_t _name[], char _email[], char _phone[20],
                         short _privilege){
        memcpy(name, _name, 40 * sizeof (wchar_t));
        memcpy(email, _email, 20);
		memcpy(phone, _phone, 20);
        privilege = _privilege;
    }
};

#ifndef CT

struct query_ticket_return{
    int listnum;
    info_ticket* data;

    query_ticket_return(int _listnum, info_ticket* _data){
        listnum = _listnum;
        data = new info_ticket[listnum];
        memcpy(data, _data, listnum * sizeof(info_ticket));
    }

    ~query_ticket_return(){
        delete [] data;
    }
};

struct query_transfer_return{
    bool success;
    info_ticket data[2];

    query_transfer_return(bool _success, info_ticket* _data){
        success = _success;
        memcpy(data, _data, 2 * sizeof(info_ticket));
    }
};

struct query_order_return{
    bool success;
    short listnum;
    info_ticket* data;

    query_order_return(bool _success, short _listnum = 0, info_ticket* _data = nullptr){
        success = _success;
        listnum = _listnum;
        data = new info_ticket[listnum];
        memcpy(data, _data, listnum * sizeof(info_ticket));
    }

    ~query_order_return(){
        delete [] data;
    }
};

/*struct info_station{
    wchar_t name[20];
    time arriv, start, stopover;
    float price[5];
};*/

struct query_train_return{
    short num_station, num_price;
    wchar_t name[40], name_price[5][20];
    char train_id[20], catalog[10];
    info_station* data;

    query_train_return(wchar_t _name[], wchar_t _name_price[][20],
                       char _train_id[], char _catalog[], short _num_station,
                       short _num_price, info_station* _data){
        num_price = _num_price;
        num_station = _num_station;
        memcpy(name, _name, 40 * sizeof (wchar_t));
		//memcpy(name_price, _name_price, num_price * 4);  It seems that this sentence should be deleted, but I'm not sure.
		for (int i = 0; i < num_price; ++i) {
            memcpy(name_price[i], _name_price[i], 20 * sizeof (wchar_t));
        }
        memcpy(train_id, _train_id, 20);
        memcpy(catalog, _catalog, 10);
        data = new info_station[num_station];
        memcpy(data, _data, num_station * sizeof (info_station));
    }

    ~query_train_return(){
        delete [] data;
    }
};

#endif // CT

#endif // RETURN
