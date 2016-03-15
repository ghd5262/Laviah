#include "MyButton.h"


CMyButton::CMyButton(
	std::string normalTextureName,						// 선택 전 버튼의 텍스쳐 이름
	std::string selectedTextureName,					// 선택 중 버튼의 텍스쳐 이름
	eMYBUTTON_STATE state,								// 상태 (해당 상태일 때 함수 호출됨)
	const std::function<void(void)> &func)				// 람다 전달
	: m_NormalTextureName(normalTextureName)
	, m_SelectedTextureName(selectedTextureName)
	, m_LabelString("")
	, m_FontSize(0)
	, m_IsSelect(false)
	, m_pNormalTexture(nullptr)
	, m_pLabel(nullptr)
{
	for (int i = 0; i < MYBUTTON_STATE_COUNT; i++)
	{
		m_FuncList[i] = nullptr;
	}
	AddState(state, func);
}

CMyButton::CMyButton(
	std::string normalTextureName,						// 버튼의 텍스쳐 이름
	std::string labelString,							// 버튼의 label 내용
	int fontSize,										// 폰트 사이즈
	eMYBUTTON_STATE state,								// 상태 (해당 상태일 때 함수 호출됨)
	const std::function<void(void)> &func)				// 람다 전달
	: m_NormalTextureName("")
	, m_SelectedTextureName("")
	, m_LabelString(labelString)
	, m_FontSize(fontSize)
	, m_IsSelect(false)
	, m_pNormalTexture(nullptr)
	, m_pLabel(nullptr)
{
	for (int i = 0; i < MYBUTTON_STATE_COUNT; i++)
	{
		m_FuncList[i] = nullptr;
	}
	AddState(state, func);
}

CMyButton* CMyButton::createWithTexture(
	std::string normalTextureName,						// 선택 전 버튼의 텍스쳐 이름
	std::string selectedTextureName,					// 선택 중 버튼의 텍스쳐 이름
	eMYBUTTON_STATE state,								// 상태 (해당 상태일 때 함수 호출됨)
	const std::function<void(void)> &func)				// 람다 전달
{
	CMyButton *pRet = new(std::nothrow) CMyButton(
		normalTextureName, 
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
	std::string normalTextureName,						// 버튼의 텍스쳐 이름
	std::string labelString,							// 버튼의 label 내용
	int fontSize,										// 폰트 사이즈
	eMYBUTTON_STATE state,								// 상태 (해당 상태일 때 함수 호출됨)
	const std::function<void(void)> &func)				// 람다 전달
{
	CMyButton *pRet = new(std::nothrow) CMyButton(
		normalTextureName,
		labelString,
		fontSize, 
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

		if (m_NormalTextureName != "")
			m_pNormalTexture = Sprite::create(m_NormalTextureName);
		if (m_pNormalTexture != nullptr){
			m_pNormalTexture->setAnchorPoint(Vec2(0.5f, 0.5f));
			addChild(m_pNormalTexture);
		}

		if (m_LabelString != "")
			m_pLabel = cocos2d::Label::createWithTTF(m_LabelString, "fonts/Marker Felt.ttf", m_FontSize);
		if (m_pLabel != nullptr){
			m_pLabel->setAnchorPoint(Vec2(0.5f, 0.5f));
			addChild(m_pLabel);
		}

	}
	catch (...){
		CCLOG("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTIONW__, __LINE__);
		assert(false);
		return false;
	}
	return true;
}

void CMyButton::AddState(eMYBUTTON_STATE state,			// 상태 (해당 상태일 때 함수 호출됨)
	const std::function<void(void)> &func)				// 람다 전달
{
	m_FuncList[state] = func;
}

bool CMyButton::touchHits(Touch  *touch)
{
	const Rect area(0, 0, m_pNormalTexture->getContentSize().width, 
		m_pNormalTexture->getContentSize().height);
	// 좌표 변환
	return area.containsPoint(m_pNormalTexture->convertToNodeSpace(touch->getLocation()));
}

bool CMyButton::onTouchBegan(Touch  *touch, Event  *event)
{
	CC_UNUSED_PARAM(event);
	if (m_pNormalTexture){
		m_IsSelect = touchHits(touch);
		if (m_IsSelect){
			if (m_FuncList[BEGIN] != nullptr){					// BEGIN 상태일때 호출해야할 함수가 있다면 호출
				m_FuncList[BEGIN]();
			}
			if (m_SelectedTextureName != ""){					// 선택 시 이미지가 있다면 이미지 교체
				m_pNormalTexture->setTexture(m_SelectedTextureName);
			}
			else {												// 교체될 이미지가 없다면 크기를 키움
				m_pNormalTexture->setScale(1.1f);
			}
		}
	}
	return m_IsSelect;
}

void CMyButton::onTouchEnded(Touch  *touch, Event  *event)
{
	CC_UNUSED_PARAM(event);
	if (m_pNormalTexture){
		m_IsSelect = touchHits(touch);
		if (m_IsSelect && m_FuncList[END] != nullptr){			// END 상태일때 호출해야할 함수가 있다면 호출
			m_FuncList[END]();
		}
		if (m_SelectedTextureName != ""){						// 이미지가 바뀌었었다면 다시 원래대로 바꿈
			m_pNormalTexture->setTexture(m_NormalTextureName);
		}
		else{													// 바뀔이미지가 없다면 크기를 원래대로 바꿈
			m_pNormalTexture->setScale(1.0f);
		}
		m_IsSelect = false;
	}
}

void CMyButton::Execute(float delta)
{
	if (m_IsSelect && m_FuncList[EXECUTE] != nullptr)
		m_FuncList[EXECUTE]();
}

