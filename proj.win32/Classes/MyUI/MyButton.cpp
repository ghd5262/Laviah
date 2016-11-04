#include "MyButton.h"

CMyButton::CMyButton()
: m_Layer(nullptr)
, m_TouchListener(nullptr)
, m_ButtonNormalImg("")
, m_ButtonClickedImg("")
, m_Contents("")
, m_FontColor(Color3B::WHITE)
, m_FontSize(40)
, m_Position(Vec2::ZERO)
, m_AnchorPoint(Vec2::ZERO)
, m_Unable(false) //TODO : 뺼지 생각해봄
, m_IsSelect(false) //TODO : 마찬가지
{
}

CMyButton::CMyButton(
	std::string textureName,					// 버튼의 텍스쳐 이름
	eMYBUTTON_STATE state,						// 상태 (해당 상태일 때 함수 호출됨)
	const BUTTON_CALLBACK &func,		// 람다 혹은 함수포인터 혹은 함수객체 전달(매개 변수는 void)
	int effect)									// 버튼 이펙트
	: m_NormalTextureName(textureName)
	, m_SelectedTextureName("")
	, m_LabelString("")
	, m_FontSize(0)
	, m_IsSelect(false)
	, m_pNormalTexture(nullptr)
	, m_pLabel(nullptr)
	, m_ButtonEffect(effect)
	, m_FontColor(Color3B::WHITE)
	, m_LayerBtn(nullptr)
	, m_ButtonKind(BUTTON_NORMAL)
	, m_Unable(false)
{
	AddState(state, func);
}

CMyButton::CMyButton(
	std::string normalTextureName,				// 선택 전 버튼의 텍스쳐 이름
	std::string selectedTextureName,			// 선택 중 버튼의 텍스쳐 이름
	eMYBUTTON_STATE state,						// 상태 (해당 상태일 때 함수 호출됨)
	const BUTTON_CALLBACK &func)		// 람다 혹은 함수포인터 혹은 함수객체 전달(매개 변수는 void)
	: m_NormalTextureName(normalTextureName)		
	, m_SelectedTextureName(selectedTextureName)
	, m_LabelString("")
	, m_FontSize(0)
	, m_IsSelect(false)
	, m_pNormalTexture(nullptr)
	, m_pLabel(nullptr)
	, m_ButtonEffect(EFFECT_TEXTURE)
	, m_FontColor(Color3B::WHITE)
	, m_LayerBtn(nullptr)
	, m_ButtonKind(BUTTON_SPRITE)
	, m_Unable(false)
{
	AddState(state, func);
}

CMyButton::CMyButton(
	std::string normalTextureName,				// 버튼의 텍스쳐 이름
	std::string labelString,					// 버튼의 label 내용
	int fontSize,								// 폰트 사이즈
	Color3B fontColor,							// 폰트 색상
	eMYBUTTON_STATE state,						// 상태 (해당 상태일 때 함수 호출됨)
	const BUTTON_CALLBACK &func,		// 람다 혹은 함수포인터 혹은 함수객체 전달(매개 변수는 void)
	int effect)									// 버튼 이펙트
	: m_NormalTextureName(normalTextureName)
	, m_SelectedTextureName("")
	, m_LabelString(labelString)
	, m_FontSize(fontSize)
	, m_IsSelect(false)
	, m_pNormalTexture(nullptr)
	, m_pLabel(nullptr)
	, m_ButtonEffect(effect)
	, m_FontColor(fontColor)
	, m_LayerBtn(nullptr)
	, m_ButtonKind(BUTTON_STRING)
	, m_Unable(false)
{
	AddState(state, func);
}

CMyButton::CMyButton(
	Size layerSize,								// 레이어 사이즈
	Color4B layerColor,							// 레이어 색상
	std::string labelString,					// 버튼의 label 내용
	int fontSize,								// 폰트 사이즈
	Color3B fontColor,							// 폰트 색상
	eMYBUTTON_STATE state,						// 상태 (해당 상태일 때 함수 호출됨)
	const BUTTON_CALLBACK &func,		// 람다 혹은 함수포인터 혹은 함수객체 전달(매개 변수는 void)
	int effect)									// 버튼 이펙트
	: m_LayerColor(layerColor)
	, m_LayerSize(layerSize)
	, m_NormalTextureName("")
	, m_SelectedTextureName("")
	, m_LabelString(labelString)
	, m_FontSize(fontSize)
	, m_IsSelect(false)
	, m_pNormalTexture(nullptr)
	, m_pLabel(nullptr)
	, m_ButtonEffect(effect)
	, m_FontColor(fontColor)
	, m_LayerBtn(nullptr)
	, m_ButtonKind(BUTTON_LAYER)
	, m_Unable(false)
{
	AddState(state, func);
}

