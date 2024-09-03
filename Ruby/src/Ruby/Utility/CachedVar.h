#pragma once


namespace Ruby
{
    template<typename T>
    struct CachedVar
    {
        T    Value;
        bool Cached;
    };

}
