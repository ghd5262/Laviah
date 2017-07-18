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

enum class eMYBUTTON_STATE{
	BEGIN = 0,
	EXECUTE = 1,
	END = 2,

	MYBUTTON_STATE_COUNT
};

enum eCLICKED_ANIMATION{
	NONE = 0x0000,
	ALPHA = 0x0001,
	SIZEUP = 0x0002,
	SIZEDOWN = 0x0004,
	GRAY = 0x0008,
	TEXTURE = 0x0010
};

class CMyButton : public cocos2d::ui::Widget
{
	typedef std::function<void(Node*)> NODE_CALLBACK;
public:
    static CMyButton* create();
    
	CMyButton* addEventListener(const NODE_CALLBACK &callback, eMYBUTTON_STATE state = eMYBUTTON_STATE::END);
	CMyButton* setClickedAnimation(const NODE_CALLBACK &callback);
    CMyButton* setLayer(cocos2d::LayerColor* layer);
    CMyButton* setButtonNormalImage(std::string imgName);
    CMyButton* setButtonClickedImage(std::string imgName);
	CMyButton* setDefaultClickedAnimation(int animationInfo);
	CMyButton* setClickSound(std::string began, std::string ended = "");
    CMyButton* setEnableSound(bool able);
	CMyButton* setTouchEnable(bool able, cocos2d::Color3B color = cocos2d::Color3B::GRAY);
    CMyButton* setButtonSingleUse(bool singleUse);
    CMyButton* setContents(std::string contents);
    CMyButton* setFont(cocos2d::Color3B fontColor, int fontSize);
    CMyButton* setButtonPosition(cocos2d::Vec2 position);
    CMyButton* setButtonAnchorPoint(cocos2d::Vec2 anchorPoint);
    CMyButton* show(cocos2d::Node* parent, int zOrder = 0);
    
    void changeContents(std::string contents);
    void changeFontColor(cocos2d::Color3B fontColor);
    void changeButtonImage(std::string imageName);
    
	/* 버튼이 가지는 Execute callback함수 호출 */
	virtual void update(float delta = 0.f) override;

	bool getTouchable() const { return m_Touchable; }

protected:
	virtual bool init() override;

private:
    
    void onTouchEvent(Ref *pSender, Widget::TouchEventType type);
    
	/* 버튼이 가지는 Begin callback함수 호출 */
    bool onTouchBegan();
    
	/* 버튼이 가지는 End callback함수 호출 */
    void onTouchEnded();
    
    /* 버튼터치가 Cancel 됬을 때 */
    void onTouchCancelled();
    
	/* 버튼 눌릴 때 Effect 실행*/
	void playButtonStartAnimation();

	/* 버튼 떨어질 때 Effect 실행 */
	void playButtonEndAnimation();

	void playClickedSound(std::string name);

	bool onEffect(eCLICKED_ANIMATION animation){ return (m_ClickedAnimationInfo & animation) == animation; }

    CMyButton();
	virtual ~CMyButton(){};

private:
	std::vector<NODE_CALLBACK> m_BeginFunctionList;
	std::vector<NODE_CALLBACK> m_ExecuteFunctionList;
	std::vector<NODE_CALLBACK> m_EndFunctionList;
	NODE_CALLBACK m_ClickedAnimationCallBack;
    cocos2d::LayerColor* m_Layer;
	cocos2d::Sprite* m_ButtonImage;
    cocos2d::Label* m_ContentsLabel;
    std::string m_ButtonNormalImage;
    std::string m_ButtonClickedImage;
    std::string m_Contents;
	std::string m_ClickBeganSound;
	std::string m_ClickEndedSound;
    cocos2d::Color3B m_FontColor;
    int m_FontSize;
    cocos2d::Vec2 m_Position;
    cocos2d::Vec2 m_AnchorPoint;
	bool m_IsSelect;	// 선택되었는지 (선택중이라도 true)
	bool m_Touchable;	// 버튼을 누를수 없는지 여부
    bool m_Sound;       // 사운드 재생 여부
    bool m_SingleUse;   // 일회성 여부
	int m_ClickedAnimationInfo;
    
    static bool m_MultiTouchesDisable;
};

