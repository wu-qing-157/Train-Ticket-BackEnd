#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>

namespace ct {
template<typename T, const char *filename>
class vector {
private:
	FILE *f;
	size_t cur_cnt;
public:
	vector()
	{
		f = fopen(filename, "rb+");
		if (!f)
		{
			f = fopen(filename, "wb+");
			fwrite(&cur_cnt, sizeof (size_t), 1, f);
		}
		else
		{
			fread(&cur_cnt, sizeof (size_t), 1, f);
		}
	}
	~vector()
	{
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
		f = fopen(filename, "rb+");
		if (!f)
		{
			f = fopen(filename, "wb+");
			fwrite(&cur_cnt, sizeof (size_t), 1, f);
		}
		else
		{
			fread(&cur_cnt, sizeof (size_t), 1, f);
		}
	}
	void clean()
	{
		char opt[20];
		sprintf(opt, "rm %s", filename);
		system(opt);
	}
	const T operator [] (const size_t &index) const
	{
		if (index < 0 || index >= cur_cnt) throw index_out_of_bound();
		fseek(f, sizeof(size_t) + index * sizeof (T), SEEK_SET);
		T temp; fread(&temp, sizeof(T), 1, f);
		return temp;
	}
	const T at(const size_t &index) const
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
		fwrite(&value, sizeof (T), 1, f);
	}
	void push_back(const T &value)
	{
		fseek(f, sizeof (size_t) + cur_cnt * sizeof (T), SEEK_SET);
		fwrite(&value, sizeof (T), 1, f);
		++cur_cnt;
	}
};
}

#endif
