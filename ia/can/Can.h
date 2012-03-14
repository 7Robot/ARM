#ifndef _CAN_H_
#define _CAN_H_

#include <stdint.h>

typedef struct {
	int id;
	int length;
	uint8_t b[8];
} can_packet;

class Can
{
	public:
		Can(int canbus);
		void push(can_packet);
		void push(int id, int length, ...);

	private:
		void send(can_packet);
		int m_canbus;
};

#endif
