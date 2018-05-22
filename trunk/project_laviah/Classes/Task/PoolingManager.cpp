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
    
	clearList(m_BulletList);
}

CPoolingManager* CPoolingManager::Instance()
{
	static CPoolingManager instance;
	return &instance;
}

void CPoolingManager::CreateBulletList(size_t count, size_t size)
{
    m_BulletSize = this->increaseBlockSizeIf64bit(size);
    
    CCLOG("%lu byte", m_BulletSize * count);
    
    this->createList(m_BulletList, m_BulletSize, count);
    
    // 오브젝트 매니저 리스트에 추가
#if(USE_MEMORY_POOLING)
    for(auto block : m_BulletList)
        CObjectManager::Instance()->AddBullet(block);
#endif
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
#if(USE_MEMORY_POOLING)
        /* 오브젝트 매니저 리스트에 추가한다. */
        CObjectManager::Instance()->AddBullet(newBlock);
#endif
        this->changeFreeMemoryToUsed(newBlock, m_BulletSize);
    }

	return newBlock;
}

void CPoolingManager::Bullet_ReturnToFreeMemory(void* bullet)
{
    auto memoryBlock = static_cast<char*>(bullet);
    this->changeUsedMemoryToFree(memoryBlock, m_BulletSize);
	memset(memoryBlock, 0, m_BulletSize + 1);
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
