#pragma once
#include <cstdint>
enum class Request : uint32_t
{
    E_HELLO,
    E_ON_CONNECT,
    E_SELECT_WORKSPACE,
    E_NEW_SHAPE
};