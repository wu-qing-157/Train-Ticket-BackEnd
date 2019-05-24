//
// Created by jiayuqi on 19-4-28.
//
#include <functional>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <cstring>
#include "utility.hpp"
#include "exceptions.hpp"
#include "find_blank.hpp"

using namespace std;
#ifndef BPTREE_BPTREE_HPP
#define BPTREE_BPTREE_HPP

namespace sjtu {
    template<class Key, class value_t, size_t K = 4096, class Compare = std::less<Key>>
    class bptree {
        //friend class iterator;
        //friend class const_iterator;
        typedef char* _buffer;
        typedef char buffer[K];
        const size_t node_size;
        const size_t leaf_size;

        FILE *file;
        find_blank finder;
        off_t head, rear, root;
        char *filename, *index_file;
        Compare comp = Compare();
        bool compare(const Key &a, const Key &b){
            return !(comp(a,b)||comp(b,a));
        }

        struct node {
            Key mainKey;
            off_t prev,next;
            size_t curSize;
            off_t pos;
            bool isLeaf;
            node(off_t Pos, off_t Prev = invalid_off, off_t Next = invalid_off, bool IsLeaf = true)
                    :pos(Pos),
                     prev(Prev),
                     next(Next),
                     isLeaf(IsLeaf),
                     curSize(0){
                mainKey = Key();
            }//leaf
            node(off_t Pos, bool Isleaf = false)
                    :pos(Pos),
                     isLeaf(Isleaf),
                     curSize(0){
                mainKey = Key();
            }//node
            node(const node &other){
                mainKey = other.mainKey;
                prev = other.prev;
                next = other.next;
                curSize = other.curSize;
                pos = other.pos;
                isLeaf = other.isLeaf;
            }
        };
    private:
        inline void save_node(const node &x){
            fseek(file, x.pos, SEEK_SET);
            fwrite(&x, sizeof(node), 1, file);
            fflush(file);
        }

        inline void delete_node(const node &x){
            finder._delete(x.pos);
        }

        void save_info() {
            fseek(file, 0, SEEK_SET);
            fwrite(&head, sizeof(off_t), 1, file);
            fwrite(&rear, sizeof(off_t), 1, file);
            fwrite(&root, sizeof(off_t), 1, file);
            fflush(file);
        }

        void init(){
            head = rear = root = invalid_off;
            fseek(file, 0, SEEK_SET);
            save_info();
        }

        inline void buffer_load_node(_buffer b, const node &x){
            fseek(file, x.pos+sizeof(node), SEEK_SET);
            if(x.curSize == 0) return;
            fread(b, 1, x.curSize*(sizeof(off_t)+sizeof(Key)), file);
        }

        inline void buffer_load_leaf(_buffer b, const node &x){
            fseek(file, x.pos+sizeof(node), SEEK_SET);
            if(x.curSize == 0) return;
            fread(b, 1, x.curSize*(sizeof(value_t)+sizeof(Key)), file);
        }

        inline void buffer_save_node(_buffer b, const node &x){
            fseek(file, x.pos+sizeof(node), SEEK_SET);
            fwrite(b, 1, x.curSize*(sizeof(off_t)+sizeof(Key)), file);
            fflush(file);
        }

        inline void buffer_save_leaf(_buffer b, const node &x){
            fseek(file, x.pos+sizeof(node), SEEK_SET);
            fwrite(b, 1, x.curSize*(sizeof(value_t)+sizeof(Key)), file);
            fflush(file);
        }

        //================get info from node==================

        Key *get_key_node(size_t num, _buffer b){
            return (Key *)(b+num*(sizeof(Key)+sizeof(off_t)));
        }

        off_t *get_son_node(size_t num, _buffer b){
            return (off_t *)(b+num*(sizeof(Key)+sizeof(off_t))+sizeof(Key));
        }

        Key *get_key_leaf(size_t num, _buffer b){
            return (Key *)(b+num*(sizeof(Key)+sizeof(value_t)));
        }

        value_t *get_value_leaf(size_t num, _buffer b){
            return (value_t *)(b+num*(sizeof(Key)+sizeof(value_t))+sizeof(Key));
        }

        node get_node(off_t t){
            node x(invalid_off, false);
            fseek(file, t, SEEK_SET);
            fread(&x, sizeof(node), 1, file);
            return x;
        }

        size_t leaf_find_pos(_buffer b, Key key, size_t n){
            size_t l=0, r=n, mid;
            while(l<r){
                mid = (l+r)/2;
                if(comp(*get_key_leaf(mid, b), key)) l = mid+1;
                else r = mid;
            }
            return l;
        }

