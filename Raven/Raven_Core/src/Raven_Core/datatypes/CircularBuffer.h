#pragma once

template <class T>
class CircularBuffer {
public:
	CircularBuffer(std::size_t size) {
		_buff = new T[size];
		_end = _buff + size;
		_first = _last = _buff;
		_size = 0;
	}
	~CircularBuffer() {
		delete[] _buff;
		_buff = _last = _first = _end = nullptr;
	}
	// Modify data
	void push_back(T t) {
		if (full()) {
			if (empty()) return;
			*_last = t;
			increment(_last);
			_first = _last;
		}
		else {
			*_last = t;
			increment(_last);
			_size++;
		}
	}
	void push_front(T t) {
		if (full()) {
			if (empty()) return;
			decrement(_first);
			*_first = t;
			_last = _first;
		}
		else {
			decrement(_first);
			*_first = t;
			_size++;
		}
	}
	void pop_back() {
		if (empty()) return;
		decrement(_last);
		_size--;
	}
	void pop_front() {
		if (empty()) return;
		increment(_first);
		_size--;
	}
	// Access data
	T& operator[] (std::size_t index) const {
		return *((_first + index) >= _end
			? _buff + (index - (_end - _first))
			: _first + index);
	}
	// Puts the first n values of the circular buffer to an array
	void put_to_array(T* arr, std::size_t n) {
		ASSERT(n <= _size, "Tried to read more data than exists");
		std::size_t alreadyCopiedFromFirst = _end - _first;
		alreadyCopiedFromFirst = alreadyCopiedFromFirst > n ? n : alreadyCopiedFromFirst;
		std::size_t i = 0;
		for (; i < alreadyCopiedFromFirst; i++) {
			arr[i] = _first[i];
		}
		for (; i < n; i++) {
			arr[i] = _buff[i - alreadyCopiedFromFirst];
		}
	}
	// Get states
	bool full() const {
		return _size == capacity();
	}
	bool empty() const {
		return _size == 0;
	}
	std::size_t capacity() const {
		return _end - _buff;
	}
	std::size_t size() const {
		return _size;
	}
private:
	T* _buff;
	T* _end;
	T* _first;
	T* _last;

	std::size_t _size;
private:
	void increment(T*& p) {
		if (++p == _end)
			p = _buff;
	}
	void decrement(T*& p) {
		if (p == _buff)
			p = _end;
		--p;
	}
};