#pragma once
#include "../Common/HSHUtility.h"
#include <vector>
#include <memory>

struct sBULLET_PATTERN
{
	int _index;
	int _height;
	int _width;
	int _level;
    int _type;
	float _widthPadding;
	char _pattern[60 * 60]; // 패턴의 최대 크기 60 * 60 = 3600byte

	sBULLET_PATTERN()
    : _index(-1)
    , _height(-1)
    , _width(-1)
    , _level(-1)
    , _type(0)
    , _widthPadding(-1)
	{
		memset(_pattern, 0, sizeof(_pattern));
	}

	sBULLET_PATTERN(const sBULLET_PATTERN& data)
    : _index(data._index)
    , _height(data._height)
    , _width(data._width)
    , _level(data._level)
    , _type(data._type)
    , _widthPadding(data._widthPadding)
	{
		int idx = 0;
		for (auto it : data._pattern)
		{
			_pattern[idx++] = it;
		}
	}

	sBULLET_PATTERN(const sBULLET_PATTERN* data)
    : _index(data->_index)
    , _height(data->_height)
    , _width(data->_width)
    , _level(data->_level)
    , _type(data->_type)
    , _widthPadding(data->_widthPadding)
	{
		int idx = 0;
		for (auto it : data->_pattern)
		{
			_pattern[idx++] = it;
		}
	}
};

class CBulletPatternDataManager
{
    typedef std::vector<const sBULLET_PATTERN*> PATTERN_LIST;
    typedef std::function<bool(const sBULLET_PATTERN*)> PATTERN_PICK;
public:
	static CBulletPatternDataManager* Instance();

	//getter & setter
	const sBULLET_PATTERN* getNormalPatternByIndex(int index) const;
	const sBULLET_PATTERN* getMissilePatternByIndex(int index) const;
	const sBULLET_PATTERN* getBonusPatternByIndex(int index) const;
    const sBULLET_PATTERN* getTutorialPatternByIndex(int index) const;
	const sBULLET_PATTERN* getRandomNormalPatternByLevel(int level, int type, bool levelBelow);
    const sBULLET_PATTERN* getRandomConstellationPatternByLevel(int level, bool below);
    const sBULLET_PATTERN* getRandomBonusTimePattern();
    const sBULLET_PATTERN* getRandomPatternFromList(const PATTERN_PICK& callFunc,
                                                    PATTERN_LIST &list);
    
	const sBULLET_PATTERN* getTestPattern() const { return m_TestPattern; };
	void setTestPattern(sBULLET_PATTERN data){
		if (m_TestPattern != nullptr)
		{
			m_TestPattern->_index			= data._index;
			m_TestPattern->_height			= data._height;
			m_TestPattern->_width			= data._width;		
			m_TestPattern->_level			= data._level;
			m_TestPattern->_widthPadding	= data._widthPadding;
			std::copy(std::begin(data._pattern), std::end(data._pattern), std::begin(m_TestPattern->_pattern));
		}
	};

private:
    void initWithJson(PATTERN_LIST &list, std::string fileName);
    
	CBulletPatternDataManager();
	virtual ~CBulletPatternDataManager();

private:
	PATTERN_LIST m_PatternList;
	PATTERN_LIST m_MissilePatternList;
	PATTERN_LIST m_BonusTimePatternList;
    PATTERN_LIST m_ConstellationPatternList;
    PATTERN_LIST m_TutorialPatternList;
	sBULLET_PATTERN* m_TestPattern;
};

