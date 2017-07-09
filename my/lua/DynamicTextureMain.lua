
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
      testDynamicTextureManager(scene,node)
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



