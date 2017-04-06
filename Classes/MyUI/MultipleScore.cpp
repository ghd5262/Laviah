#include "MultipleScore.h"
#include "ScoreUI.h"
#include "../Scene/GameScene.h"
#include "../GameObject/Player.h"
#include "../GameObject/ObjectManager.h"

using namespace cocos2d;

CMultipleScore* CMultipleScore::m_Instance = nullptr;

CMultipleScore::CMultipleScore() 
	: m_MultipleNumberLabel(nullptr)
	, m_Player(nullptr)
	, m_MultipleNumber(0)
	, m_SavedScore(0)
	, m_isAbleToMultiple(false)
	, m_Time(0.f)
	, m_ColorLevel(0)
{}

CMultipleScore::~CMultipleScore()
{
    m_Instance = nullptr;
}

CMultipleScore* CMultipleScore::Instance()
{
	if (m_Instance != nullptr) return m_Instance;

	m_Instance = new(std::nothrow) CMultipleScore();
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

bool CMultipleScore::init()
{
    if (!Node::init()) return false;

    this->scheduleUpdate();
	this->setContentSize(_director->getWinSize());
	auto popupSize = this->getContentSize();
    m_Player = CObjectManager::Instance()->getPlayer();
   
	m_MultipleNumberLabel = Label::createWithTTF("", FONT::MALGUNBD, 120,
		Size(popupSize.width, popupSize.height),
		TextHAlignment::CENTER,
		TextVAlignment::CENTER);

	m_MultipleNumberLabel->setTextColor(COLOR::WHITEGRAY_ALPHA);
    m_MultipleNumberLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_MultipleNumberLabel->setPosition(Vec2(popupSize.width * 0.5f, popupSize.height * 0.68f));
	m_MultipleNumberLabel->setOpacity(255 * 0.7f);
    m_MultipleNumberLabel->setVisible(false);
	m_MultipleNumberLabel->enableOutline(COLOR::BRIGHT_WHITEGRAY_ALPHA, 5);
    this->addChild(m_MultipleNumberLabel);

	m_ColorList = {
		Color3B(255, 255, 255),
		Color3B(240, 222, 218),
		Color3B(230, 196, 193),
		Color3B(230, 171, 163),
		Color3B(242, 148, 148),
		Color3B(240, 142, 115),
		Color3B(245, 157, 91 ),
		Color3B(235, 170, 66 ),
		Color3B(240, 190, 53 ),
		Color3B(240, 218, 26 ),
		Color3B(237, 245, 24 ),
		Color3B(227, 227, 25 ),
		Color3B(201, 224, 20 ),
		Color3B(151, 242, 24 ),
		Color3B(81,  242, 22 ),
		Color3B(7,   242, 140),
		Color3B(0,   224, 232),
		Color3B(12,  186, 235),
		Color3B(21,  163, 235),
		Color3B(24,  139, 240),
		Color3B(29,  88,  224),
		Color3B(54,  22,  240),
		Color3B(54,  22,  240),
		Color3B(54,  22,  240),
		Color3B(54,  22,  240),
	};
   
    return true;
}

void CMultipleScore::AddScore(unsigned score)
{
	// 타이머 초기화
	m_Time = 0.f;

	// 현재 스코어 배수 +1
	m_MultipleNumber += 1;

	// 현재까지의 점수에 더한다.
	m_SavedScore += score;

	m_isAbleToMultiple = true;

	// UI visible On
	m_MultipleNumberLabel->setVisible(true);
	m_MultipleNumberLabel->setString(MakeString("%d", m_MultipleNumber).c_str()); //1부터 시작해서 -1해준다.

	unsigned colorLevel = m_MultipleNumber / 100;
	if (m_ColorLevel != colorLevel && colorLevel < 25){
		m_ColorLevel = colorLevel;
		m_MultipleNumberLabel->setColor(m_ColorList[m_ColorLevel]);
		this->runAction(JumpBy::create(0.3f, Vec2(0, 0), 50, 1));
//        GLOBAL->STAR_SCORE += (m_MultipleNumber / 100) * 100000;
    }
}

void CMultipleScore::UpdateScore()
{
//    this->calculateScore();
    this->multipleScoreReset();
}

void CMultipleScore::calculateScore()
{
	// 현재 점수 계산 (배수 * 저장된 점수)
	unsigned calculatedScore = m_MultipleNumber * m_SavedScore;
	
	if (m_MultipleNumber > GLOBAL->COMBO_SCORE)
		GLOBAL->COMBO_SCORE = m_MultipleNumber;

	// 점수 반영
    GLOBAL->STAR_SCORE += calculatedScore;
}

void CMultipleScore::multipleScoreReset()
{
	m_isAbleToMultiple = false;
	m_SavedScore = 0;
	m_MultipleNumber = 0;
	m_MultipleNumberLabel->setVisible(false);// UI visible Off
}

void CMultipleScore::update(float delta)
{
    if(CObjectManager::Instance()->getIsGamePause()) return;
    
	if(m_isAbleToMultiple == true)
	{
		m_Time += delta;

		float time = (MULTIPLE_TIME_LIMIT - m_Time);
		time = std::max(0.f, time);

		float opacity = ((255.f * 0.7f) / (MULTIPLE_TIME_LIMIT - 2.f)) * time;
		opacity = std::min((255.f * 0.7f), opacity);
		m_MultipleNumberLabel->setOpacity(opacity);
		//// UI 좌표 수정
		//m_MultipleNumberLabel->setPosition(Vec2(m_Player->getOriginPos().x,
  //                                              m_Player->getOriginPos().y +
  //                                              m_Player->getBoundingRadius() + 80));

		// 배수 유지 시간이 지나면 점수 반영 후 배수 초기화
		if (m_Time > MULTIPLE_TIME_LIMIT)
		{
            this->UpdateScore();
		}
	}
}