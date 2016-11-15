#pragma once
#include "../GameObject/GameObject.h"

class CSpeechBubble : public CGameObject
{
public:
	static CSpeechBubble* create(std::string bubbleTextureName, std::string iconTextureName);
	static CSpeechBubble* create(std::string bubbleTextureName, std::string bubbleMent, std::string fontPath, size_t fontSize);

protected:
	virtual bool init() override;
	virtual bool initVariable() override;

private:
	CSpeechBubble(std::string bubbleTextureName, std::string iconTextureName)
		: m_BubbleTextureName(bubbleTextureName)
		, m_IconTextureName(iconTextureName)
		, m_BubbleMent("")
		, m_FontPath("")
		, m_BubbleTexture(nullptr)
		, m_IconTexture(nullptr)
		, m_BubbleMentLabel(nullptr)
		, m_FontSize(0){};

	CSpeechBubble(std::string bubbleTextureName, std::string bubbleMent, std::string fontPath, size_t fontSize)
		: m_BubbleTextureName(bubbleTextureName)
		, m_IconTextureName("")
		, m_BubbleMent(bubbleMent)
		, m_FontPath(fontPath)
		, m_BubbleTexture(nullptr)
		, m_IconTexture(nullptr)
		, m_BubbleMentLabel(nullptr)
		, m_FontSize(fontSize){}

	virtual ~CSpeechBubble(){};

private:
	std::string m_BubbleTextureName;
	std::string m_IconTextureName;
	std::string m_BubbleMent;
	std::string m_FontPath;
	cocos2d::Sprite* m_BubbleTexture;
	cocos2d::Sprite* m_IconTexture;
	cocos2d::Label* m_BubbleMentLabel;
	size_t m_FontSize;
};

