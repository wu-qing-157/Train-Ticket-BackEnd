//
// Created by jiayuqi on 19-4-28.
//
#include <functional>
#include <cstddef>
//#include "utility.hpp"
#include "exceptions.hpp"

using namespace std;
#ifndef BPTREE_BPTREE_HPP
#define BPTREE_BPTREE_HPP

const int K = 1000;
namespace sjtu {
    template<class Key, class value_t, int N, int K>
    class bptree {
        struct node {
            off_t POS;
            int curSize;
            bool isLeaf;
            node *prev;
            node *next;
            Key mainKey[K + 1];
            node *son[K + 1];
            value_t data[N + 1];

            node(bool is_leaf) {
                curSize = 0;
                isLeaf = is_leaf;
                prev = next = nullptr;
                for (int i = 0; i < K; ++i) {
                    mainKey[i] = Key();
                    son[i] = nullptr;
                }
                for (int i = 0; i < N; ++i) {
                    data[i] = value_t();
                }
            }
        };

    private:
        node *root, *head, *rear;

        void insertLeaf(Key key, value_t value, node *nw, int pos){
            for(int i = nw->curSize; i>pos; --i){
                nw->data[i] = nw->data[i-1];
                nw->mainKey[i] = nw->mainKey[i-1];
            }
            nw->mainKey[pos] = key;
            nw->data[pos] = value;
            ++nw->curSize;
        }

        void insertNode(Key key, node *Son, node *nw, int pos){
            for(int i = nw->curSize; i>pos; --i){
                nw->mainKey[i] = nw->mainKey[i-1];
            }
            for(int i = nw->curSize+1; i>pos+1;--i){
                nw->son[i] = nw->son[i-1];
            }
            nw->mainKey[pos] = key;
            nw->son[pos+1] = Son;
            ++nw->curSize;
        }

        void eraseLeaf(node *nw, int pos){
            if(pos<nw->curSize-1)
                for(int i = pos; i<nw->curSize-1; ++i){
                    nw->data[i] = nw->data[i+1];
                    nw->mainKey[i] = nw->mainKey[i+1];
                }
            nw->son[nw->curSize-1] = value_t();
            nw->mainKey[nw->curSize-1] = Key();
            --nw->curSize;
        }

        void eraseNode(node *nw, int pos) {
            if (pos < nw->curSize - 1) {
                for (int i = pos; i < nw->curSize - 1; ++i) {
                    nw->mainKey[i] = nw->mainKey[i + 1];
                }
                for (int i = pos + 1; i < nw->curSize; ++i) {
                nw->son[i] = nw->son[i + 1];
                }
            }
            nw->mainKey[nw->curSize - 1] = Key();
            nw->son[nw->curSize] = nullptr;
            --nw->curSize;
        }


        bool insert(node *nw, const Key &key, value_t value, node **splitNode, Key *splitKey) {
            if(nw->isLeaf == true){
                int pos = findPos(nw, key);
                if(nw->mainKey[pos] == key) {
                    nw->data[pos] = value;
                    return false;
                }
                insertLeaf(key, value, nw, pos);
                if(nw->curSize>=N){
                    *splitNode = leaf_split(nw, splitKey);
                }
                return true;
            }
            else{
                Key tmpKey = Key();node *tmpNode = nullptr;
                int pos = findPos(tmpNode, tmpKey);
                bool result = insert(nw->son[pos], key, value, &tmpNode, &tmpKey);
                if(tmpNode){
                    insertNode(key, tmpNode, nw, pos);
                    if(nw->curSize>=K){
                        *splitNode = node_split(nw, splitKey);
                    }
                }
                return result;
            }
        }

        bool erase(node *nw, Key key, node * parent, int pos) {
            if(nw->isLeaf == true){
                int pos = findPos(nw, key);
                if(pos<nw->curSize) eraseLeaf(nw, pos);
                else return false;
                if(nw->curSize>=(N+1)/2||nw == root) return true;
                node *Next = nullptr, *Prev = nullptr;
                if(pos+1<=parent->curSize) Next = parent->son[pos+1];
                if(pos-1>=0) Prev = parent->son[pos-1];
                if(Next == nullptr) balanceLeftLeaf(nw, Prev, parent, pos);
                else balanceRightLeaf(nw, Next, parent, pos);
            }
            else{
                int pos = findPos(nw, key);
                if(pos<nw->curSize&&nw->mainKey[pos]==key) pos++;
                if(nw == root&&nw->curSize == 0){
                    node *tmp = nw->son[0];
                    delete root;
                    root = tmp;
                    return true;
                }
                erase(nw->son[pos], key, nw, pos);
                if(nw->curSize>=(N+1)/2||nw == root) return true;
                node *Next = nullptr, *Prev = nullptr;
                if(pos+1<=parent->curSize) Next = parent->son[pos+1];
                if(pos-1>=0) Prev = parent->son[pos-1];
                if(Next == nullptr) balanceLeftNode(nw, Prev, parent, pos);
                else balanceRightNode(nw, Next, parent, pos);
            }
        }

