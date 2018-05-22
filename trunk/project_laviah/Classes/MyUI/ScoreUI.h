#pragma once

#include "cocos2d.h"

class CScoreUI : public cocos2d::Node
{
public:
	static CScoreUI* create(int& value);
    CScoreUI* setFont(std::string fontName,
                      size_t fontSize,
                      cocos2d::Color3B fontColor = cocos2d::Color3B::WHITE,
                      int outlineSize = 3);
	CScoreUI* setIcon(std::string iconName,
                      float posY = 0.5f,
                      cocos2d::Vec2 anchor = cocos2d::Vec2::ANCHOR_MIDDLE);
    CScoreUI* setScoreDistance(int distance);
    CScoreUI* setBGColor(cocos2d::Color4B color);
	CScoreUI* setScoreAnchorPoint(cocos2d::Vec2 anchorPoint);
	CScoreUI* show(cocos2d::Node* parent, unsigned zOrder = 0);

	virtual void update(float delta) override;

    CC_SYNTHESIZE(cocos2d::Sprite*, m_Icon, Icon);
    
private:
	void setOpacityByTimer();
	void timerReset();
	void setIconPosition();
	void setScoreString();
    void setBGPosition();

	CScoreUI(int& value)
    : m_ScoreLabel(nullptr)
    , m_BG(nullptr)
    , m_Icon(nullptr)
    , m_IconName("")
    , m_FontName("")
    , m_FontSize(50)
    , m_ScoreAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_LEFT)
    , m_IconAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE)
    , m_ScoreDistance(-1)
    , m_OutlineSize(0)
    , m_ValueRef(value)
    , m_OldValue(-1)
    , m_Time(0.f)
    , m_IconPosY(0.5f){};
    virtual ~CScoreUI(){};

private:
	cocos2d::Label* m_ScoreLabel;
    cocos2d::LayerColor* m_BG;
	std::string m_IconName;
	std::string m_FontName;
	size_t m_FontSize;
	cocos2d::Vec2 m_ScoreAnchorPoint;
    cocos2d::Vec2 m_IconAnchorPoint;
    cocos2d::Color4B m_BGColor;
    cocos2d::Color3B m_FontColor;
    int m_ScoreDistance;
    int m_OutlineSize;
	int& m_ValueRef;
	int m_OldValue;
	float m_Time;
    float m_IconPosY;
};

