#pragma once

#include <stdint.h>
#include <iostream>
#include "Ruby/Main/Platform.h"

#include <type_traits>
#include <iostream>

// a lot taken from SharedPtr, but slightly modified to better suit my needs.
namespace Ruby 
{
    template<typename T>
    class SharedPtr;

    class RB_NOVTABLE RefCounter
    {
    private:
        template<typename T>
        friend class SharedPtr;

        virtual void Destroy() noexcept = 0;

        uint64_t refs = 1;

    protected:
        constexpr RefCounter() noexcept = default;

    public:
        RefCounter(const RefCounter&) = delete;
        RefCounter& operator=(const RefCounter&) = delete;

        virtual ~RefCounter() noexcept {}

    };

    template<typename T>
    class PtrBase : public RefCounter
    {
    public:
        template<typename... Args>
        explicit PtrBase(Args&&... args)
            : RefCounter()
        {
            new (&storage) T(std::forward<Args>(args)...);
        }

        ~PtrBase() noexcept override {}

        union { T storage; };

    private:

        void Destroy() noexcept override
        {
            storage.~T();
            delete this;
        }

    };

    template<typename T>
    class SharedPtr
    {
    public:

        constexpr SharedPtr() noexcept = default;
        constexpr SharedPtr(std::nullptr_t) noexcept {}

        SharedPtr(const SharedPtr& other) noexcept
        { 
            m_Data = other.m_Data;
            if(m_Data)
                ++m_Data->refs;
        }

        template <typename T2, std::enable_if_t<std::is_assignable_v<T, T2>, int> = 0>
        SharedPtr(const SharedPtr<T2>& other) noexcept
        {
            m_Data = other.m_Data;
            if(m_Data)
                ++m_Data->refs;
        }

        SharedPtr(SharedPtr&& other) noexcept
        {
            m_Data = other.m_Data;
            other.m_Data = nullptr;
            
        }

        template <typename T2, std::enable_if_t<std::is_assignable_v<T&, T2>, int> = 0>
        SharedPtr(SharedPtr<T2>&& other) noexcept
        {
            m_Data = other.m_Data;
            other.m_Data = nullptr;
        }

        ~SharedPtr() noexcept
        {
            if(m_Data && --m_Data->refs == 0)
                m_Data->Destroy();
        }

        inline T* get() noexcept { return m_Data ? (T*)(&m_Data->refs + 1) : nullptr; }
        inline const T* get() const noexcept { return m_Data ? (T*)(&m_Data->refs + 1) : nullptr; }
        inline uint64_t getRefCount() const noexcept { return m_Data->refs; }

        inline void reset()
        {
            if(m_Data && --m_Data->refs == 0)
                m_Data->Destroy();
            m_Data = nullptr;
        }

        SharedPtr& operator=(const SharedPtr& right) noexcept 
        {
            if(m_Data && --m_Data->refs == 0)
                m_Data->Destroy();
            
            m_Data = right.m_Data;

            if (m_Data)
                ++m_Data->refs;

            return *this;
        }

        template <typename T2, std::enable_if_t<std::is_assignable_v<T&, T2>, int> = 0>
        SharedPtr& operator=(const SharedPtr<T2>& right) noexcept 
        {
            if(m_Data && --m_Data->refs == 0)
                m_Data->Destroy();
            
            m_Data = right.m_Data;

            if (m_Data)
                ++m_Data->refs;

            return *this;
        }

        SharedPtr& operator=(SharedPtr&& right) noexcept 
        {
            if(m_Data && --m_Data->refs == 0)
                m_Data->Destroy();
            
            m_Data = right.m_Data;
            right.m_Data = nullptr;

            return *this;
        }

        template <typename T2, std::enable_if_t<std::is_assignable_v<T&, T2>, int> = 0>
        SharedPtr<T>& operator=(SharedPtr<T2>&& right) noexcept 
        {
            if(m_Data && --m_Data->refs == 0)
                m_Data->Destroy();
            
            m_Data = right.m_Data;
            right.m_Data = nullptr;

            return *this;
        }

        T& operator*() noexcept 
        {
            return *get();
        }

        const T& operator*() const noexcept 
        {
            return *get();
        }

        T* operator->() noexcept 
        {
            return get();
        }

        const T* operator->() const noexcept 
        {
            return get();
        }

        operator T*() noexcept
        {
            return get();
        }

        operator const T*() const noexcept
        {
            return get();
        }

        explicit operator bool() const noexcept {
            return m_Data != nullptr;
        }

    private:
        template<typename T2>
        friend class SharedPtr;

        template<typename T2, typename... Args>
        friend SharedPtr<T2> createShared(Args&&...);

    private:
        RefCounter* m_Data{nullptr};
    };

    template<typename T, typename... Args>
    inline RB_NO_DISCARD SharedPtr<T> createShared(Args&&... args)
    {
        SharedPtr<T> temp;
        temp.m_Data = new PtrBase<T>(std::forward<Args>(args)...);
        return temp;
    }

