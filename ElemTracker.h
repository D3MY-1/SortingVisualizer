#pragma once
#include <vector>
#include <iostream>

template<typename T>
class ElemTracker
{
private:
	typedef void (*tChange)(int);
	typedef void (*tCompare)(int,int);
public:
	ElemTracker(const T& a) :value(a),index(-1) {};
	ElemTracker(const ElemTracker& a) : value(a.value),index(-1) {};
	ElemTracker(const T& a,int idx) : value(a),index(idx) {};

	ElemTracker() : index(-1) {};

	ElemTracker& operator=(const T& val)
	{
		if (track && comp != nullptr && index != -1)
			change(index);
		value = val;
		return *this;
	}
	ElemTracker& operator=(const ElemTracker& other)
	{
		if (track && comp != nullptr && index != -1)
		{
			//comp(other.value);
			change(index);
		}
			
		value = other.value;
		return *this;
	}
	ElemTracker& operator=(ElemTracker&& other) noexcept
	{
		if (track && comp != nullptr && index != -1)
		{
			change(index);
		}
		value = std::move(other.value);
		return *this;
	}
	friend bool operator<(const ElemTracker& l,const ElemTracker& r)
	{
		if (track && comp != nullptr)
		{
			comp(l.index,r.index);
		}
		return l.value < r.value;
	}
	friend bool operator<(const ElemTracker& l, const T& r)
	{
		return operator<(l, ElemTracker(r));
	}
	friend bool operator>(const ElemTracker& l, const T& r)
	{
		return operator>(l, ElemTracker(r));
	}
	friend bool operator>=(const ElemTracker& l, const T& r)
	{
		return operator>=(l, ElemTracker(r));
	}
	friend bool operator<=(const ElemTracker& l, const T& r)
	{
		return operator<=(l, ElemTracker(r));
	}
	friend bool operator>(const ElemTracker& l, const ElemTracker& r)
	{
		return r < l;
	}
	friend bool operator>=(const ElemTracker& l, const ElemTracker& r)
	{
		return !(operator<(l,r));
	}
	friend bool operator<=(const ElemTracker& l, const ElemTracker& r)
	{
		return !(operator>(l,r));
	}
	explicit operator T() const
	{
		return value;
	}

	static void EnableTracking()
	{
		track = true;
	}
	static void DisableTracking()
	{
		track = false;
	}

	static void SetComp(tCompare f)
	{
		comp = f;
	}
	static void SetChange(tChange f)
	{
		change = f;
	}
private:
	T value;
	const int index;
	
	static inline tCompare comp;
	static inline tChange change;
	static inline bool track = false;
	//static inline std::vector<IntElem>* indexes = nullptr;
	//static inline const std::vector<ElemTrac>* array = nullptr;
};