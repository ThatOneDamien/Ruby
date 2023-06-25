#pragma once

#include <utility>
#include <iostream>

// a lot taken from SharedPtr, but slightly modified to better suit my needs.
namespace Ruby {

	typedef decltype(__nullptr) nullptr_t;

	template<typename T>
	class SharedPtr;

	class __declspec(novtable) _COUNTER_BASE 
	{ // common code for reference counting
	private:
		template<typename T>
		friend class SharedPtr;

		virtual void _Destroy() noexcept = 0;

		unsigned long _Uses = 1;

	protected:
		constexpr _COUNTER_BASE() noexcept = default; // non-atomic initializations

	public:
		_COUNTER_BASE(const _COUNTER_BASE&) = delete;
		_COUNTER_BASE& operator=(const _COUNTER_BASE&) = delete;

		virtual ~_COUNTER_BASE() noexcept {} // TRANSITION, should be non-virtual

		void _Incref() noexcept { // increment use count
			_Uses++;
		}

		void _Decref() noexcept { // decrement use count
			if (--_Uses == 0) {
				_Destroy();
			}
		}

		unsigned long _Use_count() const noexcept {
			return static_cast<unsigned long>(_Uses);
		}
	};
	template<typename T>
	class _PTR_BASE : public _COUNTER_BASE
	{
	public:
		template<typename... Args>
		explicit _PTR_BASE(Args&&... args) : _COUNTER_BASE()
		{
			::std::_Construct_in_place(_Storage._Value, ::std::forward<Args>(args)...);
		}

		~_PTR_BASE() noexcept override {}

	
	private:

		template<typename T2 = T, typename... Args>
		friend SharedPtr<T2> createShared(Args&&... args);

		void _Destroy() noexcept override
		{
			_Storage._Value.~T();
			delete this;
		}

		union 
		{
			::std::_Wrap<T> _Storage;
		};
		unsigned long _Count = 1;
	};

	template<typename T>
	class SharedPtr
	{
	public:
		constexpr SharedPtr() noexcept = default;
		constexpr SharedPtr(nullptr_t) noexcept {}

		explicit SharedPtr(T* ptr) noexcept
		{
			ref = ptr;
			refCount = new unsigned long(1);
		}

		template <typename T2, ::std::enable_if_t<::std::_SP_pointer_compatible<T2, T>::value, int> = 0>
		explicit SharedPtr(T2* ptr) noexcept
		{
			ref = ptr;
			refCount = new unsigned long(1);
		}

		SharedPtr(const SharedPtr& other) noexcept
		{ 
			ref = other.ref;
			refCount = other.refCount;
			base = other.base;
			if (base)
				base->_Incref();
			else
				(*refCount)++;
		}

		template <typename T2, ::std::enable_if_t<::std::_SP_pointer_compatible<T2, T>::value, int> = 0>
		SharedPtr(const SharedPtr<T2>& other) noexcept
		{
			ref = other.ref;
			refCount = other.refCount;
			base = other.base;
			if (base)
				base->_Incref();
			else
				(*refCount)++;
		}

		SharedPtr(SharedPtr&& other) noexcept
		{
			ref = other.ref;
			refCount = other.refCount;
			base = other.base;
			other.ref = nullptr;
			other.refCount = nullptr;
			other.base = nullptr;
		}

		template <typename T2, ::std::enable_if_t<::std::_SP_pointer_compatible<T2, T>::value, int> = 0>
		SharedPtr(SharedPtr<T2>&& other) noexcept
		{
			ref = other.ref;
			refCount = other.refCount;
			base = other.base;
			other.ref = nullptr;
			other.refCount = nullptr;
			other.base = nullptr;
		}

		~SharedPtr() noexcept
		{
			if (base) base->_Decref();
			else if (refCount && --(*refCount) == 0)
			{
				delete refCount;
				delete ref;
			}
		}

		inline T* get() const noexcept { return ref; }
		inline unsigned long getRefCount() const noexcept { return *refCount; }
		inline void reset()
		{
			if (base) base->_Decref();
			else if (refCount && --(*refCount) == 0)
			{
				delete refCount;
				delete ref;
			}
			base = nullptr;
			ref = nullptr;
			refCount = nullptr;
		}

		SharedPtr& operator=(const SharedPtr& right) noexcept 
		{
			if (base) base->_Decref();
			else if (refCount && --(*refCount) == 0)
			{
				delete refCount;
				delete ref;
			}
			ref = right.ref;
			refCount = right.refCount;
			base = right.base;
			if (base)
				base->_Incref();
			else
				(*refCount)++;
			return *this;
		}

