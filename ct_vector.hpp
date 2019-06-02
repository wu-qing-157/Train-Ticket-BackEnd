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
			buffer_read(&cur_cnt, 0, sizeof (size_t), filename);
		}
		~vector()
		{
			buffer_write(&cur_cnt, 0, sizeof (size_t), filename);
		}
		size_t size() const
		{
			return cur_cnt;
		}
		void init()
		{
			buffer_read(&cur_cnt, 0, sizeof (size_t), filename);
		}
		void clean()
		{
			FILE *f = fopen(filename, "rb+");
			if (!f) return ;
			fclose(f);
			char opt[30];
			sprintf(opt, "rm %s", filename);
			system(opt);
		}
		T operator [] (const size_t &index) const
		{
			if (index < 0 || index >= cur_cnt) throw index_out_of_bound();
			T temp;
			buffer_read(&temp, sizeof(size_t) + index * sizeof (T), sizeof(T), filename);
			return temp;
		}
		void modify(const size_t &index, const T &value)
		{
			if (index < 0 || index >= cur_cnt) throw index_out_of_bound();
			buffer_write(&value, sizeof(size_t) + index * sizeof (T), sizeof(T), filename);
		}
		void push_back(const T &value)
		{
			buffer_write(&value, sizeof(size_t) + cur_cnt * sizeof (T), sizeof(T), filename);
			++cur_cnt;
		}
	};
}

#endif
