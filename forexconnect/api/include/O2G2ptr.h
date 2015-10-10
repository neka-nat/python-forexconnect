#pragma once
/** Smart pointer for IAddRef interface.
    Use for automatic calculation of instance object reference.
*/
#ifndef WIN32
#include <stddef.h> // note: must be include automatically but it does not.
#endif
template<typename T> class O2G2Ptr
{
 public:
    O2G2Ptr() : p(0){}
    O2G2Ptr(T* source) : p(source)
    {
    }
    O2G2Ptr(const O2G2Ptr<T>& source) : p(source.p)
    {
        if(p)
            p->addRef();
    }
    ~O2G2Ptr()
    {
        if(p)
            p->release();
    }

    void Release()
    {
        if (p)
        {
            p->release();
            p = NULL;
        }
    }
    T* Detach() {T* pTemp = p; p = NULL; return pTemp;}
    operator T*() const {return static_cast<T*>(p);}
    T& operator*() const {return *p;}
    T* operator->() const   {return p;}
    T* operator=(T* source)
    {
        Release();
        p=source;
        if (p!=NULL)
        {
            return static_cast<T*>(p);
        }
        return NULL;
    }
    T* operator=(const O2G2Ptr<T>& source)
    {
        Release();
        p = source.p;
        p->addRef();
        return static_cast<T*>(p);
    }
    bool operator!() const {return p==NULL;}
    operator bool() const {return p!=NULL;}
    bool operator<(T* pT) const {return p<pT;}
    bool operator==(T* pT) const {return p==pT;}
 private:
    T* p;
};

