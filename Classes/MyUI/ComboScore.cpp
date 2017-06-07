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

    GLOBAL->STAR_SCORE  += GLOBAL->COMBO_LEVEL;
    GLOBAL->COMBO_SCORE += 1;
    
	// UI visible On
	m_ComboLabel->setVisible(true);
    m_ComboLabel->setString(StringUtils::format("%d", GLOBAL->COMBO_SCORE).c_str());

    // Star score ui action
    CUILayer::Instance()->ScoreAction(GLOBAL->COMBO_LEVEL);
    
	GLOBAL->COMBO_LEVEL = (int(GLOBAL->COMBO_SCORE / 50)) + 1;
    if(m_OldLevel != GLOBAL->COMBO_LEVEL){
        m_OldLevel = GLOBAL->COMBO_LEVEL;
        m_ComboLabel->setColor(CGradientDataManager::Instance()->getScoreColorByLevel(m_OldLevel));
        //        this->runAction(JumpBy::create(0.3f, Vec2(0, 0), 50, 1));
        //        GLOBAL->STAR_SCORE += (m_MultipleNumber / 100) * 100000;
    }
}

void CComboScore::ComboScoreReset()
{
    GLOBAL->COMBO_LEVEL = 1;
    if(GLOBAL->COMBO_SCORE > GLOBAL->BEST_COMBO)
        GLOBAL->BEST_COMBO = GLOBAL->COMBO_SCORE;
    GLOBAL->COMBO_SCORE = 0;
	m_ComboLabel->setVisible(false);// UI visible Off
}

void CComboScore::update(float delta)
{
    if(CObjectManager::Instance()->getIsGamePause()) return;
    
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