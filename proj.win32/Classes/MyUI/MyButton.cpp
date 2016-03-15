#include "MyButton.h"


CMyButton::CMyButton(
	std::string normalTextureName,						// ���� �� ��ư�� �ؽ��� �̸�
	std::string selectedTextureName,					// ���� �� ��ư�� �ؽ��� �̸�
	eMYBUTTON_STATE state,								// ���� (�ش� ������ �� �Լ� ȣ���)
	const std::function<void(void)> &func)				// ���� ����
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
	std::string normalTextureName,						// ��ư�� �ؽ��� �̸�
	std::string labelString,							// ��ư�� label ����
	int fontSize,										// ��Ʈ ������
	eMYBUTTON_STATE state,								// ���� (�ش� ������ �� �Լ� ȣ���)
	const std::function<void(void)> &func)				// ���� ����
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
	std::string normalTextureName,						// ���� �� ��ư�� �ؽ��� �̸�
	std::string selectedTextureName,					// ���� �� ��ư�� �ؽ��� �̸�
	eMYBUTTON_STATE state,								// ���� (�ش� ������ �� �Լ� ȣ���)
	const std::function<void(void)> &func)				// ���� ����
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
	std::string normalTextureName,						// ��ư�� �ؽ��� �̸�
	std::string labelString,							// ��ư�� label ����
	int fontSize,										// ��Ʈ ������
	eMYBUTTON_STATE state,								// ���� (�ش� ������ �� �Լ� ȣ���)
	const std::function<void(void)> &func)				// ���� ����
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

void CMyButton::AddState(eMYBUTTON_STATE state,			// ���� (�ش� ������ �� �Լ� ȣ���)
	const std::function<void(void)> &func)				// ���� ����
{
	m_FuncList[state] = func;
}

bool CMyButton::touchHits(Touch  *touch)
{
	const Rect area(0, 0, m_pNormalTexture->getContentSize().width, 
		m_pNormalTexture->getContentSize().height);
	// ��ǥ ��ȯ
	return area.containsPoint(m_pNormalTexture->convertToNodeSpace(touch->getLocation()));
}

bool CMyButton::onTouchBegan(Touch  *touch, Event  *event)
{
	CC_UNUSED_PARAM(event);
	if (m_pNormalTexture){
		m_IsSelect = touchHits(touch);
		if (m_IsSelect){
			if (m_FuncList[BEGIN] != nullptr){					// BEGIN �����϶� ȣ���ؾ��� �Լ��� �ִٸ� ȣ��
				m_FuncList[BEGIN]();
			}
			if (m_SelectedTextureName != ""){					// ���� �� �̹����� �ִٸ� �̹��� ��ü
				m_pNormalTexture->setTexture(m_SelectedTextureName);
			}
			else {												// ��ü�� �̹����� ���ٸ� ũ�⸦ Ű��
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
		if (m_IsSelect && m_FuncList[END] != nullptr){			// END �����϶� ȣ���ؾ��� �Լ��� �ִٸ� ȣ��
			m_FuncList[END]();
		}
		if (m_SelectedTextureName != ""){						// �̹����� �ٲ�����ٸ� �ٽ� ������� �ٲ�
			m_pNormalTexture->setTexture(m_NormalTextureName);
		}
		else{													// �ٲ��̹����� ���ٸ� ũ�⸦ ������� �ٲ�
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

