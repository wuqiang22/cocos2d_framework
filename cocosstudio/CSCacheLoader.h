#ifndef __CS_CACHE_LOADER_H__
#define __CS_CACHE_LOADER_H__

#include "cocos2d.h"
#include "ClassExtensionConfig.h"
#include <unordered_map>

GAEM_NAME_SPACE_BEGIN



class CSCacheLoader{
public:
	static CSCacheLoader* getInstance();
	static void destoryInstance();

	cocos2d::Node* createNode(const std::string& path);

	cocos2d::Node* tryGetNode(const std::string& path);
	void returnNode(const std::string& path, cocos2d::Node* node);
private:
	static CSCacheLoader* instance;
	std::unordered_map<std::string, cocos2d::Node*> m_cacheNodes;
};

class CounterNode : public cocos2d::Node{
public:
	std::string filePath;
	cocos2d::Node* node;
	
	static CounterNode* createNode(const std::string& _path, cocos2d::Node* _node)
	{
		CounterNode * ret = new (std::nothrow) CounterNode(_path, _node);
		if (ret && ret->init())
		{
			ret->autorelease();
		}
		else
		{
			CC_SAFE_DELETE(ret);
		}
		return ret;
	}

	CounterNode(const std::string& _path, cocos2d::Node* _node) :filePath(_path), node(_node){
		CC_SAFE_RETAIN(node);
	}


	virtual ~CounterNode()
	{
		CSCacheLoader::getInstance()->returnNode(filePath, node);
	}
};




GAME_NAME_SPACE_END
#endif