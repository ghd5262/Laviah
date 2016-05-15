#include "ScoreUI.h"

CScoreUI* CScoreUI::create(std::string fontName, size_t fontSize, std::string valueImgName/* = "" */)
{
	CScoreUI *pRet = new(std::nothrow) CScoreUI(fontName, fontSize, valueImgName);
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

bool CScoreUI::init()
{
	if (!initVariable())
		return false;
	return true;
}

bool CScoreUI::initVariable()
{
	try{
		memset(m_ValueString, 0, sizeof(m_ValueString));
		m_ValueString[0] = '0';
		m_ValueLabel = Label::create("0", m_FontName, m_FontSize);
		if (nullptr != m_ValueLabel)
			addChild(m_ValueLabel);

		if (m_ValueImgName != "")
		{
			m_ValueImg = Sprite::create(m_ValueImgName);
			if (m_ValueImg != nullptr)
			{
				m_ValueImg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
				addChild(m_ValueImg);
			}
		}
	}
	catch (...){
		CCLOG("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTION__, __LINE__);
		assert(false);
		return false;
	}
	return true;
}

void CScoreUI::insertComma(const char* valueStr, char* resultStr)
{
	int len;
	len = strlen(valueStr);

	while ((*resultStr++ = *valueStr++)) {
		if (--len && (len % 3) == 0)
			*resultStr++ = ',';
	}

	*resultStr = '\0';
}

void CScoreUI::SetLabelAnchor(Vec2 point)
{
	m_ValueLabel->setAnchorPoint(point);
	if (m_ValueImg != nullptr)
		m_ValueImg->setPosition(
		Vec2((m_ValueLabel->getContentSize().width * -m_ValueLabel->getAnchorPoint().x) - (m_ValueImg->getContentSize().width * 0.7f), 
		m_ValueLabel->getContentSize().height * 0.05f));

}

void CScoreUI::UpdateValue(int number)
{
	m_Value += number;
	insertComma(MakeString("%u", m_Value).c_str(), m_ValueString);
	m_ValueLabel->setString(m_ValueString);
	if (m_ValueImg != nullptr)
		m_ValueImg->setPosition(
		Vec2((m_ValueLabel->getContentSize().width * -m_ValueLabel->getAnchorPoint().x) - (m_ValueImg->getContentSize().width * 0.7f), 
		m_ValueLabel->getContentSize().height * 0.05f));
}

void CScoreUI::Execute(float delta)
{
}