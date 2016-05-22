#pragma once
#include "../GameObject/GameObject.h"
#include <vector>
/*--------------------------CMyButton--------------------------
* strategy pattern
* texture 혹은 label로 초기화하여 사용하는 Button
* Lambda혹은 함수포인터, 함수객체를 인자로 받는다.
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
	/* 기본 버튼 생성 */
	static CMyButton* create(
		std::string textureName,					// 버튼의 텍스쳐 이름
		eMYBUTTON_STATE state,						// 상태 (해당 상태일 때 함수 호출됨)
		const std::function<void(void)> &func,		// 람다 전달
		int effect = EFFECT_NONE);					// 버튼 이펙트


	/* 텍스쳐와 함께 버튼 생성
	 * normalImg와 selectedImg를 인자로 전달*/
	static CMyButton* createWithTexture(
		std::string normalTextureName,				// 선택 전 버튼의 텍스쳐 이름
		std::string selectedTextureName,			// 선택 중 버튼의 텍스쳐 이름
		eMYBUTTON_STATE state,						// 상태 (해당 상태일 때 함수 호출됨)
		const std::function<void(void)> &func);		// 람다 혹은 함수포인터 혹은 함수객체 전달(매개 변수는 void)


	/* 문자열과 함께 버튼 생성 */
	static CMyButton* createWithString(
		std::string normalTextureName,				// 버튼의 텍스쳐 이름
		std::string labelString,					// 버튼의 label 내용
		int fontSize,								// 폰트 사이즈
		Color3B fontColor,							// 폰트 색상
		eMYBUTTON_STATE state,						// 상태 (해당 상태일 때 함수 호출됨)
		const std::function<void(void)> &func,		// 람다 전달
		int effect = EFFECT_NONE);					// 버튼 이펙트

	/* 버튼에 펑션을 추가 */
	void AddState(eMYBUTTON_STATE state,			// 상태 (해당 상태일 때 함수 호출됨)
		const std::function<void(void)> &func);		// 람다 혹은 함수포인터 혹은 함수객체 전달(매개 변수는 void)

	/* 버튼이 가지는 Execute callback함수 호출 */
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
	/* 해당 버튼 영역에 터치되었는지 검사
	* world to nodespace 좌표 변환 */
	bool touchHits(Touch  *touch);

	/* 버튼이 가지는 Begin callback함수 호출 */
	bool onTouchBegan(Touch  *touch, Event  *event);

	/* 버튼이 가지는 End callback함수 호출 */
	void onTouchEnded(Touch  *touch, Event  *event);

	/* 버튼 눌릴 때 Effect 실행*/
	void btnEffectStart();

	/* 버튼 떨어질 때 Effect 실행 */
	void btnEffectEnd();

	bool onEffect(eMYBUTTON_EFFECT_FLAG effect){ return (m_ButtonEffect & effect) == effect; }

	CMyButton(
		std::string textureName,					// 버튼의 텍스쳐 이름
		eMYBUTTON_STATE state,						// 상태 (해당 상태일 때 함수 호출됨)
		const std::function<void(void)> &func,		// 람다 전달
		int effect);								// 버튼 이펙트

	CMyButton(
		std::string normalTextureName,				// 선택 전 버튼의 텍스쳐 이름
		std::string selectedTextureName,			// 선택 중 버튼의 텍스쳐 이름
		eMYBUTTON_STATE state,						// 상태 (해당 상태일 때 함수 호출됨)
		const std::function<void(void)> &func);		// 람다 전달

	CMyButton(
		std::string normalTextureName,				// 버튼의 텍스쳐 이름
		std::string labelString,					// 버튼의 label 내용
		int fontSize,								// 폰트 사이즈
		Color3B fontColor,							// 폰트 색상
		eMYBUTTON_STATE state,						// 상태 (해당 상태일 때 함수 호출됨)
		const std::function<void(void)> &func,		// 람다 혹은 함수포인터 혹은 함수객체 전달(매개 변수는 void)
		int effect);								// 버튼 이펙트
		
	virtual ~CMyButton(){};

private:
	/* lambda, 함수 포인터를 리스트로 저장
	* 각 상태일 때 호출할 함수를 저장하는 리스트 */
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
	bool m_IsSelect;	//선택되었는지 (선택중이라도 true)
};

