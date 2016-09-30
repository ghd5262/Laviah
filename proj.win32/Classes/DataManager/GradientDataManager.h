#pragma once
#include "../Common/HSHUtility.h"
#include <vector>

struct sGRADIENT_PARAM{
    int _idx;
    Color3B _leftColor;
    Color3B _rightColor;
    
    sGRADIENT_PARAM()
    : _idx(0)
    , _leftColor(Color3B(0, 0, 0))
    , _rightColor(Color3B(0, 0, 0)){}
};

class CGradientDataManager
{
public:
    static CGradientDataManager* Instance();
    
    //getter & setter
    sGRADIENT_PARAM getRandomGradient() const;
    
private:
    CGradientDataManager();
    virtual ~CGradientDataManager();
    
private:
    std::vector<sGRADIENT_PARAM> m_GradientList;
};