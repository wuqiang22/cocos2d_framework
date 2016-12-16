#include "tolua_fix.h"
#include "LuaBasicConversions.h"
#include "CCLuaValue.h"
#include "cocos2d/LuaScriptHandlerMgr.h"
#include "CCLuaEngine.h"
#include "cocosstudio/lua_cocos2dx_cocostudio_manual.h"
#include "cocosstudio/CSCacheLoader.h"


static int lua_cocostudio_CSCacheLoader_createNode(lua_State* tolua_S)
{
	if (NULL == tolua_S)
		return 0;

	int argc = 0;
	game::CSCacheLoader* self = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S, 1, "game.CSCacheLoader", 0, &tolua_err)) goto tolua_lerror;
#endif

	self = static_cast<game::CSCacheLoader*>(tolua_tousertype(tolua_S, 1, 0));
#if COCOS2D_DEBUG >= 1
	if (nullptr == self) {
		tolua_error(tolua_S, "invalid 'self' in function 'lua_cocostudio_CSCacheLoader_createNode'\n", NULL);
		return 0;
	}
#endif
	bool ok = true;
	argc = lua_gettop(tolua_S) - 1;

	if (argc == 1)
	{
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2, &arg0, "game.CSCacheLoader:createNode");
		if (!ok)
		{
			tolua_error(tolua_S, "invalid arguments in function 'lua_cocostudio_CSCacheLoader_createNode'", nullptr);
			return 0;
		}
		cocos2d::Node* ret = self->createNode(arg0); 
		object_to_luaval<cocos2d::Node>(tolua_S, "cc.Node", (cocos2d::Node*)ret);
		return 1;
	}

	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "game.CSCacheLoader:createNode", argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'lua_cocostudio_CSCacheLoader_createNode'.", &tolua_err);
	return 0;
#endif
}


int lua_cocostudio_CSCacheLoader_destoryInstance(lua_State* tolua_S)
{
	int argc = 0;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
	if (!tolua_isusertype(tolua_S, 1, "game.CSCacheLoader", 0, &tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;

	if (argc == 0)
	{
		if (!ok)
		{
			tolua_error(tolua_S, "invalid arguments in function 'lua_cocostudio_CSCacheLoader_destoryInstance'", nullptr);
			return 0;
		}
		game::CSCacheLoader::destoryInstance();
		lua_settop(tolua_S, 1);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "game.CSCacheLoader:destoryInstance", argc, 0);
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'lua_cocostudio_CSCacheLoader_destoryInstance'.", &tolua_err);
#endif
	return 0;
}

int lua_cocostudio_CSCacheLoader_getInstance(lua_State* tolua_S)
{
	int argc = 0;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
	if (!tolua_isusertable(tolua_S, 1, "game.CSCacheLoader", 0, &tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;

	if (argc == 0)
	{
		if (!ok)
		{
			tolua_error(tolua_S, "invalid arguments in function 'lua_cocostudio_CSCacheLoader_getInstance'", nullptr);
			return 0;
		}
		game::CSCacheLoader* ret = game::CSCacheLoader::getInstance();
		object_to_luaval<game::CSCacheLoader>(tolua_S, "game.CSCacheLoader", (game::CSCacheLoader*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "game.CSCacheLoader:getInstance", argc, 0);
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'lua_cocostudio_CSCacheLoader_getInstance'.", &tolua_err);
#endif
	return 0;
}


int lua_register_game_CSCacheLoader(lua_State* tolua_S)
{
	tolua_usertype(tolua_S, "game.CSCacheLoader");
	tolua_cclass(tolua_S, "CSCacheLoader", "game.CSCacheLoader", "", nullptr);

	tolua_beginmodule(tolua_S, "CSCacheLoader");

	tolua_function(tolua_S, "createNode", lua_cocostudio_CSCacheLoader_createNode);
	tolua_function(tolua_S, "getInstance", lua_cocostudio_CSCacheLoader_getInstance);
	tolua_function(tolua_S, "destoryInstance", lua_cocostudio_CSCacheLoader_destoryInstance);


	tolua_endmodule(tolua_S);
	std::string typeName = typeid(game::CSCacheLoader).name();
	g_luaType[typeName] = "game.CSCacheLoader";
	g_typeCast["CSCacheLoader"] = "game.CSCacheLoader";
	return 1;
}



TOLUA_API int register_all_cocos2dx_cocostudio_manual(lua_State* tolua_S)
{
	tolua_open(tolua_S);

	tolua_module(tolua_S, "game", 0);
	tolua_beginmodule(tolua_S, "game");

	lua_register_game_CSCacheLoader(tolua_S);
	tolua_endmodule(tolua_S);
	return 1;
}
