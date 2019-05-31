#include <iostream>
#include "./vector/exceptions.hpp"

const off_t invalid_off = 0xdeadbeef;

class find_blank {
private:
    const off_t K = 4096;
    int num;
    off_t size;
    off_t *blank;
    char *index_file;
    FILE *file;

public:
    find_blank() {}

    find_blank(const char *in_file) {
        index_file = new char[strlen(in_file) + 1];
        strcpy(index_file, in_file);
        file = fopen(index_file, "rb+");

		FILE *fff = fopen("bptree2", "a");
		fprintf(fff, "open %s\n", index_file);// ++top; top > maxx ? maxx = top : 0;
		fclose(fff);

        if (!file) {
            num = 0;
            size = 3 * sizeof(off_t);
            blank = nullptr;
            file = fopen(index_file, "wb+");
            save_info();
        }
        else load_info();
    }

    ~find_blank() {

		FILE *fff = fopen("bptree2", "a");
		fprintf(fff, "close %s\n", index_file);// --top;
		fclose(fff);

		if (file) fclose(file);
		if (index_file) delete index_file;
        if (blank) delete blank;
    }

    void init(const char *in_file) {
        index_file = new char[strlen(in_file) + 1];
        strcpy(index_file, in_file);
        file = fopen(index_file, "rb+");

		FILE *fff = fopen("bptree2", "a");
		fprintf(fff, "open %s\n", index_file);// ++top; top > maxx ? maxx = top : 0;
		fclose(fff);

        if (!file) {
            num = 0;
            size = 3 * sizeof(off_t);
            blank = nullptr;
            file = fopen(index_file, "wb+");
            save_info();
        }
        else load_info();
    }

    void save_info() {
        fseek(file, 0, SEEK_SET);
        fwrite(&num, sizeof(int), 1, file);
        fwrite(&size, sizeof(off_t), 1, file);
        if (num != 0) {
            for (int i = 0; i < num; ++i)
                fwrite(&blank[i], sizeof(off_t), 1, file);
        }
        fflush(file);
    }

    void load_info() {
        fseek(file, 0, SEEK_SET);
        fread(&num, sizeof(int), 1, file);
        fread(&size, sizeof(off_t), 1, file);
        if (num) {
            blank = new off_t[num + 1];
            for (int i = 0; i < num; ++i)
                fread(&blank[i], sizeof(off_t), 1, file);
        }
        else blank = nullptr;
    }

    off_t _alloc() {
        load_info();
        if (num == 0) {
            off_t tmp = size;
            size += K;
            save_info();
            return tmp;
        }
        else {
            --num;
            save_info();
            return blank[num];
        }
    }

    void _delete(off_t pos) {
        load_info();
        if (num == 0) {
            num = 1;
            blank = new off_t[1];
            blank[0] = pos;
        }
        else
            blank[num++] = pos;
        save_info();
        num = 0;
        size = invalid_off;
        delete blank;
        blank = nullptr;
    }
};
