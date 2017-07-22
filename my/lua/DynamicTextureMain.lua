
cc.FileUtils:getInstance():setPopupNotify(false)
cc.FileUtils:getInstance():addSearchPath("src/")
cc.FileUtils:getInstance():addSearchPath("src/test/")
cc.FileUtils:getInstance():addSearchPath("res/")

require "config"
require "cocos.init"

require "cocos.spine.SpineConstants"
require "cocos.cocos2d.OpenglConstants"

local DynamicTextureManager = require("DynamicTextureManager")
local DynamicTextureInfo = DynamicTextureManager.DynamicTextureInfo
local function testDynamicTextureManager(scene,parent)
  local labelNodes = {}
    local labelInfos = {}
    table.insert(labelInfos,{text = "我来了",fontSize = 50,color = cc.c3b(255,255,255)})
    table.insert(labelInfos,{text = "哈哈",fontSize = 30,color = cc.c3b(255,255,0)})
    table.insert(labelInfos,{text = "哦哦",fontSize = 20,color = cc.c3b(255,0,255)})
    table.insert(labelInfos,{text = "这是一个大笨猪哈哈哈",fontSize = 20,color = cc.c3b(0,255,0)})
    table.insert(labelInfos,{text = "周末星期六",fontSize = 50,color = cc.c3b(0,0,255)})
    table.insert(labelInfos,{text = "动态纹理1",fontSize = 50,color = cc.c3b(125,68,183)})
    table.insert(labelInfos,{text = "动态纹理2",fontSize = 50,color = cc.c3b(125,68,183)})
    table.insert(labelInfos,{text = "动态纹理3",fontSize = 50,color = cc.c3b(125,68,183)})
    table.insert(labelInfos,{text = "动态纹理4",fontSize = 50,color = cc.c3b(125,68,183)})
    for index,info in ipairs(labelInfos or {}) do
      local label = _createLabel(info.text,info.fontSize,info.color)
      scene:addChild(label)
      table.insert(labelNodes,label)
      label:setPosition(100*index,info.fontSize and info.fontSize or 12)
      label:visit()
    end

    local listener = cc.EventListenerTouchOneByOne:create()
    local times = 0
    local function onTouchesBegan(touch, event)
        times = times + 1
        print("点击次数：",times)
        if times == 1 then
          print("第一次点击，添加所有动态纹理")
          local totalX = 0
          local totalY = 0

          local dynamicTextureInfos = {}

          for index,nodeLabel in ipairs(labelNodes or {}) do
            local textSprite = nodeLabel:getTextSprite()
            local texture = textSprite:getTexture()
            local dynamicTextureInfo = DynamicTextureInfo.new(texture,labelInfos[index])
            table.insert(dynamicTextureInfos,dynamicTextureInfo)
          end
          DynamicTextureManager:getInstance():addStringTextures(dynamicTextureInfos)
          DynamicTextureManager:getInstance():dumpInfo()
          return
        end

        if times == 2 then
          local sprites = DynamicTextureManager:getInstance():getBigSprites()
          local winSize = cc.Director:getInstance():getWinSize()
          local curX =0
          local curY = winSize.height
          for _,sprite in ipairs(sprites or {}) do
            if curX + sprite:getContentSize().width > winSize.width then
              curX = 0
              curY = curY - 50
            end
            parent:addChild(sprite)
            sprite:setPosition(curX,curY)
            curX = curX + sprite:getContentSize().width
            if curX > winSize.width then
              curX = 0
              curY = curY - 50
            end
          end
          
          return
        end

        if times == 6 then
          DynamicTextureManager:getInstance():destoryInstance()
          times = 0
          parent:removeAllChildren()
          return 
        end

        for index,labelInfo in ipairs(labelInfos or {}) do
            local textureInfo = DynamicTextureInfo.new(nil,labelInfo)
            local sprite,pageIndex = DynamicTextureManager:getInstance():getSprite(textureInfo)
            if not sprite then
              print("获取纹理失败"..textureInfo.key)
            else
              parent:addChild(sprite,pageIndex)
              sprite:setPosition(100*index,(times - 1) * (labelInfo.fontSize and labelInfo.fontSize or 12))
              sprite:setColor(labelInfo.color and labelInfo.color or cc.c3b(255,255,255))
            end
          
          end

        return true
    end
    
    listener:registerScriptHandler(onTouchesBegan, cc.Handler.EVENT_TOUCH_BEGAN)
    
    listener:setSwallowTouches(false)
    local eventDispatcher = cc.Director:getInstance():getEventDispatcher()
    eventDispatcher:addEventListenerWithFixedPriority(listener,  1000)

    onTouchesBegan()
