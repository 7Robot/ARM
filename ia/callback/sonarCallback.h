#ifndef _SONAR_CALLBACK_H_
#define _SONAR_CALLBACK_H_

typedef struct sonarAnswer {
    int id;
	int edge;
    int value;
} sonarAnswer;

template<class T>
class sonarCallback: public Callback
{
    public:
        sonarCallback(T* object, void (T::*methode)(int, int, int)):
            m_object(object), m_methode(methode)
        {
            std::cout << "sonarCallback::CallBackMethode()" << std::endl;
        }
        void call(void * args)
        {
            sonarAnswer * answer = (sonarAnswer*)args;
            (m_object->*m_methode)(answer->id, answer->edge, answer->value);
        }


    private:
        T* m_object;
        void (T::*m_methode)(int id, int edge, int value);
};

#endif
