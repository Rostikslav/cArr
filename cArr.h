#pragma once
#include <ostream>

class cArrException : public std::exception
{
public:
	cArrException(const char *message) : message(message) {}
	const char *what() const throw()
	{
		return message;
	}

private:
	const char *message;
};

template <class T>
class cArr
{
private:
	int Capacity;
	int Size;
	T **arr;

	void init(size_t from);
	int partition(int low, int high);
	int partition(int low, int high, bool (*f)(T, T));

public:
	cArr();
	cArr(const cArr &obj);
	cArr(const int amount, T *value);
	cArr(const int amount, T value);
	cArr(const int amount);
	cArr(const std::initializer_list<T> &list);
	~cArr();

	void sort(int l, int r);
	void sort();
	void sort(int l, int r, bool (*f)(T, T));
	void sort(bool (*f)(T, T));
	void expand();
	void push(const T &obj);
	void map(void (*f)(T &e));

	template <class L>
	void push(const std::initializer_list<L> list);

	void pop();
	void pop(const int &n);
	void clear();
	void fill(T n);
	void erase(const int idx, int len = 0);

	inline const int size() const { return this->Size; }
	inline const int length() const { return this->Size; }
	inline const int capacity() const { return this->Capacity; }
	inline const bool isEmpty() const { return !this->Size; }

	bool has(T obj);
	bool has(T *obj);
	template <class L>
	bool has(const std::initializer_list<L> list);

	cArr<T> &operator=(const cArr<T> &rhs);

	template <class L>
	cArr<T> operator=(const std::initializer_list<L> &list);

	T &operator[](const int i);
	T operator[](const int i) const;
	bool operator==(const cArr &rhs) const;
	bool operator!=(const cArr &rhs) const;

	template <class U>
	friend std::ostream &operator<<(std::ostream &os, const cArr<U> &rhs);
	template <class U>
	friend std::ostream &operator<<(std::ostream &os, const cArr<cArr<U>> &rhs);
};

template <class T>
void cArr<T>::init(size_t from)
{
	for (from; from < this->Capacity; from++)
	{
		this->arr[from] = nullptr;
	}
}

template <class T>
inline int cArr<T>::partition(int low, int high)
{
	using std::swap;
	T pivot = *arr[high];
	int i = low;
	for (int j = low; j <= high - 1; j++)
	{
		if (*arr[j] < pivot)
		{
			swap(arr[i], arr[j]);
			i++;
		}
	}
	swap(arr[i], arr[high]);
	return i;
}

template <class T>
inline int cArr<T>::partition(int low, int high, bool (*f)(T, T))
{
	using std::swap;
	T pivot = *arr[high];
	int i = low;
	for (int j = low; j <= high - 1; j++)
	{
		if (f(*arr[j], pivot))
		{
			swap(arr[i], arr[j]);
			i++;
		}
	}
	swap(arr[i], arr[high]);
	return i;
}

template <class T>
cArr<T>::cArr()
	: Size(0),
	  Capacity(2),
	  arr(new T *[this->Capacity])
{
	this->init(0);
}

template <class T>
cArr<T>::~cArr()
{
	for (size_t i = 0; i < this->Size; i++)
	{
		delete this->arr[i];
	}
	delete[] this->arr;
}

template <class T>
cArr<T>::cArr(const cArr &obj)
	: Size(obj.Size),
	  Capacity(obj.Capacity),
	  arr(new T *[this->Capacity])
{
	for (size_t i = 0; i < this->Size; i++)
	{
		this->arr[i] = new T(*obj.arr[i]);
	}
}

template <class T>
cArr<T>::cArr(const int amount, T *value)
	: Size(amount),
	  Capacity(amount + 2),
	  arr(new T *[this->Capacity])
{
	this->init(this->Size);
	for (size_t i = 0; i < this->Size; i++)
	{
		this->arr[i] = new T(*value);
	}
}

template <class T>
cArr<T>::cArr(const int amount, T value)
	: Size(amount),
	  Capacity(amount + 2),
	  arr(new T *[this->Capacity])
{
	this->init(this->Size);
	for (size_t i = 0; i < this->Size; i++)
	{
		this->arr[i] = new T(value);
	}
}

template <class T>
inline cArr<T>::cArr(const int amount)
	: Size(amount),
	  Capacity(amount + 2),
	  arr(new T *[this->Capacity])
{
	this->init(amount);
	for (size_t i = 0; i < this->Size; i++)
	{
		this->arr[i] = new T(T());
	}
}

template <class T>
cArr<T>::cArr(const std::initializer_list<T> &list)
	: Size(0),
	  Capacity(2),
	  arr(new T *[this->Capacity])
{
	this->init(this->Size);
	for (auto e : list)
		this->push(e);
}

template <class T>
void cArr<T>::sort(int l, int r)
{
	if (l < r)
	{
		int pivot = this->partition(l, r);

		this->sort(l, pivot - 1);
		this->sort(pivot + 1, r);
	}
}

template <class T>
void cArr<T>::sort()
{
	int pivot = this->partition(0, this->Size - 1);

	this->sort(0, pivot - 1);
	this->sort(pivot + 1, this->Size - 1);
}

template <class T>
void cArr<T>::sort(int l, int r, bool (*f)(T, T))
{
	if (l < r)
	{
		int pivot = this->partition(l, r, f);

		this->sort(l, pivot - 1, f);
		this->sort(pivot + 1, r, f);
	}
}

