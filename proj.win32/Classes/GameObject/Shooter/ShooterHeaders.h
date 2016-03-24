#pragma once
#include "Shooter.h"
#include "AimingMissileShooter.h"
#include "DoubleScrewShooter.h"
#include "RandomMissileShooter.h"
#include "RandomShooter.h"
#include "ScrewShooter.h"

//--------------------------BonusLetter Shooter--------------------------
/* BonusLetterShooter : 보너스타임을 할 수 있는 문자를 쏘는 슈터*/
void BonusLetterShoot(float speed = 100.0f, float interval = 0.1f, int maxBulletCount = 1);
//------------------------------------------------------------------

/* BonusLetterShooter : 보너스타임을 할 수 있는 문자를 쏘는 슈터*/
void PlayItemShoot(float speed = 100.0f, float interval = 0.1f, int maxBulletCount = 1);
//------------------------------------------------------------------
