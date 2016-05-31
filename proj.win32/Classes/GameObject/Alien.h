#include "Mover.h"

struct sALIEN_PARAM{
    sALIEN_PARAM(){}
};

class CAlien : public CMover {
public:
    virtual void ReturnToMemoryBlock() override;
    
protected:
    // PoolingManager에서 메모리를 할당 받는다.
    void* operator new (size_t size, const std::nothrow_t);
    
    // 실제 메모리 해제는 memorypooling에서 담당하지만
    // 소멸자를 호출하여 Node계열이 아닌 메모리들을 삭제하기 위함
    void operator delete(void* ptr){};
    
    CAlien(sALIEN_PARAM param, float distance)
    : CMover(0.0f)
    , m_AlienParam(param)
    , m_fDistance(distance){}
    virtual ~CAlien();
    
    //getter & setter
    CC_SYNTHESIZE(float, m_fDistance, Distance);// 0 > Distance 이면 bullet의 원래 distance값을 사용함
    CC_SYNTHESIZE(sALIEN_PARAM, m_AlienParam, AlienParam);
};