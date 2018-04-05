#include "tuple.h"

using namespace runtime;

tuple::tuple(const std::initializer_list<my::any>& list)
	:elements_(list)
{
}

std::size_t tuple::size()const
{
	return elements_.size();
}

bool tuple::operator==(const tuple& other)const
{
	if (elements_.size() != other.elements_.size())
		throw std::invalid_argument("Can not compire tuple objects with diffirent element count!");

	for(std::size_t i=0; i<elements_.size(); ++i)
	{
		if (elements_[i] != other.elements_[i])
			return false;
	}

	return true;
}

bool tuple::operator!=(const tuple& other)const
{
	return !(*this == other);
}

bool tuple::operator<(const tuple& other)const
{
	if (elements_.size() != other.elements_.size())
		throw std::invalid_argument("Can not compire tuple objects with diffirent element count!");

	for (std::size_t i = 0; i<elements_.size(); ++i)
	{
		if (elements_[i] < other.elements_[i])
			return true;

		if (elements_[i] > other.elements_[i])
			return false;
	}
	return false;
}

bool tuple::operator>(const tuple& other)const
{
	return (*this!=other) && (!(*this<other));
}

bool tuple::operator<=(const tuple& other)const
{
	return (*this<other) || (*this==other);
}

bool tuple::operator>=(const tuple& other)const
{
	return (*this>other) || (*this == other);
}
