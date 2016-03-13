#pragma once
#include "../GameObject/GameObject.h"
/*--------------------------CMyButton--------------------------
* texture 혹은 label로 초기화하여 사용하는 Button
* Lambda를 인자로 받는다.
* 눌렀을때 체크
* 누르고 있을 때 체크
* 끝났을 때 체크 
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
		std::string normalTextureName,				// 선택 전 버튼의 텍스쳐 이름
		std::string selectedTextureName,			// 선택 중 버튼의 텍스쳐 이름
		eMYBUTTON_STATE state,						// 상태 (해당 상태일 때 함수 호출됨)
		const std::function<void(void)> &func);		// 람다 전달

	static CMyButton* createWithString(
		std::string normalTextureName,				// 버튼의 텍스쳐 이름
		std::string labelString,					// 버튼의 label 내용
		int fontSize,								// 폰트 사이즈
		eMYBUTTON_STATE state,						// 상태 (해당 상태일 때 함수 호출됨)
		const std::function<void(void)> &func);		// 람다 전달

	void AddState(eMYBUTTON_STATE state,			// 상태 (해당 상태일 때 함수 호출됨)
		const std::function<void(void)> &func);		// 람다 전달

	virtual void Execute(float delta = 0.f);

protected:
	virtual bool init() override;
	virtual bool initVariable() override;

private:
	bool touchHits(Touch  *touch);
	bool onTouchBegan(Touch  *touch, Event  *event);
	void onTouchEnded(Touch  *touch, Event  *event);

	CMyButton(
		std::string normalTextureName,				// 선택 전 버튼의 텍스쳐 이름
		std::string selectedTextureName,			// 선택 중 버튼의 텍스쳐 이름
		eMYBUTTON_STATE state,						// 상태 (해당 상태일 때 함수 호출됨)
		const std::function<void(void)> &func);		// 람다 전달

	CMyButton(
		std::string normalTextureName,				// 버튼의 텍스쳐 이름
		std::string labelString,					// 버튼의 label 내용
		int fontSize,								// 폰트 사이즈
		eMYBUTTON_STATE state,						// 상태 (해당 상태일 때 함수 호출됨)
		const std::function<void(void)> &func);		// 람다 전달

	virtual ~CMyButton(){};

private:
	// lambda를 리스트로 저장
	// 각 상태일 때 호출할 함수를 저장하는 리스트
	std::function<void(void)> m_FuncList[MYBUTTON_STATE_COUNT];
	std::string m_NormalTextureName;
	std::string m_SelectedTextureName;
	std::string m_LabelString;
	Sprite* m_pNormalTexture;
	Label* m_pLabel;
	int m_FontSize;
	bool m_IsSelect;	//선택되었는지 (선택중이라도 true)
};

