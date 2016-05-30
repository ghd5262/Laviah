#include "ScoreBullet.h"
#include "../ObjectManager.h"
#include "../Planet.h"
#include "../Player.h"
#include "../../Particle/Particles.h"

CScoreBullet::CScoreBullet(int score/* = 0 */)
	: CBullet(sBULLET_PARAM("scoreBullet", 0, 0, 0), 0.0f, 0.0f)
	, m_nScore(score)
	, m_strScore("")
	, m_labelScore(nullptr)
{}

CScoreBullet* CScoreBullet::create(int score)
{
	CScoreBullet* pRet =
		(CScoreBullet*)new(std::nothrow)CScoreBullet(score);

	if (pRet && pRet->init())
	{
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool CScoreBullet::init()
{
	if (!initVariable())
		return false;
	return true;
}

bool CScoreBullet::initVariable()
{
	try{
		m_strScore = MakeString("%d", m_nScore);
		m_labelScore = Label::create(MakeString("+ %s", m_strScore.c_str()), "fonts/Number.ttf", 25);
		if (m_labelScore != nullptr){
			m_labelScore->setTextColor(Color4B(30, 255, 0, 255));
			m_labelScore->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			addChild(m_labelScore);
		}

		this->scheduleOnce([this](float delta)
		{
			auto action = Sequence::create(
				FadeTo::create(0.5f, 0.1f),
				CallFunc::create([this](){
				this->ReturnToMemoryBlock();
			}), nullptr);
			m_labelScore->runAction(action);

		}, 0.5f, MakeString("ScoreBullet_%d", random<int>(0, 100)));
	}
	catch (...){
		CCLOG("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTION__, __LINE__);
		assert(false);
		return false;
	}
	return true;
}