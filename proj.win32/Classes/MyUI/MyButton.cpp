#include "MyButton.h"

CMyButton::CMyButton(
	std::string textureName,					// ��ư�� �ؽ��� �̸�
	eMYBUTTON_STATE state,						// ���� (�ش� ������ �� �Լ� ȣ���)
	const std::function<void(void)> &func,		// ���� Ȥ�� �Լ������� Ȥ�� �Լ���ü ����(�Ű� ������ void)
	int effect)									// ��ư ����Ʈ
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
	std::string normalTextureName,				// ���� �� ��ư�� �ؽ��� �̸�
	std::string selectedTextureName,			// ���� �� ��ư�� �ؽ��� �̸�
	eMYBUTTON_STATE state,						// ���� (�ش� ������ �� �Լ� ȣ���)
	const std::function<void(void)> &func)		// ���� Ȥ�� �Լ������� Ȥ�� �Լ���ü ����(�Ű� ������ void)
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
	std::string normalTextureName,				// ��ư�� �ؽ��� �̸�
	std::string labelString,					// ��ư�� label ����
	int fontSize,								// ��Ʈ ������
	Color3B fontColor,							// ��Ʈ ����
	eMYBUTTON_STATE state,						// ���� (�ش� ������ �� �Լ� ȣ���)
	const std::function<void(void)> &func,		// ���� Ȥ�� �Լ������� Ȥ�� �Լ���ü ����(�Ű� ������ void)
	int effect)									// ��ư ����Ʈ
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
			m_pLabel = cocos2d::Label::createWithTTF(m_LabelString, "fonts/Marker Felt.ttf", m_FontSize);
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

/* ���� (�ش� ������ �� �Լ� ȣ���)
 * ���� Ȥ�� �Լ������� Ȥ�� �Լ���ü ����(�Ű� ������ void) */
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


/* ��ư ���� �� Effect ����*/
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


/* ��ư ������ �� Effect ���� */
void CMyButton::btnEffectEnd()
{
	if (m_pNormalTexture == nullptr)
		CCASSERT(false, "WRONG ACCESS PLEASE CHECK THE BTN TEXTURE");

	// �� ���·� ����
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


/* ��ġ�� ��ǥ�� ��ư�� ��ǥ�� ��ȯ �� �Ŀ� ��ư�� ��ġ�Ǿ����� �˻� */
bool CMyButton::touchHits(Touch  *touch)
{
	const Rect area(0, 0, m_pNormalTexture->getContentSize().width,
		m_pNormalTexture->getContentSize().height);

	// world to nodespace ��ǥ ��ȯ
	return area.containsPoint(m_pNormalTexture->convertToNodeSpace(touch->getLocation()));
}

/* ��ư�� ������ �� BEGIN */
bool CMyButton::onTouchBegan(Touch  *touch, Event  *event)
{
	CC_UNUSED_PARAM(event);
	if (m_pNormalTexture){

		// ��ǥ ��ȯ �� ��ġ üũ
		m_IsSelect = touchHits(touch);

		// ��ư�� ��ǥ���� ��¥�� ��ġ�Ǿ��� ���
		if (m_IsSelect){

			// BEGIN �����϶� ȣ���ؾ��� �Լ��� �ִٸ� ȣ��
			std::for_each(m_BeginFuncList.begin(), m_BeginFuncList.end(),
				[](const std::function<void(void)> &func){
				func();
			});

			btnEffectStart();
		}
	}
	return m_IsSelect;
}

/* ��ư���� �������� �� END */
void CMyButton::onTouchEnded(Touch  *touch, Event  *event)
{
	CC_UNUSED_PARAM(event);
	if (m_pNormalTexture){

		// END �����϶� ȣ���ؾ��� �Լ��� �ִٸ� ȣ��
		std::for_each(m_EndFuncList.begin(), m_EndFuncList.end(),
			[](const std::function<void(void)> &func){
			func();
		});

		btnEffectEnd();

		// ��ư ���� ����
		m_IsSelect = false;
	}
}

/* ��ư�� ������ �ִ� ���϶� EXECUTE */
void CMyButton::Execute(float delta)
{
	// ��ư ���� �����̸�, EXECUTE ������ �� ����Ǿ�� �� �Լ��� 1�� �̻��� ��
	if (m_IsSelect && m_ExecuteFuncList.size())
	{
		// EXECUTE �����϶� ȣ���ؾ��� �Լ��� �ִٸ� ȣ��
		std::for_each(m_ExecuteFuncList.begin(), m_ExecuteFuncList.end(),
			[](const std::function<void(void)> &func){
			func();
		});
	}
}

