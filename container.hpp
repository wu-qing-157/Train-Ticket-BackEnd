#ifndef CONTAINER
#define CONTAINER
#include "./vector/vector.hpp"

struct trainWithStation {
	str<char, 20> train_id;
	short num;

	trainWithStation() = default;
};

typedef sjtu::vector<trainWithStation> ctn_train;

typedef sjtu::vector<query_train_return> ctn_query_ticket;


#endif //CONTAINER