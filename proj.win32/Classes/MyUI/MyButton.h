#pragma once
#include "../GameObject/GameObject.h"
/*--------------------------CMyButton--------------------------
* texture Ȥ�� label�� �ʱ�ȭ�Ͽ� ����ϴ� Button
* Lambda�� ���ڷ� �޴´�.
* �������� üũ
* ������ ���� �� üũ
* ������ �� üũ 
--------------------------------------------------------------*/

enum eMYBUTTON_STATE{
	BEGIN = 0,
	EXECUTE = 1,
	END = 2,

	MYBUTTON_STATE_COUNT
};

class CMyButton : public CGameObject
{
public:
	static CMyButton* createWithTexture(
		std::string normalTextureName,				// ���� �� ��ư�� �ؽ��� �̸�
		std::string selectedTextureName,			// ���� �� ��ư�� �ؽ��� �̸�
		eMYBUTTON_STATE state,						// ���� (�ش� ������ �� �Լ� ȣ���)
		const std::function<void(void)> &func);		// ���� ����

	static CMyButton* createWithString(
		std::string normalTextureName,				// ��ư�� �ؽ��� �̸�
		std::string labelString,					// ��ư�� label ����
		int fontSize,								// ��Ʈ ������
		eMYBUTTON_STATE state,						// ���� (�ش� ������ �� �Լ� ȣ���)
		const std::function<void(void)> &func);		// ���� ����

	void AddState(eMYBUTTON_STATE state,			// ���� (�ش� ������ �� �Լ� ȣ���)
		const std::function<void(void)> &func);		// ���� ����

	virtual void Execute(float delta = 0.f);

protected:
	virtual bool init() override;
	virtual bool initVariable() override;

private:
	bool touchHits(Touch  *touch);
	bool onTouchBegan(Touch  *touch, Event  *event);
	void onTouchEnded(Touch  *touch, Event  *event);

	CMyButton(
		std::string normalTextureName,				// ���� �� ��ư�� �ؽ��� �̸�
		std::string selectedTextureName,			// ���� �� ��ư�� �ؽ��� �̸�
		eMYBUTTON_STATE state,						// ���� (�ش� ������ �� �Լ� ȣ���)
		const std::function<void(void)> &func);		// ���� ����

	CMyButton(
		std::string normalTextureName,				// ��ư�� �ؽ��� �̸�
		std::string labelString,					// ��ư�� label ����
		int fontSize,								// ��Ʈ ������
		eMYBUTTON_STATE state,						// ���� (�ش� ������ �� �Լ� ȣ���)
		const std::function<void(void)> &func);		// ���� ����

	virtual ~CMyButton(){};

private:
	// lambda�� ����Ʈ�� ����
	// �� ������ �� ȣ���� �Լ��� �����ϴ� ����Ʈ
	std::function<void(void)> m_FuncList[MYBUTTON_STATE_COUNT];
	std::string m_NormalTextureName;
	std::string m_SelectedTextureName;
	std::string m_LabelString;
	Sprite* m_pNormalTexture;
	Label* m_pLabel;
	int m_FontSize;
	bool m_IsSelect;	//���õǾ����� (�������̶� true)
};

