#include "SoccerPitch.h"


CSoccerPitch::~CSoccerPitch()
{
	CC_SAFE_RELEASE(m_pImgPitch);

	CC_SAFE_DELETE(m_pPlayerPost);
	CC_SAFE_DELETE(m_pOtherPost);
}

bool CSoccerPitch::init()
{
	if (!initVariable())
		return false;

	initWalls();
	initRegions();

	return true;
}

bool CSoccerPitch::initVariable()
{
	try{
		m_nAudience = 50;
		m_vecWalls = new std::vector<sWALL>();
		m_vecRegions = new std::vector<sREGION>();

		m_pImgPitch = Sprite::create("SoccerPitch.png");
		if (m_pImgPitch != nullptr){
			CC_SAFE_RETAIN(m_pImgPitch);
			m_pImgPitch->setAnchorPoint(Vec2(0, 0));
			addChild(m_pImgPitch);
			this->setContentSize(Size(m_pImgPitch->getContentSize().width, m_pImgPitch->getContentSize().height));
		}
		Sprite* pimgPost1 = Sprite::create("PlayerPost.png");
		if (pimgPost1 != nullptr){
			addChild(pimgPost1);
			m_pPlayerPost = new sGOALPOST(Point(0, m_pImgPitch->getContentSize().height * 0.5f), Vec2(1, 0), pimgPost1, (pimgPost1->getContentSize().height * 0.5f));
		}
		Sprite* pimgPost2 = Sprite::create("OtherPost.png");
		if (pimgPost2 != nullptr){
			addChild(pimgPost2);
			m_pOtherPost = new sGOALPOST(Point((m_pImgPitch->getContentSize().width - pimgPost2->getContentSize().width),
				m_pImgPitch->getContentSize().height * 0.5f), Vec2(1, 0), pimgPost2, (pimgPost2->getContentSize().height * 0.5f));
		}
	}
	catch (...){
		CCLOG("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTIONW__, __LINE__);
		assert(false); /// 후에 적절한 c++예외로 변환 및 처리가 필요함
		return false;
	}
	return true;
}

void CSoccerPitch::initWalls()
{
	/// 벽 4개 초기화
	/// LT						 RT
	/// |-------------------------|
	/// |			 ↓		      |
	/// |-→						←-|
	/// |			 ↑			  |
	/// |-------------------------|
	/// LB						 RB

	Point winPosLT(m_nAudience, Director::getInstance()->getWinSize().height - m_nAudience);
	Point winPosRT(Director::getInstance()->getWinSize().width - m_nAudience, Director::getInstance()->getWinSize().height - m_nAudience);
	Point winPosLB(m_nAudience, m_nAudience);
	Point WinPosRB(Director::getInstance()->getWinSize().width - m_nAudience, m_nAudience);

	m_vecWalls->push_back(sWALL(winPosLT, winPosRT, Vec2(0, -1)));
	DrawDebugLine(winPosLT, winPosRT, "LT -> RT");
	m_vecWalls->push_back(sWALL(winPosRT, WinPosRB, Vec2(-1, 0)));
	DrawDebugLine(winPosRT, WinPosRB, "RT -> RB");
	m_vecWalls->push_back(sWALL(WinPosRB, winPosLB, Vec2(0, 1)));
	DrawDebugLine(WinPosRB, winPosLB, "RB -> LB");
	m_vecWalls->push_back(sWALL(winPosLB, winPosLT, Vec2(1, 0)));
	DrawDebugLine(winPosLB, winPosLT, "LB -> LT");
}

void CSoccerPitch::initRegions()
{
	/// 경기장 인덱스
	/// h |---------Width---------|
	/// e |-----------------------|
	/// i | 2 | 5 | 8 | 11| 14| 17|
	/// g | 1 | 4 | 7 | 10| 13| 16|
	/// h | 0 | 3 | 6 | 9 | 12| 15|
	/// t |-----------------------|

	int index = 0;
	float regionx = (Director::getInstance()->getWinSize().width - 100) / PITCHSIZEARR.x;
	float regiony = (Director::getInstance()->getWinSize().height - 100) / PITCHSIZEARR.y;
	for (int x = 0; x < PITCHSIZEARR.x; x++)
	{
		for (int y = 0; y < PITCHSIZEARR.y; y++)
		{
			float posx1 = (x * regionx) + m_nAudience;
			float posy1 = (y * regiony) + m_nAudience;
			float posx2 = (posx1 + regionx);
			float posy2 = (posy1 + regiony);
			float centerx = (posx1 + (regionx * 0.5f));
			float centery = (posy1 + (regiony * 0.5f));

			CCLOG("index : %d, Pos1 : %2.f, %2.f, Pos2 : %2.f, %2.f, Center : %2.f, %2.f\n", index, posx1, posy1, posx2, posy2, centerx, centery);
			DrawDebugRect(Point(posx1, posy1), Point(posx2, posy2), MakeString("%d", index));
			m_vecRegions->push_back(sREGION(Point(posx1, posy1), Point(posx2, posy2), Point(centerx, centery), index));
			index++;
		}
		CCLOG("\n");
	}
}