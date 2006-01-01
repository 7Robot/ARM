#ifndef _CALLBACK_H_
#define _CALLBACK_H_

class Callback
{
	public:
		virtual void call(void * args) = 0;
};


template<class T>
class cb_asserv: public Callback
{
    public:
        cb_asserv(T* object, void (T::*methode)()):
            m_object(object), m_methode(methode) {}
        
		void call(void * args)
        {
            (m_object->*m_methode)();
        }


    private:
        T* m_object;
        void (T::*m_methode)();
};
/*
typedef struct {
    int pbID;
    int state;
} args_microswitch;

template<class T>
class cb_microswitch:
{
    public:
        cb_microswitch(T* object, void (T::*methode)(int, bool)):
            m_object(object), m_methode(methode) {}
        void call(void * args)
        {
            pushButtonAnswer * answer = (pushButtonAnswer*)args;
            (m_object->*m_methode)(answer->pbID, answer->state);
        }


    private:
        T* m_object;
        void (T::*m_methode)(int pbID, bool state);
};

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
};*/

#endif
