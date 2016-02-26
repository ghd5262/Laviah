#pragma once

#include "GameObject.h"
#include <vector>

const int  PITCHSIZEHEIGHT = 3;
const int  PITCHSIZEWIDTH = 6;
const Vec2 PITCHSIZEARR(PITCHSIZEWIDTH, PITCHSIZEHEIGHT);

typedef struct sWALL{
	Point		_pos1;
	Point		_pos2;
	Vec2		_normalVec;

	sWALL(Point pos1, Point pos2, Vec2 normalVec)
		: _pos1(pos1)
		, _pos2(pos2)
		, _normalVec(normalVec){}
};

typedef struct sREGION{
	Point		_posLT;
	Point		_posRB;
	Point		_posCT;
	int			_index;

	sREGION(Point posLT, Point posRB, Point posCT, int index)
		: _posLT(posLT)
		, _posRB(posRB)
		, _posCT(posCT)
		, _index(index){}
};

typedef struct sGOALPOST{
	Point		_postPos;
	Point		_leftPoint;
	Point		_rightPoint;
	Vec2		_facingVec;
	Sprite*		_pImgPost;

	sGOALPOST(Point postPos, Vec2 facingVec, Sprite* pImgPost, float scale = 100.0f)
		: _postPos(postPos)
		, _leftPoint(facingVec.x >= 0 ? Point(postPos.x, postPos.y + scale) : Point(postPos.x, postPos.y - scale))
		, _rightPoint(facingVec.x >= 0 ? Point(postPos.x, postPos.y - scale) : Point(postPos.x, postPos.y + scale))
		, _facingVec(facingVec)
		, _pImgPost(pImgPost)
	{
		_pImgPost->setPosition(_postPos);
		_pImgPost->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		if (_facingVec.x < 0){
			_pImgPost->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
			_pImgPost->setFlippedY(true);
		}
	}
};

class CSoccerPitch : public CGameObject
{
public:
	virtual ~CSoccerPitch();
	CREATE_FUNC(CSoccerPitch);

	///getter & setter
	std::vector<sWALL>* getM_VecWalls() { return m_vecWalls; }
	std::vector<sREGION>* getM_VecRegions() { return m_vecRegions; }
	sGOALPOST* getM_PlayerPost() { return m_pPlayerPost; }
	sGOALPOST* getM_OtherPost() { return m_pOtherPost; }

protected:
	virtual bool init();

private:
	bool initVariable();
	void initWalls();
	void initRegions();

private:
	std::vector<sWALL>* m_vecWalls;
	std::vector<sREGION>* m_vecRegions;

	sGOALPOST* m_pPlayerPost;
	sGOALPOST* m_pOtherPost;
	Sprite* m_pImgPitch;
	int m_nAudience;
};