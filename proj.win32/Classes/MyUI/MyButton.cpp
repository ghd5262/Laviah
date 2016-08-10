#include "MyButton.h"

CMyButton::CMyButton(
	std::string textureName,					// 버튼의 텍스쳐 이름
	eMYBUTTON_STATE state,						// 상태 (해당 상태일 때 함수 호출됨)
	const std::function<void(void)> &func,		// 람다 혹은 함수포인터 혹은 함수객체 전달(매개 변수는 void)
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
	const std::function<void(void)> &func)		// 람다 혹은 함수포인터 혹은 함수객체 전달(매개 변수는 void)
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
	const std::function<void(void)> &func,		// 람다 혹은 함수포인터 혹은 함수객체 전달(매개 변수는 void)
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
	const std::function<void(void)> &func,		// 람다 혹은 함수포인터 혹은 함수객체 전달(매개 변수는 void)
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

CMyButton* CMyButton::create(
	std::string textureName,
	eMYBUTTON_STATE state,
	const std::function<void(void)> &func,
	int effect/* = NONE*/)
{
	CMyButton *pRet = new(std::nothrow) CMyButton(textureName,
		state,
		func,
		effect);
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

CMyButton* CMyButton::createWithTexture(
	std::string normalTextureName,
	std::string selectedTextureName,
	eMYBUTTON_STATE state,
	const std::function<void(void)> &func)
{
	CMyButton *pRet = new(std::nothrow) CMyButton(normalTextureName,
		selectedTextureName,
		state,
		func);
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

CMyButton* CMyButton::createWithString(
	std::string normalTextureName,			
	std::string labelString,				
	int fontSize,							
	Color3B fontColor,						
	eMYBUTTON_STATE state,					
	const std::function<void(void)> &func,	
	int effect/* = NONE*/)					
{
	CMyButton *pRet = new(std::nothrow) CMyButton(normalTextureName,
		labelString,
		fontSize,
		fontColor,
		state,
		func,
		effect);
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

/* LayerColor와 함께 버튼 생성*/
CMyButton* CMyButton::createWithLayerColor(
	Size layerSize,
	Color4B layerColor,
	std::string labelString,
	int fontSize,
	Color3B fontColor,
	eMYBUTTON_STATE state,
	const std::function<void(void)> &func,
	int effect/* = NONE*/)
{
	CMyButton *pRet = new(std::nothrow) CMyButton(layerSize,
		layerColor,
		labelString,
		fontSize,
		fontColor,
		state,
		func,
		effect);
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


bool CMyButton::init()
{
	if (!initVariable())
		return false;
	return true;
}

bool CMyButton::initVariable()
{
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

/* 상태 (해당 상태일 때 함수 호출됨)
 * 람다 혹은 함수포인터 혹은 함수객체 전달(매개 변수는 void) */
void CMyButton::AddState(eMYBUTTON_STATE state,
	const std::function<void(void)> &func)
{
	switch (state)
	{
	case BEGIN:
	{
		m_BeginFuncList.emplace_back(func);
	}break;
	case EXECUTE:
	{
		m_ExecuteFuncList.emplace_back(func);
	}break;
	case END:
	{
		m_EndFuncList.emplace_back(func);
	}break;
	default:
	{
		throw "WRONG BUTTON STATE. PLEASE CHECK THE STATE.";
	}
	}
}


/* 버튼 눌릴 때 Effect 실행*/
void CMyButton::btnEffectStart()
{
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
		if (m_ButtonKind == BUTTON_LAYER)
			m_LayerBtn->setColor(Color3B::GRAY);
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
    switch (type)
    {
        case Widget::TouchEventType::BEGAN:
            onTouchBegan();
            break;
            
        case Widget::TouchEventType::ENDED:
            onTouchEnded();
            break;
            
        case Widget::TouchEventType::CANCELED:
            onTouchCancelled();
            break;
            
        default:
            break;
    }
}

/* 버튼이 눌렸을 때 BEGIN */
bool CMyButton::onTouchBegan()
{
	if (!m_Unable){
        
        // BEGIN 상태일때 호출해야할 함수가 있다면 호출
        if(m_BeginFuncList.size() > 0){
            
            std::for_each(m_BeginFuncList.begin(), m_BeginFuncList.end(),
                          [](const std::function<void(void)> &func){
                              func();
                          });
        }
        btnEffectStart();
        m_IsSelect = true;
    }
	return m_IsSelect;
}

/* 버튼에서 떨어졌을 때 END */
void CMyButton::onTouchEnded()
{
	if (!m_Unable){

        // END 상태일때 호출해야할 함수가 있다면 호출
        if(m_EndFuncList.size() > 0){
            
            std::for_each(m_EndFuncList.begin(), m_EndFuncList.end(),
                          [](const std::function<void(void)> &func){
                              func();
                          });
        }
        // 종료 이펙트
        btnEffectEnd();
        
        // 버튼 눌림 종료
        m_IsSelect = false;
    }
}

/* 버튼터치가 Cancel 됬을 때 */
void CMyButton::onTouchCancelled()
{
    if (!m_Unable){
        // 종료 이펙트
        btnEffectEnd();
        
        // 버튼 눌림 종료
        m_IsSelect = false;
    }
}

/* 버튼이 눌리고 있는 중일때 EXECUTE */
void CMyButton::update(float delta)
{
	// 터치불가 상태 
	if (m_Unable) return;

	// 버튼 눌림 상태이며, EXECUTE 상태일 때 실행되어야 할 함수가 1개 이상일 때
	if (m_IsSelect && m_ExecuteFuncList.size())
	{
		// EXECUTE 상태일때 호출해야할 함수가 있다면 호출
		std::for_each(m_ExecuteFuncList.begin(), m_ExecuteFuncList.end(),
			[](const std::function<void(void)> &func){
			func();
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
		if (m_ButtonKind == BUTTON_LAYER)
			m_LayerBtn->setColor(Color3B::GRAY);
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