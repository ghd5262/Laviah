#include "UIManager.h"


CUIManager* CUIManager::create()
{
	CUIManager *pRet = new(std::nothrow) CUIManager();
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

bool CUIManager::AddUIWithName(CGameObject* ui, std::string name)
{
	bool addSuccess = m_UIList.emplace(std::pair<std::string, CGameObject*>(name, ui)).second;
	if (!addSuccess)
		CCASSERT(addSuccess, "UI KEY WAS DUPLICATED");
	return addSuccess;
}

CGameObject* CUIManager::FindUIWithName(std::string name)
{
	auto ui = m_UIList.find(name)->second;
	if(ui == nullptr)
		CCASSERT(ui != nullptr, "UI KEY IS WRONG");
	return ui;
}

void CUIManager::Execute(float delta)
{
	std::for_each(m_UIList.begin(), m_UIList.end(), [=](std::pair<std::string, CGameObject*> ui)
	{
		ui.second->Execute(delta);
	});
}
