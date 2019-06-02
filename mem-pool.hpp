#ifndef MEM_POOL
#define MEM_POOL

#include <cstring>


typedef unsigned long long ull;
const int BLK = 8192;
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
	mem_unit *qnext, *qprev;
	char mem[BLK];
	FILE_KEY key;
	mem_unit(FILE_KEY fkey, mem_unit *_next, mem_unit *_qnext)
			: key(fkey), next(_next), prev(nullptr), qnext(_qnext), qprev(nullptr)
	{
		FILE *f = fopen(fkey.filename, "rb+");
		if (!f) f = fopen(fkey.filename, "wb+");
		fseek(f, key.block_pos * BLK, SEEK_SET);
		fread(mem, 1, BLK, f);
		fclose(f);
	}
	void release()
	{
		FILE *f = fopen(key.filename, "rb+");
		if (!f) f = fopen(key.filename, "wb+");
		fseek(f, key.block_pos * BLK, SEEK_SET);
		fwrite(mem, 1, BLK, f); fflush(f);
		fclose(f);
	}
};
const int Moha = 33331;
const int K = 1526;
class buffer_pool {
private:
	mem_unit *head, *tail;
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
		cur_size = 0; head = tail = nullptr;
		memset(last, 0, sizeof (last));
	}
	~buffer_pool()
	{
		if (cleaned) return ;
		for (mem_unit *iter = head; iter; )
		{
			mem_unit *nxt = iter -> qnext;
			iter -> release();
			delete iter;
			iter = nxt;
		}
	}
	void remove_node(mem_unit *x)
	{
		x -> qprev ? x -> qprev -> qnext = x -> qnext : head = x -> qnext;
		x -> qnext ? x -> qnext -> qprev = x -> qprev : tail = x -> qprev;
		x -> qprev = x -> qnext = nullptr;
	}
	mem_unit *insert_block(FILE_KEY fkey)
	{
		mem_unit *ret = find(fkey);
		if (ret)
		{
			remove_node(ret);
			head ? head -> qprev = ret, ret -> qnext = head : tail = ret;
			head = ret;
			return ret;
		}
		ull hashkey; int pos;
		if (cur_size == K)
		{
			mem_unit *now = tail;
			now -> release();
			remove_node(now);
			hashkey = now->key.hash_key(); pos = hashkey % Moha;
			now -> next ? now -> next -> prev = now -> prev : 0;
			now -> prev ? now -> prev -> next = now -> next : last[pos] = now -> next;
			delete now;
			--cur_size;
		}
		hashkey = fkey.hash_key(); pos = hashkey % Moha;
		ret = new mem_unit(fkey, last[pos], head);
		last[pos] ? last[pos] -> prev = ret : 0;
		last[pos] = ret;
		head == nullptr ? tail = ret : head -> qprev = ret;
		head = ret;
		++cur_size;
		return ret;
	}
	void clean()
	{
		cur_size = 0;
		for (mem_unit *iter = head; iter; )
		{
			mem_unit *nxt = iter -> qnext;
			iter -> release();
			delete iter;
			iter = nxt;
		}
		head = tail = nullptr;
	}
} ;

buffer_pool mem_pool;

void buffer_read(void * value, int pos, int size, const char *filename)
{
	if (cleaned) return ;
	int block_l = pos / BLK, block_r = (pos + size - 1) / BLK, index = 0; pos %= BLK;
	for (int i = block_l; i <= block_r; ++i)
	{
		int len = size > BLK - pos ? BLK - pos : size;
		mem_unit *now = mem_pool.insert_block(FILE_KEY(filename, i));
		memcpy(value + index, now -> mem + pos, len); pos = 0; index += len; size -= len;
	}
}

void buffer_write(const void * value, int pos, int size, const char *filename)
{
	if (cleaned) return ;
	int block_l = pos / BLK, block_r = (pos + size - 1) / BLK, index = 0; pos %= BLK;
	for (int i = block_l; i <= block_r; ++i)
	{
		int len = size > BLK - pos ? BLK - pos : size;
		mem_unit *now = mem_pool.insert_block(FILE_KEY(filename, i));
		memcpy(now -> mem + pos, value + index, len); pos = 0; index += len; size -= len;
	}
}

#endif