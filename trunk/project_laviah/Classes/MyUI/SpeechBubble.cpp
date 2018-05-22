#include "SpeechBubble.h"

using namespace cocos2d;

CSpeechBubble* CSpeechBubble::create(std::string bubbleTextureName, std::string iconTextureName)
{
	CSpeechBubble *pRet = new(std::nothrow) CSpeechBubble(bubbleTextureName, iconTextureName);
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

CSpeechBubble* CSpeechBubble::create(std::string bubbleTextureName, std::string bubbleMent, std::string fontPath, size_t fontSize)
{
	CSpeechBubble *pRet = new(std::nothrow) CSpeechBubble(bubbleTextureName, bubbleMent, fontPath, fontSize);
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

bool CSpeechBubble::init()
{
    if (!CGameObject::init()) return false;
    
    if (m_BubbleTextureName != "")
    {
        m_BubbleTexture = Sprite::create(m_BubbleTextureName);
        if (m_BubbleTexture != nullptr)
        {
            m_BubbleTexture->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            m_BubbleTexture->setPosition(Vec2::ZERO);
            addChild(m_BubbleTexture);
        }
    }
    if (m_IconTextureName != "")
    {
        m_IconTexture = Sprite::create(m_IconTextureName);
        if (m_IconTexture != nullptr)
        {
            m_IconTexture->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            m_IconTexture->setPosition(m_BubbleTexture->getContentSize().width * 0.5f, m_BubbleTexture->getContentSize().height * 0.5f);
            m_BubbleTexture->addChild(m_IconTexture);
        }
    }
    if (m_BubbleMent != "")
    {
        m_BubbleMentLabel = Label::createWithSystemFont(m_BubbleMent.c_str(), m_FontPath, m_FontSize);
        if (m_BubbleMentLabel != nullptr)
        {
            m_BubbleMentLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            m_BubbleMentLabel->setPosition(Vec2::ZERO);
            m_BubbleTexture->addChild(m_BubbleMentLabel);
        }
    }
    
    return true;
}

