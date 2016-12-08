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
{}

CMultipleScore* CMultipleScore::Instance()
{
    if(m_Instance != nullptr) return m_Instance;
    
	CMultipleScore *pRet = new(std::nothrow) CMultipleScore();
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

bool CMultipleScore::init()
{
    if (!Node::init()) return false;
    m_Instance = this;
    this->scheduleUpdate();
    
    m_Player = CObjectManager::Instance()->getPlayer();
    
    m_MultipleNumberLabel = Label::createWithTTF("", "fonts/malgunbd.ttf", 30);
    m_MultipleNumberLabel->setColor(COLOR::BRIGHTGRAY);
    m_MultipleNumberLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_MultipleNumberLabel->setPosition(this->getContentSize() / 2);
    m_MultipleNumberLabel->setVisible(false);
    
    this->addChild(m_MultipleNumberLabel);
    this->setVisible(false);
   
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
	m_MultipleNumberLabel->setString(MakeString("X %d", m_MultipleNumber).c_str()); //1부터 시작해서 -1해준다.
}

void CMultipleScore::calculateScore()
{
	// 현재 점수 계산 (배수 * 저장된 점수)
	unsigned calculatedScore = m_MultipleNumber * m_SavedScore;
	
	// 점수 반영
	GLOBAL::STARSCORE += calculatedScore;
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

		// UI 좌표 수정
		this->setPosition(Vec2(m_Player->getOriginPos().x,
                               m_Player->getOriginPos().y +
                               m_Player->getBoundingRadius() + 50));

		// 배수 유지 시간이 지나면 점수 반영 후 배수 초기화
		if (m_Time > MULTIPLE_TIME_LIMIT)
		{
            calculateScore();
            multipleScoreReset();
		}
	}
}