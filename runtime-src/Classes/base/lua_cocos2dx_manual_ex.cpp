#include "lua_cocos2dx_manual_ex.h"

#include <cocos2d.h>
#include "CCLuaEngine.h"
#include "CCLuaValue.h"
#include "LuaBasicConversions.h"
#include "tolua_fix.h"

#include <strstream>


static int tolua_cocos2dx_node_scheduleNode_ex(lua_State* tolua_S)
{
	if (NULL == tolua_S)
		return 0;

	int argc = 0;
	Node* self = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S, 1, "cc.Node", 0, &tolua_err)) goto tolua_lerror;
#endif

	self = static_cast<cocos2d::Node*>(tolua_tousertype(tolua_S, 1, 0));
#if COCOS2D_DEBUG >= 1
	if (nullptr == self) {
		tolua_error(tolua_S, "invalid 'self' in function 'tolua_cocos2dx_node_scheduleNode_ex'\n", NULL);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S) - 1;

	if (argc == 4)
	{
#if COCOS2D_DEBUG >= 1
		if (!toluafix_isfunction(tolua_S, 2, "LUA_FUNCTION", 0, &tolua_err))
			goto tolua_lerror;


		if (!tolua_isnumber(tolua_S, 3, 0, &tolua_err) || !tolua_isnumber(tolua_S, 4, 0, &tolua_err) || !tolua_isnumber(tolua_S,5,0,&tolua_err))
		{
			goto tolua_lerror;
		}
#endif
		LUA_FUNCTION handler = toluafix_ref_function(tolua_S, 2, 0);
		ScriptHandlerMgr::getInstance()->addCustomHandler((void*)self, handler);
	
		std::strstream ss;
		std::string key;
		ss << handler;
		ss >> key;

		float inteval = (float)tolua_tonumber(tolua_S, 3, 0);
		unsigned int repeat = (unsigned int)tolua_tonumber(tolua_S, 4, 0);
		float delay = (float)tolua_tonumber(tolua_S, 5, 0);


		self->schedule([=](float dt){
			LuaEngine::getInstance()->executeSchedule(handler, dt);
		}, inteval, repeat, delay, key);
		tolua_pushstring(tolua_S, key.c_str()); 
		return 1;
	}

	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "cc.Node:scheduleNode", argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'tolua_cocos2dx_node_scheduleNode_ex'.", &tolua_err);
	return 0;
#endif
}

static int tolua_cocos2dx_node_unscheduleNode_ex(lua_State* tolua_S)
{
	if (NULL == tolua_S)
		return 0;

	int argc = 0;
	Node* self = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S, 1, "cc.Node", 0, &tolua_err)) goto tolua_lerror;
#endif

	self = static_cast<cocos2d::Node*>(tolua_tousertype(tolua_S, 1, 0));
#if COCOS2D_DEBUG >= 1
	if (nullptr == self) {
		tolua_error(tolua_S, "invalid 'self' in function 'tolua_cocos2dx_node_unscheduleNode_ex'\n", NULL);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S) - 1;

	if (argc == 1)
	{
#if COCOS2D_DEBUG >= 1
		if (!tolua_isstring(tolua_S,2,0,&tolua_err))
		{
			goto tolua_lerror;
		}
#endif
		std::string key = lua_tostring(tolua_S, 2);
		self->unschedule(key);
		return 1;
	}

	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "cc.Node:scheduleNode", argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'tolua_cocos2dx_node_unscheduleNode_ex'.", &tolua_err);
	return 0;
#endif
}



static void extendNodeEx(lua_State* tolua_S)
{
	lua_pushstring(tolua_S,"cc.Node");
	lua_rawget(tolua_S, LUA_REGISTRYINDEX);
	if (lua_istable(tolua_S, -1))
	{
		lua_pushstring(tolua_S, "scheduleNode");
		lua_pushcfunction(tolua_S, tolua_cocos2dx_node_scheduleNode_ex);
		lua_rawset(tolua_S, -3);

		lua_pushstring(tolua_S, "unscheduleNode");
		lua_pushcfunction(tolua_S, tolua_cocos2dx_node_unscheduleNode_ex);
		lua_rawset(tolua_S, -3);
	}

	lua_pop(tolua_S, 1);
}

TOLUA_API int register_all_cocos2dx_manual_ex(lua_State* tolua_S)
{
	if (NULL == tolua_S)
		return 0;

	extendNodeEx(tolua_S);

	return 0;
}
