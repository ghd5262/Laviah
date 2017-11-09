#include "ComboScore.h"
#include "ScoreUI.h"
#include "UILayer.hpp"
#include "../Scene/GameScene.h"
#include "../GameObject/ObjectManager.h"
#include "../DataManager/GradientDataManager.h"

using namespace cocos2d;

CComboScore* CComboScore::m_Instance = nullptr;

CComboScore::CComboScore() 
: m_ComboLabel(nullptr)
, m_Time(0.f)
, m_OldLevel(0)
, m_IsPause(false)
{}

CComboScore::~CComboScore()
{
    m_Instance = nullptr;
}

CComboScore* CComboScore::Instance()
{
	if (m_Instance != nullptr) return m_Instance;

	m_Instance = new(std::nothrow) CComboScore();
	if (m_Instance && m_Instance->init())
	{
		m_Instance->autorelease();
		return m_Instance;
	}
	else
	{
		delete m_Instance;
		m_Instance = NULL;
		return NULL;
	}
}

bool CComboScore::init()
{
    if (!Node::init()) return false;

    this->scheduleUpdate();
	this->setContentSize(_director->getWinSize());
	auto popupSize = this->getContentSize();
   
	m_ComboLabel = Label::createWithTTF("", FONT::MALGUNBD, 120,
		Size(popupSize.width, popupSize.height),
		TextHAlignment::CENTER,
		TextVAlignment::CENTER);

	m_ComboLabel->setTextColor(COLOR::WHITEGRAY_ALPHA);
    m_ComboLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_ComboLabel->setPosition(Vec2(popupSize.width * 0.5f, popupSize.height * 0.7f));
	m_ComboLabel->setOpacity(255 * 0.7f);
    m_ComboLabel->setVisible(false);
	m_ComboLabel->enableOutline(COLOR::BRIGHT_WHITEGRAY_ALPHA, 5);
    this->addChild(m_ComboLabel);

    return true;
}

void CComboScore::AddCombo()
{
	// 타이머 초기화
	m_Time = 0.f;
    
    m_IsPause = false;

    GVALUE->STAR_SCORE  += GVALUE->COMBO_LEVEL;
    GVALUE->COMBO_SCORE += 1;
    
    this->LabelUpdate();

    // Star score ui action
    CUILayer::Instance()->ScoreAction(GVALUE->COMBO_LEVEL);
    
	GVALUE->COMBO_LEVEL = (int(GVALUE->COMBO_SCORE / 50)) + 1;
    if(m_OldLevel != GVALUE->COMBO_LEVEL){
        m_OldLevel = GVALUE->COMBO_LEVEL;
        m_ComboLabel->setColor(CGradientDataManager::Instance()->getScoreColorByLevel(m_OldLevel));
        //        this->runAction(JumpBy::create(0.3f, Vec2(0, 0), 50, 1));
        //        GVALUE->STAR_SCORE += (m_MultipleNumber / 100) * 100000;
    }
}

void CComboScore::ComboScoreReset()
{
    GVALUE->COMBO_LEVEL = 1;
    if(GVALUE->COMBO_SCORE > GVALUE->BEST_COMBO)
        GVALUE->BEST_COMBO = GVALUE->COMBO_SCORE;
    GVALUE->COMBO_SCORE = 0;
	m_ComboLabel->setVisible(false);// UI visible Off
    m_IsPause = false;
}

void CComboScore::LabelUpdate()
{
    if(!m_ComboLabel) return;
    
    // UI visible On
    m_ComboLabel->setVisible(true);
    m_ComboLabel->setString(StringUtils::format("%d", GVALUE->COMBO_SCORE).c_str());
}

void CComboScore::update(float delta)
{
    if(CObjectManager::Instance()->getIsGamePause()) return;
    if(m_IsPause) return;
    m_Time += delta;
    
    float time = (MULTIPLE_TIME_LIMIT - m_Time);
    time = std::max(0.f, time);
    
    float opacity = ((255.f * 0.7f) / (MULTIPLE_TIME_LIMIT - 2.f)) * time;
    opacity = std::min((255.f * 0.7f), opacity);
    m_ComboLabel->setOpacity(opacity);
    
    // 배수 유지 시간이 지나면 점수 반영 후 배수 초기화
    if (m_Time > MULTIPLE_TIME_LIMIT)
    {
        this->ComboScoreReset();
    }
}