		template <typename T2, ::std::enable_if_t<::std::_SP_pointer_compatible<T2, T>::value, int> = 0>
		SharedPtr& operator=(const SharedPtr<T2>& right) noexcept 
		{
			if (base) base->_Decref();
			else if (refCount && --(*refCount) == 0)
			{
				delete refCount;
				delete ref;
			}
			ref = right.ref;
			refCount = right.refCount;
			base = right.base;
			if (base)
				base->_Incref();
			else
				(*refCount)++;
			return *this;
		}

		SharedPtr& operator=(SharedPtr&& right) noexcept 
		{
			if (base) base->_Decref();
			else if (refCount && --(*refCount) == 0)
			{
				delete refCount;
				delete ref;
			}
			ref = right.ref;
			refCount = right.refCount;
			base = right.base;
			right.ref = nullptr;
			right.refCount = nullptr;
			right.base = nullptr;
			return *this;
		}

		template <typename T2, ::std::enable_if_t<::std::_SP_pointer_compatible<T2, T>::value, int> = 0>
		SharedPtr& operator=(SharedPtr<T2>&& right) noexcept 
		{
			if (base) base->_Decref();
			else if (refCount && --(*refCount) == 0)
			{
				delete refCount;
				delete ref;
			}
			ref = right.ref;
			refCount = right.refCount;
			base = right.base;
			right.ref = nullptr;
			right.refCount = nullptr;
			right.base = nullptr;
			return *this;
		}

		template <typename T2 = T, ::std::enable_if_t<!::std::disjunction_v<::std::is_array<T2>, ::std::is_void<T2>>, int> = 0>
		T2& operator*() const noexcept 
		{
			return *ref;
		}

		template <typename T2 = T, ::std::enable_if_t<!::std::is_array_v<T2>, int> = 0>
		T2* operator->() const noexcept 
		{
			return ref;
		}

		explicit operator bool() const noexcept {
			return ref != nullptr;
		}

	private:
		inline void setData(T* const _ref)
		{
			if (!base) return;
			ref = _ref;
			refCount = &(base->_Uses);
		}

		template<typename T2>
		friend class SharedPtr;

		template<typename T2, typename... Args>
		friend SharedPtr<T2> createShared(Args&&... args);

		T* ref{ nullptr };
		unsigned long* refCount{ nullptr };
		_COUNTER_BASE* base{ nullptr };
	};

	template<typename T, typename... Args>
	inline [[nodiscard("Like createShared, do not discard the returned object, it should be assigned to another variable.")]]
		SharedPtr<T> createShared(Args&&... args)
	{
		SharedPtr<T> temp;
		const auto base = new _PTR_BASE<T>(::std::forward<Args>(args)...);
		temp.base = base;
		temp.setData(&(base->_Storage._Value));
		return temp;
	}

	template <typename T, typename T2>
	bool operator==(const SharedPtr<T>& left, const SharedPtr<T2>& right) noexcept 
	{
		return left.get() == right.get();
	}

	template <typename T, typename T2>
	bool operator==(const SharedPtr<T>& left, nullptr_t) noexcept
	{
		return left.get() == nullptr;
	}

	template <typename T, typename T2>
	bool operator==(nullptr_t, const SharedPtr<T>& right) noexcept
	{
		return right.get() == nullptr;
	}

	template <typename T, typename T2>
	bool operator!=(const SharedPtr<T>& left, const SharedPtr<T2>& right) noexcept
	{
		return left.get() != right.get();
	}

	template <typename T, typename T2>
	bool operator!=(const SharedPtr<T>& left, nullptr_t) noexcept
	{
		return left.get() != nullptr;
	}

	template <typename T, typename T2>
	bool operator!=(nullptr_t, const SharedPtr<T>& right) noexcept
	{
		return right.get() != nullptr;
	}

	template <typename Elem, typename Traits, typename T>
	::std::basic_ostream<Elem, Traits>& operator<<(::std::basic_ostream<Elem, Traits>& os, const SharedPtr<T>& ptr) 
	{
		return os << ptr.get();
	}

	template<typename T>
	class UniPtr // Points to a sole object, remake of unique_ptr for customizable usage.
	{
	public:
		constexpr UniPtr() noexcept = default;
		constexpr UniPtr(nullptr_t) noexcept {}

		explicit UniPtr(T* ptr) noexcept
		{
			ref = ptr;
		}

		template <typename T2, ::std::enable_if_t<::std::_SP_pointer_compatible<T2, T>::value, int> = 0>
		explicit UniPtr(T2* ptr) noexcept
		{
			ref = ptr;
		}

