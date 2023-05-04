#pragma once
#ifndef __PCH__
#define __PCH__

#include "stdafx.hpp"

#define let inline constexpr 
#define ultimatum static inline constexpr 
#define con const
#define var decltype(auto)

#define NODISCARD [[nodiscard]]
#define UNUSED [[maybe_unused]]
#define DEPRECATED [[deprecated]]
#define DEPRECATE(msg) [[deprecated(msg)]]
#define NOADDRESS [[msvc::no_unique_address]]

#endif // __PCH__
