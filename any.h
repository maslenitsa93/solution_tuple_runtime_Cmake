#ifdef _MSC_VER
#pragma once
#endif
//----------------------------------------------------------
#ifndef anyH
#define anyH
//----------------------------------------------------------
// Класс my::any позваляет хранить в себе объект любого класса который
// 1.  Может создается через new и удаляется через delete
// (но только не массивы)
// 2. Имеет конструтор по умолчанию, конструктор копирования.
// 3. Имеет определенные операторы сравнения < > = (необходимо для runtime::tuple)
//
// метод cast<T> даёт доступ к значению внутри.

#define USE_COMPARE_OPERATORS
#define USE_CPP_11
//----------------------------------------------------------
namespace my
{
//----------------------------------------------------------
class base_holder
{
public:
	virtual void* clone(void* ptr) = 0;
	virtual void destroy(void* ptr) = 0;

#ifdef USE_COMPARE_OPERATORS
	virtual bool equal(void* ptr1, void* ptr2) = 0;
	virtual bool less(void* ptr1, void* ptr2) = 0;
	virtual bool greater(void* ptr1, void* ptr2) = 0;
#endif
};
//----------------------------------------------------------
template<typename T>
class holder : public base_holder
{
public:
	void* clone(void* ptr)
	{
		return new T(*static_cast<T*>(ptr));
	}

	virtual void destroy(void* ptr)
	{
		if (ptr)
			delete static_cast<T*>(ptr);
	}

#ifdef USE_COMPARE_OPERATORS
	virtual bool equal(void* ptr1, void* ptr2) // ==
	{
		if (ptr1 && ptr2)
			return  (*static_cast<T*>(ptr1)) == (*static_cast<T*>(ptr2));

		return (ptr1==0) && (ptr2==0);
	}

	virtual bool less(void* ptr1, void* ptr2) // <
	{
		if(ptr1 && ptr2)
			return  (*static_cast<T*>(ptr1)) < (*static_cast<T*>(ptr2));

		return false;
	}

	virtual bool greater(void* ptr1, void* ptr2) // >
	{
		if (ptr1 && ptr2)
			return  (*static_cast<T*>(ptr1)) > (*static_cast<T*>(ptr2));

		return false;
	}
#endif
};
//----------------------------------------------------------
template<typename T>
holder<T>* make_holder()
{
	static holder<T> holder;
	return &holder;
}
//======================== any =============================
class any
{
public:
	any();

	template<typename T>
	any(const T& value);

	any(const any& other);
	any& operator=(const any& other);

#ifdef USE_CPP_11
	any(any&& other);
	any& operator=(any&& other);
#endif

	~any();

	void clear();
	bool empty()const;

	template<typename T>
	bool is_type()const;


	template<typename T>
	any& operator=(const T& value);

	template<typename T>
	const T& cast() const;

	template<typename T>
	T& cast();

	template<typename T>
	bool try_cast(T& value) const;

	void swap(any& other);

	bool is_type(const any& other)const;

#ifdef USE_COMPARE_OPERATORS
	bool operator==(const any& other)const;
	bool operator!=(const any& other)const;

	bool operator<(const any& other)const;
	bool operator>(const any& other)const;

	bool operator<=(const any& other)const;
	bool operator>=(const any& other)const;
#endif

private:
	void check_type(const any& other)const;

	void* ptr_;
	base_holder* holder_;
};
//================== not member ============================
template<typename T>
const T& any_cast(const any& other)
{
	return other.cast<T>();
}
//----------------------------------------------------------
void swap(any &a1, any &a2);
//================== member ================================
template<typename T>
any::any(const T &value)
	: ptr_(new T(value)),
	  holder_(make_holder<T>())
{
}
//----------------------------------------------------------
template<typename T>
any &any::operator=(const T& value)
{
	void* ptr = new T(value);
	std::swap(ptr_, ptr);

  if (ptr)
		holder_->destroy(ptr);

  holder_ = make_holder<T>();
	return *this;
}
//----------------------------------------------------------
template<typename T>
bool any::is_type()const
{
	return holder_ == make_holder<T>();
}
//----------------------------------------------------------
template<typename T>
const T& any::cast() const
{
	if (is_type<T>())
		return *(static_cast<T*>(ptr_));
	else
		throw std::bad_cast();
}
//----------------------------------------------------------
template<typename T>
T& any::cast()
{
	if (is_type<T>())
		return *(static_cast<T*>(ptr_));
	else
		throw std::bad_cast();
}
//----------------------------------------------------------
template<typename T>
bool any::try_cast(T& value) const
{
	if (is_type<T>())
	{
		value = *(static_cast<T*>(ptr_));
		return true;
	}
	return false;
}

//----------------------------------------------------------
}
//----------------------------------------------------------
#endif
