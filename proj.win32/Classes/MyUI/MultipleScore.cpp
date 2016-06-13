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

void CMultipleScore::AddScore(UINT score)
{
	// Ÿ�̸� �ʱ�ȭ
	m_fTime = 0.f;

	// ���� ���ھ� ��� +1
	m_MultipleNumber += 1;

	// ��������� ������ ���Ѵ�.
	m_SavedScore += score;

	m_isAbleToMultiple = true;

	// UI visible On
	m_MultipleNumberLabel->setVisible(true);
	m_MultipleNumberLabel->setString(MakeString("X %d", m_MultipleNumber).c_str()); //1���� �����ؼ� -1���ش�.
}

void CMultipleScore::calculateScore()
{
	// ���� ���� ��� (��� * ����� ����)
	UINT calculatedScore = m_MultipleNumber * m_SavedScore;
	
	// ���� �ݿ�
	m_StarScore->UpdateValue(calculatedScore);
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

		// UI ��ǥ ����
		m_MultipleNumberLabel->setPosition(Vec2(m_Player->getOriginPos().x, m_Player->getOriginPos().y + m_Player->getBRadius() + 50));

		// ��� ���� �ð��� ������ ���� �ݿ� �� ��� �ʱ�ȭ
		if (m_fTime > MULTIPLE_TIME_LIMIT)
		{
			calculateScore();
			multipleScoreReset();
		}
	}
}