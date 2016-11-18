#include "ScoreUI.h"

using namespace cocos2d;

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
    if (!CGameObject::init()) return false;
	
    m_ValueLabel = Label::createWithTTF("0", m_FontName, m_FontSize);
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
    
    return true;
}

std::string CScoreUI::insertComma(unsigned value)
{
	std::string valueStr = StringUtils::format("%u", value).c_str();
	std::string resultStr = "";
	//memset(resultStr, 0, sizeof(resultStr));

	int len = valueStr.length();

	int idx = 0;
	while (len) {
		resultStr += valueStr[idx++];
		if ((len % 4) == 0)
			resultStr += ',';
		len--;
	}

	return resultStr;
}

void CScoreUI::setLabelAnchor(Vec2 point)
{
	m_ValueLabel->setAnchorPoint(point);
	if (m_ValueImg != nullptr)
		m_ValueImg->setPosition(
		Vec2((m_ValueLabel->getContentSize().width * -m_ValueLabel->getAnchorPoint().x) - (m_ValueImg->getContentSize().width * 0.7f), 
		m_ValueLabel->getContentSize().height * 0.05f));

}

void CScoreUI::addValue(int value)
{
	m_Value += value;
	
	setValue(m_Value);
}

void CScoreUI::setValue(int value)
{
	m_ValueString = insertComma(value);
	m_ValueLabel->setString(m_ValueString);
	if (m_ValueImg != nullptr)
		m_ValueImg->setPosition(
		Vec2((m_ValueLabel->getContentSize().width * -m_ValueLabel->getAnchorPoint().x) - (m_ValueImg->getContentSize().width * 0.7f),
		m_ValueLabel->getContentSize().height * 0.05f));
}

void CScoreUI::Execute(float delta)
{
}