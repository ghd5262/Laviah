#pragma once

#include "cocos2d.h"

using namespace cocos2d;

class CSplitCircle : public TiledGrid3DAction
{
public:
	/**
	* creates the action with the number of columns to split and the duration
	* @param duration in seconds
	*/
	static CSplitCircle* create(float duration, unsigned int cols);

	// Overrides
	virtual CSplitCircle* clone() const override;
	/**
	* @param time in seconds
	*/
	virtual void update(float time) override;
	virtual void startWithTarget(Node *target) override;

CC_CONSTRUCTOR_ACCESS:
	CSplitCircle(){}
	virtual ~CSplitCircle(){}

	/** initializes the action with the number of columns to split and the duration */
	bool initWithDuration(float duration, unsigned int cols);

protected:
	unsigned int _cols;
	Size _winSize;

private:
	CC_DISALLOW_COPY_AND_ASSIGN(CSplitCircle);
};