        void balanceRightLeaf(node *nw, node *brother, node *parent, int pos){
            if(brother->curSize>=(N+1)/2+1){
                nw->mainKey[nw->curSize] = brother->mainKey[0];
                nw->data[nw->curSize] = brother->data[0];
                nw->curSize++;
                eraseLeaf(brother, 0);
                parent->mainKey[pos] = brother->mainKey[0];
            }
            else{
                leaf_merge(nw, brother);
                eraseNode(parent, pos);
            }
        }

        void balanceLeftLeaf(node *nw, node *brother, node *parent, int pos){
            if(brother->curSize>=(N+1)/2+1){
                int tmp = brother->curSize-1;
                insertLeaf(brother->mainKey[tmp], brother->data[tmp], nw, 0);
                brother->mainKey[tmp] = Key();
                brother->data[tmp] = value_t(tmp);
                brother->curSize--;
            }
            else{
                leaf_merge(brother, nw);
                eraseNode(parent, pos);
            }
        }

        void balanceRightNode(node *nw, node *brother, node *parent, int pos){
            if(brother->curSize>=(K+1)/2+1){
                nw->mainKey[nw->curSize] = brother->mainKey[0];
                nw->son[nw->curSize+1] = brother->son[0];
                nw->curSize++;
                eraseNode(brother, 0);
                parent->mainKey[pos] = brother->mainKey[0];
            }
            else{
                nw->mainKey[nw->curSize] = parent->mainKey[pos];
                for(int i = 0; i<brother->curSize; ++i){
                    nw->mainKey[i+nw->curSize+1] = brother->mainKey[i];
                    nw->son[i+nw->curSize+1] = brother->son[i];
                }
                nw->curSize += brother->curSize+1;
                nw->son[nw->curSize] = brother->son[brother->curSize];
                delete brother;
                eraseNode(parent, pos);
            }
        }

        void balanceLeftNode(node *nw, node *brother, node *parent, int pos){
            if(brother->curSize>=(K+1)/2+1){
                int tmp = brother->curSize-1;
                insertNode(brother->mainKey(tmp), brother->son[tmp+1], nw, 0);
                brother->mainKey[tmp] = Key();
                brother->son[tmp+1] = nullptr;
                brother->curSize--;
            }
            else{
                brother->mainKey[brother->curSize] = parent->mainKey[pos];
                for(int i = 0; i<nw->curSize; ++i){
                    brother->mainKey[i+brother->curSize+1] = nw->mainKey[i];
                    brother->son[i+brother->curSize+1] = nw->son[i];
                }
                brother->curSize += nw->curSize+1;
                brother->son[brother->curSize] = nw->son[nw->curSize];
                delete nw;
                eraseNode(parent, pos);
            }
        }

        node *leaf_split(node *nw, Key *newKey) {
            int mid = (nw->curSize)/2;
            node *nxt = new node(true);
            nxt->curSize = nw->curSize-mid;
            nxt->next = nw->next;
            if(nxt->next = nullptr) rear = nxt;
            else nw->next->prev = nxt;
            nxt->prev = nw;
            nw->next = nxt;
            for(int i = 0; i<nw->curSize-mid; ++i){
                nxt->data[i] = nw->data[i+mid];
                nw->data[i+mid] = value_t();
                nxt->mainKey[i] = nw->mainKey[i+mid];
                nw->mainKey[i+mid] = Key();
            }
            nw->curSize = mid;
            *newKey = nxt->mainKey[0];
            return nxt;
        }

