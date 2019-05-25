#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>

namespace sjtu {
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template<typename T>
class vector {
private:
     T* data;
     int curLen, maxSize;
     void doubleSpace();

public:
	/**
	 * TODO
	 * a type for actions of the elements of a vector, and you should write
	 *   a class named const_iterator with same interfaces.
	 */
	/**
	 * you can see RandomAccessIterator at CppReference for help.
	 */
	class const_iterator;
	class iterator {
	private:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */

        vector* vec;
        int cur;

	public:
		/**
		 * return a new iterator which pointer n-next elements
		 *   even if there are not enough elements, just return the answer.
		 * as well as operator-
		 */

		iterator(vector *_vec, int _cur){
			vec = _vec;
			cur = _cur;
		}

		iterator(const iterator &o) = default;

		iterator operator+(const int &n) const {

            iterator tem(*this);
            tem.cur += n;
            return tem;
			//TODO
		}
		iterator operator-(const int &n) const {
			if (cur < n) throw invalid_iterator();//maybe it's not what's supposed to be
			else {
                iterator tem(*this);
                tem.cur -= n;
                return tem;
			}

			//TODO
		}
		// return th distance between two iterator,
		// if these two iterators points to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const {
            if (vec == rhs.vec) throw invalid_iterator();
            return (cur - rhs.cur);
			//TODO
		}
		iterator operator+=(const int &n) {
			cur += n;
			return *this;
			//TODO
		}
		iterator operator-=(const int &n) {
			cur -= n;
			return *this;
			//TODO
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
			iterator tmp(*this);
			cur++;
			return tmp;
		}
		/**
		 * TODO ++iter
		 */
		iterator& operator++() {
			cur ++;
			return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
			iterator tmp(*this);
			cur --;
			return tmp;
		}
		/**
		 * TODO --iter
		 */
		iterator& operator--() {
			cur --;
			return *this;
		}
		/**
		 * TODO *it
		 */
		T& operator*() const{
			return *(vec->data + cur);
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		bool operator==(const iterator &rhs) const {
			if (cur != rhs.cur) return false;
			if (vec != rhs.vec) return false;
			return true;
		}
		bool operator==(const const_iterator &rhs) const {
			if (cur != rhs.cur) return false;
			if (vec != rhs.vec) return false;
			return true;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
			return !(*this == rhs);
		}
		bool operator!=(const const_iterator &rhs) const {
			return !(*this == rhs);
		}
	};
	/**
	 * TODO
	 * has same function as iterator, just for a const object.
	 */
	class const_iterator {
		private:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */

        const vector* vec;
        int cur;

	public:
		/**
		 * return a new iterator which pointer n-next elements
		 *   even if there are not enough elements, just return the answer.
		 * as well as operator-
		 */

		const_iterator(const vector *_vec, int _cur) :vec(_vec) {
			cur = _cur;
		}

		const_iterator(const const_iterator &o) = default;

		const_iterator operator+(const int &n) const {

            iterator tem(*this);
            tem.cur += n;
            return *tem;
			//TODO
		}
		const_iterator operator-(const int &n) const {
			if (cur < n) throw invalid_iterator();
			else {
                iterator tem(*this);
                tem.cur -= n;
                return *tem;
			}

			//TODO
		}
		// return th distance between two iterator,
		// if these two iterators points to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const {
            if (vec == rhs.vec) throw invalid_iterator();
            return (cur - rhs.cur);
			//TODO
		}
		const_iterator operator+=(const int &n) {
			cur += n;
			return *this;
			//TODO
		}
		const_iterator operator-=(const int &n) {
			cur -= n;
			return *this;
			//TODO
		}
		/**
		 * TODO iter++
		 */
		const_iterator operator++(int) {
			iterator tmp(*this);
			cur++;
			return tmp;
		}
		/**
		 * TODO ++iter
		 */
		const_iterator& operator++() {
			cur ++;
			return *this;
		}
		/**
		 * TODO iter--
		 */
		const_iterator operator--(int) {
			iterator tmp(*this);
			cur --;
			return tmp;
		}
		/**
		 * TODO --iter
		 */
		const_iterator& operator--() {
			cur --;
			return *this;
		}
		/**
		 * TODO *it
		 */
		T& operator*() const{
			return *(vec->data + cur);
		}

		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		bool operator==(const iterator &rhs) const {
			if (cur != rhs.cur) return false;
			if (vec != rhs.vec) return false;
			return true;
		}
		bool operator==(const const_iterator &rhs) const {
			if (cur != rhs.cur) return false;
			if (vec != rhs.vec) return false;
			return true;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
			return !(*this == rhs);
		}
		bool operator!=(const const_iterator &rhs) const {
			return !(*this == rhs);
		}
	};
	/**
	 * TODO Constructs
	 * Atleast three: default constructor, copy constructor and a constructor for std::vector
	 */


