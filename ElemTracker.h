#pragma once
#include <vector>
#include <iostream>

template<typename T>
class ElemTracker
{
private:
	typedef void (*tFunc)(int);
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
			if (l.index != -1)
				comp(l.index);
			if (r.index != -1)
				comp(r.index);
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

	static void SetComp(tFunc f)
	{
		comp = f;
	}
	static void SetChange(tFunc f)
	{
		change = f;
	}
private:
	T value;
	const int index;
	
	static inline tFunc comp;
	static inline tFunc change;
	static inline bool track = false;
	//static inline std::vector<IntElem>* indexes = nullptr;
	//static inline const std::vector<ElemTrac>* array = nullptr;
};