CMyButton* CMyButton::create()
{
    CMyButton *pRet = new(std::nothrow) CMyButton();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

CMyButton* CMyButton::create(
	std::string textureName,
	eMYBUTTON_STATE state,
	const BUTTON_CALLBACK &func,
	int effect/* = NONE*/)
{
//	CMyButton *pRet = new(std::nothrow) CMyButton(textureName,
//		state,
//		func,
//		effect);
//	if (pRet && pRet->init())
//	{
//		pRet->autorelease();
//		return pRet;
//	}
//	else
//	{
//		delete pRet;
//		pRet = NULL;
//		return NULL;
//	}
    
    return CMyButton::create()
    ->addEventListener([=](Ref* sender, Widget::TouchEventType type){
        func(static_cast<Node*>(sender));
    }, state)
    ->setButtonNormalImage(textureName);
}

CMyButton* CMyButton::createWithTexture(
	std::string normalTextureName,
	std::string selectedTextureName,
	eMYBUTTON_STATE state,
	const BUTTON_CALLBACK &func)
{
//	CMyButton *pRet = new(std::nothrow) CMyButton(normalTextureName,
//		selectedTextureName,
//		state,
//		func);
//	if (pRet && pRet->init())
//	{
//		pRet->autorelease();
//		return pRet;
//	}
//	else
//	{
//		delete pRet;
//		pRet = NULL;
//		return NULL;
//	}
    
    return CMyButton::create()
    ->addEventListener([=](Ref* sender, Widget::TouchEventType type){
        func(static_cast<Node*>(sender));
    }, state)
    ->setButtonNormalImage(normalTextureName);
}

CMyButton* CMyButton::createWithString(
	std::string normalTextureName,			
	std::string labelString,				
	int fontSize,							
	Color3B fontColor,						
	eMYBUTTON_STATE state,					
	const BUTTON_CALLBACK &func,	
	int effect/* = NONE*/)					
{
//	CMyButton *pRet = new(std::nothrow) CMyButton(normalTextureName,
//		labelString,
//		fontSize,
//		fontColor,
//		state,
//		func,
//		effect);
//	if (pRet && pRet->init())
//	{
//		pRet->autorelease();
//		return pRet;
//	}
//	else
//	{
//		delete pRet;
//		pRet = NULL;
//		return NULL;
//	}
    
    return CMyButton::create()
    ->addEventListener([=](Ref* sender, Widget::TouchEventType type){
        func(static_cast<Node*>(sender));
    }, state)
    ->setButtonNormalImage(normalTextureName)
    ->setContents(labelString)
    ->setFont(fontColor, fontSize);
}

/* LayerColor와 함께 버튼 생성*/
CMyButton* CMyButton::createWithLayerColor(
	Size layerSize,
	Color4B layerColor,
	std::string labelString,
	int fontSize,
	Color3B fontColor,
	eMYBUTTON_STATE state,
	const BUTTON_CALLBACK &func,
	int effect/* = NONE*/)
{
//	CMyButton *pRet = new(std::nothrow) CMyButton(layerSize,
//		layerColor,
//		labelString,
//		fontSize,
//		fontColor,
//		state,
//		func,
//		effect);
//	if (pRet && pRet->init())
//	{
//		pRet->autorelease();
//		return pRet;
//	}
//	else
//	{
//		delete pRet;
//		pRet = NULL;
//		return NULL;
//	}
    
    return CMyButton::create()
    ->addEventListener([=](Ref* sender, Widget::TouchEventType type){
        func(static_cast<Node*>(sender));
    }, state)
    ->setLayer(LayerColor::create(layerColor, layerSize.width, layerSize.height))
    ->setContents(labelString)
    ->setFont(fontColor, fontSize);
}


bool CMyButton::init()
{
	if (!Widget::init())
		return false;

    this->setTouchEnabled(true);
    this->scheduleUpdate();
    
    m_BeginFunctionList.clear();
    m_ExecuteFunctionList.clear();
    m_EndFunctionList.clear();
    
    m_BeginFuncList.clear();
    m_ExecuteFuncList.clear();
    m_EndFuncList.clear();
    
    return true;
}

bool CMyButton::initVariable()
{
    return true;
	try{
        if (Widget::init())
        {
            this->scheduleUpdate();
            this->addTouchEventListener(CC_CALLBACK_2(CMyButton::onTouchEvent, this));
            this->setTouchEnabled(true);
            
            m_BeginFuncList.reserve(5);
            m_ExecuteFuncList.reserve(5);
            m_EndFuncList.reserve(5);
            
            if (m_NormalTextureName != "")
                m_pNormalTexture = Sprite::create(m_NormalTextureName);
            if (m_pNormalTexture != nullptr){
                this->setContentSize(m_pNormalTexture->getContentSize());
                
                m_pNormalTexture->setPosition(Vec2(this->getContentSize().width * 0.5f, this->getContentSize().height * 0.5f));
                m_pNormalTexture->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                addChild(m_pNormalTexture);
            }
            
            if (m_ButtonKind == BUTTON_LAYER)
                m_LayerBtn = LayerColor::create(m_LayerColor, m_LayerSize.width, m_LayerSize.height);
            if (m_LayerBtn != nullptr)
            {
                this->setContentSize(m_LayerBtn->getContentSize());
                
                m_LayerBtn->setPosition(Vec2(this->getContentSize().width * 0.5f, this->getContentSize().height * 0.5f));
                m_LayerBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                m_LayerBtn->setIgnoreAnchorPointForPosition(false);
                addChild(m_LayerBtn);
            }
            
            if (m_LabelString != "")
                m_pLabel = cocos2d::Label::createWithTTF(m_LabelString, "fonts/malgunbd.ttf", m_FontSize);
            if (m_pLabel != nullptr){
                m_pLabel->setPosition(Vec2(this->getContentSize().width * 0.5f, this->getContentSize().height * 0.5f));
                m_pLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                m_pLabel->setColor(m_FontColor);
                addChild(m_pLabel);
            }
            
            
        }
    }
    catch (...){
        throw StringUtils::format("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTION__, __LINE__);
		return false;
	}
	return true;
}

CMyButton* CMyButton::show(Node* parent, int zOrder/* = 0*/)
{
    auto createContentLabel = [=](Node* sender) {
        if(m_Contents != "")
        {
            m_ContentsLabel = Label::createWithTTF(m_Contents, "fonts/malgunbd.ttf", m_FontSize);
            m_ContentsLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            m_ContentsLabel->setPosition(sender->getContentSize() / 2);
            m_ContentsLabel->setColor(m_FontColor);
            sender->addChild(m_ContentsLabel);
        }
    };
    
    if(m_Layer != nullptr)
    {
        this->setContentSize(m_Layer->getContentSize());
        m_Layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        m_Layer->setIgnoreAnchorPointForPosition(false);
        m_Layer->setPosition(this->getContentSize() / 2);
        this->addChild(m_Layer);
        
        createContentLabel(m_Layer);
    }
    
    if(m_ButtonNormalImg != "")
    {
        auto image = Sprite::create(m_ButtonNormalImg);
        this->setContentSize(image->getContentSize());
        image->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        image->setPosition(this->getContentSize() / 2);
        this->addChild(image);
        
        createContentLabel(image);
    }
    
    if(m_ButtonClickedImg != "")
    {
//        auto image = Sprite::create(m_ButtonNormalImg);
//        image->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//        image->setPosition(this->getContentSize() / 2);
//        this->addChild(image);
//        createContentLabel(image);
//        this->loadTexturePressed(m_ButtonClickedImg);
    }

    this->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
        this->onTouchEvent(pSender, type);
    });
    
    this->setAnchorPoint(m_AnchorPoint);
    this->setPosition(m_Position);
    parent->addChild(this, zOrder);
    
    return this;
}

