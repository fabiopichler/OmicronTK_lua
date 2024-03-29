/*---------------------------------------------------------------------------

Copyright (c) 2020 Fábio Pichler

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

-----------------------------------------------------------------------------*/

#pragma once

#include "OmicronTK/lua/Value.hpp"
#include "OmicronTK/lua/alternative_api/util/ObjectUtil.hpp"
#include "OmicronTK/lua/helpers.hpp"

#include <memory>
#include <utility>
#include <cassert>

namespace OmicronTK {
namespace lua {

template<typename _Class, const char *_className>
class Object
{
    using ObjUtil = ObjectUtil<_Class, _className>;

public:
    template<const ValueType... _types>
    inline static int constructor(lua_State *L)
    {
        assert ((lua_gettop(L) - 1) == (sizeof... (_types)));

        luaL_checktype(L, 1, LUA_TTABLE);

        _Class *object = callConstructor<_types...>(L, std::make_index_sequence<sizeof... (_types)>{});

        lua_pushvalue(L, 1);
        ObjUtil::newUserData(L, 1, object);

        return 0;
    }

    template<const ValueType... _types>
    inline static int initializer(lua_State *L)
    {
        assert ((lua_gettop(L) - 1) == (sizeof... (_types)));

        luaL_checktype(L, 1, LUA_TTABLE);

        _Class *object = initialize<_types...>(L, std::make_index_sequence<sizeof... (_types)>{});

        lua_pushvalue(L, 1);
        ObjUtil::newUserData(L, 1, object);

        return 0;
    }

    template<typename _Method, const _Method *_method, const ValueType... _types>
    inline static int method(lua_State *L)
    {
        _Class *object = getUserData<_types...>(L);

        callMethod<_types...>(L, object, _method, std::make_index_sequence<sizeof... (_types)>{});

        return 0;
    }

    template<typename _Method, const _Method *_method, const ValueType _returnType, const ValueType... _types>
    inline static int method_r(lua_State *L)
    {
        _Class *object = getUserData<_types...>(L);

        Value val = callMethod_r<_types...>(L, object, _method, std::make_index_sequence<sizeof... (_types)>{});

        pushValue(L, static_cast<_ValueType<_returnType>>(val));

        return 1;
    }

    template<typename _Field, const _Field *_field, const ValueType _type>
    inline static int setter(lua_State *L)
    {
        _Class *object = ObjUtil::checkUserData(L, 1);

        (object->*(*_field)) = static_cast<_ValueType<_type>>(toValue(L, _type, 2));

        return 0;
    }

    template<typename _Field, const _Field *_field, const ValueType _type>
    inline static int getter(lua_State *L)
    {
        _Class *object = ObjUtil::checkUserData(L, 1);

        pushValue(L, static_cast<_ValueType<_type>>((object->*(*_field))));

        return 1;
    }

    inline static int __gc(lua_State *L)
    {
        _Class *object = *static_cast<_Class **>(luaL_checkudata(L, -1, _className));

        if (object)
        {
            delete object;
            return 0;
        }

        lua_pushstring(L, "C++ object garbage failure");
        lua_error(L);
        return 0;
    }

    class util
    {
    public:
        template<const ValueType... _types>
        inline static LuaReg constructor()
        {
            return LuaReg { "constructor", Object::constructor<_types...> };
        }

        inline static LuaReg __gc()
        {
            return LuaReg { "__gc", Object::__gc };
        }
    };

private:
    template<const ValueType... _types>
    inline static _Class *getUserData(lua_State *L)
    {
        assert ((lua_gettop(L) - 1) == (sizeof... (_types)));

        return ObjUtil::checkUserData(L, 1);
    }

    template<const ValueType... _types, std::size_t... I>
    inline static _Class *callConstructor(lua_State *L, std::index_sequence<I...>)
    {
        constexpr ValueType types[] { _types... };

        return new _Class(static_cast<_ValueType<types[I]>>(toValue(L, types[I], I + 2))...);
    }

    template<const ValueType... _types, std::size_t... I>
    inline static _Class *initialize(lua_State *L, std::index_sequence<I...>)
    {
        constexpr ValueType types[] { _types... };

        return new _Class{static_cast<_ValueType<types[I]>>(toValue(L, types[I], I + 2))...};
    }

    template<const ValueType... _types, typename _Method, std::size_t... I>
    inline static void callMethod(lua_State *L, _Class *_object, _Method&& _method, std::index_sequence<I...>)
    {
        constexpr ValueType types[] { _types... };

        (_object->*(*_method))(static_cast<_ValueType<types[I]>>(toValue(L, types[I], I + 2))...);
    }

    template<const ValueType... _types, typename _Method, std::size_t... I>
    inline static auto callMethod_r(lua_State *L, _Class *_object, _Method&& _method, std::index_sequence<I...>)
    {
        constexpr ValueType types[] { _types... };

        return std::forward<Value>((_object->*(*_method))(
                                       static_cast<_ValueType<types[I]>>(toValue(L, types[I], I + 2))...));
    }
};

}
}
