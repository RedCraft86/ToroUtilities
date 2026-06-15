// Copyright (C) 2026 RedCraft86. Licensed under the MIT License.
// See <https://opensource.org/license/mit> for the full details of the MIT License.

// Credits: Rob is possible thanks to: http://bloglitb.blogspot.com/2011/12/access-to-private-members-safer.html

#pragma once

/** <b>USAGE INFO</b>
 * <pre>
 * Define var with ROB_DEFINE_VAR
 * Then, access with Obj->*RobAccess(class, var);
 *
 * or
 *
 * Define func with ROB_DEFINE_FUNC(_CONST)
 * Then, call with Obj->*RobAccess(class, func)(params);
 * </pre>
 */
template<typename Tag, typename Tag::Type M>
struct Rob
{
	friend Tag::Type Access(Tag)
	{
		return M;
	}
};

#define ROB_DEFINE_VAR(_Class, _VarName, _VarType) \
	struct F##_Class##_VarName##Tag \
	{ \
		using Type = _VarType _Class::*; \
		friend Type Access(F##_Class##_VarName##Tag); \
	}; \
	template struct Rob<F##_Class##_VarName##Tag, &_Class::_VarName>

#define ROB_DEFINE_FUNC(_Class, _FuncName, _RetType, ...) \
	struct F##_Class##_FuncName##Tag \
	{ \
		using Type = _RetType (_Class::*)(__VA_ARGS__); \
		friend Type Access(F##_Class##_FuncName##Tag); \
	}; \
	template struct Rob<F##_Class##_FuncName##Tag, &_Class::_FuncName>

#define ROB_DEFINE_FUNC_CONST(_Class, _FuncName, _RetType, ...) \
	struct F##_Class##_FuncName##Tag \
	{ \
		using Type = _RetType (_Class::*)(__VA_ARGS__) const; \
		friend Type Access(F##_Class##_FuncName##Tag); \
	}; \
	template struct Rob<F##_Class##_FuncName##Tag, &_Class::_FuncName>

#define RobAccess(_Class, _MemberName) Access(F##_Class##_MemberName##Tag())