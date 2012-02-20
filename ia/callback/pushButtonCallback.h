#ifndef _PUSH_BUTTON_CALLBACK_H_
#define _PUSH_BUTTON_CALLBACK_H_

typedef struct pushButtonAnswer {
    int pbID;
    int state;
} pushButtonAnswer;

template<class T>
class pushButtonCallback: public Callback
{
    public:
        pushButtonCallback(T* object, void (T::*methode)(int, bool)):
            m_object(object), m_methode(methode)
        {
            std::cout << "pushButtonCallback::CallBackMethode()" << std::endl;
        }
        void call(void * args)
        {
            pushButtonAnswer * answer = (pushButtonAnswer*)args;
            (m_object->*m_methode)(answer->pbID, answer->state);
        }


    private:
        T* m_object;
        void (T::*m_methode)(int pbID, bool state);
};

#endif