        size_t node_find_pos(_buffer b, Key key, size_t n){
            size_t l=0, r=n, mid;
            while(l<r){
                mid = (l+r)/2;
                if(comp(*get_key_node(mid, b), key)) l = mid+1;
                else r = mid;
            }
            return l;
        }

        void insert_trans_node(_buffer b, node &p, size_t t){
            for(size_t i = p.curSize; i>t; --i){
                *get_key_node(i, b) = *get_key_node(i-1, b);
                *get_son_node(i, b) = *get_son_node(i-1, b);
            }
        }

        void insert_trans_leaf(_buffer b, node &p, size_t t){
            for(size_t i = p.curSize; i>t; --i){
                *get_key_leaf(i, b) = *get_key_leaf(i-1, b);
                *get_value_leaf(i, b) = *get_value_leaf(i-1, b);
            }
        }

        node split_node(_buffer b, node &x){
            size_t sz = x.curSize/2, new_sz = x.curSize - sz;
            node new_node(finder._alloc(), false);
            new_node.mainKey = *get_key_node(sz, b);
            new_node.curSize = new_sz;
            x.curSize = sz;
            save_node(x);
            buffer_save_node(b, x);
            save_node(new_node);
            buffer_save_node(b+sz*(sizeof(Key)+sizeof(off_t)), new_node);
            return new_node;
        }

        node split_leaf(_buffer b, node &x){
            size_t sz = x.curSize/2, new_sz = x.curSize-sz;
            off_t nxt = x.next;
            node new_leaf(finder._alloc(), x.pos, x.next, true);
            x.next = new_leaf.pos;
            x.curSize = sz;
            new_leaf.curSize = new_sz;
            new_leaf.mainKey = *get_key_leaf(sz, b);
            if(new_leaf.next == invalid_off){
                rear = new_leaf.pos;
                save_info();
            }
            else{
                node new_leaf_nxt = get_node(nxt);
                new_leaf_nxt.prev = new_leaf.pos;
                save_node(new_leaf_nxt);
            }
            save_node(x);
            buffer_save_leaf(b ,x);
            save_node(new_leaf);
            buffer_save_leaf(b+sz*(sizeof(Key)+sizeof(value_t)), new_leaf);
            return new_leaf;
        }

        node insert(node &x, Key key, value_t value, bool &is_change){
            if(x.isLeaf){
                buffer b;
                buffer_load_leaf(b, x);
                node new_node = buffer_insert_leaf(b, x, key, value, is_change);
                return new_node;
            }
            else{
                buffer b;
                buffer_load_node(b, x);
                size_t pos = node_find_pos(b, key, x.curSize);
                if(!compare(key,*get_key_node(pos, b))&&pos > 0) pos--;
                off_t pre_son = *get_son_node(pos, b);
                node pre_son_node = get_node(pre_son);
                node new_node = insert(pre_son_node, key, value, is_change);
                if(new_node.pos != pre_son_node.pos){
                    //*get_key_node(pos, b) = pre_son_node.mainKey;
                    node n_node = buffer_insert_node(b, x, new_node.mainKey, new_node.pos, pos+1);
                    return n_node;
                }
                else{
                    *get_key_node(pos, b) = pre_son_node.mainKey;
                    x.mainKey = *get_key_node(0, b);
                    save_node(x);
                    buffer_save_node(b, x);
                    return x;
                }

            }
        }

        node buffer_insert_node(_buffer b, node &x, Key key, off_t son, size_t t){
            //size_t t = node_find_pos(b, key, x.curSize);
            //if(t<x.curSize&&comp(key, *get_key_node(t, b)))
            //    return x;
            insert_trans_node(b, x, t);
            *get_key_node(t, b) = key;
            *get_son_node(t, b) = son;
            x.mainKey = *get_key_node(0, b);
            ++x.curSize;
            save_node(x);
            if(x.curSize > node_size){
                node y = split_node(b, x);
                return y;
            }
            else{
                buffer_save_node(b, x);
                return x;
            }
        }

        node buffer_insert_leaf(_buffer b, node &x, Key key, value_t value, bool &is_change){
            size_t t = leaf_find_pos(b, key, x.curSize);
            if(t<x.curSize&&compare(key, *get_key_leaf(t, b))) {
                *get_value_leaf(t, b) = value;
                is_change = true;
                return x;
            }
            //if(!compare(key, *get_key_leaf(t, b))&&t>0) t--;
            insert_trans_leaf(b, x, t);
            *get_key_leaf(t, b) = key;
            *get_value_leaf(t, b) = value;
            x.mainKey = *get_key_node(0, b);
            ++x.curSize;
            save_node(x);//1///////////////////////////////////////////////////////////////
            is_change = false;
            if(x.curSize > leaf_size){
                node y = split_leaf(b, x);
                return y;
            }
            else{
                buffer_save_leaf(b, x);
                return x;
            }
        }
        //================================================erase=================================================================================

