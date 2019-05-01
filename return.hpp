#ifndef RETURN
#define RETURN


#incldue <iostream>
#include <cstring>
#include "bptree.hpp"
#include "flow.hpp"

/*
struct time{
    short data;

    time():data(0){}
    time(char[] s){
        if (char[0] == 'x') data = -1;
        else data = (((s[0]-'0')*10 + s[1]-'0')*10 + s[3]-'0')*10 + s[4]-'0';
    }

    short hour(){return (data / 100);}
    short minute(){return (data % 100);}
};

struct date{
    short year;
    short month;
    short day;

    date() = default;
    date(char[] s){
        year = (((s[0]-'0')*10 + s[1]-'0')*10 + s[3]-'0')*10 + s[4]-'0';
        month = (s[5]-'0')*10 + s[6]-'0';
        day = (s[8]-'0')*10 + s[9]-'0';
    }
};

These above are written in flow.hpp, which I haven't pushed
*/

struct query_profile_return {
    wchar_t name[20];
    char email[20], phone[20];
    short privilege;

    query_profile_return (wchar_t _name[], char _email[], char _phone[],
                         short _privilege){
        memcpy(name, _name, 20 * sizeof(wchar_t));
        memcpy(email, _email, 20);
        memcpy(phone, _phone, 20);
        privilege = _privilege;
    }
};

struct info_ticket{
    date date_from, date_to;
    time time_from, time_to;
    wchar_t loc_from[20], loc_to[20], ticket_kind[5][20];
    short num_price, ticket_quantity[5];
    float price[5];
};

struct query_ticket_return{
    int listnum;
    info_ticket* data;

    query_ticket_return(int _listnum, const info_ticket& _data[]){
        listnum = _listnum;
        data = new info_ticket[listnum];
        memcpy(data, _data, listnum * sizeof(info_ticket));
    }

    ~query_order_return(){
        delete [] data;
    }
};

struct query_transfer_return{
    bool success;
    info_ticket data[2];

    query_transfer_return(bool _success, info_ticket _data[]){
        success = _success;
        memcpy(data, _data, 2 * sizeof(info_ticket));
    }
};

struct query_order_return{
    bool success;
    short listnum;
    info_ticket* data;

    query_order_return(bool _success, short listnum, info_ticket _data[]){
        success = _success;
        listnum = _listnum;
        data = new info_ticket[listnum];
        memcpy(data, _data, listnum * sizeof(info_ticket));
    }

    ~query_order_return(){
        delete [] data;
    }
};

struct info_station{
    wchar_t name[20];
    time arriv, start, stopover;
    float price[5];
};

struct query_train_return{
    short num_station, num_price;
    wchar_t name[20], name_price[5][20];
    char train_id[20], catalog[10];
    info_station* data;

    query_train_return(wchar_t _name[], wchar_t _name_price[][20],
                       char _train_id[], char _catalog[], short _num_station,
                       short _num_price, info_station _data[]){
        num_price = _num_price;
        num_station = _numstation;
        memcpy(name, _name, 20 * sizeof(wchar_t));
        for (int i = 0; i < num_price; ++i){
            memcpy(name_price[i], _name_price[i], 20 * sizeof(wchar_t));
        }
        memcpy(train_id, _train_id, 20);
        memcpy(catalog, _catalog, 10);
        data = new info_station[listnum];
        memcpy(data, _data, listnum * sizeof(info_station));
    }

    ~query_train_return(){
        delete [] data;
    }
};

#endif // RETURN
