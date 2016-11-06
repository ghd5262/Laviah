#include "MultipleScore.h"
#include "ScoreUI.h"
#include "UIManager.h"
#include "../Scene/GameScene.h"
#include "../GameObject/Player.h"
#include "../GameObject/ObjectManager.h"


CMultipleScore::CMultipleScore() 
	: m_StarScore(nullptr)
	, m_MultipleNumberLabel(nullptr)
	, m_Player(nullptr)
	, m_MultipleNumber(0)
	, m_SavedScore(0)
	, m_isAbleToMultiple(false)
	, m_fTime(0.f)
{}

CMultipleScore* CMultipleScore::create()
{
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
	if (!initVariable())
		return false;
	return true;
}

bool CMultipleScore::initVariable()
{
	try{
		m_StarScore = static_cast<CScoreUI*>(CUIManager::Instance()->FindUIWithName("StarScoreUI"));
		
		m_Player = CObjectManager::Instance()->getPlayer();

		m_MultipleNumberLabel = Label::createWithTTF("", "fonts/malgunbd.ttf", 30);
		m_MultipleNumberLabel->setColor(g_labelColor2);
		m_MultipleNumberLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		m_MultipleNumberLabel->setPosition(Vec2(m_Player->getOriginPos().x, m_Player->getOriginPos().y + m_Player->getBRadius() + 50));
		m_MultipleNumberLabel->setVisible(false);
		CGameScene::getGridWorld()->addChild(m_MultipleNumberLabel);
	}
	catch (...){
		CCLOG("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTION__, __LINE__);
		assert(false);
		return false;
	}
	return true;
}

void CMultipleScore::AddScore(unsigned score)
{
	// 타이머 초기화
	m_fTime = 0.f;

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
	m_StarScore->addValue(calculatedScore);
}

void CMultipleScore::multipleScoreReset()
{
	m_isAbleToMultiple = false;
	m_SavedScore = 0;
	m_MultipleNumber = 0;
	m_MultipleNumberLabel->setVisible(false);// UI visible Off
}

void CMultipleScore::Execute(float delta)
{
	if (m_isAbleToMultiple == true)
	{
		m_fTime += delta;

		// UI 좌표 수정
		m_MultipleNumberLabel->setPosition(Vec2(m_Player->getOriginPos().x, m_Player->getOriginPos().y + m_Player->getBRadius() + 50));

		// 배수 유지 시간이 지나면 점수 반영 후 배수 초기화
		if (m_fTime > MULTIPLE_TIME_LIMIT)
		{
			calculateScore();
			multipleScoreReset();
		}
	}
}