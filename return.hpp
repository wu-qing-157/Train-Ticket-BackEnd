#ifndef RETURN
#define RETURN

#include <iostream>
#include <cstring>
#include "structs.hpp"
#include "info.hpp"
#include "./vector/vector.hpp"

#ifndef CT

struct query_order_return{
    bool success;
	sjtu::vector<info_ticket> data;

	query_order_return() {
		success = false;
	}
	query_order_return(bool _success, sjtu::vector<info_ticket> _data){
        success = _success;
		data = _data;
    }
};

/*struct info_station{
    char name[20];
    time arriv, start, stopover;
    double price[5];
};*/

#endif // CT

#endif // RETURN
