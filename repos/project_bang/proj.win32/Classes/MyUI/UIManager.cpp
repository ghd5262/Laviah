#include "UIManager.h"
#include "../GameObject/GameObject.h"

CUIManager* CUIManager::Instance()
{
	static CUIManager instance;
	return &instance;
}

void CUIManager::Clear()
{
	m_UIList.clear();
}

bool CUIManager::AddUIWithName(CGameObject* ui, std::string name)
{
	bool addSuccess = m_UIList.emplace(std::pair<std::string, CGameObject*>(name, ui)).second;
	if (!addSuccess)
		CCASSERT(addSuccess, "UI KEY WAS DUPLICATED");
	return addSuccess;
}

CGameObject* CUIManager::FindUIWithName(std::string name) const
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
