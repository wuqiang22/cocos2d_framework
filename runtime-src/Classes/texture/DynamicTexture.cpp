#include "DynamicTexture.h"

NS_CC_BEGIN

DynamicTexture* DynamicTexture::create()
{
	DynamicTexture* instance = new (std::nothrow) DynamicTexture();
	if (instance && instance->init()){
		instance->autorelease();
		return instance;
	}
	CC_SAFE_DELETE(instance);
	return nullptr; 
}


DynamicTexture* DynamicTexture::create(int width, int height)
{
	DynamicTexture* instance = new (std::nothrow) DynamicTexture();
	if (instance && instance->init(width,height)){
		instance->autorelease();
		return instance;
	}
	CC_SAFE_DELETE(instance);
	return nullptr;
}

bool DynamicTexture::init()
{
	return init(DEFAULT_TEXTURE_WIDTH, DEFAULT_TEXTURE_HEIGHT);
}

bool DynamicTexture::init(int width, int height)
{
	bool ret = false;
	auto tempTexture = new (std::nothrow) Texture2D;
	if (!tempTexture){
		return false;
	}
	do 
	{
		Size  imageSize = Size((float)width, (float)height);
		int dataLen = width * height * 4;
		_textureDatas = new unsigned char[dataLen];
		if (!_textureDatas){
			break;
		}
		memset(_textureDatas, 0, dataLen);
		if (tempTexture->initWithData(_textureDatas, dataLen, Texture2D::PixelFormat::RGBA8888, width, height, imageSize))
		{
			tempTexture->setHasPremultipliedAlpha(DynamicTexture::hasPremultipliedAlphaForTextureOfText());
			_textureWidth = width;
			_textureHeight = height;
			_texture = tempTexture;
			ret = true;
			break;
		}
		CC_SAFE_DELETE(_textureDatas);
		CC_SAFE_DELETE(tempTexture);
	} while (0);
	return ret;
}

bool DynamicTexture::hasPremultipliedAlphaForTextureOfText()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) ||(CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
	return true;
#else
	return false;
#endif
}

/*
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) ||(CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
bool DynamicTexture::hasPremultipliedAlpha = true
#else
bool DynamicTexture::hasPremultipliedAlpha = false
#endif
*/

DynamicTexture::DynamicTexture(void) :
_texture(nullptr),
_currentLine(1),
_textureWidth(0),
_textureHeight(0),
_textureSpace(2),
_textureDatas(nullptr),
_isEnd(false),
_currentLineHeight(0)
{
	_currentX = _textureSpace;
	_currentY = _textureSpace;

}


DynamicTexture::~DynamicTexture()
{
	CC_SAFE_DELETE(_textureDatas);
	CC_SAFE_RELEASE(_texture);
}

Texture2D* DynamicTexture::createWithTexture(cocos2d::Texture2D *texture)
{
	if (!texture)
	{
		return nullptr;
	}
	Texture2D* ret = new (std::nothrow) Texture2D;
	size_t height = texture->getPixelsHigh();
	size_t width = texture->getPixelsWide();

	size_t dataLen = width * height * 4;

	unsigned char* _textureDatas = new unsigned char[dataLen];
	memset(_textureDatas, 0, dataLen);
	Size  imageSize = Size((float)width, (float)height);

	unsigned char* textureStringDatas = (unsigned char*)texture->getStringDatas();

	for (int y = 0; y < height; y++){

		int offsetTargetY = 4 * y * width;
		int offsetSourceY = 4 * y * width;
		for (int x = 0; x < width; x++){
			int offsetTargetX = 4 * x;
			int offsetSourceX = 4 * x;

			{
				unsigned char r = textureStringDatas[offsetSourceY + offsetSourceX + 0];
				unsigned char g = textureStringDatas[offsetSourceY + offsetSourceX + 1];
				unsigned char b = textureStringDatas[offsetSourceY + offsetSourceX + 2];
				unsigned char a = textureStringDatas[offsetSourceY + offsetSourceX + 3];


				_textureDatas[offsetTargetY + offsetTargetX + 0] = r;
				_textureDatas[offsetTargetY + offsetTargetX + 1] = g;
				_textureDatas[offsetTargetY + offsetTargetX + 2] = b;
				_textureDatas[offsetTargetY + offsetTargetX + 3] = a;

			}

		}
	}

	ret->initWithData(_textureDatas, dataLen, Texture2D::PixelFormat::RGBA8888, width, height, imageSize);
	ret->setHasPremultipliedAlpha(DynamicTexture::hasPremultipliedAlphaForTextureOfText());
	if (_textureDatas)
	{
		free(_textureDatas);
		_textureDatas = nullptr;
	}
	return ret;
}



