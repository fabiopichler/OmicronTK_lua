#include "OmicronTK/lua/Function.hpp"
#include "OmicronTK/lua/Lua.hpp"
#include "OmicronTK/lua/Object.hpp"
#include "OmicronTK/lua/helpers.hpp"

#include <lua.hpp>
#include <iostream>

namespace OmicronTK {
namespace lua {

Function::Function(Lua *lua, Object *object, int ref)
    : m_object(object)
    , m_ref(ref)
    , m_lua(lua)
{
}

Function::~Function()
{
    luaL_unref(m_lua->state(), LUA_REGISTRYINDEX, m_ref);
}

void Function::call(const ValueVec &values)
{
    lua_rawgeti(m_lua->state(), LUA_REGISTRYINDEX, m_ref);
    lua_rawgeti(m_lua->state(), LUA_REGISTRYINDEX, m_object->getRef());

    for (const auto &value : values)
        pushValue(m_lua->state(), value);

    if (lua_pcall(m_lua->state(), values.size() + 1, 0, 0) != 0)
    {
        std::cerr << "Failed to call the function! " << lua_tostring(m_lua->state(), -1) << std::endl;
        return;
    }
}

}
}