        node *node_split(node *nw, Key *newKey) {
            int mid = (nw->curSize)/2;
            node *nxt = new node(false);
            nxt->curSize = nw->curSize-mid-1;
            for(int i = 0; i<nw->curSize-mid-1; ++i){
                nxt->son[i] = nw->son[i+mid+1];
                nw->son[i+mid+1] = nullptr;
                nxt->mainKey[i] = nw->mainKey[i+mid+1];
                nw->mainKey[i+mid+1] = Key();
            }
            nxt->son[nw->curSize-mid-1] = nw->son[nw->curSize];
            nw->son[nw->curSize] = nullptr;
            *newKey = nw->mainKey[mid];
            nw->mainKey[mid] = Key();
            nw->curSize = mid;

            return nxt;
        }//内节点

        void leaf_merge(node *nw, node *nxt) {
            nw->next = nxt->next;
            if(nw->next == nullptr) rear = nw;
            else nxt->next->prev = nw;
            for(int i = 0;i<nxt->curSize; ++i){
                nw->data[i+nw->curSize+1] = nxt->data[i];
                nxt->data[i] = value_t();
                nw->mainKey[i+nw->curSize+1] = nxt->mainKey[i];
                nxt->mainKey[i] = Key();
            }
            nw->curSize+=nxt->curSize;
            delete nxt;
        }//merge nxt to nw

        void node_merge(node *nw, node *nxt) {
            for(int i = 0;i<nxt->curSize; ++i){
                nw->son[i+nw->curSize+1] = nxt->son[i];
                nxt->son[i] = nullptr;
                nw->mainKey[i+nw->curSize+1] = nxt->mainKey[i];
                nxt->mainKey[i] = Key();
            }
            nw->curSize+=nxt->curSize;
            delete nxt;
        }

        int findPos(node *nw, Key key) {
            int l = 0, r = nw->curSize, mid;
            while(l<r){
                mid = (l + r)/2;
                if(nw->mainKey[mid]<key) l = mid;
                else r = mid;
                if(l == r && l == mid) break;
            }
            return r;
        }

    public:
        bptree() {
            root = nullptr;
        }

        inline bool empty() {
            return root->curSize==0;
        }

        int count(Key key) {
            node *tmp = root;int Pos;
            while(!tmp->isLeaf){
                int pos = findPos(tmp, key);
                Pos = pos;
                if(pos < tmp->curSize&&tmp->mainKey[pos] == key) ++pos;
                tmp = tmp->son[pos];
            }
            if(Pos == tmp->curSize || tmp->key[Pos] != key) return 0;
            return  1;
        }//Returns the number of elements with key that compares equivalent to the specified argument, which is either 1 or 0

        void insert(const Key &key, const value_t &value) {
            node *newSon; Key newKey;
            if(root == nullptr){
                root = head = rear = new node(true);
            }
            insert(root, key, value, &newSon, &newKey);
            if(newSon){
                node *newRoot = new node(false);
                newRoot->mainKey[0] = newKey;
                newRoot->son[0] = root;
                newRoot->son[1] = newSon;
                newRoot->num = 1;
                root = newRoot;
            }
        }

        void erase(const Key &key) {
            erase(root, key, nullptr, -1);
        }

		value_t at(const Key& key) const {
			node* tmp = root; int Pos;
			while (!tmp->isLeaf) {
				int pos = findPos(tmp, key);
				Pos = pos;
				if (pos < tmp->curSize && tmp->mainKey[pos] == key) ++pos;
				tmp = tmp->son[pos];
			}
			if (Pos == tmp->curSize || tmp->key[Pos] != key) throw index_out_of_bound();
			return tmp->data[Pos];
		} // added by yy.

        value_t	& at(const Key &key) {
            node *tmp = root;int Pos;
            while(!tmp->isLeaf){
                int pos = findPos(tmp, key);
                Pos = pos;
                if(pos < tmp->curSize&&tmp->mainKey[pos] == key) ++pos;
                tmp = tmp->son[pos];
            }
            if(Pos == tmp->curSize || tmp->key[Pos] != key) throw index_out_of_bound();
            return tmp->data[Pos];
        }

        //void traverse() {}
        bool modify(const Key &key, const value_t &value){
            node *newSon; Key newKey;
            if(root == nullptr){
                root = head = rear = new node(true);
            }
            bool result = insert(root, key, value, &newSon, &newKey);
            if(newSon){
                node *newRoot = new node(false);
                newRoot->mainKey[0] = newKey;
                newRoot->son[0] = root;
                newRoot->son[1] = newSon;
                newRoot->num = 1;
                root = newRoot;
            }
            return result;
        }
    };
}

#endif //BPTREE_BPTREE_HPP
