#ifdef _MSC_VER
#pragma once
#endif
//----------------------------------------------------------
#ifndef tupleH
#define tupleH
//----------------------------------------------------------
#include "any.h"
//----------------------------------------------------------
#include <vector>
#include <initializer_list>
//----------------------------------------------------------
namespace runtime
{
class tuple
{
public:
	tuple() = default;
	tuple(const tuple & other) = default;
	tuple(const std::initializer_list<my::any>& list);

	template<typename T>
	T& get(std::size_t index)
	{
		return elements_[index].cast<T>();
	}

	template<typename T>
	const T& get(std::size_t index)const
	{
		return elements_[index].cast<T>();
	}

	template<typename T>
	const T& at(std::size_t index)const
	{
		return elements_.at(index).cast<T>();
	}

	template<typename T>
	T& at(std::size_t index)
	{
		return elements_.at(index).cast<T>();
	}

	std::size_t size()const;

	bool operator==(const tuple& other)const;
	bool operator!=(const tuple& other)const;

	bool operator<(const tuple& other)const;
	bool operator>(const tuple& other)const;

	bool operator<=(const tuple& other)const;
	bool operator>=(const tuple& other)const;

private:
	std::vector<my::any> elements_;
};
}
//--------------------------------------------------------------------------
#endif