        void merge_node_right(_buffer bx, node &x, node &y){
            buffer by;
            buffer_load_node(by, y);
            for(size_t i = 0; i<y.curSize; ++i){
                *get_son_node(i+x.curSize, bx) = *get_son_node(i, by);
                *get_key_node(i+x.curSize, bx) = *get_key_node(i, by);
            }
            x.curSize+=y.curSize;
            delete_node(y);
            save_node(x);
            buffer_save_node(bx, x);
        }

        void merge_node_left(_buffer bx, node &x, node &y){
            buffer by;
            buffer_load_node(by, y);
            for(size_t i = 0; i<x.curSize; ++i){
                *get_son_node(i+y.curSize, by) = *get_son_node(i, bx);
                *get_key_node(i+y.curSize, by) = *get_key_node(i, bx);
            }
            y.curSize+=x.curSize;
            delete_node(x);
            save_node(y);
            buffer_save_node(by, y);
        }

        void merge_leaf_right(_buffer bx, node &x, node &y){
            buffer by;
            buffer_load_leaf(by, y);
            for(size_t i = 0; i<y.curSize; ++i){
                *get_value_leaf(i+x.curSize, bx) = *get_value_leaf(i, by);
                *get_key_leaf(i+x.curSize, bx) = *get_key_leaf(i, by);
            }
            x.curSize += y.curSize;
            x.next = y.next;
            if(y.next != invalid_off){
                node ny = get_node(y.next);
                ny.prev = x.pos;
                save_node(ny);
            }
            delete_node(y);
            save_node(x);
            buffer_save_leaf(bx, x);
        }

        void merge_leaf_left(_buffer bx, node &x, node &y){
            buffer by;
            buffer_load_leaf(by, y);
            for(size_t i = 0; i<x.curSize; ++i){
                *get_value_leaf(i+y.curSize, by) = *get_value_leaf(i, bx);
                *get_key_leaf(i+y.curSize, by) = *get_key_leaf(i, bx);
            }
            y.curSize += x.curSize;
            y.next = x.next;
            if(x.next != invalid_off){
                node nx = get_node(x.next);
                nx.prev = y.pos;
                save_node(nx);
            }
            delete_node(x);
            save_node(y);
            buffer_save_leaf(by, y);
        }

        void buffer_erase_node(_buffer b, node &x, Key key){
            size_t t = node_find_pos(b, key, x.curSize);
            if(!(t<x.curSize&&compare(key, *get_key_node(t, b))))
                throw "erase node";
            for(size_t i = t; i<x.curSize-1; ++i){
                *get_key_node(i, b) = *get_key_node(i+1, b);
                *get_son_node(i, b) = *get_son_node(i+1, b);
            }
            x.mainKey = *get_key_node(b, 0);
            --x.curSize;
        }

        void buffer_erase_leaf(_buffer b, node &x, Key key){
            size_t t = leaf_find_pos(b, key, x.curSize);
            if(!(t<x.curSize&&compare(key, *get_key_leaf(t, b))))
                throw "erase_leaf";
            for(size_t i = t; i<x.curSize-1; ++i){
                *get_key_leaf(i, b) = *get_key_leaf(i+1, b);
                *get_value_leaf(i, b) = *get_value_leaf(i+1, b);
            }
            x.mainKey = *get_key_leaf(0, b);
            --x.curSize;
        }

