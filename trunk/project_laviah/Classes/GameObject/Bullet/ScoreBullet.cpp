#include "ScoreBullet.h"
#include "../ObjectManager.h"
#include "../Planet.h"
#include "../Player.h"
#include "../../Particle/Particles.h"
#include "../../DataManager/GradientDataManager.h"

using namespace cocos2d;

CScoreBullet::CScoreBullet(int score/* = 0 */)
: m_nScore(score)
, m_strScore("")
, m_labelScore(nullptr)
{}

CScoreBullet* CScoreBullet::create(int score)
{
	CScoreBullet* pRet =
		(CScoreBullet*)new(std::nothrow)CScoreBullet(score);

	if (pRet && pRet->init())
	{
#if(!USE_MEMORY_POOLING)
        pRet->autorelease();
#endif
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
    if (!CBullet::init()) return false;

    m_strScore = MakeString("%d", m_nScore);
    m_labelScore = Label::createWithTTF(MakeString("+ %s", m_strScore.c_str()), FONT::NUMBER, 40);
    if (m_labelScore != nullptr){
        m_labelScore->setTextColor(Color4B(30, 255, 0, 255));
        m_labelScore->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        addChild(m_labelScore);
    }
    
    auto delay    = DelayTime::create(0.3f);
    auto fade     = FadeTo::create(0.5f, 0.1f);
    auto callFunc = CallFunc::create([=](){
        this->ReturnToMemoryBlock();
    });
    auto sequence = Sequence::create(delay, fade, callFunc, NULL);
    m_labelScore->runAction(sequence);
    
    return true;
}