CMyButton* CMyButton::addEventListener(const ccWidgetTouchCallback &callback,
                                       eMYBUTTON_STATE state/* = eMYBUTTON_STATE::END*/)
{
    switch (state)
    {
        case eMYBUTTON_STATE::BEGIN: m_BeginFunctionList.emplace_back(callback); break;
        case eMYBUTTON_STATE::EXECUTE: m_ExecuteFunctionList.emplace_back(callback); break;
        case eMYBUTTON_STATE::END: m_EndFunctionList.emplace_back(callback); break;
        default: throw "WRONG BUTTON STATE. PLEASE CHECK THE STATE.";
    }
    
    return this;
}

CMyButton* CMyButton::setLayer(LayerColor* layer)
{
    m_Layer = layer;
    
    return this;
}

CMyButton* CMyButton::setButtonNormalImage(std::string imgName)
{
    m_ButtonNormalImg = imgName;
    
    return this;
}

CMyButton* CMyButton::setButtonClickedImage(std::string imgName)
{
    m_ButtonClickedImg = imgName;
    
    return this;
}

CMyButton* CMyButton::setContents(std::string contents)
{
    m_Contents = contents;
    
    return this;
}

CMyButton* CMyButton::setFont(Color3B fontColor, int fontSize)
{
    m_FontColor = fontColor;
    m_FontSize = fontSize;
    
    return this;
}

