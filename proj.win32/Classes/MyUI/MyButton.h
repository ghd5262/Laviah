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

enum eMYBUTTON_EFFECT_FLAG{
	EFFECT_NONE = 0x0000,
	EFFECT_ALPHA = 0x0001,
	EFFECT_SIZEUP = 0x0002,
	EFFECT_SIZEDOWN = 0x0004,
	EFFECT_GRAY = 0x0008,
	EFFECT_TEXTURE = 0x0010
};

class CMyButton : public CGameObject
{
public:
	/* �⺻ ��ư ���� */
	static CMyButton* create(
		std::string textureName,					// ��ư�� �ؽ��� �̸�
		eMYBUTTON_STATE state,						// ���� (�ش� ������ �� �Լ� ȣ���)
		const std::function<void(void)> &func,		// ���� ����
		int effect = EFFECT_NONE);					// ��ư ����Ʈ


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
		Color3B fontColor,							// ��Ʈ ����
		eMYBUTTON_STATE state,						// ���� (�ش� ������ �� �Լ� ȣ���)
		const std::function<void(void)> &func,		// ���� ����
		int effect = EFFECT_NONE);					// ��ư ����Ʈ

	/* ��ư�� ����� �߰� */
	void AddState(eMYBUTTON_STATE state,			// ���� (�ش� ������ �� �Լ� ȣ���)
		const std::function<void(void)> &func);		// ���� Ȥ�� �Լ������� Ȥ�� �Լ���ü ����(�Ű� ������ void)

	/* ��ư�� ������ Execute callback�Լ� ȣ�� */
	virtual void Execute(float delta = 0.f);

	virtual const Size& getContentSize() const override { 
		if (m_pNormalTexture != nullptr) 
			return m_pNormalTexture->getContentSize();
		CCASSERT(m_pNormalTexture != nullptr, "Texture is nullptr");
	}

	// getter & setter
	CC_SYNTHESIZE(Label*, m_pLabel, BtnLabel);
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

	/* ��ư ���� �� Effect ����*/
	void btnEffectStart();

	/* ��ư ������ �� Effect ���� */
	void btnEffectEnd();

	bool onEffect(eMYBUTTON_EFFECT_FLAG effect){ return (m_ButtonEffect & effect) == effect; }

	CMyButton(
		std::string textureName,					// ��ư�� �ؽ��� �̸�
		eMYBUTTON_STATE state,						// ���� (�ش� ������ �� �Լ� ȣ���)
		const std::function<void(void)> &func,		// ���� ����
		int effect);								// ��ư ����Ʈ

	CMyButton(
		std::string normalTextureName,				// ���� �� ��ư�� �ؽ��� �̸�
		std::string selectedTextureName,			// ���� �� ��ư�� �ؽ��� �̸�
		eMYBUTTON_STATE state,						// ���� (�ش� ������ �� �Լ� ȣ���)
		const std::function<void(void)> &func);		// ���� ����

	CMyButton(
		std::string normalTextureName,				// ��ư�� �ؽ��� �̸�
		std::string labelString,					// ��ư�� label ����
		int fontSize,								// ��Ʈ ������
		Color3B fontColor,							// ��Ʈ ����
		eMYBUTTON_STATE state,						// ���� (�ش� ������ �� �Լ� ȣ���)
		const std::function<void(void)> &func,		// ���� Ȥ�� �Լ������� Ȥ�� �Լ���ü ����(�Ű� ������ void)
		int effect);								// ��ư ����Ʈ
		
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
	Color3B m_FontColor;
	int m_FontSize;
	int m_ButtonEffect;
	bool m_IsSelect;	//���õǾ����� (�������̶� true)
};

