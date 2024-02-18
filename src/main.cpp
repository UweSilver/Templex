#include <iostream>
#include <type_traits>
#include <concepts>
#include <string>
#include <array>
#include <cstddef>
#include <functional>
#include <string_view>

template <typename T>
concept Exp = requires {
    {
        T::value
    } -> std::convertible_to<int>;
};

template <int N>
struct Num
{
public:
    static constexpr int value = N;
};

template <Exp lhs, Exp rhs>
struct Add
{
public:
    static constexpr int value = lhs::value + rhs::value;
};

template <Exp lhs, Exp rhs>
struct Sub
{
public:
    static constexpr int value = lhs::value - rhs::value;
};

template <Exp lhs, Exp rhs>
struct Mul
{
public:
    static constexpr int value = lhs::value * rhs::value;
};

template <Exp lhs, Exp rhs>
struct Div
{
public:
    static constexpr int value = lhs::value / rhs::value;
};

template <Exp lhs, Exp rhs>
struct Great
{
public:
    static constexpr int value = lhs::value > rhs::value;
};

template <Exp lhs, Exp rhs>
struct Less
{
public:
    static constexpr int value = lhs::value < rhs::value;
};

template <Exp lhs, Exp rhs>
struct Equal
{
public:
    static constexpr int value = lhs::value == rhs::value;
};

template <Exp lhs, Exp rhs>
struct NotEqual
{
public:
    static constexpr int value = lhs::value != rhs::value;
};

template <Exp lhs, Exp rhs>
struct GreatEqual
{
public:
    static constexpr int value = lhs::value >= rhs::value;
};

template <Exp lhs, Exp rhs>
struct LessEqual
{
public:
    static constexpr int value = lhs::value <= rhs::value;
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

template <auto var>
struct test
{
    static constexpr auto name = var.name;
};

int main()
{
    std::cout << "hello, world" << std::endl;
    std::cout << Equal<Num<3>, Num<2>>::value << std::endl;
    std::cout << Equal<Num<3>, Num<3>>::value << std::endl;

    return 0;
}