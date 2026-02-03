#include "OmicronTK/lua/NativeClass.hpp"
#include "OmicronTK/lua/Lua.hpp"
#include "OmicronTK/lua/helpers.hpp"

#include <lua.hpp>

namespace OmicronTK {
namespace lua {

NativeClass::NativeClass(Lua &lua, const std::string &name, const std::string &nspace)
    : m_lua(lua)
    , m_name(name)
    , m_nspace(nspace) {}

void NativeClass::setStatics(const ValueMap &statics)
{
    m_statics = statics;
}

void NativeClass::addStatic(const std::string &field, const Value &value)
{
    m_statics[field] = value;
}

void NativeClass::setMembers(const ValueMap &members)
{
    m_members = members;
}

void NativeClass::addMember(const std::string &field, const Value &value)
{
    m_members[field] = value;
}

void NativeClass::setConstructor(const Value &constructor)
{
    m_members["constructor"] = constructor;
}

void NativeClass::create()
{
    lua_State *state = m_lua.state();

    pcall(state, "class", { m_name.c_str() }, 1);

    LuaRegVector_forEach(state, m_statics);

    if (!m_members.empty())
    {
        lua_getfield(state, -1, "proto");
        LuaRegVector_forEach(state, m_members);
        lua_pop(state, 1);
    }

    if (m_nspace.empty())
    {
        lua_setglobal(state, m_name.c_str());
    }
    else
    {
        lua_getglobal(state, m_nspace.c_str());
        lua_pushvalue(state, -2);
        lua_setfield(state, -2, m_name.c_str());
        lua_pop(state, 1);
    }

    lua_pop(state, 1);
}

}
}