template <class T>
void cArr<T>::sort(bool (*f)(T, T))
{
	int pivot = this->partition(0, this->Size - 1, f);

	this->sort(0, pivot - 1, f);
	this->sort(pivot + 1, this->Size - 1, f);
}

template <class T>
void cArr<T>::expand()
{
	this->Capacity *= 2;

	T **tempArr = new T *[this->Capacity];

	for (size_t i = 0; i < this->Size; i++)
	{
		tempArr[i] = this->arr[i];
	}

	delete[] this->arr;

	this->arr = tempArr;
	this->init(this->Size);
}

template <class T>
void cArr<T>::push(const T &obj)
{
	if (this->Size >= this->Capacity)
		expand();

	this->arr[this->Size] = new T(obj);
	++Size;
}

template <class T>
void cArr<T>::map(void (*f)(T &e))
{
	for (size_t i = 0; i < this->Size; i++)
	{
		f(*(arr[i]));
	}
}

template <class T>
template <class L>
void cArr<T>::push(const std::initializer_list<L> list)
{
	if (!list.size())
	{
		this->push(*new T);
	}
	else
	{
		for (auto &e : list)
			this->push(e);
	}
}

template <class T>
void cArr<T>::pop()
{
	if (!this->Size)
		throw cArrException("cArr: cannot pop() an empty array");

	--(this->Size);
	this->init(this->Size);
}

template <class T>
void cArr<T>::pop(const int &n)
{
	if (n > this->Size)
		throw cArrException("cArr: pop() out of range");
	this->Size -= n;
	this->init(this->Size);
}

template <class T>
void cArr<T>::clear()
{
	for (size_t i = 0; i < this->Size; i++)
	{
		delete this->arr[i];
	}
	this->Size = 0;
	this->Capacity = 2;
	delete[] this->arr;
	this->arr = new T *[this->Capacity];
	this->init(0);
}

template <class T>
void cArr<T>::fill(T n)
{
	for (size_t i = 0; i < this->Size; i++)
		this->arr[i] = new T(n);
}

template <class T>
void cArr<T>::erase(const int idx, int len)
{
	if (idx < 0 || idx > this->Size)
		throw cArrException("cArr::erase: invalid index");

	cArr<T> tmpArr;
	for (int i = 0; i < idx; i++)
		tmpArr.push(*this->arr[i]);

	if (len)
		for (int i = idx + len; i < this->Size; i++)
			tmpArr.push(*this->arr[i]);

	*this = tmpArr;
}

template <class T>
inline bool cArr<T>::has(T obj)
{
	for (size_t i = 0; i < this->Size; i++)
	{
		if (*arr[i] == obj)
			return true;
	}
	return false;
}

template <class T>
inline bool cArr<T>::has(T *obj)
{
	for (size_t i = 0; i < this->Size; i++)
	{
		if (*arr[i] == *obj)
			return true;
	}
	return false;
}

template <class T>
template <class L>
bool cArr<T>::has(const std::initializer_list<L> list)
{
	cArr<L> obj = list;
	for (size_t i = 0; i < this->Size; i++)
	{
		if (*arr[i] == obj)
			return true;
	}
	return false;
}

template <class T>
cArr<T> &cArr<T>::operator=(const cArr<T> &rhs)
{
	for (size_t i = 0; i < this->Size; i++)
		delete this->arr[i];

	delete[] this->arr;

	this->Size = rhs.Size;
	this->Capacity = rhs.Capacity;
	this->arr = new T *[this->Capacity];

	for (size_t i = 0; i < this->Size; i++)
	{
		this->arr[i] = new T(rhs[i]);
	}

	return *this;
}

template <class T>
template <class L>
cArr<T> cArr<T>::operator=(const std::initializer_list<L> &list)
{
	for (auto e : list)
	{
		this->push(e);
	}
	return *this;
}

template <class T>
T &cArr<T>::operator[](const int i)
{
	if (i < -this->Size || i >= this->Size)
		throw cArrException("cArr: index out of range");

	if (i >= -this->Size && i < 0)
		return *this->arr[this->Size + i];

	return *this->arr[i];
}

template <class T>
T cArr<T>::operator[](const int i) const
{
	if (i < -this->Size || i >= this->Size)
		throw cArrException("cArr: index out of range");

	if (i >= -this->Size && i < 0)
		return *this->arr[this->Size + i];

	return *this->arr[i];
}

template <class T>
bool cArr<T>::operator==(const cArr &rhs) const
{
	if (this->Size != rhs.Size)
		return 0;

	for (size_t i = 0; i < this->Size; i++)
	{
		if (*(this->arr[i]) != rhs[i])
			return 0;
	}

	return 1;
}

template <class T>
bool cArr<T>::operator!=(const cArr &rhs) const
{
	return !(*this == rhs);
}

template <class U>
std::ostream &operator<<(std::ostream &os, const cArr<U> &rhs)
{
	for (int i = 0; i < rhs.size(); i++)
	{
		os << rhs[i] << ' ';
	}
	return os;
}

template <class U>
std::ostream &operator<<(std::ostream &os, const cArr<cArr<U>> &rhs)
{
	for (int i = 0; i < rhs.size(); i++)
	{
		os << rhs[i] << (i == rhs.Size - 1 ? "" : "\n");
	}
	return os;
}