CMyButton* CMyButton::setButtonPosition(cocos2d::Vec2 position)
{
    m_Position = position;
    
    return this;
}

CMyButton* CMyButton::setButtonAnchorPoint(cocos2d::Vec2 anchorPoint)
{
    m_AnchorPoint = anchorPoint;
    
    return this;
}

void CMyButton::changeContents(std::string contents)
{
    if(m_ContentsLabel != nullptr)
    {
        m_ContentsLabel->setString(contents);
    }
}

void CMyButton::changeFontColor(cocos2d::Color3B fontColor)
{
    if(m_ContentsLabel != nullptr)
    {
        m_ContentsLabel->setColor(fontColor);
    }
}


/* 상태 (해당 상태일 때 함수 호출됨)
 * 람다 혹은 함수포인터 혹은 함수객체 전달(매개 변수는 void) */
void CMyButton::AddState(eMYBUTTON_STATE state,
	const BUTTON_CALLBACK &func)
{
	switch (state)
	{
        case eMYBUTTON_STATE::BEGIN: m_BeginFuncList.emplace_back(func); break;
        case eMYBUTTON_STATE::EXECUTE: m_ExecuteFuncList.emplace_back(func); break;
        case eMYBUTTON_STATE::END: m_EndFuncList.emplace_back(func); break;
        default: throw "WRONG BUTTON STATE. PLEASE CHECK THE STATE.";
	}
}


/* 버튼 눌릴 때 Effect 실행*/
void CMyButton::btnEffectStart()
{
    return; // TODO: 빼야함
    
	if (onEffect(EFFECT_ALPHA))
	{
		if (m_pNormalTexture != nullptr)
			m_pNormalTexture->setOpacity(80.f);
		if (m_pLabel != nullptr)
			m_pLabel->setOpacity(80.f);
		if (m_ButtonKind == BUTTON_LAYER)
			m_LayerBtn->setOpacity(80.f);
	}

	if (onEffect(EFFECT_SIZEUP))
	{
		if (m_pNormalTexture != nullptr)
			m_pNormalTexture->setScale(1.1f);
		if (m_pLabel != nullptr)
			m_pLabel->setScale(1.1f);
		if (m_ButtonKind == BUTTON_LAYER)
			m_LayerBtn->setScale(1.1f);
	}

	if (onEffect(EFFECT_SIZEDOWN))
	{
		if (m_pNormalTexture != nullptr)
			m_pNormalTexture->setScale(0.9f);
		if (m_pLabel != nullptr)
			m_pLabel->setScale(0.9f);
		if (m_ButtonKind == BUTTON_LAYER)
			m_LayerBtn->setScale(0.9f);
	}

	if (onEffect(EFFECT_GRAY))
	{
		if (m_pNormalTexture != nullptr)
			m_pNormalTexture->setColor(Color3B::GRAY);
		if (m_pLabel != nullptr)
			m_pLabel->setColor(Color3B::GRAY);
        if (m_ButtonKind == BUTTON_LAYER){
			m_LayerBtn->setColor(Color3B::GRAY);
            m_LayerBtn->setOpacity(m_LayerColor.a);
        }
	}

	if (onEffect(EFFECT_TEXTURE))
	{
		if (m_ButtonKind == BUTTON_SPRITE){
			if (m_pNormalTexture != nullptr)
				m_pNormalTexture->setTexture(m_SelectedTextureName);
		}
	}
}


