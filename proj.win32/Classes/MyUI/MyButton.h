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

class CMyButton : public Widget
{
	typedef std::function<void(Node*)> NODE_CALLBACK;
public:
    static CMyButton* create();
    
	CMyButton* addEventListener(const NODE_CALLBACK &callback, eMYBUTTON_STATE state = eMYBUTTON_STATE::END);
	CMyButton* setClickedAnimation(const NODE_CALLBACK &callback);
    CMyButton* setLayer(LayerColor* layer);
    CMyButton* setButtonNormalImage(std::string imgName);
    CMyButton* setButtonClickedImage(std::string imgName);
	CMyButton* setDefaultClickedAnimation(int animationInfo);
	CMyButton* setTouchEnable(bool able);
    CMyButton* setContents(std::string contents);
    CMyButton* setFont(Color3B fontColor, int fontSize);
    CMyButton* setButtonPosition(Vec2 position);
    CMyButton* setButtonAnchorPoint(Vec2 anchorPoint);
    CMyButton* show(Node* parent, int zOrder = 0);
    
    void changeContents(std::string contents);
    void changeFontColor(Color3B fontColor);
 
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

	bool onEffect(eCLICKED_ANIMATION animation){ return (m_ClickedAnimationInfo & animation) == animation; }

    CMyButton();
	virtual ~CMyButton(){};

private:
	std::vector<NODE_CALLBACK> m_BeginFunctionList;
	std::vector<NODE_CALLBACK> m_ExecuteFunctionList;
	std::vector<NODE_CALLBACK> m_EndFunctionList;
	NODE_CALLBACK m_ClickedAnimationCallBack;
    LayerColor* m_Layer;
	Sprite* m_ButtonImage;
    Label* m_ContentsLabel;
    std::string m_ButtonNormalImage;
    std::string m_ButtonClickedImage;
    std::string m_Contents;
    Color3B m_FontColor;
    int m_FontSize;
    Vec2 m_Position;
    Vec2 m_AnchorPoint;
	bool m_IsSelect;	//선택되었는지 (선택중이라도 true)
	bool m_Touchable;	//버튼을 누를수 없는지 여부
	int m_ClickedAnimationInfo;
};

