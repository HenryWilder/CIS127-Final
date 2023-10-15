#pragma once
#include <array>

template<size_t _Size>
class nbool
{
public:
    nbool() = default;

    template<std::same_as<bool> ..._Args, typename std::enable_if<sizeof...(_Args) == _Size, bool>::type = false>
    constexpr nbool(_Args... args) noexcept
    {
        this->values = { args... };
    }

    constexpr nbool(bool fill)
    {
        for (bool& v : values)
        {
            v = fill;
        }
    }

    constexpr bool& operator[](size_t index) { return values[index]; }
    constexpr bool operator[](size_t index) const { return values[index]; }

    constexpr nbool& operator&=(nbool other) noexcept { for (size_t i = 0; i < _Size; ++i) { values[i] = values[i] && other[i]; } return *this; }
    constexpr nbool& operator|=(nbool other) noexcept { for (size_t i = 0; i < _Size; ++i) { values[i] = values[i] || other[i]; } return *this; }
    constexpr nbool& operator^=(nbool other) noexcept { for (size_t i = 0; i < _Size; ++i) { values[i] = values[i] != other[i]; } return *this; }

    constexpr nbool operator&(nbool other) const noexcept { nbool result = *this; result &= other; return result; }
    constexpr nbool operator|(nbool other) const noexcept { nbool result = *this; result |= other; return result; }
    constexpr nbool operator^(nbool other) const noexcept { nbool result = *this; result ^= other; return result; }
    constexpr nbool operator~() const noexcept { nbool result = *this; for (size_t i = 0; i < _Size; ++i) { result[i] = !result[i]; } return result; }

    constexpr bool operator==(nbool other) const noexcept { for (size_t i = 0; i < _Size; ++i) { if (values[i] != other[i]) return false; } return true; }
    constexpr bool operator!=(nbool other) const noexcept { for (size_t i = 0; i < _Size; ++i) { if (values[i] != other[i]) return true; } return false; }

    constexpr bool none() const noexcept { for (bool x : values) { if (x) return false; } return true; }
    constexpr bool all() const noexcept { for (bool x : values) { if (!x) return false; } return true; }
    constexpr bool any() const noexcept { for (bool x : values) { if (x) return true; } return false; }

    constexpr size_t size() const noexcept { return _Size; }

private:
    std::array<bool, _Size> values;
};
using bool2 = nbool<2>;
using bool3 = nbool<3>;

static_assert(bool2(false, true)[0] == false);
static_assert(bool2(false, true)[1] == true);

static_assert(!nbool<4>(true, true, false, true).all());
static_assert(nbool<4>(true, true, true, true).all());
static_assert(nbool<4>(false, false, true, false).any());
static_assert(!nbool<4>(false, false, false, false).any());
static_assert(nbool<4>(false, false, false, false).none());
static_assert(!nbool<4>(false, false, true, false).none());
