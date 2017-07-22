DynamicTextureManager = class("DynamicTextureManager")

local DynamicTextureInfo = class("DynamicTextureInfo")
DynamicTextureInfo.texture = nil
DynamicTextureInfo.key = nil
DynamicTextureInfo.text = nil
DynamicTextureInfo.fontSize = nil
DynamicTextureInfo.color = nil
DynamicTextureInfo.valid = false
DynamicTextureInfo.pageIndex = 0 --加入到了动态纹理之后，动态纹理的id


function DynamicTextureInfo.createKey(params)
	if not params then return nil end
	return string.format("%s",params.text)
	--return string.format("%s_%i",params.text,params.fontSize)
end

function DynamicTextureInfo:ctor(texture,params)
	self.texture = texture
	if params then
		self.text =  params.text and  params.text or ""
		self.fontSize =  params.fontSize and  params.fontSize or 20
		self.color =  params.color and  params.color or cc.c3b(255,255,255)
		self.key = DynamicTextureInfo.createKey(self)
		self.valid = true
	end
	self.pageIndex = 0

end

function DynamicTextureInfo:getKey()
	if self.key then return self.key end
	self.key = self.createKey(self)
	return self.key
end

function DynamicTextureInfo:fillDefaultAttribute()
	if not self.fontSize then
		self.fontSize = 20
	end

	if not self.color then
		self.color = cc.c3b(255,255,255)
	end
	if not self.text then
		self.text = ""
	end
end

function DynamicTextureInfo:isValid()
	return self.valid
end

DynamicTextureManager.DynamicTextureInfo = DynamicTextureInfo



function DynamicTextureManager:createTextureInfo(params,texture)
	if not params then return end
	return DynamicTextureInfo.new(texture,params)
end

local kDefaultBigTextureWidth = 512
local kDefaultBigTextureHeight = 512

DynamicTextureManager.instance = nil

function DynamicTextureManager:ctor()
	DynamicTextureManager.instance = self
	self.dynamicTextures = {}
	self.mapDynamicKey2TextreInfo = {} --动态纹理信息key --->[动态纹理index,存储的原始纹理信息]
end

function DynamicTextureManager:getInstance()
	if not DynamicTextureManager.instance then
		DynamicTextureManager.new()
	end
	return DynamicTextureManager.instance
end

function DynamicTextureManager:createDynamicTexture(width,height)
	width = width and width or kDefaultBigTextureWidth
	height = height and height or kDefaultBigTextureHeight
	local instance = cc.DynamicTexture:create(width,height)
	instance:retain()
	table.insert(self.dynamicTextures,instance)
	return instance
end

--wait:是否立即刷新动态纹理
function DynamicTextureManager:addStringTexture(texture,key,wait)
	wait = wait and wait or false
	local dynamicTexture = self.dynamicTextures[#self.dynamicTextures]
	if not dynamicTexture then
		dynamicTexture = self:createDynamicTexture()
	end
	if dynamicTexture:addStringTexture(texture,key,wait) then
		return true
	else
		local reason = dynamicTexture:isTextureEnd() == true and "纹理填充结束" or "未知异常"
		print("纹理添加失败，原因：",reason,key)
	end
	return false
end

function DynamicTextureManager:addTextureInfo(textureInfo,wait)
	wait = wait and wait or false
	if not textureInfo or not textureInfo:isValid() then return false end
	if self.mapDynamicKey2TextreInfo[textureInfo.key] then
		return true
	end
	if self:addStringTexture(textureInfo.texture, textureInfo.key, wait) then
		textureInfo.pageIndex = #self.dynamicTextures
		self.mapDynamicKey2TextreInfo[textureInfo.key] = textureInfo --纹理图id,存储的纹理信息
		return true
	end
	return false
end

--结束当前的动态纹理，同步纹理数据
function DynamicTextureManager:generateCurrentDynamicTexture()
	local dynamicTexture = self.dynamicTextures[#self.dynamicTextures]
	if not dynamicTexture then
		return 
	end
	dynamicTexture:generateTexture()
end

function DynamicTextureManager:addTextureInfos(textureInfos)
	local rightIndexs = {}
	local failRecords = {}
	for index,textureInfo in ipairs(textureInfos or {}) do
		
		if textureInfo and textureInfo.isValid and textureInfo:isValid() then
			local success = self:addTextureInfo(textureInfo, true)
			if not success and failRecords[index] == nil then
				failRecords[index] = false
			--	self:generateCurrentDynamicTexture()
				self:createDynamicTexture()
				self:addTextureInfo(textureInfo, true)
			elseif success then
				failRecords[index] = nil
				table.insert(rightIndexs,index)
			end
		end
	end
	--self:generateCurrentDynamicTexture()
	return rightIndexs
end

function DynamicTextureManager:getSprite(textureInfo)
	if not textureInfo then return end
	if not textureInfo.isValid then return end
	if not textureInfo or not textureInfo:isValid() then return nil end
	textureInfo:fillDefaultAttribute()

	local key = textureInfo:getKey()
	local originTextureInfo = self.mapDynamicKey2TextreInfo[key]
	
	if not originTextureInfo or originTextureInfo.pageIndex <= 0 then
		return nil
	end
	local pageIndex = originTextureInfo.pageIndex

	local dynamicTexture = self.dynamicTextures[pageIndex]
	if not dynamicTexture then return nil end
	local texture = dynamicTexture:getTexture()
	if not texture then return nil end
	local rect = dynamicTexture:getSubTextureInfo(key)
	print("当前纹理:",key,rect.x,rect.y,rect.width,rect.height,texture:getDescription())
	if not rect or (rect.x == 0 and rect.y == 0 and rect.width == 0 and rect.height == 0) then
		return nil
	end

	local ret = cc.Sprite:createWithTexture(texture,rect)
	if ret then
		ret:setColor(textureInfo.color)
		ret:setScale(textureInfo.fontSize/originTextureInfo.fontSize)
	end
	return ret,pageIndex;
end

function DynamicTextureUpdateManager:hasDynamicTexture(key)
	return self.mapDynamicKey2TextreInfo[key]
end

function DynamicTextureManager:destoryInstance()
	for _,dynamicTexture in ipairs(self.dynamicTextures or {}) do
		dynamicTexture:release()
	end
	self.dynamicTextures = {}
	self.mapDynamicKey2TextreInfo = {}
end

function DynamicTextureManager:getBigSprites()
	local sprites = {}
	for _,dynamicTexture in ipairs(self.dynamicTextures or {}) do
		local texture = dynamicTexture:getTexture()
		local bigSprite = cc.Sprite:createWithTexture(texture)
        bigSprite:setAnchorPoint(cc.p(0,1))
        table.insert(sprites,bigSprite)
	end
	return sprites
end

function DynamicTextureManager:dumpInfo()
	print("当前纹理个数：",#self.dynamicTextures)
	for key ,originTextureInfo in pairs(self.mapDynamicKey2TextreInfo or {}) do
		local pageIndex = originTextureInfo.pageIndex
		local dynamicTexture = self.dynamicTextures[pageIndex]
		local rect = dynamicTexture:getSubTextureInfo(key)
		print("纹理对应关系：",key,pageIndex,rect.x,rect.y,rect.width,rect.height)
	end
end

return DynamicTextureManager