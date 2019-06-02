#ifndef MEM_POOL
#define MEM_POOL

#include <cstring>


typedef unsigned long long ull;
struct FILE_KEY {
	char *filename;
	int block_pos, len;
	FILE_KEY() = default;
	FILE_KEY(const FILE_KEY &that)
	{
		len = that.len;
		filename = new char [len + 1];
		memcpy(filename, that.filename, len + 1);
		block_pos = that.block_pos;
	}
	FILE_KEY(const char *file, int _block_pos)
	{
		len = strlen(file);
		filename = new char [len + 1];
		memcpy(filename, file, len + 1);
		block_pos = _block_pos;
	}
	~FILE_KEY() {delete [] filename;}
	inline bool operator == (const FILE_KEY &that) const
	{
		if (len != that.len || block_pos != that.block_pos) return 0;
		for (int i = 0; i < len; ++i)
			if (filename[i] != that.filename[i]) return 0;
		return 1;
	}
	ull hash_key()
	{
		ull ret = block_pos;
		for (int i = 0; i < len; ++i) ret = ret * 311 + filename[i];
		return ret;
	}
};
struct mem_unit {
	mem_unit *next, *prev;
	char mem[8192];
	FILE_KEY key;
	mem_unit(FILE_KEY fkey, mem_unit *_next) : key(fkey), next(_next), prev(nullptr)
	{
		FILE *f = fopen(fkey.filename, "rb+");
		if (!f) f = fopen(fkey.filename, "wb+");
		fseek(f, key.block_pos * 8192, SEEK_SET);
		fread(mem, 1, 8192, f);
		fclose(f);
	}
	void release()
	{
		FILE *f = fopen(key.filename, "rb+");
		if (!f) f = fopen(key.filename, "wb+");
		fseek(f, key.block_pos * 8192, SEEK_SET);
		fwrite(mem, 1, 8192, f); fflush(f);
		fclose(f);
	}
};
const int Moha = 33331;
const int K = 1926;
class buffer_pool {
private:
	mem_unit *mem[K];
	mem_unit *last[Moha];
	int cur_size;
	mem_unit *find(FILE_KEY fkey)
	{
		int pos = fkey.hash_key() % Moha;
		for (mem_unit *iter = last[pos]; iter; iter = iter -> next)
			if (fkey == iter -> key) return iter;
		return nullptr;
	}
public:
	buffer_pool()
	{
		cur_size = 0;
		memset(mem, 0, sizeof (mem));
		memset(last, 0, sizeof (last));
	}
	~buffer_pool()
	{
		if (cleaned) return ;
		for (int i = 0; i < K; ++i)
			if (mem[i])
			{
				mem[i] -> release();
				delete mem[i];
			}
	}
	mem_unit *insert_block(FILE_KEY fkey)
	{
		mem_unit *ret = find(fkey);
		if (ret) return ret;
		ull hashkey; int pos;
		if (mem[cur_size])
		{
			mem[cur_size] -> release();
			hashkey = mem[cur_size]->key.hash_key(); pos = hashkey % Moha;
			mem[cur_size] -> next ? mem[cur_size] -> next -> prev = mem[cur_size] -> prev : 0;
			mem[cur_size] -> prev ? mem[cur_size] -> prev -> next = mem[cur_size] -> next : last[pos] = mem[cur_size] -> next;
			delete mem[cur_size];
		}
		hashkey = fkey.hash_key(); pos = hashkey % Moha;
		ret = new mem_unit(fkey, last[pos]);
		last[pos] ? last[pos] -> prev = ret : 0;
		last[pos] = ret;
		mem[cur_size] = ret;
		cur_size = (cur_size + 1) % K;
		return ret;
	}
	void clean()
	{
		for (int i = 0; i < K; ++i)
			if (mem[i])
			{
				mem[i] -> release();
				delete mem[i];
			}
	}
} ;

buffer_pool mem_pool;

void buffer_read(void * value, int pos, int size, const char *filename)
{
	if (cleaned) return ;
	int block_l = pos >> 13, block_r = (pos + size - 1) >> 13, index = 0; pos %= 8192;
	for (int i = block_l; i <= block_r; ++i)
	{
		int len = size > 8192 - pos ? 8192 - pos : size;
		mem_unit *now = mem_pool.insert_block(FILE_KEY(filename, i));
		memcpy(value + index, now -> mem + pos, len); pos = 0; index += len; size -= len;
	}
}

void buffer_write(const void * value, int pos, int size, const char *filename)
{
	if (cleaned) return ;
	int block_l = pos >> 13, block_r = (pos + size - 1) >> 13, index = 0; pos %= 8192;
	for (int i = block_l; i <= block_r; ++i)
	{
		int len = size > 8192 - pos ? 8192 - pos : size;
		mem_unit *now = mem_pool.insert_block(FILE_KEY(filename, i));
		memcpy(now -> mem + pos, value + index, len); pos = 0; index += len; size -= len;
	}
}

#endif