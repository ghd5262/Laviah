#include "PoolingManager.h"
#include "../GameObject/ObjectManager.h"
#include "../GameObject/MenuSceneObjectManager.h"

CPoolingManager::CPoolingManager(){}

CPoolingManager::~CPoolingManager()
{
	DeleteAllMemory();
}

/* 모든 메모리를 해제한다. (게임이 종료되거나 Scene이 변경될때 호출) */
void CPoolingManager::DeleteAllMemory()
{
    auto clearList = [=](BLOCK_LIST& list){
        for (auto block : list)
        {
            delete[] block;
            block = nullptr;
        }
        list.clear();
    };
    
    clearList(m_ShooterList);
    clearList(m_BulletList);
    clearList(m_AlienList);
}

CPoolingManager* CPoolingManager::Instance()
{
	static CPoolingManager instance;
	return &instance;
}

void CPoolingManager::CreateBulletList(size_t count, size_t size)
{
    m_BulletSize = this->increaseBlockSizeIf64bit(size);
    this->createList(m_BulletList, m_BulletSize, count);
    
    // 오브젝트 매니저 리스트에 추가
    for(auto block : m_BulletList)
        CObjectManager::Instance()->AddBullet(block);
}

void CPoolingManager::CreateShooterList(size_t count, size_t size)
{
    m_ShooterSize = this->increaseBlockSizeIf64bit(size);
    this->createList(m_ShooterList, m_ShooterSize, count);
    
    // 오브젝트 매니저 리스트에 추가
    for(auto block : m_ShooterList)
        CObjectManager::Instance()->AddShooter(block);
}

void CPoolingManager::CreateAlienList(size_t count, size_t size)
{
    m_AlienSize = this->increaseBlockSizeIf64bit(size);
    this->createList(m_AlienList, m_AlienSize, count);
    
    // 오브젝트 매니저 리스트에 추가
    for(auto block : m_AlienList)
        CMenuSceneObjectManager::Instance()->AddAlien(block);
}

void* CPoolingManager::BulletNew()
{
    MEMORYBLOCK newBlock = nullptr;
    
    newBlock = this->getFreeMemoryFromList(m_BulletList, m_BulletSize);
    
    if(newBlock == nullptr)
    {
        CCLOG("BULLET LIST OVERFLOWED");
        this->addBlockToList(m_BulletList, m_BulletSize);
        
        newBlock = m_BulletList.back();
        
        /* 오브젝트 매니저 리스트에 추가한다. */
        CObjectManager::Instance()->AddBullet(newBlock);
        
        this->changeFreeMemoryToUsed(newBlock, m_BulletSize);
    }

	return newBlock;
}

void* CPoolingManager::ShooterNew()
{
    MEMORYBLOCK newBlock = nullptr;
    
    newBlock = this->getFreeMemoryFromList(m_ShooterList, m_ShooterSize);
    
    if(newBlock == nullptr)
    {
        CCLOG("SHOOTER LIST OVERFLOWED");
        this->addBlockToList(m_ShooterList, m_ShooterSize);
        
        newBlock = m_ShooterList.back();
        
        /* 오브젝트 매니저 리스트에 추가한다. */
        CObjectManager::Instance()->AddShooter(newBlock);
        
        this->changeFreeMemoryToUsed(newBlock, m_ShooterSize);
    }
    
    return newBlock;
}

void* CPoolingManager::AlienNew()
{
    MEMORYBLOCK newBlock = nullptr;
    
    newBlock = this->getFreeMemoryFromList(m_AlienList, m_AlienSize);
    
    if(newBlock == nullptr)
    {
        CCLOG("ALIEN LIST OVERFLOWED");
        this->addBlockToList(m_AlienList, m_AlienSize);
        
        newBlock = m_AlienList.back();
        
        /* 오브젝트 매니저 리스트에 추가한다. */
        CMenuSceneObjectManager::Instance()->AddAlien(newBlock);
        
        this->changeFreeMemoryToUsed(newBlock, m_AlienSize);
    }
    
    return newBlock;
}

void CPoolingManager::Bullet_ReturnToFreeMemory(void* bullet)
{
    this->changeUsedMemoryToFree(static_cast<char*>(bullet), m_BulletSize);
	//memset(bullet, 0, m_BulletSize + 1);
}

void CPoolingManager::Shooter_ReturnToFreeMemory(void* shooter)
{
    this->changeUsedMemoryToFree(static_cast<char*>(shooter), m_ShooterSize);
	//memset(shooter, 0, m_ShooterSize + 1);
}

void CPoolingManager::Alien_ReturnToFreeMemory(void* alien)
{
    this->changeUsedMemoryToFree(static_cast<char*>(alien), m_AlienSize);
    //memset(shooter, 0, m_ShooterSize + 1);
}

CPoolingManager::MEMORYBLOCK CPoolingManager::newMemoryBlock(size_t size) const
{
    /* memory alive를 위한 1바이트 추가 생성 */
    MEMORYBLOCK block = new char[size + 1];
    
    /* memory 초기화 및 memory alive = false */
    memset(block, 0, size + 1);
    return block;
}

CPoolingManager::MEMORYBLOCK CPoolingManager::getFreeMemoryFromList(BLOCK_LIST& list, size_t size)
{
    for (auto block : list)
    {
        /* 메모리가 Free(false)상태면 메모리를 사용 중 상태(true)로 전환 후 반환 */
        if (this->isFreeMemory(block, size)) {
            changeFreeMemoryToUsed(block, size);
            return block;
        }
    }
    
    return nullptr;
}

void CPoolingManager::addBlockToList(BLOCK_LIST& list, size_t size)
{
    list.emplace_back(newMemoryBlock(size));
}

void CPoolingManager::createList( BLOCK_LIST& list, size_t size, size_t count)
{
    list.reserve(count);
    while (count--) addBlockToList(list, size);
}

void CPoolingManager::changeFreeMemoryToUsed(MEMORYBLOCK block, size_t size)
{
    block[size] = true;
}

void CPoolingManager::changeUsedMemoryToFree(MEMORYBLOCK block, size_t size)
{
    block[size] = false;
}

bool CPoolingManager::isFreeMemory(MEMORYBLOCK block, size_t size)
{
    return (block[size] == false);
}

size_t CPoolingManager::increaseBlockSizeIf64bit(size_t size)
{
#if(defined(__x86_64__))
    return size * 2;
#else
    return size;
#endif
}
