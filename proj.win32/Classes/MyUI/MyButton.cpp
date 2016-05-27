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

bool CMyButton::init()
{
	if (!initVariable())
		return false;
	return true;
}

bool CMyButton::initVariable()
{
	try{
		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		listener->onTouchBegan = CC_CALLBACK_2(CMyButton::onTouchBegan, this);
		listener->onTouchEnded = CC_CALLBACK_2(CMyButton::onTouchEnded, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

		m_BeginFuncList.reserve(5);
		m_ExecuteFuncList.reserve(5);
		m_EndFuncList.reserve(5);

		if (m_NormalTextureName != "")
			m_pNormalTexture = Sprite::create(m_NormalTextureName);
		if (m_pNormalTexture != nullptr){
			m_pNormalTexture->setAnchorPoint(Vec2(0.5f, 0.5f));
			addChild(m_pNormalTexture);
		}

		if (m_LabelString != "")
			m_pLabel = cocos2d::Label::createWithTTF(m_LabelString, "fonts/malgunbd.ttf", m_FontSize);
		if (m_pLabel != nullptr){
			m_pLabel->setAnchorPoint(Vec2(0.5f, 0.5f));
			m_pLabel->setColor(m_FontColor);
			addChild(m_pLabel);
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
	if (m_pNormalTexture == nullptr)
		CCASSERT(false, "WRONG ACCESS PLEASE CHECK THE BTN TEXTURE");

	if (onEffect(EFFECT_ALPHA))
	{
		m_pNormalTexture->setOpacity(80.f);
		if (m_pLabel != nullptr)
			m_pLabel->setOpacity(80.f);
	}

	if (onEffect(EFFECT_SIZEUP))
	{
		m_pNormalTexture->setScale(1.1f);
		if (m_pLabel != nullptr)
			m_pLabel->setScale(1.1f);
	}

	if (onEffect(EFFECT_SIZEDOWN))
	{
		m_pNormalTexture->setScale(0.9f);
		if (m_pLabel != nullptr)
			m_pLabel->setScale(0.9f);
	}

	if (onEffect(EFFECT_GRAY))
	{
		m_pNormalTexture->setColor(Color3B::GRAY);
		if (m_pLabel != nullptr)
			m_pLabel->setColor(Color3B::GRAY);
	}

	if (onEffect(EFFECT_TEXTURE))
	{
		if (m_SelectedTextureName != ""){
			m_pNormalTexture->setTexture(m_SelectedTextureName);
		}
	}
}


/* 버튼 떨어질 때 Effect 실행 */
void CMyButton::btnEffectEnd()
{
	if (m_pNormalTexture == nullptr)
		CCASSERT(false, "WRONG ACCESS PLEASE CHECK THE BTN TEXTURE");

	// 원 상태로 복구
	m_pNormalTexture->setOpacity(255.f);
	m_pNormalTexture->setColor(Color3B::WHITE);
	m_pNormalTexture->setScale(1.f);

	if (m_pLabel != nullptr){
		m_pLabel->setOpacity(255.f);
		m_pLabel->setColor(m_FontColor);
		m_pLabel->setScale(1.f);
	}

	if (m_SelectedTextureName != ""){
		m_pNormalTexture->setTexture(m_NormalTextureName);
	}
}


/* 터치된 좌표를 버튼의 좌표로 변환 한 후에 버튼이 터치되었는지 검사 */
bool CMyButton::touchHits(Touch  *touch)
{
	const Rect area(0, 0, m_pNormalTexture->getContentSize().width,
		m_pNormalTexture->getContentSize().height);

	// world to nodespace 좌표 변환
	return area.containsPoint(m_pNormalTexture->convertToNodeSpace(touch->getLocation()));
}

/* 버튼이 눌렸을 때 BEGIN */
bool CMyButton::onTouchBegan(Touch  *touch, Event  *event)
{
	CC_UNUSED_PARAM(event);
	if (m_pNormalTexture){

		// 좌표 변환 후 터치 체크
		m_IsSelect = touchHits(touch);

		// 버튼의 좌표에서 진짜로 터치되었을 경우
		if (m_IsSelect){

			// BEGIN 상태일때 호출해야할 함수가 있다면 호출
			std::for_each(m_BeginFuncList.begin(), m_BeginFuncList.end(),
				[](const std::function<void(void)> &func){
				func();
			});

			btnEffectStart();
		}
	}
	return m_IsSelect;
}

/* 버튼에서 떨어졌을 때 END */
void CMyButton::onTouchEnded(Touch  *touch, Event  *event)
{
	CC_UNUSED_PARAM(event);
	if (m_pNormalTexture){

		// 좌표 변환 후 터치 체크
		m_IsSelect = touchHits(touch);

		// 버튼의 좌표에서 진짜로 터치되었을 경우
		if (m_IsSelect){

			// END 상태일때 호출해야할 함수가 있다면 호출
			std::for_each(m_EndFuncList.begin(), m_EndFuncList.end(),
				[](const std::function<void(void)> &func){
				func();
			});

		}
		btnEffectEnd();

		// 버튼 눌림 종료
		m_IsSelect = false;
	}
}

/* 버튼이 눌리고 있는 중일때 EXECUTE */
void CMyButton::Execute(float delta)
{
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