    template <typename T, typename T2>
    bool operator==(const SharedPtr<T>& left, const SharedPtr<T2>& right) noexcept 
    {
        return left.get() == right.get();
    }

    template <typename T, typename = void>
    bool operator==(const SharedPtr<T>& left, std::nullptr_t) noexcept
    {
        return !(bool)left;
    }

    template <typename T, typename = void>
    bool operator==(std::nullptr_t, const SharedPtr<T>& right) noexcept
    {
        return !(bool)right;
    }

    template <typename T, typename T2>
    bool operator!=(const SharedPtr<T>& left, const SharedPtr<T2>& right) noexcept
    {
        return left.get() != right.get();
    }

    template <typename T, typename = void>
    bool operator!=(const SharedPtr<T>& left, std::nullptr_t) noexcept
    {
        return (bool)left;
    }

    template <typename T, typename = void>
    bool operator!=(std::nullptr_t, const SharedPtr<T>& right) noexcept
    {
        return (bool)right;
    }

    template <typename Elem, typename Traits, typename T>
    std::basic_ostream<Elem, Traits>& operator<<(std::basic_ostream<Elem, Traits>& os, const SharedPtr<T>& ptr) 
    {
        return os << ptr.get();
    }

    template<typename T>
    class UniPtr // Points to a unique object, remake of unique_ptr for customizable usage.
    {
    public:
        constexpr UniPtr() noexcept = default;
        constexpr UniPtr(std::nullptr_t) noexcept {}

        explicit UniPtr(T* ptr) noexcept
        {
            ref = ptr;
        }

        template <typename T2, std::enable_if_t<std::is_assignable_v<T&, T2>>>
        explicit UniPtr(T2* ptr) noexcept
        {
            ref = ptr;
        }

        // Creates new UniPtr with other's data. Does not point to the same pointer.
        UniPtr(const UniPtr& other) noexcept
        {
            if(other.ref)
                ref = new T(*other.ref);
        }

        template <typename T2, std::enable_if_t<std::is_assignable_v<T&, T2>, int> = 0>
        UniPtr(const UniPtr<T2>& other) noexcept
        {
            if (other.ref)
                ref = new T2(*other.ref);
        }

        UniPtr(UniPtr&& other) noexcept
        {
            ref = other.ref;
            other.ref = nullptr;
        }

        template <typename T2, std::enable_if_t<std::is_assignable_v<T&, T2>, int> = 0>
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
            if (right.ref)
                ref = new T(*right.ref);
            return *this;
        }

        template <typename T2, std::enable_if_t<std::is_assignable_v<T&, T2>, int> = 0>
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

        template <typename T2, std::enable_if_t<std::is_assignable_v<T&, T2>, int> = 0>
        UniPtr& operator=(UniPtr<T2>&& right) noexcept
        {
            delete ref;
            ref = right.ref;
            right.ref = nullptr;
            return *this;
        }

        template <typename T2 = T, std::enable_if_t<!std::disjunction_v<std::is_array<T2>, std::is_void<T2>>, int> = 0>
        T2& operator*() const noexcept
        {
            return *ref;
        }

        template <typename T2 = T, std::enable_if_t<!std::is_array_v<T2>, int> = 0>
        T2* operator->() const noexcept
        {
            return ref;
        }

        explicit operator bool() const noexcept 
        {
            return ref != nullptr;
        }

    private:

        template<typename T2>
        friend class UniPtr;

        template<typename T2, typename... Args>
        friend UniPtr<T2> createUniPtr(Args&&... args);

        T* ref{ nullptr };
    };

    template<typename T, typename... Args>
    inline RB_NO_DISCARD UniPtr<T> createUniPtr(Args&&... args)
    {
        UniPtr<T> temp;
        temp.ref = new T(std::forward<Args>(args)...);
        return temp;
    }

    template <typename T, typename T2>
    bool operator==(const UniPtr<T>& left, const UniPtr<T2>& right) noexcept
    {
        return left.get() == right.get();
    }

    template <typename T, typename T2>
    bool operator==(const UniPtr<T>& left, std::nullptr_t) noexcept
    {
        return !left.get();
    }

    template <typename T, typename T2>
    bool operator==(std::nullptr_t, const UniPtr<T>& right) noexcept
    {
        return !right.get();
    }

    template <typename T, typename T2>
    bool operator!=(const UniPtr<T>& left, const UniPtr<T2>& right) noexcept
    {
        return left.get() != right.get();
    }

    template <typename T, typename T2>
    bool operator!=(const UniPtr<T>& left, std::nullptr_t) noexcept
    {
        return left.get();
    }

    template <typename T, typename T2>
    bool operator!=(std::nullptr_t, const UniPtr<T>& right) noexcept
    {
        return right.get();
    }

    template <typename Elem, typename Traits, typename T>
    std::basic_ostream<Elem, Traits>& operator<<(std::basic_ostream<Elem, Traits>& os, const UniPtr<T>& ptr)
    {
        return os << ptr.get();
    }


}
