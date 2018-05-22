#pragma once
#include "cocos2d.h"
#include "ui/UIWidget.h"
USING_NS_CC;

struct PLANET;
class CPlanetSelectPopupDP : public cocos2d::ui::Widget
{
public:
    static CPlanetSelectPopupDP* create(const PLANET* planet);
    
    //setter & getter
    const PLANET* getPlanetParam() const { return m_Planet; }
    
    CC_SYNTHESIZE(Sprite*, m_PlanetImg, PlanetImg);
    
protected:
    virtual bool init() override;
    
private:
    
    CPlanetSelectPopupDP(const PLANET* planet)
    : m_Planet(planet)
    , m_PlanetImg(nullptr){};
    virtual ~CPlanetSelectPopupDP(){};
    
private:
    const PLANET* m_Planet;
};
