#ifndef _CALLBACK_H_
#define _CALLBACK_H_

class Callback
{
	public:
		virtual void call(void * args) = 0;
};


template<class T>
class CB_Asserv: public Callback
{
    public:
        CB_Asserv(T* object, void (T::*methode)(int)):
            m_object(object), m_methode(methode) {}
        
		void call(void * arg)
        {
            (m_object->*m_methode)(*((int*)arg));
        }


    private:
        T* m_object;
        void (T::*m_methode)(int);
};

typedef struct {
    int id;
    bool state;
} args_microswitch;

template<class T>
class CB_Microswitch: public Callback
{
    public:
        CB_Microswitch(T* object, void (T::*methode)(int, bool)):
            m_object(object), m_methode(methode) {}

        void call(void * arg)
        {
            args_microswitch * args = (args_microswitch*)arg;
            (m_object->*m_methode)(args->id, args->state);
        }


    private:
        T* m_object;
        void (T::*m_methode)(int, bool);
};

typedef struct {
    int id;
	bool edge;
    int value;
} args_sonar;

template<class T>
class CB_Sonar: public Callback
{
    public:
        CB_Sonar(T* object, void (T::*methode)(int, bool, int)):
            m_object(object), m_methode(methode) {}

        void call(void * arg) {
            args_sonar * args = (args_sonar*)arg;
			(m_object->*m_methode)(args->id, args->edge, args->value);
        }


    private:
        T* m_object;
        void (T::*m_methode)(int, bool, int);
};

#endif
