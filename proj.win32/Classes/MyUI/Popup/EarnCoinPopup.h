#pragma once
#include "../Popup.h"

class CMyButton;
class CEarnCoinPopup : public CSpecificPopupBase
{
public:
	static CEarnCoinPopup* create();

protected:
	virtual bool initVariable() override;

private:
	void Video();

	CEarnCoinPopup(){};
	virtual ~CEarnCoinPopup(){};

};