end

local function testDynamicTextureManager4(scene,node)
    local labelNodes = {}
    local labelInfos = {}
    table.insert(labelInfos,{text = "我来了",fontSize = 30,color = cc.c3b(255,255,0)})
    table.insert(labelInfos,{text = "哈喽哇",fontSize = 20,color = cc.c3b(255,255,255)})
    table.insert(labelInfos,{text = "我来了",fontSize = 50,color = cc.c3b(255,0,0)})

    local textures = {}
    local textureInfos = {}
    for index,labelInfo in ipairs(labelInfos or {}) do
      local texture = cc.Texture2D:create()
      texture:retain()
      texture:createPixelsWithString(labelInfo.text,"",labelInfo.fontSize)
      table.insert(textures,texture)

      local textureInfo = DynamicTextureManager:getInstance():createTextureInfo(labelInfo,texture)
      table.insert(textureInfos,textureInfo)

      local label = _createLabel(labelInfo.text,labelInfo.fontSize,labelInfo.color)
      table.insert(labelNodes,label)
      label:setPosition(100*index,100)
      node:addChild(label)
    end

    local listener = cc.EventListenerTouchOneByOne:create()
    local function onTouchesBegan(touch, event)
      times = times + 1
      if times == 1 then
        
        DynamicTextureManager:getInstance():addTextureInfos(textureInfos)

        for _,textureInfo in ipairs(textureInfos or {}) do
          textureInfo.texture:release()
          textureInfo.texture = nil
        end
        print("上传纹理")
        return true
      end

      if times == 2 then
        print("打印纹理大图")
        local currentX = 0
        local currentY = cc.Director:getInstance():getWinSize().height
        local sprites = DynamicTextureManager:getInstance():getBigSprites()
        print("#######纹理个数######",#sprites)
        for _,sprite in ipairs(sprites or {}) do
          node:addChild(sprite)
          sprite:setAnchorPoint(cc.p(0,1))
          sprite:setPosition(cc.p(currentX,currentY))
          currentX = currentX + sprite:getContentSize().width
        end
        return
      end

      for index,textureInfo in ipairs(textureInfos or {}) do
        local sprite = DynamicTextureManager:getInstance():getSprite(textureInfo)
        sprite:setPosition(cc.p(100*index,150 + (times - 2)*80))
        node:addChild(sprite)
      end

      return true
    end


    
    listener:registerScriptHandler(onTouchesBegan, cc.Handler.EVENT_TOUCH_BEGAN)
    
    listener:setSwallowTouches(false)
    local eventDispatcher = cc.Director:getInstance():getEventDispatcher()
    eventDispatcher:addEventListenerWithFixedPriority(listener,  1000)

end

local function main()
   -- require("app.MyApp"):create():run()
   local scene = cc.Scene:create()
    local director = cc.Director:getInstance();
    cc.Director:getInstance():replaceScene(scene)
    
    if CC_SHOW_FPS then
        -- turn on display FPS
        director:setDisplayStats(true);
    end

    -- set FPS. the default value is 1.0/60 if you don't call this
    director:setAnimationInterval(1.0 / CC_NORMAL_FPS);

    director:getOpenGLView():setDesignResolutionSize(CC_DESIGN_RESOLUTION.width, CC_DESIGN_RESOLUTION.height, cc.ResolutionPolicy.FIXED_WIDTH);
    
    math.randomseed(os.time());

    local node = cc.Node:create()
    scene:addChild(node)
    testDynamicTextureManager4(scene,node)
end


-- cclog
local cclog = function(...)
    print(string.format(...))
end

__G__TRACKBACK__ = function(msg)
 

    cclog("----------------------------------------")
    cclog("LUA ERROR: " .. tostring(msg) .. "\n")
    cclog(debug.traceback())
    cclog("----------------------------------------")
    return msg
end


local status, msg = xpcall(main, __G__TRACKBACK__)
if not status then
    release_print(msg)
end



