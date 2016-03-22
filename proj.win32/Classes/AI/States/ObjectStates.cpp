#include "ObjectStates.h"


//------------------------------------------------------------------------

CNormalState* CNormalState::Instance(){
	static CNormalState instance;

	return &instance;
}

void CNormalState::Enter(CObjectManager* objectMng){
	//objectMng->AddEnemy("RandomShoot", )
}

void CNormalState::Execute(CObjectManager* objectMng, float delta){

}

void CNormalState::Exit(CObjectManager* objectMng){

}


//------------------------------------------------------------------------

CBonusTimeState* CBonusTimeState::Instance(){
	static CBonusTimeState instance;

	return &instance;
}

void CBonusTimeState::Enter(CObjectManager* objectMng){

}

void CBonusTimeState::Execute(CObjectManager* objectMng, float delta){

}

void CBonusTimeState::Exit(CObjectManager* objectMng){

}


//------------------------------------------------------------------------

CCrazyState* CCrazyState::Instance(){
	static CCrazyState instance;

	return &instance;
}

void CCrazyState::Enter(CObjectManager* objectMng){

}

void CCrazyState::Execute(CObjectManager* objectMng, float delta){

}

void CCrazyState::Exit(CObjectManager* objectMng){

}


//------------------------------------------------------------------------

CGiantItemState* CGiantItemState::Instance(){
	static CGiantItemState instance;

	return &instance;
}

void CGiantItemState::Enter(CObjectManager* objectMng){

}

void CGiantItemState::Execute(CObjectManager* objectMng, float delta){

}

void CGiantItemState::Exit(CObjectManager* objectMng){

}


//------------------------------------------------------------------------

CMagnetItemState* CMagnetItemState::Instance(){
	static CMagnetItemState instance;

	return &instance;
}

void CMagnetItemState::Enter(CObjectManager* objectMng){
	//objectMng->PlayerGetMagnet();
}

void CMagnetItemState::Execute(CObjectManager* objectMng, float delta){
	//m_fTime += delta;
	//if ()
}

void CMagnetItemState::Exit(CObjectManager* objectMng){

}


//------------------------------------------------------------------------

CStarItemState* CStarItemState::Instance(){
	static CStarItemState instance;

	return &instance;
}

void CStarItemState::Enter(CObjectManager* objectMng){

}

void CStarItemState::Execute(CObjectManager* objectMng, float delta){

}

void CStarItemState::Exit(CObjectManager* objectMng){

}


//------------------------------------------------------------------------

CCoinItemState* CCoinItemState::Instance(){
	static CCoinItemState instance;

	return &instance;
}

void CCoinItemState::Enter(CObjectManager* objectMng){

}

void CCoinItemState::Execute(CObjectManager* objectMng, float delta){

}

void CCoinItemState::Exit(CObjectManager* objectMng){

}


//------------------------------------------------------------------------

CShieldItemState* CShieldItemState::Instance(){
	static CShieldItemState instance;

	return &instance;
}

void CShieldItemState::Enter(CObjectManager* objectMng){

}

void CShieldItemState::Execute(CObjectManager* objectMng, float delta){

}

void CShieldItemState::Exit(CObjectManager* objectMng){

}
