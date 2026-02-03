#include "OmicronTK/lua/Object.hpp"
#include "OmicronTK/lua/Lua.hpp"
#include "OmicronTK/lua/Function.hpp"

#include <lua.hpp>

namespace OmicronTK {
namespace lua {

Object::Object(Lua *lua, int ref)
    : m_ref(ref)
    , m_lua(lua)
{
}

Object::~Object()
{
    luaL_unref(m_lua->state(), LUA_REGISTRYINDEX, m_ref);
}

Function *Object::getFunction(const std::string &name)
{
    lua_rawgeti(m_lua->state(), LUA_REGISTRYINDEX, m_ref);
    lua_getfield(m_lua->state(), -1, name.c_str());

    if (!lua_isfunction(m_lua->state(), -1))
        return nullptr;

    int ref = luaL_ref(m_lua->state(), LUA_REGISTRYINDEX);

    lua_pop(m_lua->state(), 1);

    return new Function(m_lua, this, ref);
}

}
}
