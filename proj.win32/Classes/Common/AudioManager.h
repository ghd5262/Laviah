#pragma once
#include <map>
#include "../../cocos2d/cocos/audio/include/AudioEngine.h"

using namespace cocos2d::experimental;
class CAudioManager
{
	struct sAUDIO_INFO
	{
		std::string _strPath;
		int _nCount;

		sAUDIO_INFO(std::string path = "empty", int count = 0)
			: _strPath(path)
			, _nCount(count){}
	};

public:
	static CAudioManager* Instance();

	void PlayEffectSound(const std::string& filePath, bool loop = false, float volume = 1.0f, const AudioProfile *profile = nullptr);
	void PlayBGM(const std::string& filePath, bool loop = false, float volume = 1.0f, const AudioProfile *profile = nullptr);
	void EmptyCurrentPlayingList();
	void setBGMVolume(float volume);

	//getter & setter
	CC_SYNTHESIZE(float, m_EffectSoundVolume, EffectSoundVolume);

private:
	// ����Ϸ��� ������� ����Ʈ�� �ִٸ� ��ȯ ���ٸ� �߰� �� ��ȯ
	sAUDIO_INFO* FindAudioIndex(std::string name);

	CAudioManager();
	~CAudioManager();

private:
	const int AUDIO_PLAY_MAX = 5;
	std::map<std::string, sAUDIO_INFO*> m_CurrentPlayingList;
	int m_BGMID;
};

