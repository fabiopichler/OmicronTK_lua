#include "OmicronTK/lua/Class.hpp"
#include "OmicronTK/lua/Lua.hpp"
#include "OmicronTK/lua/Object.hpp"
#include "OmicronTK/lua/helpers.hpp"

#include <lua.hpp>

#include <iostream>

namespace OmicronTK {
namespace lua {

Class::Class(Lua *lua, int ref)
    : m_ref(ref)
    , m_lua(lua)
{
}

Class::~Class()
{
    luaL_unref(m_lua->state(), LUA_REGISTRYINDEX, m_ref);
}

Object *Class::callConstructor(const ValueVec &values)
{
    lua_rawgeti(m_lua->state(), LUA_REGISTRYINDEX, m_ref);
    lua_getfield(m_lua->state(), -1, "new");

    if (!lua_isfunction(m_lua->state(), -1))
        return nullptr;

    for (const auto &value : values)
        pushValue(m_lua->state(), value);

    if (lua_pcall(m_lua->state(), values.size(), 1, 0) != 0)
    {
        std::cerr << "Failed to call the function! " << lua_tostring(m_lua->state(), -1) << std::endl;
        return nullptr;
    }

    int ref = luaL_ref(m_lua->state(), LUA_REGISTRYINDEX);

    lua_pop(m_lua->state(), 1);

    return new Object(m_lua, ref);
}

}
}
