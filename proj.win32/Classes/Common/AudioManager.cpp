#include "AudioManager.h"
#include "HSHUtility.h"
#include "../DataManager/UserDataManager.h"
CAudioManager::CAudioManager()
: m_BGMID(0)
, m_BGMSoundVolume(CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::BGM_VOLUME) / 100.f)
, m_EffectSoundVolume(CUserDataManager::Instance()->getUserData_Number(USERDATA_KEY::EFFECT_VOLUME) / 100.f)
{
	PUBLIC_CLICK_SOUND = std::pair<std::string, std::string>("", "");
}

CAudioManager::~CAudioManager()
{
	EmptyCurrentPlayingList();
}

CAudioManager* CAudioManager::Instance()
{
	static CAudioManager instance;
	return &instance;
}

void CAudioManager::Clear()
{
	AudioEngine::stopAll();
	m_BGMID = 0;
    m_BGMSoundVolume = 1.f;
	m_EffectSoundVolume = 1.f;
	m_CurrentPlayingList.clear();
}

void CAudioManager::PlayEffectSound(
	const std::string& filePath,
	bool loop/* = false*/, 
	float volume/* = -1.0f*/,
	const AudioProfile *profile/* = nullptr*/)
{
	sAUDIO_INFO* audio = nullptr;

	// 재생하려는 오디오가 리스트에 있다면 반환 없다면 추가 후 반환
	audio = FindAudioIndex(filePath);
	if (audio->_nCount < 10)
	{
		audio->_nCount++;
        volume = volume != -1.f ? volume : m_EffectSoundVolume;
        //		volume = m_EffectSoundVolume != 1.f ? m_EffectSoundVolume : volume;
		int id = AudioEngine::play2d(filePath, loop, volume, profile);
		AudioEngine::setFinishCallback(id, [this](int id, const std::string& path)
		{
			sAUDIO_INFO* temp;
			temp = FindAudioIndex(path);
			temp->_nCount--;
		});
	}
}

void CAudioManager::PlayBGM(
	const std::string& filePath,
	bool loop/* = false*/,
	float volume/* = -1.0f*/,
	const AudioProfile *profile/* = nullptr*/)
{
    volume = volume != -1.f ? volume : m_BGMSoundVolume;
	m_BGMID = AudioEngine::play2d(filePath, loop, volume, profile);
}

void CAudioManager::setBGMVolume(float volume)
{
    m_BGMSoundVolume = volume;
	AudioEngine::setVolume(m_BGMID, volume);
}

float CAudioManager::getBGMVolume()
{
    return m_BGMSoundVolume;
}

void CAudioManager::EmptyCurrentPlayingList()
{
    for(auto audio : m_CurrentPlayingList)
    {
        delete audio.second;
    }

	m_CurrentPlayingList.clear();
}


CAudioManager::sAUDIO_INFO* CAudioManager::FindAudioIndex(std::string name)
{
	if (m_CurrentPlayingList.find(name) == m_CurrentPlayingList.end())
	{
		// 아이디 +1
		CCLOG("CREATE NEW AUDIO %s", name.c_str());
		sAUDIO_INFO* newAudio = new sAUDIO_INFO(name, 0);
		m_CurrentPlayingList.emplace(std::pair<std::string, sAUDIO_INFO*>(name, newAudio));
		return newAudio;
	}

	return m_CurrentPlayingList.find(name)->second;
}

void CAudioManager::AllPause()
{
    AudioEngine::pauseAll();
}

void CAudioManager::AllResume()
{
    AudioEngine::resumeAll();
}