/* 버튼 떨어질 때 Effect 실행 */
void CMyButton::btnEffectEnd()
{
    return; // TODO: 빼야함
	// 원 상태로 복구
	if (m_pNormalTexture != nullptr)
	{
		m_pNormalTexture->setOpacity(255.f);
		m_pNormalTexture->setColor(Color3B::WHITE);
		m_pNormalTexture->setScale(1.f);
		if (m_ButtonKind == BUTTON_SPRITE){
			m_pNormalTexture->setTexture(m_NormalTextureName);
		}
	}
	if (m_pLabel != nullptr){
		m_pLabel->setOpacity(255.f);
		m_pLabel->setColor(m_FontColor);
		m_pLabel->setScale(1.f);
	}
	if (m_ButtonKind == BUTTON_LAYER)
	{
		m_LayerBtn->setOpacity(m_LayerColor.a);
		m_LayerBtn->setColor(Color3B(m_LayerColor.r, m_LayerColor.g, m_LayerColor.b));
		m_LayerBtn->setScale(1.f);
	}
}

void CMyButton::onTouchEvent(Ref *pSender, Widget::TouchEventType type)
{
    switch(type)
    {
        case Widget::TouchEventType::BEGAN:     this->onTouchBegan(); break;
        case Widget::TouchEventType::ENDED:     this->onTouchEnded(); break;
        case Widget::TouchEventType::CANCELED:  this->onTouchCancelled(); break;
        default: break;
    }
}

/* 버튼이 눌렸을 때 BEGIN */
bool CMyButton::onTouchBegan()
{
    // 터치불가 상태
    if (m_Unable) return false;
    
    // BEGIN 상태일때 호출해야할 함수가 있다면 호출
    if(m_BeginFunctionList.size() > 0){
        
        std::for_each(m_BeginFunctionList.begin(), m_BeginFunctionList.end(),
                      [=](const ccWidgetTouchCallback &func){
                          func(this, TouchEventType::BEGAN);
                      });
    }
    btnEffectStart();
    m_IsSelect = true;
    
    return m_IsSelect;
}

/* 버튼에서 떨어졌을 때 END */
void CMyButton::onTouchEnded()
{
    // 터치불가 상태
    if (m_Unable) return;
	
    // END 상태일때 호출해야할 함수가 있다면 호출
    if(m_EndFunctionList.size() > 0){
        
        std::for_each(m_EndFunctionList.begin(), m_EndFunctionList.end(),
                      [=](const ccWidgetTouchCallback &func){
                          func(this, TouchEventType::ENDED);
                      });
    }
    // 종료 이펙트
    btnEffectEnd();
    
    // 버튼 눌림 종료
    m_IsSelect = false;
}

/* 버튼터치가 Cancel 됬을 때 */
void CMyButton::onTouchCancelled()
{
    // 터치불가 상태
    if (m_Unable) return;
    
    // 종료 이펙트
    btnEffectEnd();
    
    // 버튼 눌림 종료
    m_IsSelect = false;
}

/* 버튼이 눌리고 있는 중일때 EXECUTE */
void CMyButton::update(float delta/* = 0*/)
{
	// 터치불가 상태 
	if (m_Unable) return;

	// 버튼 눌림 상태이며, EXECUTE 상태일 때 실행되어야 할 함수가 1개 이상일 때
	if (m_IsSelect && m_ExecuteFunctionList.size())
	{
		// EXECUTE 상태일때 호출해야할 함수가 있다면 호출
		std::for_each(m_ExecuteFunctionList.begin(), m_ExecuteFunctionList.end(),
			[=](const ccWidgetTouchCallback &func){
			func(this, TouchEventType::MOVED);
		});
	}
}

void CMyButton::setBtnUnable(bool unable)
{
	m_Unable = unable;
	if (m_Unable)
	{
		if (m_pNormalTexture != nullptr)
			m_pNormalTexture->setColor(Color3B::GRAY);
		if (m_pLabel != nullptr)
			m_pLabel->setColor(Color3B::GRAY);
        if (m_ButtonKind == BUTTON_LAYER){
			m_LayerBtn->setColor(Color3B::GRAY);
            m_LayerBtn->setOpacity(m_LayerColor.a);
        }
	}
	else
	{
		if (m_pNormalTexture != nullptr)
			m_pNormalTexture->setColor(Color3B::WHITE);
		if (m_pLabel != nullptr)
			m_pLabel->setColor(m_FontColor);
		if (m_ButtonKind == BUTTON_LAYER)
			m_LayerBtn->setColor(Color3B::WHITE);
	}
}