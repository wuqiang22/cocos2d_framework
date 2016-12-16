#include "cocosstudio/CSCacheLoader.h"
#include "CCFileUtils.h"
#include "cocostudio/ActionTimeline/CSLoader.h"

GAEM_NAME_SPACE_BEGIN

CSCacheLoader* CSCacheLoader::instance = nullptr;

void CSCacheLoader::returnNode(const std::string& path, cocos2d::Node* node)
{
	CC_SAFE_RELEASE(node);
	if (node->getReferenceCount() == 1)
	{
		CC_SAFE_RELEASE(node);
		m_cacheNodes.erase(path);
	}
}

cocos2d::Node* CSCacheLoader::tryGetNode(const std::string& path)
{
	cocos2d::Node* cacheNode = m_cacheNodes[path];
	if (!cacheNode)
	{
		cacheNode = cocos2d::CSLoader::getInstance()->createNode(path);
		cacheNode->retain();
		m_cacheNodes[path] = cacheNode;
	}
	return cacheNode;
}



CSCacheLoader* CSCacheLoader::getInstance()
{
	if (!instance)
	{
		instance = new CSCacheLoader();
	}
	return instance;
}

void CSCacheLoader::destoryInstance()
{
	if (instance)
	{
		delete instance;
		instance = nullptr;
	}

}


cocos2d::Node* CSCacheLoader::createNode(const std::string& path)
{
	const std::string& fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(path);
	cocos2d::Node* cacheNode = tryGetNode(path);
	if (!cacheNode)
	{
		CCLOG("create node fail filename = %s", path);
		return nullptr;
	}
	cocos2d::Node* cloneNode = cacheNode->clone();
	CounterNode* counterNode = CounterNode::createNode(path, cacheNode);
	cloneNode->addChild(counterNode);
	return cloneNode;

}

GAME_NAME_SPACE_END