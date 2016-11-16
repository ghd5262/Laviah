#pragma once
#include "../GameObject/Shooter/ShooterHeaders.h"
#include <map>

struct sBULLET_PATTERN
{
    int _index;
    int _height;
    int _width;
    float _widthPadding;
    float _heightPadding;
    std::string _patternName;
    char _pattern[60 * 60]; // 패턴의 최대 크기 60 * 60 = 3600byte
    
    sBULLET_PATTERN()
    : _index(-1)
    , _height(-1)
    , _width(-1)
    , _widthPadding(-1)
    , _heightPadding(-1)
    , _patternName(""){
        std::memset(_pattern, 0, sizeof(_pattern));
    }
    
    sBULLET_PATTERN(const sBULLET_PATTERN& data)
    : _index(data._index)
    , _height(data._height)
    , _width(data._width)
    , _widthPadding(data._widthPadding)
    , _heightPadding(data._heightPadding)
    , _patternName(data._patternName){
        int idx = 0;
        for(auto it : data._pattern)
        {
            _pattern[idx++] = it;
        }
    }
    
    sBULLET_PATTERN(const sBULLET_PATTERN* data)
    : _index(data->_index)
    , _height(data->_height)
    , _width(data->_width)
    , _widthPadding(data->_widthPadding)
    , _heightPadding(data->_heightPadding)
    , _patternName(data->_patternName){
        int idx = 0;
        for(auto it : data->_pattern)
        {
            _pattern[idx++] = it;
        }
    }
};

class CBulletPatternDataManager
{
    typedef std::map<std::string, const sBULLET_PATTERN*> PATTERN_LIST;
public:
	static CBulletPatternDataManager* Instance();
	void AddPattern(const sBULLET_PATTERN& data);

	//getter & setter
	const sBULLET_PATTERN* getDataByName(std::string name) const;
    const sBULLET_PATTERN* getRandomPattern() const;
	
private:
	CBulletPatternDataManager();
	virtual ~CBulletPatternDataManager();

private:
	PATTERN_LIST m_PatternList;
};