        pair<int, Key> leaf_balance(_buffer b, node &x, off_t prev, off_t next){
            //-1 not merged and balance with next    0 not merged and balance with prev     1 merged with prev    2 merged with next      3 deleted
            if(prev == invalid_off && next == invalid_off){
                if(!x.curSize){
                    delete_node(x);
                    head = rear = root = invalid_off;
                    save_info();
                    save_node(x);
                    buffer_save_leaf(b, x);
                    return pair<int, Key>(3, Key());
                }
                return pair<int, Key>(0, Key());
            }
            if(prev == invalid_off){
                node n = get_node(next);
                if(n.curSize>=leaf_size/2+1){
                    buffer bn;
                    buffer_load_leaf(bn, n);
                    *get_key_leaf(x.curSize, b) = *get_key_leaf(0, bn);
                    *get_value_leaf(x.curSize, b) = *get_value_leaf(0, bn);
                    for(size_t i = 0; i<n.curSize-1; ++i){
                        *get_key_leaf(i, bn) = *get_key_leaf(i+1, bn);
                        *get_value_leaf(i, bn) = *get_value_leaf(i+1, bn);
                    }
                    n.mainKey = *get_key_leaf(0, bn);
                    x.curSize++;
                    n.curSize--;
                    save_node(x);
                    buffer_save_leaf(b, x);
                    save_node(n);
                    buffer_save_leaf(bn, n);
                    return pair<int, Key>(-1, n.mainKey);
                }
                else {
                    merge_leaf_right(b, x, n);
                    return pair<int, Key>(2, Key());
                }
            }
            if(next == invalid_off){
                node p = get_node(prev);
                if(p.curSize>=leaf_size/2+1){
                    buffer bp;
                    buffer_load_leaf(bp, p);
                    for(size_t i = x.curSize; i > 0; --i){
                        *get_key_leaf(i, b) = *get_key_leaf(i-1, b);
                        *get_value_leaf(i, b) = *get_value_leaf(i-1, b);
                    }
                    *get_key_leaf(0, b) = *get_key_leaf(p.curSize-1, bp);
                    *get_value_leaf(0, b) = *get_value_leaf(p.curSize-1, bp);
                    x.mainKey = *get_key_leaf(0, b);
                    x.curSize++;
                    p.curSize--;
                    save_node(x);
                    buffer_save_leaf(b, x);
                    save_node(p);
                    buffer_save_leaf(bp, p);
                    return pair<int, Key>(0, x.mainKey);
                }
                else{
                    merge_leaf_left(b, x, p);
                    return pair<int, Key>(1, Key());
                }
            }
            node p = get_node(prev), n = get_node(next);
            if(p.curSize>=leaf_size/2+1){
                buffer bp;
                buffer_load_leaf(bp, p);
                for(size_t i = x.curSize; i > 0; --i){
                    *get_key_leaf(i, b) = *get_key_leaf(i-1, b);
                    *get_value_leaf(i, b) = *get_value_leaf(i-1, b);
                }
                *get_key_leaf(0, b) = *get_key_leaf(p.curSize-1, bp);
                *get_value_leaf(0, b) = *get_value_leaf(p.curSize-1, bp);
                x.mainKey = *get_key_leaf(0, b);
                x.curSize++;
                p.curSize--;
                save_node(x);
                buffer_save_leaf(b, x);
                save_node(p);
                buffer_save_leaf(bp, p);
                return pair<int, Key>(0, x.mainKey);
            }
            else{
                if(n.curSize>=leaf_size/2+1){
                    buffer bn;
                    buffer_load_leaf(bn, n);
                    *get_key_leaf(x.curSize, b) = *get_key_leaf(0, bn);
                    *get_value_leaf(x.curSize, b) = *get_value_leaf(0, bn);
                    for(size_t i = 0; i<n.curSize-1; ++i){
                        *get_key_leaf(i, bn) = *get_key_leaf(i+1, bn);
                        *get_value_leaf(i, bn) = *get_value_leaf(i+1, bn);
                    }
                    n.mainKey = *get_key_leaf(0, bn);
                    x.curSize++;
                    n.curSize--;
                    save_node(x);
                    buffer_save_leaf(b, x);
                    save_node(n);
                    buffer_save_leaf(bn, n);
                    return pair<int, Key>(-1, n.mainKey);
                }
                else{
                    merge_leaf_right(b, x, n);
                    return pair<int, Key>(2, Key());
                }
            }
        }

