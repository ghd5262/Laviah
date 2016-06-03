#include "CharacterSelectPopupDP.h"
#include "../MyButton.h"
#include "../../DataManager/CharacterDataManager.h"

CCharacterSelectPopupDP* CCharacterSelectPopupDP::create(int CharacterIdx)
{
    CCharacterSelectPopupDP *pRet = new(std::nothrow) CCharacterSelectPopupDP(CharacterIdx);
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

bool CCharacterSelectPopupDP::init()
{
    if (!initVariable())
        return false;
    return true;
}

bool CCharacterSelectPopupDP::initVariable()
{
    try{
        auto characterInfo = CCharacterDataManager::Instance()->getCharacterInfoByIndex(m_CharacterIdx);
        
        auto dpBack = CMyButton::create
        
        auto characterImg = Sprite::create(characterInfo._textureName);
        if(characterImg != nullptr)
        {
            
        }
    }
    catch (...){
        throw StringUtils::format("FILE %s, FUNC %s, LINE %d", __FILE__, __FUNCTION__, __LINE__);
        return false;
    }
    return true;
}

void CCharacterSelectPopupDP::Select()
{
    
}