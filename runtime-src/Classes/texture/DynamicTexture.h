#ifndef __DYNAMIC_TEXTURE__H__
#define __DYNAMIC_TEXTURE__H__

#include "2d/CCNode.h"


NS_CC_BEGIN

/************************************************************************/
/* 动态纹理                                                             */
/************************************************************************/
class DynamicTexture : public cocos2d::Node
{
public:

	static const int DEFAULT_TEXTURE_WIDTH = 512;
	static const int DEFAULT_TEXTURE_HEIGHT = 512;

	static Texture2D* createWithTexture(Texture2D* texture);
	static DynamicTexture* create();
	static DynamicTexture* create(int width, int height);
	static bool hasPremultipliedAlphaForTextureOfText();

	//添加字符串纹理
	bool addStringTexture(Texture2D* texture, std::string key,bool wait = false);
	bool generateTexture();
	//大纹理
	inline Texture2D* getTexture(){ return this->_texture; };
	//小纹理信息
	Rect getSubTextureInfo(std::string key);
	//纹理填充是否结束
	inline bool isTextureEnd(){ return _isEnd; };
	//输出信息
	void dump();

CC_CONSTRUCTOR_ACCESS:
	DynamicTexture();
	virtual ~DynamicTexture();

	virtual bool init();
	virtual bool init(int width, int height);

private:
	Texture2D* _texture;
	int _currentX, _currentY,_currentLine,_currentLineHeight,_textureWidth, _textureHeight,_textureSpace;
	unsigned char* _textureDatas;
	bool _isEnd;
	std::map<std::string, cocos2d::Rect> _mapKeyes;
};


NS_CC_END
#endif