        pair<int, Key> node_balance(_buffer b, node &x, off_t prev, off_t next){
            //-1 not merged and balance with next    0 not merged and balance with prev     1 merged with prev    2 merged with next      3 deleted
            if(prev == invalid_off && next == invalid_off){
                if(!x.curSize){
                    delete_node(x);
                    head = rear = root = invalid_off;
                    save_info();
                    save_node(x);
                    buffer_save_node(b, x);
                    return pair<int, Key>(3, Key());
                }
                return pair<int, Key>(0, Key());
            }
            if(prev == invalid_off){
                node n = get_node(next);
                if(n.curSize>=node_size/2+1){
                    buffer bn;
                    buffer_load_node(bn, n);
                    *get_key_node(x.curSize, b) = *get_key_node(0, bn);
                    *get_son_node(x.curSize, b) = *get_son_node(0, bn);
                    for(size_t i = 0; i<n.curSize-1; ++i){
                        *get_key_node(i, bn) = *get_key_node(i+1, bn);
                        *get_son_node(i, bn) = *get_son_node(i+1, bn);
                    }
                    n.mainKey = *get_key_node(0, bn);
                    x.curSize++;
                    n.curSize--;
                    save_node(x);
                    buffer_save_node(b, x);
                    save_node(n);
                    buffer_save_node(bn, n);
                    return pair<int, Key>(-1, n.mainKey);
                }
                else {
                    merge_node_right(b, x, n);
                    return pair<int, Key>(2, Key());
                }
            }
            if(next == invalid_off){
                node p = get_node(prev);
                if(p.curSize>=node_size/2+1){
                    buffer bp;
                    buffer_load_node(bp, p);
                    for(size_t i = x.curSize; i > 0; --i){
                        *get_key_node(i, b) = *get_key_node(i-1, b);
                        *get_son_node(i, b) = *get_son_node(i-1, b);
                    }
                    *get_key_node(0, b) = *get_key_node(p.curSize-1, bp);
                    *get_son_node(0, b) = *get_son_node(p.curSize-1, bp);
                    x.mainKey = *get_key_node(0, b);
                    x.curSize++;
                    p.curSize--;
                    save_node(x);
                    buffer_save_node(b, x);
                    save_node(p);
                    buffer_save_node(bp, p);
                    return pair<int, Key>(0, x.mainKey);
                }
                else{
                    merge_node_left(b, x, p);
                    return pair<int, Key>(1, Key());
                }
            }
            node p = get_node(prev), n = get_node(next);
            if(p.curSize>=node_size/2+1){
                buffer bp;
                buffer_load_node(bp, p);
                for(size_t i = x.curSize; i > 0; --i){
                    *get_key_node(i, b) = *get_key_node(i-1, b);
                    *get_son_node(i, b) = *get_son_node(i-1, b);
                }
                *get_key_node(0, b) = *get_key_node(p.curSize-1, bp);
                *get_son_node(0, b) = *get_son_node(p.curSize-1, bp);
                x.mainKey = *get_key_node(0, b);
                x.curSize++;
                p.curSize--;
                save_node(x);
                buffer_save_node(b, x);
                save_node(p);
                buffer_save_node(bp, p);
                return pair<int, Key>(0, x.mainKey);
            }
            else{
                if(n.curSize>=node_size/2+1){
                    buffer bn;
                    buffer_load_node(bn, n);
                    *get_key_node(x.curSize, b) = *get_key_node(0, bn);
                    *get_son_node(x.curSize, b) = *get_son_node(0, bn);
                    for(size_t i = 0; i<n.curSize-1; ++i){
                        *get_key_node(i, bn) = *get_key_node(i+1, bn);
                        *get_son_node(i, bn) = *get_son_node(i+1, bn);
                    }
                    n.mainKey = *get_key_node(0, bn);
                    x.curSize++;
                    n.curSize--;
                    save_node(x);
                    buffer_save_node(b, x);
                    save_node(n);
                    buffer_save_node(bn, n);
                    return pair<int, Key>(-1, n.mainKey);
                }
                else{
                    merge_node_right(b, x, n);
                    return pair<int, Key>(2, Key());
                }
            }
        }

