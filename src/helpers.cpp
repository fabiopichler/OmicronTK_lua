#include "OmicronTK/lua/helpers.hpp"

#include <string>
#include <lua.hpp>

namespace OmicronTK {
namespace lua {

void LuaRegVector_forEach(lua_State *L, const ValueMap &values)
{
    for (const auto &value : values)
    {
        pushValue(L, value.second);
        lua_setfield(L, -2, value.first.c_str());
    }
}

void pcall(lua_State *state, const std::string &name, const ValueVec &values, size_t returnsSize)
{
    lua_getglobal(state, name.c_str());

    for (const auto &value : values)
        pushValue(state, value);

    lua_pcall(state, values.size(), returnsSize, 0);
}

void pcallTable(lua_State *state, bool isObject, const std::string &table, const std::string &field,
                const ValueVec &values, size_t returnsSize)
{
    lua_getglobal(state, table.c_str());
    lua_getfield(state, -1, field.c_str());

    size_t valuesSize = values.size();

    if (isObject)
    {
        ++valuesSize;
        lua_pushvalue(state, -2);
    }
    else
    {
        lua_pushvalue(state, -1);
    }

    for (const auto &value : values)
        pushValue(state, value);

    lua_pcall(state, valuesSize, returnsSize, 0);
}

ValueVec pcallReturn(lua_State *state, const std::vector<ValueType> &returns)
{
    ValueVec valueVector;

    for (size_t idx = 0; idx < returns.size(); ++idx)
        valueVector.push_back(toValue(state, returns[idx], idx + 1));

    lua_settop(state, 0);

    return valueVector;
}

Value toValue(lua_State *state, ValueType type, uint32_t idx)
{
    switch (type)
    {
        case ValueType::Nil:
            luaL_checktype(state, idx, LUA_TNIL);
        break;

        case ValueType::Number:
            luaL_checktype(state, idx, LUA_TNUMBER);
            return double (lua_tonumber(state, idx));

        case ValueType::Float:
            luaL_checktype(state, idx, LUA_TNUMBER);
            return float (lua_tonumber(state, idx));

        case ValueType::Integer:
            luaL_checktype(state, idx, LUA_TNUMBER);
            return int (lua_tointeger(state, idx));

        case ValueType::UInt:
            luaL_checktype(state, idx, LUA_TNUMBER);
            return static_cast<unsigned int>(lua_tonumber(state, idx));

        case ValueType::Long:
            luaL_checktype(state, idx, LUA_TNUMBER);
            return long (lua_tonumber(state, idx));

        case ValueType::ULong:
            luaL_checktype(state, idx, LUA_TNUMBER);
            return static_cast<unsigned long>(lua_tonumber(state, idx));

        case ValueType::String:
            luaL_checktype(state, idx, LUA_TSTRING);
            return lua_tostring(state, idx);

        case ValueType::CFunction:
            luaL_checktype(state, idx, LUA_TFUNCTION);
            return lua_tocfunction(state, idx);

        case ValueType::Boolean:
            luaL_checktype(state, idx, LUA_TBOOLEAN);
            return bool (lua_toboolean(state, idx));

        case ValueType::UserData:
        {
            if (lua_istable(state, idx))
            {
                lua_getfield(state, idx, "__userdata");
                luaL_checktype(state, -1, LUA_TUSERDATA);

                return *static_cast<void **>(lua_touserdata(state, -1));
            }

            luaL_checktype(state, idx, LUA_TLIGHTUSERDATA);

            return lua_touserdata(state, idx);
        }
    }

    return Value(); // nil
}

void pushValue(lua_State *state, const Value &value)
{
    switch (value.type())
    {
        case ValueType::Nil:
            lua_pushnil(state);
        break;

        case ValueType::Number:
            lua_pushnumber(state, value.number_value());
        break;

        case ValueType::Float:
            lua_pushnumber(state, value.float_value());
        break;

        case ValueType::Integer:
            lua_pushinteger(state, value.integer_value());
        break;

        case ValueType::UInt:
            lua_pushnumber(state, value.uint_value());
        break;

        case ValueType::Long:
            lua_pushnumber(state, value.long_value());
        break;

        case ValueType::ULong:
            lua_pushnumber(state, value.ulong_value());
        break;

        case ValueType::String:
            lua_pushstring(state, value.c_str_value());
        break;

        case ValueType::CFunction:
            lua_pushcfunction(state, value.cfunction_value());
        break;

        case ValueType::Boolean:
            lua_pushboolean(state, value.boolean_value());
        break;

        case ValueType::UserData:
            lua_pushlightuserdata(state, value.userdata_value());
        break;
    }
}

}
}
