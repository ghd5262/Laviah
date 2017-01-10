#pragma once
#include "../Common/HSHUtility.h"
#include <vector>

struct sGRADIENT_PARAM{
    int _idx;
    cocos2d::Color3B _leftColor;
    cocos2d::Color3B _rightColor;
    
	sGRADIENT_PARAM()
		: _idx(0)
		, _leftColor(cocos2d::Color3B(0, 0, 0))
		, _rightColor(cocos2d::Color3B(0, 0, 0)){}

	sGRADIENT_PARAM(const sGRADIENT_PARAM& data)
		: _idx(data._idx)
		, _leftColor(data._leftColor)
		, _rightColor(data._rightColor){}

	sGRADIENT_PARAM(const sGRADIENT_PARAM* data)
		: _idx(data->_idx)
		, _leftColor(data->_leftColor)
		, _rightColor(data->_rightColor){}

	sGRADIENT_PARAM(int idx,
		cocos2d::Color3B left,
		cocos2d::Color3B right)
		: _idx(idx)
		, _leftColor(left)
		, _rightColor(right){}
};

class CGradientDataManager
{
public:
    static CGradientDataManager* Instance();
    
    //getter & setter
    sGRADIENT_PARAM getRandomGradient() const;
	sGRADIENT_PARAM getNextGradient();

	CC_SYNTHESIZE(int, m_GradientIdx, GradientIdx);
private:
    CGradientDataManager();
    virtual ~CGradientDataManager();
    
private:
    std::vector<sGRADIENT_PARAM> m_GradientList;
};