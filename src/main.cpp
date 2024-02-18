#include <iostream>
#include <type_traits>
#include <concepts>
#include <string>
#include <array>
#include <cstddef>
#include <functional>
#include <string_view>

template <size_t Size>
class cexpr_map
{
public:
    using Key = std::string_view;
    using Value = int;
    static_assert(std::is_invocable_v<std::less<>, Key, Key>, "Key must be < comparable");
    using value_type = std::pair<Key, Value>;
    using container_type = std::array<value_type, Size>;
    container_type _arr;

    template <size_t... Is>
    constexpr cexpr_map(const value_type (&arr)[Size], std::index_sequence<Is...>) noexcept : _arr{{{arr[Is].first, arr[Is].second}...}}
    {
        for (size_t i = 0; i < _arr.size() - 1; ++i)
        {
            for (size_t j = 0; j < _arr.size() - 1 - i; ++j)
            {
                if (_arr[j + 1].first < _arr[j].first)
                {
                    auto tmp = _arr[j];
                    _arr[j].first = _arr[j + 1].first;
                    _arr[j].second = _arr[j + 1].second;
                    _arr[j + 1].first = tmp.first;
                    _arr[j + 1].second = tmp.second;
                }
            }
        }
    }

    constexpr cexpr_map(const std::array<value_type, Size> array) noexcept
    {
        _arr = array;

        for (size_t i = 0; i < _arr.size() - 1; ++i)
        {
            for (size_t j = 0; j < _arr.size() - 1 - i; ++j)
            {
                if (_arr[j + 1].first < _arr[j].first)
                {
                    auto tmp = _arr[j];
                    _arr[j].first = _arr[j + 1].first;
                    _arr[j].second = _arr[j + 1].second;
                    _arr[j + 1].first = tmp.first;
                    _arr[j + 1].second = tmp.second;
                }
            }
        }
    }

public:
    template <typename K, typename V, size_t N>
    friend constexpr auto make_map(const std::pair<K, V> (&arr)[N]);

    constexpr auto begin() const noexcept
    {
        return _arr.begin();
    }

    constexpr auto end() const noexcept
    {
        return _arr.end();
    }

    constexpr auto find(const Key &key) const noexcept -> typename container_type::const_iterator
    {
        auto left = begin();
        auto right = end();
        while (left < right)
        {
            const auto mid = left + (right - left) / 2;
            if (mid->first < key)
            {
                left = mid + 1;
            }
            else
            {
                right = mid;
            }
        }

        if (left != end() && left->first == key)
        {
            return left;
        }

        return _arr.end();
    }
};

template <size_t N>
constexpr auto make_map(const std::pair<std::string_view, int> (&arr)[N])
{
    cexpr_map<N> result{arr, std::make_index_sequence<N>{}};
    return result;
}

template <typename T>
concept Exp = requires {
    {
        T::value()
    } -> std::convertible_to<int>;
};

template <int N>
struct Num
{
public:
    static constexpr int value() { return N; }
};

template <Exp lhs, Exp rhs>
struct Add
{
public:
    static constexpr int value() { return lhs::value() + rhs::value(); }
};

template <Exp lhs, Exp rhs>
struct Sub
{
public:
    static constexpr int value() { return lhs::value() - rhs::value(); }
};

template <Exp ex0, Exp ex1>
struct Couple
{
public:
    static constexpr int value()
    {
        constexpr auto a = ex0::value();
        constexpr auto b = ex1::value();
        constexpr auto c = a + b;
        return b;
    };
};

template <int size>
struct Var
{
public:
    static constexpr int length = size - 1;
    constexpr Var(const char (&s_literal)[size])
    {
        for (int i = 0; i < size; i++)
        {
            name[i] = s_literal[i];
        }
        name[length] = '\0';
    }
    char name[size];
};

template <size_t N>
constexpr auto make_env(const std::pair<std::string_view, int> (&arr)[N])
{
    return make_map(arr);
}

template <size_t N, size_t M>
constexpr auto concat_env(const cexpr_map<N> n, const cexpr_map<M> m)
{
    std::array<std::pair<std::string_view, int>, N + M> result;
    std::copy(n.begin(), n.end(), result.begin());
    std::copy(m.begin(), m.end(), result.begin() + N);
    cexpr_map<N + M> new_env(result);
    return new_env;
}

int main()
{
    std::cout << "hello, world" << std::endl;
    std::cout << Add<Couple<Num<3>, Num<2>>, Num<-5>>::value() << std::endl;
    constexpr auto m = make_env({{"stuff", 100},
                                 {"k0", 0}});

    constexpr auto n = make_env({{"k3", 3},
                                 {"k1", 2},
                                 {"k2", 1}});
    constexpr auto mn = concat_env(m, n);

    std::cout
        << mn.find(Var{"stuff"}.name)->second << std::endl;
    return 0;
}