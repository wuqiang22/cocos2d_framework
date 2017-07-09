#include "lua_dynamic_texure_manual.h"
#include "tolua_fix.h"
#include "LuaBasicConversions.h"
#include "CCLuaValue.h"
#include "texture/DynamicTexture.h"

int lua_dynamictexture_create(lua_State* tolua_S)
{
	int argc = 0;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
	if (!tolua_isusertable(tolua_S, 1, "cc.DynamicTexture", 0, &tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;

	if (argc == 0)
	{
		if (!ok)
		{
			tolua_error(tolua_S, "invalid arguments in function 'lua_dynamictexture_create'", nullptr);
			return 0;
		}
		cocos2d::DynamicTexture* ret = cocos2d::DynamicTexture::create();
		object_to_luaval<cocos2d::DynamicTexture>(tolua_S, "cc.DynamicTexture", (cocos2d::DynamicTexture*)ret);
		return 1;
	}
	else if (argc == 2){
		int arg0,arg1;

		ok &= luaval_to_int32(tolua_S, 2, (int *)&arg0, "cc.DynamicTexture:create");
		if (!ok)
		{
			tolua_error(tolua_S, "invalid arguments in function 'lua_dynamictexture_create'", nullptr);
			return 0;
		}
		
		ok &= luaval_to_int32(tolua_S, 3, (int *)&arg1, "cc.DynamicTexture:create");
		if (!ok)
		{
			tolua_error(tolua_S, "invalid arguments in function 'lua_dynamictexture_create'", nullptr);
			return 0;
		}
		cocos2d::DynamicTexture* ret = cocos2d::DynamicTexture::create(arg0,arg1);
		object_to_luaval<cocos2d::DynamicTexture>(tolua_S, "cc.DynamicTexture", (cocos2d::DynamicTexture*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "cc.DynamicTexture:create", argc, 0);
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'lua_dynamictexture_create'.", &tolua_err);
#endif
	return 0;
}

int lua_dynamictexture_addStringTexture(lua_State* tolua_S)
{
	int argc = 0;
	cocos2d::DynamicTexture* cobj = nullptr;
	bool ok = true;
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif
	cobj = (cocos2d::DynamicTexture*)tolua_tousertype(tolua_S, 1, 0);

#if COCOS2D_DEBUG >= 1
	if (!cobj)
	{
		tolua_error(tolua_S, "invalid 'cobj' in function 'lua_dynamictexture_addStringTexture'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S) - 1;

	do
	{
		if (argc == 2)
		{
			cocos2d::Texture2D* arg0;
			ok &= luaval_to_object<cocos2d::Texture2D>(tolua_S, 2, "cc.Texture2D", &arg0);
			if (!ok) { break; }
			std::string arg1;
			ok &= luaval_to_std_string(tolua_S, 3, &arg1, "cc.DynamicTexture:addStringTexture");
			if (!ok) { break; }
			bool ret = cobj->addStringTexture(arg0, arg1);
			
			tolua_pushboolean(tolua_S, ret);
			return 1;
		}
		else if (argc == 3)
		{
			cocos2d::Texture2D* arg0;
			ok &= luaval_to_object<cocos2d::Texture2D>(tolua_S, 2, "cc.Texture2D", &arg0);
			if (!ok) { break; }
			std::string arg1;
			ok &= luaval_to_std_string(tolua_S, 3, &arg1, "cc.DynamicTexture:addStringTexture");
			if (!ok) { break; }
			bool arg2;
			ok &= luaval_to_boolean(tolua_S, 4, &arg2, "cc.DynamicTexture:addStringTexture");
			if (!ok) { break; }
			bool ret = cobj->addStringTexture(arg0, arg1,arg2);

			tolua_pushboolean(tolua_S, ret);
			return 1;
		}
	} while (0);
	
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d", "cc.DynamicTexture:addStringTexture", argc, 2);
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'lua_dynamictexture_addStringTexture'.", &tolua_err);
#endif
	return 0;
}

int lua_dynamictexture_getSubTextureInfo(lua_State* tolua_S)
{
	int argc = 0;
	cocos2d::DynamicTexture* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
	if (!tolua_isusertype(tolua_S, 1, "cc.DynamicTexture", 0, &tolua_err)) goto tolua_lerror;
#endif

	cobj = (cocos2d::DynamicTexture*)tolua_tousertype(tolua_S, 1, 0);

#if COCOS2D_DEBUG >= 1
	if (!cobj)
	{
		tolua_error(tolua_S, "invalid 'cobj' in function 'lua_dynamictexture_getSubTextureInfo'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 1)
	{
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2, &arg0, "cc.DynamicTexture:getSubTextureInfo");
		if (!ok)
		{
			tolua_error(tolua_S, "invalid arguments in function 'lua_dynamictexture_getSubTextureInfo'", nullptr);
			return 0;
		}
		cocos2d::Rect ret = cobj->getSubTextureInfo(arg0);
		rect_to_luaval(tolua_S, ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "cc.DynamicTexture:getSubTextureInfo", argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'lua_dynamictexture_getSubTextureInfo'.", &tolua_err);
#endif

	return 0;
}

int lua_dynamictexture_getTexture(lua_State* tolua_S)
{
	int argc = 0;
	cocos2d::DynamicTexture* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
	if (!tolua_isusertype(tolua_S, 1, "cc.DynamicTexture", 0, &tolua_err)) goto tolua_lerror;
#endif

	cobj = (cocos2d::DynamicTexture*)tolua_tousertype(tolua_S, 1, 0);

#if COCOS2D_DEBUG >= 1
	if (!cobj)
	{
		tolua_error(tolua_S, "invalid 'cobj' in function 'lua_dynamictexture_getTexture'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 0)
	{
		if (!ok)
		{
			tolua_error(tolua_S, "invalid arguments in function 'lua_dynamictexture_getTexture'", nullptr);
			return 0;
		}
		cocos2d::Texture2D* ret = cobj->getTexture();
		object_to_luaval<cocos2d::Texture2D>(tolua_S, "cc.Texture2D", (cocos2d::Texture2D*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "cc.DynamicTexture:getTexture", argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'lua_dynamictexture_getTexture'.", &tolua_err);
#endif

	return 0;
}

int lua_dynamictexture_isTextureEnd(lua_State* tolua_S)
{
	int argc = 0;
	cocos2d::DynamicTexture* cobj = nullptr;
	bool ok = true;
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif
	cobj = (cocos2d::DynamicTexture*)tolua_tousertype(tolua_S, 1, 0);

#if COCOS2D_DEBUG >= 1
	if (!cobj)
	{
		tolua_error(tolua_S, "invalid 'cobj' in function 'lua_dynamictexture_isTextureEnd'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S) - 1;

	do
	{
		if (argc == 0)
		{
			
			bool ret = cobj->isTextureEnd();
			tolua_pushboolean(tolua_S, ret);
			return 1;
		}
	} while (0);

	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d", "cc.DynamicTexture:isTextureEnd", argc, 0);
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'lua_dynamictexture_isTextureEnd'.", &tolua_err);
#endif
	return 0;
}

int lua_dynamictexture_generateTexture(lua_State* tolua_S)
{
	int argc = 0;
	cocos2d::DynamicTexture* cobj = nullptr;
	bool ok = true;
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif
	cobj = (cocos2d::DynamicTexture*)tolua_tousertype(tolua_S, 1, 0);

#if COCOS2D_DEBUG >= 1
	if (!cobj)
	{
		tolua_error(tolua_S, "invalid 'cobj' in function 'lua_dynamictexture_generateTexture'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S) - 1;

	do
	{
		if (argc == 0)
		{

			bool ret = cobj->generateTexture();
			tolua_pushboolean(tolua_S, ret);
			return 1;
		}
	} while (0);

	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d", "cc.DynamicTexture:generateTexture", argc, 0);
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'lua_dynamictexture_generateTexture'.", &tolua_err);
#endif
	return 0;
}



int lua_dynamictexture_dump(lua_State* tolua_S)
{
	int argc = 0;
	cocos2d::DynamicTexture* cobj = nullptr;
	bool ok = true;
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif
	cobj = (cocos2d::DynamicTexture*)tolua_tousertype(tolua_S, 1, 0);

#if COCOS2D_DEBUG >= 1
	if (!cobj)
	{
		tolua_error(tolua_S, "invalid 'cobj' in function 'lua_dynamictexture_dump'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S) - 1;

	do
	{
		if (argc == 0)
		{

			cobj->dump();
			return 1;
		}
	} while (0);

	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d", "cc.DynamicTexture:dump", argc, 0);
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'lua_dynamictexture_dump'.", &tolua_err);
#endif
	return 0;
}



int lua_register_DynamicTexture(lua_State* tolua_S)
{
	tolua_usertype(tolua_S, "cc.DynamicTexture");
	tolua_cclass(tolua_S, "DynamicTexture", "cc.DynamicTexture", "cc.Node", nullptr);

	tolua_beginmodule(tolua_S, "DynamicTexture");
	tolua_function(tolua_S, "create", lua_dynamictexture_create);
	tolua_function(tolua_S, "addStringTexture", lua_dynamictexture_addStringTexture);
	tolua_function(tolua_S, "getSubTextureInfo", lua_dynamictexture_getSubTextureInfo);
	tolua_function(tolua_S, "getTexture", lua_dynamictexture_getTexture);
	tolua_function(tolua_S, "generateTexture", lua_dynamictexture_generateTexture);
	tolua_function(tolua_S, "isTextureEnd", lua_dynamictexture_isTextureEnd);
	tolua_function(tolua_S, "dump", lua_dynamictexture_dump);
	
	
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(cocos2d::DynamicTexture).name();
	g_luaType[typeName] = "cc.DynamicTexture";
	g_typeCast["DynamicTexture"] = "cc.DynamicTexture";
	return 1;
}

TOLUA_API int register_dynamictexture_manual(lua_State* tolua_S)
{
	tolua_open(tolua_S);

	tolua_module(tolua_S, "cc", 0);
	tolua_beginmodule(tolua_S, "cc");

	lua_register_DynamicTexture(tolua_S);

	tolua_endmodule(tolua_S);

	return 1;
}