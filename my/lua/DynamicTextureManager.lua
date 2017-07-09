local DynamicTextureManager = class("DynamicTextureManager")

local DynamicTextureInfo = class("DynamicTextureInfo")
DynamicTextureInfo.texture = nil
DynamicTextureInfo.key = nil
DynamicTextureInfo.text = nil
DynamicTextureInfo.fontSize = nil
DynamicTextureInfo.color = nil
DynamicTextureInfo.valid = false

function DynamicTextureInfo.getKey(params)
	if not params then return nil end
	return string.format("%s_%i_%i:%i:%i",params.text,params.fontSize,params.color.r,params.color.g,params.color.b)
end

function DynamicTextureInfo:ctor(texture,params)
	self.texture = texture
	if params then
		self.text =  params.text and  params.text or ""
		self.fontSize =  params.fontSize and  params.fontSize or 20
		self.color =  params.color and  params.color or cc.c3b(255,255,255)
		self.key = DynamicTextureInfo.getKey(self)
		self.valid = true
	end

end

function DynamicTextureInfo:isValid()
	return self.valid
end

DynamicTextureManager.DynamicTextureInfo = DynamicTextureInfo


local kDefaultBigTextureWidth = 300
local kDefaultBigTextureHeight = 300

DynamicTextureManager.instance = nil

function DynamicTextureManager:ctor()
	DynamicTextureManager.instance = self
	self.dynamicTextures = {}
	self.mapDynamicKey2PageIndex = {} --动态纹理信息key --->动态纹理index
end

function DynamicTextureManager:getInstance()
	if not DynamicTextureManager.instance then
		DynamicTextureManager.new()
	end
	return DynamicTextureManager.instance
end

function DynamicTextureManager:createDynamicTextureInstance(width,height)
	width = width and width or kDefaultBigTextureWidth
	height = height and height or kDefaultBigTextureHeight
	local instance = cc.DynamicTexture:create(width,height)
	instance:retain()
	table.insert(self.dynamicTextures,instance)
	return instance
end

function DynamicTextureManager:addStringTexture(texture,key,wait)
	wait = wait and wait or false
	local currentDynamicTextureInstance = self.dynamicTextures[#self.dynamicTextures]
	if not currentDynamicTextureInstance then
		currentDynamicTextureInstance = self:createDynamicTextureInstance()
	end
	if currentDynamicTextureInstance:addStringTexture(texture,key,wait) then
		return true
	else
		local reason = currentDynamicTextureInstance:isTextureEnd() == true and "纹理填充结束" or "未知异常"
		print("纹理添加失败，原因：",reason,key)
	end
	return false
end

function DynamicTextureManager:addTextureInfo(textureInfo,wait)
	wait = wait and wait or false
	if not textureInfo or not textureInfo:isValid() then return false end
	if self:addStringTexture(textureInfo.texture, textureInfo.key, wait) then
		self.mapDynamicKey2PageIndex[textureInfo.key] = #self.dynamicTextures
		return true
	end
	return false
end

--结束当前的动态纹理，同步纹理数据
function DynamicTextureManager:generateCurrentDynamicTexture()
	local currentDynamicTextureInstance = self.dynamicTextures[#self.dynamicTextures]
	if not currentDynamicTextureInstance then
		return 
	end
	currentDynamicTextureInstance:generateTexture()
end

function DynamicTextureManager:addStringTextures(textureInfos)
	for index,textureInfo in ipairs(textureInfos or {}) do
		if textureInfo and textureInfo:isValid() then
			if not self:addTextureInfo(textureInfo, true) then
				self:generateCurrentDynamicTexture()
				self:createDynamicTextureInstance()
				self:addTextureInfo(textureInfo, true)
			end
		end
	end
	self:generateCurrentDynamicTexture()
end

function DynamicTextureManager:getSprite(textureInfo)
	if not textureInfo then return nil end
	local key = textureInfo.key
	local pageIndex = self.mapDynamicKey2PageIndex[key]
	if not pageIndex or pageIndex <=0 then
		return nil
	end

	local dynamicTextureInstance = self.dynamicTextures[pageIndex]
	if not dynamicTextureInstance then return nil end
	local texture = dynamicTextureInstance:getTexture()
	if not texture then return nil end
	local rect = dynamicTextureInstance:getSubTextureInfo(key)
	if not rect or (rect.x == 0 and rect.y == 0 and rect.width == 0 and rect.height == 0) then
		return nil
	end
	return cc.Sprite:createWithTexture(texture,rect),pageIndex
end

function DynamicTextureManager:destoryInstance()
	for _,dynamicTextureInstance in ipairs(self.dynamicTextures or {}) do
		dynamicTextureInstance:release()
	end
	self.dynamicTextures = {}
	self.mapDynamicKey2PageIndex = {}
end

function DynamicTextureManager:getBigSprites()
	local sprites = {}
	for _,dynamicTextureInstance in ipairs(self.dynamicTextures or {}) do
			local texture = dynamicTextureInstance:getTexture()
			local bigSprite = cc.Sprite:createWithTexture(texture)
          bigSprite:setAnchorPoint(cc.p(0,1))
          table.insert(sprites,bigSprite)
	end
	return sprites
end

function DynamicTextureManager:dumpInfo()
	print("当前纹理个数：",#self.dynamicTextures)
	for key ,pageIndex in pairs(self.mapDynamicKey2PageIndex or {}) do
		print("纹理对应关系：",key,pageIndex)
	end
end

return DynamicTextureManager