        pair<int , Key> erase(node &x, const Key &key, node &parent){
            //-1 not merged and balance with next    0 not merged and balance with prev     1 merged with prev    2 merged with next      3 deleted
            buffer b;
            if(x.isLeaf){
                buffer_load_leaf(b, x);
                buffer_erase_leaf(b, x, key);
                if(x.curSize == 0 && x.pos == root){
                    root = head = rear = invalid_off;
                    save_info();
                    return pair<int, Key>(0, x.mainKey);
                }
                if(x.curSize<leaf_size/2&&x.pos != root){
                    buffer bp;
                    buffer_load_node(bp, parent);
                    size_t t = node_find_pos(bp, x.mainKey, parent.curSize);
                    if(!compare(x.mainKey,*get_key_node(t, bp))&&t>0) t--;
                    if (t == parent.curSize) --t;
                    off_t prev = (t>0)?(*get_son_node(t-1, bp)):invalid_off;
                    off_t next = (t<parent.curSize-1)?(*get_son_node(t+1, bp)):invalid_off;
                    return leaf_balance(b, x, prev, next);
                }
                save_node(x);
                buffer_save_leaf(b, x);
                return pair<int, Key>(0, x.mainKey);
            }
            else{
                buffer_load_node(b, x);
                size_t tx = node_find_pos(b, key, x.curSize);
                if(!compare(key, *get_key_node(tx, b))&&tx>0)tx--;
                node x_son = get_node(*get_son_node(tx, b));
                pair<int, Key> tmp = erase(x_son, key, x);
                switch (tmp.first){
                    case -1:{
                        *get_key_node(tx, b)   = x_son.mainKey;
                        *get_key_node(tx+1, b) = tmp.second;
                        if(!tx) x.mainKey = x_son.mainKey;
                        save_node(x);
                        buffer_save_node(b, x);
                        break;
                    }
                    case 0:{
                        *get_key_node(tx, b) = tmp.second;
                        save_node(x);
                        buffer_save_node(b, x);
                        break;
                    }
                    case 1:{
                        for(size_t i = tx; i<x.curSize-1; ++i){
                            *get_son_node(i, b) = *get_son_node(i+1, b);
                            *get_key_node(i, b) = *get_key_node(i+1, b);
                        }
                        x.curSize--;
                        save_node(x);
                        buffer_save_node(b, x);
                        break;
                    }
                    case 2:{
                        for(size_t i = tx+1; i<x.curSize-1; ++i){
                            *get_son_node(i, b) = *get_son_node(i+1, b);
                            *get_key_node(i, b) = *get_key_node(i+1, b);
                        }
                        if(!tx) x.mainKey = x_son.mainKey;
                        x.curSize--;
                        save_node(x);
                        buffer_save_node(b, x);
                        break;
                    }
                    default:{
                        delete_node(x);
                        head = rear = root = invalid_off;
                        save_info();
                        return pair<int, Key>(3, Key());
                    }
                }

                if(x.pos == root&&x.curSize == 1){
                    root = *get_son_node(0, b);
                    save_info();
                    return pair<int, Key>(0, Key());
                }

                if(x.curSize<node_size/2){
                    if(root == x.pos) return node_balance(b, x, invalid_off, invalid_off);
                    buffer bp;
                    buffer_load_node(bp, parent);
                    size_t t = node_find_pos(bp, x.mainKey, parent.curSize);
                    if(!compare(x.mainKey,*get_key_node(t, bp))&&t>0) t--;
                    off_t prev = (t>0)?(*get_son_node(t-1, bp)):invalid_off;
                    off_t next = (t<parent.curSize-1)?(*get_son_node(t+1, bp)):invalid_off;
                    return node_balance(b, x, prev, next);
                }
                x.mainKey = *get_key_node(0, b);
                save_node(x);
                buffer_save_node(b, x);
                return pair<int, Key>(0, x.mainKey);
            }
        }
//=====================================================================================================================================================

    public:
        bptree(const char *fname, const char *in_file)://leaf_size(5),node_size(5)
        leaf_size((K - sizeof(node))/(sizeof(Key)+sizeof(value_t))-1),
        node_size((K - sizeof(node))/(sizeof(Key)+sizeof(off_t))-1)
        {
            filename = new char[strlen(fname)+1];
            strcpy(filename, fname);
            index_file = new char[strlen(in_file)+1];
            strcpy(index_file, in_file);
            file = fopen(filename, "rb+");
            //finder.load(index_file);
            finder.init(in_file);
            if(!file) {
                file = fopen(fname, "wb+");
                init();
            }
            else{
                fseek(file, 0, SEEK_SET);
                fread(&head, sizeof(off_t), 1, file);
                fread(&rear, sizeof(off_t), 1, file);
                fread(&root, sizeof(off_t), 1, file);
            }
        }

        ~bptree(){
            save_info();
            if(file) fclose(file);
            delete filename;
            delete index_file;
        }

        void clear(){
            char delete_file[40] = "rm ";
            strncpy(delete_file+3, filename,  strlen(filename));
            system(delete_file);
            char delete_infile[40] = "rm ";
            strncpy(delete_infile+3, index_file, strlen(index_file));
            system(delete_infile);
        }

        class const_iterator;
        class iterator{
            friend const_iterator;
        private:
            node leaf;
            size_t pos;
            bptree *tree;

        public:
            iterator():leaf(invalid_off, invalid_off, invalid_off, true), pos(0), tree(nullptr){}

            iterator(node _leaf, size_t p, bptree *_tree):leaf(_leaf){
                pos  = p;
                tree = _tree;
            }

            iterator(const iterator &other):leaf(other.leaf){
                pos  = other.pos;
                tree = other.tree;
            }

            iterator operator++(int){
                iterator tmp(*this);
                ++(*this);
                return tmp;
            }

            iterator operator++(){
                if(pos == leaf.curSize-1) {
                    if(leaf.pos != tree->rear) {
                        leaf = tree->get_node(leaf.next);
                        pos = 0;
                    }
                    else {
                        node tmp(invalid_off, invalid_off, invalid_off, true);
                        leaf = tmp;
                        pos = 0;
                    }
                }
                else{
                    pos++;
                }
                return *this;
            }

            iterator operator--(int){
                iterator tmp(*this);
                --(*this);
                return  tmp;
            }

            iterator operator--(){
                if(pos == 0){
                    if(leaf.pos != tree->head){
                        leaf = tree->get_node(leaf.prev);
                        pos = leaf.curSize-1;
                    }
                    else {
                        leaf = tree->get_node(tree->head);
                        pos = 0;
                    }
                }
                else{
                    pos--;
                }
                return *this;
            }

