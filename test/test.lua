local function testSpriteFrame(scene)
  cc.SpriteFrameCache:getInstance():addSpriteFrames("texture.plist","texture.png")
  local spriteFrame = cc.SpriteFrameCache:getInstance():getSpriteFrame("02.png")
  local texture = spriteFrame:getTexture()
  --(texture:�Լ��������ü���Ϊ1����Ϊ��30��spriteFrame,ÿ��spriteFrame��ӵ��һ������)
  --(spriteframe:�Լ��������ü���Ϊ1�����뵽��SpriteFrameCache��hash�����ּ���һ������)
  print("########1###",spriteFrame:getReferenceCount(),texture:getReferenceCount())  --  2,31 
  local sprite = cc.Sprite:createWithSpriteFrameName("ui/prime/hero/strengthen/main/pic/02.png")
  scene:addChild(sprite)
  local size = cc.Director:getInstance():getWinSize()
  sprite:setPosition(cc.p(size.width/2,size.height/4))
  
  print("########2###",spriteFrame:getReferenceCount(),texture:getReferenceCount())		--3,32(spriteframe,texture:spriteҲ������һ������)

  local listener = cc.EventListenerTouchOneByOne:create()
    local function onTouchesBegan(touch, event)
	--(spriteframe:��֡��֮���Զ��ͷ���һ������)
	--(texture�� ��֡��֮�����û���ԭ����texture����autorelease�ģ������ȥ�ͷ��������ͻ���פ�ڴ�)
     print("########3###",spriteFrame:getReferenceCount(),texture:getReferenceCount())		--2 32
        return true
    end
    
    listener:registerScriptHandler(onTouchesBegan, cc.Handler.EVENT_TOUCH_BEGAN)
    
    listener:setSwallowTouches(false)
    local eventDispatcher = cc.Director:getInstance():getEventDispatcher()
    eventDispatcher:addEventListenerWithFixedPriority(listener,  1000)
end

��removeUnusedSpriteFrames��ʱ�����û�б�����sprite�������������õ�����ô��Щspriteframe�ͻᱻ�ͷŵ���
�ɲο���https://github.com/cocos2d/cocos2d-x/issues/16719


�μǣ�
1:spriteFrame֮����û�й�ϵ�ģ����������Ƿ�����ͬһ������ͼ�����û�õ�spriteFrame,��ôremoveUnusedSpriteFrames֮��ͻᱻ�ͷŵ���
2:ÿ��spriteFrame����һ����������á�
3:����texture�����Զ��ͷŵġ�Ĭ�����ü���Ϊ1��removeUnusedTextures�����ж��Ƿ����ü���Ϊ1��˵����û�б��õ���