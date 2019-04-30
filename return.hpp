#incldue <iostream>
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

These above are written in flow.hpp
*/

struct query_profile_return{
    wchar_t name[20];
    char email[20], phone[20];
    short privilege;
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
    info_ticket data[]; //it hasn't been finished
};

struct query_transfer_return{
    bool sucess;
    info_ticket data[2];
};

struct query_order_return{
    bool success;
    info_ticket data[]; //it hasn't been finished
};

struct info_station{
    wchar_t name[20];
    time arriv, start, stopover;
    float price[5];
};

struct query_train_return{
    wchar_t name[20], name_price[5][20];
    char train_id[20], catalog[10];
    short num_station, num_price;
    info_station data[]; //it hasn't been finished
};