	vector() {
		data = (T*)malloc(sizeof(T));
		maxSize = 1;
		curLen = 0;
	}
	vector(const vector &o) {
		curLen = o.curLen;
		maxSize = o.maxSize;
		data = (T*) malloc(sizeof(T) * maxSize); //to enable 'vector a;'
		//memcpy (data, o.data, sizeof(T) * maxSize); //A typical mistake, but I didn;t quite understand
		for (int i = 0; i < curLen; ++i){
			data[i] = o.data[i];
		}
	}
	/**
	 * TODO Destructor
	 */
	~vector() {
		for (size_t i = 0; i < curLen; ++i) {
			(data + i) -> ~T();
		}
		free(data);
		curLen = 0;
	}
	/**
	 * TODO Assignment operator
	 */
	vector &operator=(const vector &o) {
		if (this == &o) return *this;
		for (size_t i = 0; i < curLen; ++i) {
			(data + i) -> ~T();
		}
		free(data);
		maxSize = o.maxSize;
		curLen = o.curLen;
		data = (T*)malloc(sizeof(T) * maxSize);
		for (int i = 0; i < curLen; ++i) {
            new (data + i) T(o.data[i]);
		}
		return *this;
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 */
	T & at(const size_t &pos) {
		if (pos < 0 || pos >= curLen) throw index_out_of_bound();
		return data[pos];
	}
	const T & at(const size_t &pos) const {
		if (pos < 0 || pos >= curLen) throw index_out_of_bound();
		return data[pos];
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 * !!! Pay attentions
	 *   In STL this operator does not check the boundary but I want you to do.
	 */
	T & operator[](const size_t &pos) {
		if (pos < 0 || pos >= curLen) throw index_out_of_bound();
		return data[pos];
	}
	const T & operator[](const size_t &pos) const {
		if (pos < 0 || pos >= curLen) throw index_out_of_bound();
		return data[pos];
	}
	/**
	 * access the first element.
	 * throw container_is_empty if size == 0
	 */
	const T & front() const {
		if (curLen == 0) throw container_is_empty();
		return data[0];
	}
	/**
	 * access the last element.
	 * throw container_is_empty if size == 0
	 */
	const T & back() const {
		if (curLen == 0) throw container_is_empty();
		return data[curLen - 1];
	}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() {
		return iterator(this, 0);
	}
	const_iterator cbegin() const {
		return const_iterator(this, 0);
	}
	/**
	 * returns an iterator to the end.
	 */
	iterator end() {
		return iterator(this, curLen);
	}
	const_iterator cend() const {
		return const_iterator(this, curLen);
	}
	/**
	 * checks whether the container is empty
	 */
	bool empty() const {
		return (curLen == 0);
	}
	/**
	 * returns the number of elements
	 */
	size_t size() const {
		return curLen;
	}
	/**
	 * returns the number of elements that can be held in currently allocated storage.
	 */
	size_t capacity() const {
		return maxSize;
	}
	/**
	 * clears the contents
	 */
	void clear() {
		for (size_t i = 0; i < curLen; ++i) {
			(data + i) -> ~T();
		}
		free(data);
		curLen = 0;
		maxSize = 1;
		data = (T*)malloc(sizeof(T));
	}
	/**
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value.
	 */

	//void push_back(const T &value);

	iterator insert(iterator pos, const T &value) {
		if (pos == end()) {
			push_back(value);
			return (end() - 1);
		}

		else {
			curLen++;
			if (curLen > maxSize) doubleSpace();
			new (data + curLen - 1) T(data[curLen - 2]);
			iterator i = end() - 2;
			for (; i != pos; i--) {
				*i = *(i - 1);
			}
			*i = value;
			return pos;
		}
	}
	/**
	 * inserts value at index ind.
	 * after inserting, this->at(ind) == value is true
	 * returns an iterator pointing to the inserted value.
	 * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
	 */
	iterator insert(const size_t &ind, const T &value) {
		if (ind > curLen) throw index_out_of_bound();
		if (ind == curLen) {
			push_back(value);
			return (begin() + curLen - 1);
		}
		else {
			curLen++;
			new (data + curLen - 1) T(data[curLen - 2]);
			if (curLen > maxSize) doubleSpace();
			size_t i;
			for (i = curLen - 2; i != ind; --i) {
				data[i] = data[i - 1];
			}
			data[i] = value;
			return (begin() + i);
		}
	}
	/**
	 * removes the element at pos.
	 * return an iterator pointing to the following element.
	 * If the iterator pos refers the last element, the end() iterator is returned.
	 */
	iterator erase(iterator pos) {
		for (iterator i = pos; i != end() - 1; ++i) {
			*i = *(i + 1);
		}
		(data + curLen - 1) -> ~T();
		curLen--;
		return pos;
	}
	/**
	 * removes the element with index ind.
	 * return an iterator pointing to the following element.
	 * throw index_out_of_bound if ind >= size
	 */
	iterator erase(const size_t &ind) {
		if (ind >= curLen) throw index_out_of_bound();
		for (size_t i = ind; i != curLen - 1; ++i){
			data[i] = data[i + 1];
		}
		(data + curLen - 1) -> ~T();
		curLen--;
		return begin() + ind;
	}
	/**
	 * adds an element to the end.
	 */
	void push_back(const T &value) {
		curLen ++;
		if (curLen > maxSize) doubleSpace();
		new (data + curLen - 1) T(value);
	}
	/**
	 * remove the last element from the end.
	 * throw container_is_empty if size() == 0
	 */
	void pop_back() {
		if (!maxSize) throw container_is_empty();
		(data + curLen - 1) -> ~T();
		curLen --;
	}
};

    template <class T>
    void vector<T>::doubleSpace(){

        T *tmp = data;
        maxSize *= 2;
        data = (T*) malloc(sizeof(T) * maxSize);
        memcpy(data, tmp, sizeof(T) * (curLen - 1));
        
        //data = (T*)memcpy(malloc(sizeof(T) * maxSize), data, sizeof(T) * curLen);
        free(tmp);
    }
}

#endif
