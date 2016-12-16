local function testSpriteFrame(scene)
  cc.SpriteFrameCache:getInstance():addSpriteFrames("texture.plist","texture.png")
  local spriteFrame = cc.SpriteFrameCache:getInstance():getSpriteFrame("02.png")
  local texture = spriteFrame:getTexture()
  --(texture:自己创建引用计数为1，因为有30个spriteFrame,每个spriteFrame都拥有一个引用)
  --(spriteframe:自己创建引用计数为1，加入到了SpriteFrameCache的hash表中又加了一个引用)
  print("########1###",spriteFrame:getReferenceCount(),texture:getReferenceCount())  --  2,31 
  local sprite = cc.Sprite:createWithSpriteFrameName("ui/prime/hero/strengthen/main/pic/02.png")
  scene:addChild(sprite)
  local size = cc.Director:getInstance():getWinSize()
  sprite:setPosition(cc.p(size.width/2,size.height/4))
  
  print("########2###",spriteFrame:getReferenceCount(),texture:getReferenceCount())		--3,32(spriteframe,texture:sprite也加入了一个引用)

  local listener = cc.EventListenerTouchOneByOne:create()
    local function onTouchesBegan(touch, event)
	--(spriteframe:变帧了之后自动释放了一个引用)
	--(texture： 变帧了之后计算没变的原因是texture不是autorelease的，如果不去释放它，它就会永驻内存)
     print("########3###",spriteFrame:getReferenceCount(),texture:getReferenceCount())		--2 32
        return true
    end
    
    listener:registerScriptHandler(onTouchesBegan, cc.Handler.EVENT_TOUCH_BEGAN)
    
    listener:setSwallowTouches(false)
    local eventDispatcher = cc.Director:getInstance():getEventDispatcher()
    eventDispatcher:addEventListenerWithFixedPriority(listener,  1000)
end

当removeUnusedSpriteFrames的时候，如果没有被其他sprite或者其他的引用到，那么有些spriteframe就会被释放掉。
可参考：https://github.com/cocos2d/cocos2d-x/issues/16719


牢记：
1:spriteFrame之间是没有关系的，不管它们是否来自同一张纹理图。如果没用到spriteFrame,那么removeUnusedSpriteFrames之后就会被释放掉。
2:每个spriteFrame都有一个纹理的引用。
3:纹理texture不会自动释放的。默认引用计数为1。removeUnusedTextures就是判断是否引用计数为1，说明它没有被用到。