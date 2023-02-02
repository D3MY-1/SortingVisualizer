#pragma once3
#include <vector>
#include <iostream>

class IntElem
{
private:
	typedef void (*tFunc)(int);
public:
	IntElem(int a) :value(a) {};
	IntElem(const IntElem& a) : value(a.value) {};

	IntElem() = default;

	void setIndex(int idx)
	{
		index = idx;
	}

	IntElem& operator=(int rhs)
	{
		if (track && comp != nullptr)
		{
			comp(rhs);
			comp(value);
		}
		value = rhs;
		return *this;
	}
	IntElem& operator=(const IntElem& other)
	{
		if (track && comp != nullptr && index != -1)
		{
			//comp(other.value);
			comp(index);
		}
			
		value = other.value;
		return *this;
	}
	IntElem& operator=(IntElem&& other) noexcept
	{
		if (track && comp != nullptr && index != -1)
		{
			comp(index);
		}
		value = std::move(other.value);
		return *this;
	}
	bool operator<(const IntElem& other)
	{
		if (track && comp != nullptr && index != -1)
		{
			comp(index);
		}
		return array->at(value) < array->at(other.value);
	}
	bool operator>(const IntElem& other)
	{
		if (track && comp != nullptr && index != -1)
		{
			comp(index);
		}
		return array->at(value) > array->at(other.value);
	}
	bool operator>=(const IntElem& other)
	{
		return !(*this < other);
	}
	bool operator<=(const IntElem& other)
	{
		return !(* this > other);
	}
	operator int() const
	{
		return value;
	}

	static void SetArray(const std::vector<int>* arr)
	{
		array = arr;
	}

	static void EnableTracking()
	{
		track = true;
	}
	static void DisableTracking()
	{
		track = false;
	}

	static void SetComp(tFunc f)
	{
		comp = f;
	}
	static void SetSwap(tFunc f)
	{
		swap = f;
	}
private:
	int value;
	int index = -1;
	
	static inline tFunc comp;
	static inline tFunc swap;
	static inline bool track = false;
	//static inline std::vector<IntElem>* indexes = nullptr;
	static inline const std::vector<int>* array = nullptr;
};