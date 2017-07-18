#include "MyButton.h"
#include "../Common/AudioManager.h"
#include "../Scene/MenuScene.h"

using namespace cocos2d;

bool CMyButton::m_MultiTouchesDisable = false;
static const std::string SOUND_PATH = "sounds/";

CMyButton::CMyButton()
: m_Layer(nullptr)
, m_ButtonImage(nullptr)
, m_ContentsLabel(nullptr)
, m_ClickedAnimationCallBack(nullptr)
, m_ButtonNormalImage("")
, m_ButtonClickedImage("")
, m_Contents("")
, m_ClickBeganSound("")
, m_ClickEndedSound("")
, m_FontColor(Color3B::WHITE)
, m_FontSize(40)
, m_Position(Vec2::ZERO)
, m_AnchorPoint(Vec2::ZERO)
, m_Touchable(true) //TODO : 뺼지 생각해봄
, m_IsSelect(false) //TODO : 마찬가지
, m_Sound(true)
, m_SingleUse(false)
, m_ClickedAnimationInfo(eCLICKED_ANIMATION::ALPHA | eCLICKED_ANIMATION::SIZEDOWN)
{
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

bool CMyButton::init()
{
	if (!Widget::init())
		return false;

    this->setTouchEnabled(true);
	this->setCascadeOpacityEnabled(true);
	this->setCascadeColorEnabled(true);
    this->scheduleUpdate();
    
    m_BeginFunctionList.clear();
    m_ExecuteFunctionList.clear();
    m_EndFunctionList.clear();

    return true;
}

CMyButton* CMyButton::show(Node* parent, int zOrder/* = 0*/)
{
    if(m_Layer != nullptr)
    {
        this->setContentSize(m_Layer->getContentSize());
        m_Layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        m_Layer->setIgnoreAnchorPointForPosition(false);
        m_Layer->setPosition(this->getContentSize() / 2);
        this->addChild(m_Layer);
    }
    
    if(m_ButtonNormalImage != "")
    {
		m_ButtonImage = Sprite::create(m_ButtonNormalImage);
		this->setContentSize(m_ButtonImage->getContentSize());
		m_ButtonImage->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		m_ButtonImage->setPosition(this->getContentSize() / 2);
		this->addChild(m_ButtonImage);
    }
    
    if(m_Contents != "")
    {
        m_ContentsLabel = Label::createWithSystemFont(m_Contents, FONT::MALGUNBD, m_FontSize);
        if(!m_Layer && m_ButtonNormalImage == "")
            this->setContentSize(m_ContentsLabel->getContentSize() * 1.3f); // 터치 범위 보정
        else{
            m_ContentsLabel->setDimensions(this->getContentSize().width * 0.85f,
                                           this->getContentSize().height);
            m_ContentsLabel->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        }
        m_ContentsLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        m_ContentsLabel->setPosition(this->getContentSize() / 2);
        m_ContentsLabel->setColor(m_FontColor);
        this->addChild(m_ContentsLabel);
    }
    
    this->addTouchEventListener([=](Ref *pSender, Widget::TouchEventType type){
        this->onTouchEvent(pSender, type);
    });

    this->setAnchorPoint(m_AnchorPoint);
    this->setPosition(m_Position);
    parent->addChild(this, zOrder);
    
    return this;
}

CMyButton* CMyButton::addEventListener(const NODE_CALLBACK &callback,
                                       eMYBUTTON_STATE state/* = eMYBUTTON_STATE::END*/)
{
    switch (state)
    {
        case eMYBUTTON_STATE::BEGIN:    m_BeginFunctionList.emplace_back(callback);     break;
        case eMYBUTTON_STATE::EXECUTE:  m_ExecuteFunctionList.emplace_back(callback);   break;
        case eMYBUTTON_STATE::END:      m_EndFunctionList.emplace_back(callback);       break;
        default: throw "WRONG BUTTON STATE. PLEASE CHECK THE STATE.";
    }
    
    return this;
}

CMyButton* CMyButton::setClickedAnimation(const NODE_CALLBACK &callback)
{
	m_ClickedAnimationCallBack = callback;

	return this;
}

CMyButton* CMyButton::setLayer(LayerColor* layer)
{
    m_Layer = layer;
    
    return this;
}

CMyButton* CMyButton::setButtonNormalImage(std::string imgName)
{
    m_ButtonNormalImage = imgName;
    
    return this;
}

CMyButton* CMyButton::setButtonClickedImage(std::string imgName)
{
    m_ButtonClickedImage = imgName;
    
    return this;
}

CMyButton* CMyButton::setDefaultClickedAnimation(int animationInfo)
{
	m_ClickedAnimationInfo = animationInfo;

	return this;
}


CMyButton* CMyButton::setClickSound(std::string began, std::string ended/* = "" */)
{
	m_ClickBeganSound = began;
	m_ClickEndedSound = ended;

	return this;
}

CMyButton* CMyButton::setEnableSound(bool able)
{
    m_Sound = able;
    
    return this;
}

CMyButton* CMyButton::setTouchEnable(bool able, Color3B color/* = cocos2d::Color3B::GRAY*/)
{
	m_Touchable = able;

	if (!m_Touchable)
		this->setColor(color);
	else
		this->setColor(m_FontColor);

	return this;
}

CMyButton* CMyButton::setButtonSingleUse(bool singleUse)
{
    m_SingleUse = singleUse;
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

void CMyButton::changeButtonImage(std::string imageName)
{
    if(m_ButtonImage != nullptr)
    {
        m_ButtonImage->setTexture(imageName);
    }
}

/* 버튼 눌릴 때 Effect 실행*/
void CMyButton::playButtonStartAnimation()
{
	if (m_ClickedAnimationCallBack != nullptr)
	{
		this->retain();
		m_ClickedAnimationCallBack(this);
		this->release();
	}
	else{

		if (onEffect(eCLICKED_ANIMATION::ALPHA))    this->setOpacity(80.f);
		if (onEffect(eCLICKED_ANIMATION::SIZEUP))   this->setScale(1.2f);
		if (onEffect(eCLICKED_ANIMATION::SIZEDOWN)) this->setScale(0.9f);
		if (onEffect(eCLICKED_ANIMATION::GRAY))     this->setColor(Color3B::GRAY);
		if (onEffect(eCLICKED_ANIMATION::TEXTURE)) {
			if (m_ButtonImage && m_ButtonClickedImage != "")
				m_ButtonImage->setTexture(m_ButtonClickedImage);
		}
	}
}


/* 버튼 떨어질 때 Effect 실행 */
void CMyButton::playButtonEndAnimation()
{
	if (onEffect(eCLICKED_ANIMATION::ALPHA))    this->setOpacity(255.f);
	if (onEffect(eCLICKED_ANIMATION::SIZEUP))   this->setScale(1.0f);
	if (onEffect(eCLICKED_ANIMATION::SIZEDOWN)) this->setScale(1.0f);
	if (onEffect(eCLICKED_ANIMATION::GRAY))     this->setColor(Color3B::WHITE);
	if (onEffect(eCLICKED_ANIMATION::TEXTURE)) {
		if (m_ButtonImage && m_ButtonNormalImage != "")
			m_ButtonImage->setTexture(m_ButtonNormalImage);
	}
}

void CMyButton::playClickedSound(std::string name)
{
	if (name == "" || !m_Sound) return;
    
	CAudioManager::Instance()->PlayEffectSound(SOUND_PATH + name);
}

void CMyButton::onTouchEvent(Ref *pSender, Widget::TouchEventType type)
{
	// 터치불가 상태
	if (!m_Touchable) return;

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
//    if(m_MultiTouchesDisable) return false;
    
    // BEGIN 상태일때 호출해야할 함수가 있다면 호출
    
    for(auto func : m_BeginFunctionList)
        if (func != nullptr) func(this);
    

	this->playButtonStartAnimation();

	//this->playClickedSound(m_ClickBeganSound);
	if (CAudioManager::Instance())
		this->playClickedSound(CAudioManager::Instance()->PUBLIC_CLICK_SOUND.first);

    m_IsSelect = true;
//    m_MultiTouchesDisable = true;
    
    return m_IsSelect;
}

/* 버튼에서 떨어졌을 때 END */
void CMyButton::onTouchEnded()
{
    // END 상태일때 호출해야할 함수가 있다면 호출
    for(auto func : m_EndFunctionList)
    {
        if (func != nullptr) func(this);
        if (m_SingleUse) this->setTouchEnable(false, this->getColor());
    }
    
	// 종료 이펙트
	this->playButtonEndAnimation();
	
	//this->playClickedSound(m_ClickEndedSound);
	if (CAudioManager::Instance())
		this->playClickedSound(CAudioManager::Instance()->PUBLIC_CLICK_SOUND.second);

    // 버튼 눌림 종료
    m_IsSelect = false;
//    m_MultiTouchesDisable = false;
}

/* 버튼터치가 Cancel 됬을 때 */
void CMyButton::onTouchCancelled()
{
    // 종료 이펙트
	this->playButtonEndAnimation();
	
	//this->playClickedSound(m_ClickEndedSound);
	if (CAudioManager::Instance())
		this->playClickedSound(CAudioManager::Instance()->PUBLIC_CLICK_SOUND.second);

    // 버튼 눌림 종료
    m_IsSelect = false;
//    m_MultiTouchesDisable = false;
}

/* 버튼이 눌리고 있는 중일때 EXECUTE */
void CMyButton::update(float delta/* = 0*/)
{
	// 버튼 눌림 상태이며, EXECUTE 상태일 때 실행되어야 할 함수가 1개 이상일 때
	if (m_IsSelect && m_ExecuteFunctionList.size())
	{
		// EXECUTE 상태일때 호출해야할 함수가 있다면 호출
        for(auto func : m_ExecuteFunctionList)
        {
            if (func != nullptr) func(this);
        }
	}
}