		// Creates new UniPtr with other's data. Does not point to the same pointer.
		UniPtr(const UniPtr& other) noexcept
		{
			if(other.ref)
				ref = new T(*other.ref); // Requires copy constructor
		}

		template <typename T2, ::std::enable_if_t<::std::_SP_pointer_compatible<T2, T>::value, int> = 0>
		UniPtr(const UniPtr<T2>& other) noexcept
		{
			if (other.ref)
				ref = new T2(*other.ref); // Requires copy constructor
		}

		UniPtr(UniPtr&& other) noexcept
		{
			ref = other.ref;
			other.ref = nullptr;
		}

		template <typename T2, ::std::enable_if_t<::std::_SP_pointer_compatible<T2, T>::value, int> = 0>
		UniPtr(UniPtr<T2>&& other) noexcept
		{
			ref = other.ref;
			other.ref = nullptr;
		}

		~UniPtr() noexcept
		{
			delete ref;
		}

		inline T* get() const noexcept { return ref; }
		inline void reset()
		{
			delete ref;

			ref = nullptr;
		}

		UniPtr& operator=(const UniPtr& right) noexcept
		{
			delete ref;
			ref = nullptr;
			if (other.ref)
				ref = new T(*right.ref);
			return *this;
		}

		template <typename T2, ::std::enable_if_t<::std::_SP_pointer_compatible<T2, T>::value, int> = 0>
		UniPtr& operator=(const UniPtr<T2>& right) noexcept
		{
			delete ref;
			ref = nullptr;
			if (right.ref)
				ref = new T2(*right.ref);
			return *this;
		}

		UniPtr& operator=(UniPtr&& right) noexcept
		{
			delete ref;
			ref = right.ref;
			right.ref = nullptr;
			return *this;
		}

		template <typename T2, ::std::enable_if_t<::std::_SP_pointer_compatible<T2, T>::value, int> = 0>
		UniPtr& operator=(UniPtr<T2>&& right) noexcept
		{
			delete ref;
			ref = right.ref;
			right.ref = nullptr;
			return *this;
		}

		template <typename T2 = T, ::std::enable_if_t<!::std::disjunction_v<::std::is_array<T2>, ::std::is_void<T2>>, int> = 0>
		T2& operator*() const noexcept
		{
			return *ref;
		}

		template <typename T2 = T, ::std::enable_if_t<!::std::is_array_v<T2>, int> = 0>
		T2* operator->() const noexcept
		{
			return ref;
		}

		explicit operator bool() const noexcept {
			return ref != nullptr;
		}

	private:
		inline void setData(T* const _ref)
		{
			if (!base) return;
			ref = _ref;
			refCount = &(base->_Uses);
		}

		template<typename T2>
		friend class UniPtr;

		template<typename T2, typename... Args>
		friend UniPtr<T2> createUniPtr(Args&&... args);

		T* ref{ nullptr };
	};

	template<typename T, typename... Args>
	inline [[nodiscard("Like createShared, do not discard the returned object, it should be assigned to another variable.")]]
		UniPtr<T> createUniPtr(Args&&... args)
	{
		UniPtr<T> temp;
		temp.ref = new T(::std::forward<Args>(args)...);
		return temp;
	}

	template <typename T, typename T2>
	bool operator==(const UniPtr<T>& left, const UniPtr<T2>& right) noexcept
	{
		return left.get() == right.get();
	}

	template <typename T, typename T2>
	bool operator==(const UniPtr<T>& left, nullptr_t) noexcept
	{
		return left.get() == nullptr;
	}

	template <typename T, typename T2>
	bool operator==(nullptr_t, const UniPtr<T>& right) noexcept
	{
		return right.get() == nullptr;
	}

	template <typename T, typename T2>
	bool operator!=(const UniPtr<T>& left, const UniPtr<T2>& right) noexcept
	{
		return left.get() != right.get();
	}

	template <typename T, typename T2>
	bool operator!=(const UniPtr<T>& left, nullptr_t) noexcept
	{
		return left.get() != nullptr;
	}

	template <typename T, typename T2>
	bool operator!=(nullptr_t, const UniPtr<T>& right) noexcept
	{
		return right.get() != nullptr;
	}

	template <typename Elem, typename Traits, typename T>
	::std::basic_ostream<Elem, Traits>& operator<<(::std::basic_ostream<Elem, Traits>& os, const UniPtr<T>& ptr)
	{
		return os << ptr.get();
	}


}