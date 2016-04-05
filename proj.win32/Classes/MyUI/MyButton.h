#pragma once
#include "../GameObject/GameObject.h"
#include <vector>
/*--------------------------CMyButton--------------------------
* strategy pattern
* texture Ȥ�� label�� �ʱ�ȭ�Ͽ� ����ϴ� Button
* LambdaȤ�� �Լ�������, �Լ���ü�� ���ڷ� �޴´�.
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
	/* �ؽ��Ŀ� �Բ� ��ư ����
	* normalImg�� selectedImg�� ���ڷ� ����*/
	static CMyButton* createWithTexture(
		std::string normalTextureName,				// ���� �� ��ư�� �ؽ��� �̸�
		std::string selectedTextureName,			// ���� �� ��ư�� �ؽ��� �̸�
		eMYBUTTON_STATE state,						// ���� (�ش� ������ �� �Լ� ȣ���)
		const std::function<void(void)> &func);		// ���� Ȥ�� �Լ������� Ȥ�� �Լ���ü ����(�Ű� ������ void)


	/* ���ڿ��� �Բ� ��ư ���� */
	static CMyButton* createWithString(
		std::string normalTextureName,				// ��ư�� �ؽ��� �̸�
		std::string labelString,					// ��ư�� label ����
		int fontSize,								// ��Ʈ ������
		eMYBUTTON_STATE state,						// ���� (�ش� ������ �� �Լ� ȣ���)
		const std::function<void(void)> &func);		// ���� ����


	/* ��ư�� ����� �߰� */
	void AddState(eMYBUTTON_STATE state,			// ���� (�ش� ������ �� �Լ� ȣ���)
		const std::function<void(void)> &func);		// ���� Ȥ�� �Լ������� Ȥ�� �Լ���ü ����(�Ű� ������ void)

	/* ��ư�� ������ Execute callback�Լ� ȣ�� */
	virtual void Execute(float delta = 0.f);

protected:
	virtual bool init() override;
	virtual bool initVariable() override;

private:
	/* �ش� ��ư ������ ��ġ�Ǿ����� �˻�
	* world to nodespace ��ǥ ��ȯ */
	bool touchHits(Touch  *touch);

	/* ��ư�� ������ Begin callback�Լ� ȣ�� */
	bool onTouchBegan(Touch  *touch, Event  *event);

	/* ��ư�� ������ End callback�Լ� ȣ�� */
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
		const std::function<void(void)> &func);		// ���� Ȥ�� �Լ������� Ȥ�� �Լ���ü ����(�Ű� ������ void)

	virtual ~CMyButton(){};

private:
	/* lambda, �Լ� �����͸� ����Ʈ�� ����
	* �� ������ �� ȣ���� �Լ��� �����ϴ� ����Ʈ */
	std::vector<std::function<void(void)>> m_BeginFuncList;
	std::vector<std::function<void(void)>> m_ExecuteFuncList;
	std::vector<std::function<void(void)>> m_EndFuncList;
	std::string m_NormalTextureName;
	std::string m_SelectedTextureName;
	std::string m_LabelString;
	Sprite* m_pNormalTexture;
	Label* m_pLabel;
	int m_FontSize;
	bool m_IsSelect;	//���õǾ����� (�������̶� true)
};

