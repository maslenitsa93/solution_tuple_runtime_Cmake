#include "any.h"

#include <algorithm>
#include <stdexcept>
#include <typeinfo>

using namespace my;
//---------------------------------------------------------
any::any()
	:ptr_(0),
	 holder_(0)
{
}
//---------------------------------------------------------
any::any(const any &other)
{
	if (other.empty())
		clear();
	else
	{
		ptr_ = other.holder_->clone(other.ptr_);
		holder_ = other.holder_;
	}
}
//----------------------------------------------------------
any &any::operator=(const any &other)
{
	if (other.empty())
		clear();
	else
	{
		void* ptr = other.holder_->clone(other.ptr_);
		std::swap(ptr_, ptr);
		holder_->destroy(ptr);
		holder_ = other.holder_;
	}
	return *this;
}
//----------------------------------------------------------
#ifdef USE_CPP_11
//----------------------------------------------------------
any::any(any&& other)
	:any()
{
	this->swap(other);
}
//----------------------------------------------------------
any& any::operator=(any&& other)
{
	this->swap(other);
	return *this;
}
//----------------------------------------------------------
#endif
//----------------------------------------------------------
any::~any()
{
  if(ptr_)
		holder_->destroy(ptr_);
}
//----------------------------------------------------------
bool any::empty() const
{
	return ptr_ == 0;
}
//----------------------------------------------------------
void any::clear()
{
	if(!empty())
	{
		holder_->destroy(ptr_);
		holder_ = 0;
		ptr_ = 0;
	}
}
//----------------------------------------------------------
void any::swap(any &other)
{
	std::swap(ptr_, other.ptr_);
	std::swap(holder_, other.holder_);
}
//---------------------------------------------------------
bool any::is_type(const any& other)const
{
	return holder_ == other.holder_;
}
//---------------------------------------------------------
void my::swap(any &a1, any &a2)
{
	a1.swap(a2);
}
//---------------------------------------------------------
void any::check_type(const any& other)const
{
	if (!is_type(other))
		throw std::invalid_argument("Can not compare objects with different value types!");
}
//---------------------------------------------------------
#ifdef USE_COMPARE_OPERATORS
//---------------------------------------------------------
bool any::operator==(const any& other)const
{
	check_type(other);
	return holder_->equal(ptr_, other.ptr_);
}
//---------------------------------------------------------
bool any::operator!=(const any& other)const
{
	check_type(other);
	return !holder_->equal(ptr_, other.ptr_);
}
//---------------------------------------------------------
bool any::operator<(const any& other)const
{
	check_type(other);
	return holder_->less(ptr_, other.ptr_);
}
//---------------------------------------------------------
bool any::operator>(const any& other)const
{
	check_type(other);
	return holder_->greater(ptr_, other.ptr_);
}
//---------------------------------------------------------
bool any::operator<=(const any& other)const
{
	check_type(other);
	return holder_->less(ptr_, other.ptr_) || holder_->equal(ptr_, other.ptr_);
}
//---------------------------------------------------------
bool any::operator>=(const any& other)const
{
	check_type(other);
	return holder_->greater(ptr_, other.ptr_) || holder_->equal(ptr_, other.ptr_);
}
//---------------------------------------------------------
#endif
