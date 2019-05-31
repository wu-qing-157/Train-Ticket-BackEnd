#ifndef CT_VECTOR_HPP
#define CT_VECTOR_HPP

#include "ct_exceptions.hpp"

#include <climits>
#include <cstddef>
#include "debuger_counter.hpp"

namespace ct {
template<typename T, const char *filename>
class vector {
private:
	FILE *f;
	size_t cur_cnt{0};
public:
	vector()
	{
		f = fopen(filename, "rb+");

		FILE *fff = fopen("vec", "a");
		fprintf(fff, "open %s\n", filename);// ++top; top > maxx ? maxx = top : 0;
		fclose(fff);

		if (!f)
		{
			f = fopen(filename, "wb+");
			if (f == nullptr)
			    fprintf(stderr, "Error: %d\n", errno);
		}
		else
		{
		    fseek(f, 0, SEEK_SET);
			fread(&cur_cnt, sizeof (size_t), 1, f);
		}
	}
	~vector()
	{
//		f = fopen(filename, "rb+");
		FILE *fff = fopen("vec", "a");
		fprintf(fff, "delete %s\n", filename);// --top;
		fclose(fff);
		if (f)
		{
			fseek(f, 0, SEEK_SET);
			fwrite(&cur_cnt, sizeof (size_t), 1, f);
			fflush(f);
			fclose(f);
		}
	}
	size_t size() const
	{
		return cur_cnt;
	}
	void init()
	{
//		f = fopen(filename, "rb+");
		if (!f)
		{
			f = fopen(filename, "wb+");
		}
		else
		{
            fseek(f, 0, SEEK_SET);
			fread(&cur_cnt, sizeof (size_t), 1, f);
		}
	}
	void clean()
	{
		fclose(f); f = NULL;
		char opt[30];
		sprintf(opt, "rm %s", filename);
		system(opt);
	}
	T operator [] (const size_t &index) const
	{
		if (index < 0 || index >= cur_cnt) throw index_out_of_bound();
		fseek(f, sizeof(size_t) + index * sizeof (T), SEEK_SET);
		T temp; fread(&temp, sizeof(T), 1, f);
		return temp;
	}
	T at(const size_t &index) const
	{
		if (index < 0 || index >= cur_cnt) throw index_out_of_bound();
		fseek(f, sizeof(size_t) + index * sizeof (T), SEEK_SET);
		T temp; fread(&temp, sizeof(T), 1, f);
		return temp;
	}
	void modify(const size_t &index, const T &value)
	{
		if (index < 0 || index >= cur_cnt) throw index_out_of_bound();
		fseek(f, sizeof (size_t) + index * sizeof (T), SEEK_SET);
		fwrite(&value, sizeof (T), 1, f); fflush(f);
	}
	void push_back(const T &value)
	{
		fseek(f, sizeof (size_t) + cur_cnt * sizeof (T), SEEK_SET);
		fwrite(&value, sizeof (T), 1, f); fflush(f);
		++cur_cnt;
	}
};
}

#endif
