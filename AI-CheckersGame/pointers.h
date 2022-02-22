#ifndef POINTERS_H
#define POINTERS_H

template <class T>
class pointers
{
private:
    T* pData;
public:
    pointers(T* pVal): pData(pVal) {}
    ~pointers()    {delete pData;}
    T& operator*() {return *pData;}
    T* operator->() {return pData;}
};

#endif //POINTERS_H
