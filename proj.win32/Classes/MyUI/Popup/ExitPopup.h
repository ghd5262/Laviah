#pragma once
#include "../Popup.h"

class CMyButton;
class CExitPopup : public CSpecificPopupBase
{
public:
	static CExitPopup* create();

protected:
	virtual bool initVariable() override;

private:
	void Exit();

	CExitPopup(){};
	virtual ~CExitPopup(){};

};