DynamicTextureUpdateManager = class("DynamicTextureUpdateManager")

local DynamicTextureUpdateInfo = {}
DynamicTextureUpdateInfo.textureInfo = nil
DynamicTextureUpdateInfo.callbacks = nil

DynamicTextureUpdateManager.instance = nil 

DynamicTextureUpdateManager.cacheUpdateInfos = {} --等待插入的纹理信息

function DynamicTextureUpdateManager:ctor()
	DynamicTextureUpdateManager.instance = self
end

function DynamicTextureUpdateManager:getInstance()
	if not DynamicTextureUpdateManager.instance then
		DynamicTextureUpdateManager.new()
	end
	return DynamicTextureUpdateManager.instance
end

--开始更新机制
function DynamicTextureUpdateManager:startUpdate()
	self:stopUpdate()
	self.scheduler = GlobalTimer.scheduleGlobal(function()
		self:onUpdate()
	end,3)
end

function DynamicTextureUpdateManager:onUpdate()
	print("DynamicTextureUpdateManager:onUpdate")
	if #self.cacheUpdateInfos <= 0 then return end

	local textureInfos = {}
	local textureInfoAddStates = {} --纹理添加状态
	for index,updateInfo in ipairs(self.cacheUpdateInfos or {}) do
		table.insert(textureInfos,updateInfo.textureInfo)
		textureInfoAddStates[index] = false  --默认为没有还没有添加成功
	end

	local rightIndexs = DynamicTextureManager:getInstance():addTextureInfos(textureInfos)
	for _,rightIndex in ipairs(rightIndexs or {}) do
		local updateInfo = self.cacheUpdateInfos[rightIndex]
		local texureInfo = updateInfo.textureInfo

		textureInfo.texture:release()
		textureInfo.texture = nil

		for _,callback in ipairs(updateInfo.callbacks or {}) do
			local sprite,pageIndex = DynamicTextureManager:getInstance():getSprite(updateInfo.textureInfo)
			if callback then callback(sprite) end
		end
		textureInfoAddStates[rightIndex] = true --纹理添加成功
	end

	for index,state in ipairs(textureInfoAddStates or {}) do
		if state == false then
			local updateInfo = self.cacheUpdateInfos[index]
			for _,callback in ipairs(updateInfo.callbacks or {}) do
				if callback then callback(nil) end
			end
		end
	end
	self.cacheUpdateInfos = {}

end

--停止更新机制
function DynamicTextureUpdateManager:stopUpdate()
	if self.scheduler then
		GlobalTimer.unscheduleGlobal(self.scheduler)
		self.scheduler = nil
	end
end



function DynamicTextureUpdateManager:tryAddTextureInfo(textureInfo,callback)
	if not textureInfo then return end
	textureInfo:fillDefaultAttribute()
	print("####注册###",textureInfo.text,textureInfo.fontSize)
	
	if DynamicTextureManager:getInstance():hasDynamicTexture(textureInfo.key) then
		local sprite,pageIndex = DynamicTextureManager:getInstance():getSprite(textureInfo)
		sprite:setColor(textureInfo.color and textureInfo.color or cc.c3b(255,255,255))
		if callback then callback(sprite) end
		return
	end
	cc.Texture2D:setDefaultAlphaPixelFormat(cc.TEXTURE2_D_PIXEL_FORMAT_RGB_A8888)
	local texture = cc.Texture2D:create()
    texture:createPixelsWithString(textureInfo.text,nil,textureInfo.fontSize)
    texture:retain()

    textureInfo.texture = texture
	for _,cacheUpdateInfo in ipairs(self.cacheUpdateInfos or {}) do
		if cacheUpdateInfo.textureInfo.key == textureInfo.key then
			table.insert(cacheUpdateInfo.callbacks,callback)
			return
		end
	end
	local cacheUpdateInfo = {}
	cacheUpdateInfo.textureInfo = textureInfo
	cacheUpdateInfo.callbacks = {}
	table.insert(cacheUpdateInfo.callbacks,callback)
	table.insert(self.cacheUpdateInfos,cacheUpdateInfo)
end


return DynamicTextureUpdateManager