            value_t& operator*(){
                buffer b;
                tree->buffer_load_leaf(b, leaf);
                return *tree->get_value_leaf(pos, b);
            }

            bool operator==(const iterator &rhs)const{
                return (leaf.pos == rhs.leaf.pos&&pos == rhs.pos);
            }

            bool operator!=(const iterator &rhs)const{
                return !(*this == rhs);
            }

            bool operator==(const const_iterator &rhs)const{
                return (leaf.pos == rhs.leaf.pos&&pos == rhs.pos);
            }

            bool operator!=(const const_iterator &rhs)const{
                return !(*this == rhs);
            }
        };

        class const_iterator{
            friend iterator;
        private:
            node leaf;
            size_t pos;
            bptree *tree;

        public:
            const_iterator():leaf(invalid_off, invalid_off, invalid_off, true), pos(0), tree(nullptr){}

            const_iterator(node _leaf, size_t p, bptree *_tree):leaf(_leaf){
                    pos  = p;
                    tree = _tree;
            }

            const_iterator(const iterator &other):leaf(other.leaf){
                pos  = other.pos;
                tree = other.tree;
            }

            const_iterator operator++(int){
                const_iterator tmp(*this);
                ++(*this);
                return tmp;
            }

            const_iterator operator++(){
                if(pos == leaf.curSize-1) {
                    if(leaf.pos != tree->rear) {
                        leaf = tree->get_node(leaf.next);
                        pos = 0;
                    }
                    else {
                        node tmp(invalid_off, invalid_off, invalid_off, true);
                        leaf = tmp;
                        pos = 0;
                    }
                }
                else{
                    pos++;
                }
                return *this;
            }

            const_iterator operator--(int){
                const_iterator tmp(*this);
                --(*this);
                return  tmp;
            }

            const_iterator operator--(){
                if(pos == 0){
                    if(leaf.pos != tree->head){
                        leaf = tree->get_node(leaf.prev);
                        pos = leaf.curSize-1;
                    }
                    else {
                        leaf = tree->get_node(tree->head);
                        pos = 0;
                    }
                }
                else{
                    pos--;
                }
                return *this;
            }

            value_t& operator*(){
                buffer b;
                tree->buffer_load_leaf(b, leaf);
                return *tree->get_value_leaf(pos, b);
            }

            bool operator==(const iterator &rhs)const{
                return (leaf.pos == rhs.leaf.pos&&pos == rhs.pos);
            }

            bool operator!=(const iterator &rhs)const{
                return !(*this == rhs);
            }

            bool operator==(const const_iterator &rhs)const{
                return (leaf.pos == rhs.leaf.pos&&pos == rhs.pos);
            }

            bool operator!=(const const_iterator &rhs)const{
                return !(*this == rhs);
            }
        };

        iterator begin(){
            return iterator(get_node(head), 0, this);
        }

        iterator end(){
            return iterator(node(invalid_off, invalid_off, invalid_off, true), 0, this);
        }

        const_iterator cbegin(){
            return const_iterator(get_node(head), 0, this);
        }

        const_iterator cend(){
            return const_iterator(node(invalid_off, invalid_off, invalid_off, true), 0, this);
        }

        inline bool empty() {
            return root == invalid_off;
        }

        int count(const Key &key, node &x){
            if(comp(key, x.mainKey))
                return 0;
            if(x.isLeaf){
                buffer b;
                buffer_load_leaf(b, x);
                size_t tmp = leaf_find_pos(b, key, x.curSize);
                if(tmp<x.curSize && compare(key, *get_key_leaf(tmp, b)))
                    return 1;
                else
                    return 0;
            }
            buffer b;
            buffer_load_node(b ,x);
            size_t tmp = node_find_pos(b, key, x.curSize);
            if(tmp>0 && !compare(key, *get_key_node(tmp, b))) tmp--;
            if(tmp == x.curSize) tmp--;
            node y = get_node(*get_son_node(tmp, b));
            return count(key, y);
        }

        int count(const Key &key) {
            if(empty())
                return 0;
            node x = get_node(root);
            return count(key, x);
        }//Returns the number of elements with key that compares equivalent to the specified argument, which is either 1 or 0

