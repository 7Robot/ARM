#ifndef _ASSERV_CALLBACK_H_
#define _ASSERV_CALLBACK_H_

template<class T>
class asservCallback: public Callback
{
    public:
        asservCallback(T* object, void (T::*methode)()):
            m_object(object), m_methode(methode)
        {
            std::cout << "asservCallback::CallBackMethode()" << std::endl;
        }
        void call(void * args)
        {
            (m_object->*m_methode)();
        }


    private:
        T* m_object;
        void (T::*m_methode)();
};

#endif