bool DynamicTexture::addStringTexture(Texture2D* texture, std::string key, bool wait /* = false */)
{
	if (_mapKeyes.find(key) != _mapKeyes.end()) return false; //已经存在
	if (!texture) return false;
	if (!texture->isSaveStringTextureData() || !texture->getStringDatas()) return false;//没有字符串数据
	if (_isEnd) return false;//已经结束
	
	int pixelWidth = texture->getPixelsWide();
	int pixelHeight = texture->getPixelsHigh();
	//这个纹理比512*512还大,周围空出2个像素
	if (pixelWidth >= _textureWidth - 2 * _textureSpace || pixelHeight >= _textureHeight - 2 * _textureSpace) return false;
	if (_currentX + pixelWidth > _textureWidth - _textureSpace) //当前行不足容下这个纹理
	{
		int nextLine = _currentLine + 1;
		int nextX = _textureSpace;
		int nextY = _currentY + _currentLineHeight + _textureSpace;
		if (nextY + pixelHeight > _textureHeight - _textureSpace) //下一行也不足容下这个纹理
		{
			_isEnd = true;
			return false;
		}
		_currentLine = nextLine;
		_currentX = nextX;
		_currentY = nextY;
		_currentLineHeight = pixelHeight;
	}
	else if (_currentY + pixelHeight >= _textureHeight - _textureSpace) //高度容不下这个纹理
	{
		_isEnd = true;
		return false;
	}
	else //当前行和高度都可以放下这个纹理
	{
		if (pixelHeight > _currentLineHeight){
			_currentLineHeight = pixelHeight;
		}
	}
	unsigned char* textureStringDatas =(unsigned char*) texture->getStringDatas();
	for (int y = 0; y < pixelHeight; y++){
		int tempY = _currentY + y;
	
		int offsetTargetY = 4 * tempY * _textureWidth;
		int offsetSourceY = 4 * y * pixelWidth;
		for (int x = 0; x < pixelWidth; x++){
			int offsetTargetX = 4 * (_currentX + x);
			int offsetSourceX = 4 * x;
			_textureDatas[offsetTargetY + offsetTargetX + 0] = textureStringDatas[offsetSourceY + offsetSourceX];
			_textureDatas[offsetTargetY + offsetTargetX + 1] = textureStringDatas[offsetSourceY + offsetSourceX + 1];
			_textureDatas[offsetTargetY + offsetTargetX + 2] = textureStringDatas[offsetSourceY + offsetSourceX + 2];
			_textureDatas[offsetTargetY + offsetTargetX + 3] = textureStringDatas[offsetSourceY + offsetSourceX + 3];
		}
	}
	_mapKeyes.insert(std::make_pair(key, cocos2d::Rect(_currentX, _currentY, pixelWidth, pixelHeight)));

	_texture->updateWithData(textureStringDatas, _currentX, _currentY, pixelWidth, pixelHeight);

	_currentX += pixelWidth + _textureSpace;
	if (_currentX >= _textureWidth - _textureSpace){
		_currentX = _textureSpace;
		_currentY += _textureSpace + _currentLineHeight;
		_currentLine += 1;
		if (_textureHeight - _currentY < 20){ //如果新的一行起始Y值到底部只差20像素，直接结束。20数值取决于最小的字体大小
			_isEnd = true;
		}
	}	
	
	if (!wait)
	{
	//	return generateTexture();
	}

	
	return true;
}

bool DynamicTexture::generateTexture()
{
	if (!_texture || !_textureDatas)
	{
		return false;
	}

	Size  imageSize = Size((float)_textureWidth, (float)_textureHeight);
	int dataLen = _textureWidth * _textureHeight * 4;

	bool ret = _texture->initWithData(_textureDatas, dataLen, Texture2D::PixelFormat::RGBA8888, _textureWidth, _textureHeight, imageSize);
	_texture->setHasPremultipliedAlpha(DynamicTexture::hasPremultipliedAlphaForTextureOfText());
	return ret;
}

Rect DynamicTexture::getSubTextureInfo(std::string key)
{
	if (_mapKeyes.find(key) == _mapKeyes.end())
	{
		return cocos2d::Rect(0, 0, 0, 0);
	}

	return _mapKeyes[key];
}

void DynamicTexture::dump()
{
	cocos2d::log("the dynamictexture detailinfo x = %i,y = %i line = %i,lineHeight = %i", _currentX, _currentY,_currentLine,_currentLineHeight);
	cocos2d::log("the width and height of the texture width = %i,height = %i", _textureWidth, _textureHeight);
	cocos2d::log("is the texture end isEnd = %s", _isEnd == true ? "true":"false");
}

NS_CC_END