        void insert(const Key &key, const value_t &value) {
            bool is_change;
            if(empty()) {
                node x(finder._alloc(), invalid_off, invalid_off, true);
                buffer b;
                buffer_insert_leaf(b, x, key, value, is_change);
                head = rear = root = x.pos;
                save_node(x);
                buffer_save_leaf(b, x);
                save_info();
                return;
            }
            node x = get_node(root);
            node result = insert(x, key, value, is_change);
            if(result.pos != x.pos){
                buffer b;
                node new_root(finder._alloc(), invalid_off, invalid_off, false);
                root = new_root.pos;
                *get_key_node(0, b) = x.mainKey;
                *get_son_node(0, b) = x.pos;
                *get_key_node(1, b) = result.mainKey;
                *get_son_node(1, b) = result.pos;
                new_root.curSize = 2;
                save_node(new_root);
                buffer_save_node(b, new_root);
                save_info();
                return;
            }
            else{
                save_node(x);
            }
        }

        void erase(const Key &key) {
            node rt = get_node(root);
            erase(rt, key, rt);
        }

        value_t at(const Key &key, node &x){
            if(comp(key, x.mainKey)) {
                value_t value = value_t();
                return value;
            }
            if(x.isLeaf){
                buffer b;
                buffer_load_leaf(b, x);
                size_t tmp = leaf_find_pos(b, key, x.curSize);
                if(tmp<x.curSize && compare(key, *get_key_leaf(tmp, b)))
                    return *get_value_leaf(tmp, b);
                else {
                    value_t value = value_t();
                    return value;
                }
            }
            buffer b;
            buffer_load_node(b ,x);
            size_t tmp = node_find_pos(b, key, x.curSize);
            if(tmp>0 && !compare(key, *get_key_node(tmp, b))) tmp--;
            if(tmp == x.curSize) tmp--;
            node y = get_node(*get_son_node(tmp, b));
            return at(key, y);
        }

        value_t at(const Key &key) {
            if(empty()) throw "at";
            node x = get_node(root);
            return at(key, x);
        }
		
		const value_t at(const Key& key) const{
			if (empty()) throw "at";
			node x = get_node(root);
			return at(key, x);
		}

        void traverse(void *fun(Key *key, value_t *value)) {
            node x = get_node(head);
            buffer b;
            while(true){
                buffer_load_leaf(b, x);
                for(size_t i = 0; i<x.curSize;++i){
                    cout<<*get_key_leaf(i, b)<<' '<<*get_value_leaf(i, b)<<endl;
                }
                if(x.next == invalid_off) break;
                else x = get_node(x.next);
            }
        }

        void traverse_tree(node &x){
            buffer b;
            if(x.isLeaf){
                buffer_load_leaf(b, x);
                cout<<"Leaf\n"<<" having "<<x.curSize<< "datas\n";
                for(size_t i = 0; i<x.curSize; ++i){
                    cout<<"("<<*get_key_leaf(i, b)<<','<<*get_value_leaf(i, b)<<")\n";
                }
            }
            else{
                buffer_load_node(b, x);
                cout<<"Node\n"<<" having "<<x.curSize<<"sons\n";
                for(size_t i = 0; i<x.curSize; ++i){
                    cout<<*get_key_node(i, b)<<' ';
                }
                cout<<"\nsons\n";
                for(size_t i = 0; i<x.curSize; ++i){
                    cout<<*get_son_node(i, b)<<' ';
                }
                cout<<"\n\n";
                for(size_t i = 0; i<x.curSize; ++i){
                    node son = get_node(*get_son_node(i, b));
                    traverse_tree(son);
                }
                cout<<"\n\n";
            }
        }
        void traverse_tree(){
            //buffer btmp;
            //node test = get_node(53260);
            //buffer_load_node(btmp, test);
            //cout<<'\n'<<"last node's 4th son is"<<*get_key_node(3, btmp)<<endl;
            node rt = get_node(root);
            traverse_tree(rt);
        }
        bool modify(const Key &key, const value_t &value){
            bool is_change;
            if(empty()) {
                node x(finder._alloc(), invalid_off, invalid_off, true);
                buffer b;
                buffer_insert_leaf(b, x, key, value, is_change);
                head = rear = root = x.pos;
                save_node(x);
                buffer_save_leaf(b, x);
                save_info();
                return is_change;
            }
            node x = get_node(root);
            node result = insert(x, key, value, is_change);
            if(result.pos != x.pos){
                buffer b;
                node new_root(finder._alloc(), invalid_off, invalid_off, false);
                root = new_root.pos;
                *get_key_node(0, b) = x.mainKey;
                *get_son_node(0, b) = x.pos;
                *get_key_node(1, b) = result.mainKey;
                *get_son_node(1, b) = result.pos;
                new_root.curSize = 2;
                save_node(new_root);
                buffer_save_node(b, new_root);
                save_info();
                return is_change;
            }
            else{
                save_node(x);
                return is_change;
            }
        }
    };
}

#endif //BPTREE_BPTREE_HPP

