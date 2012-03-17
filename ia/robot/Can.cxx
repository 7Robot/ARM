#include "Can.h"

#include <unistd.h> // fsync
#include <string.h> // memcpy
#include <stdio.h> // perror
#include <stdarg.h> // va_list
#include <fcntl.h> // fcntl

// Constructeur, enregistre le fd du bus can
Can::Can(int canbus, Queue * queue):
	m_canbus(canbus), m_queue(queue), m_thr(&Can::recv, this) {}

void Can::wait()
{
	printf("Waiting for can.\n");
	m_thr.join();
}

// Construit un packet et l'ajoute à la file d'envois
Task * Can::push(int id, int length, ...)
{
	va_list ap;
	int i;
	can_packet packet;
	
	packet.id = id;
	packet.length = length;

	va_start(ap, length);
	for (i = 0 ; i < length ; i++) {
		packet.b[i] = (uint8_t)va_arg(ap, int);
	}
	va_end(ap);

	return push(packet);
}

// Ajoute un packet à la file de tâche, ou envoit immédiat si pas de file
Task * Can::push(can_packet packet)
{
	if (m_queue == NULL) {
		send(packet);
		return NULL;
	} else {
		Task * t = new Task([this, packet]() {
			this->send(packet);
		});
		m_queue->push(t);
		return t;
	}
}

// Non réentrante !
void Can::send(can_packet packet)
{
	static uint8_t raw[12] = { 0xFD };
	raw[1] = (packet.id >> 8) + (packet.length << 4);
	raw[2] = packet.id & 0xFF;
	memcpy(raw+3, packet.b, packet.length);
	raw[packet.length+3] = 0xBF;
	if (write(m_canbus, raw, packet.length+4) == -1) {
		perror("write");
	}
	fsync(m_canbus);

	printf("Packet sended:  ");
	printf("\t%-6i\t", packet.id);
	for (int i = 0 ; i < packet.length ; i++) {
		printf("   %-3i", packet.b[i]);
	}
	printf("\n");
}

void Can::recv()
{
	// Socket en mode *bloquante*
	int flags;
	if ((flags = fcntl(m_canbus, F_GETFL)) < 0) {
		perror("fcntl(F_GETFL)");
		return;
	}
	if (fcntl(m_canbus, F_SETFL, flags & ~O_NONBLOCK) < 0) {
		perror("fcntl(F_SETFL)");
		return;
	}

	/* state = 1 : waiting FD
	 * state = 2 : waiting length + ID high
	 * state = 3 : waiting ID low
	 * state = 0 : waiting BF
	 * state = -k : waiting k bytes of data */
	int state = 1;
	can_packet packet;
	uint8_t c;
	
	while (1) {
		if (read(m_canbus, &c, 1) <= 0) {
			perror("read");
			return;
		}
		if (state == 1) { /* waiting FD */
			if (c == 0xFD) {
                state = 2; /* waiting length + id high */
            }
        } else if (state == 2) { /* waiting length + id high */
            packet.length = c>>4;
            if (packet.length > 8) {
                state = 1; /* waiting FD */
            } else {
                packet.id = (c%16)<<8;
                state = 3; /* waiting id low */
            }
        } else if (state == 3) { /* waiting id low */
            packet.id |= c;
            state = -packet.length;
        } else if (state == 0) { /* waiting BF */
            if (c == 0xBF) {
				if (m_queue == NULL) {
					dispatch(packet);
				} else {
					Task * t = new Task([this, packet]() {
						dispatch(packet);
					});
					m_queue->push(t);
				}
                state = 1; /* waiting FD */
            } else {
                state = 1; /* waiting FD */
            }
        } else {
            packet.b[packet.length + state] = c;
            state++;
        }
	}
}

void Can::dispatch(can_packet packet)
{
	printf("Packet received:");
	printf("\t%-6i\t", packet.id);
	for (int i = 0 ; i < packet.length ; i++) {
		printf("   %-3i", packet.b[i]);
	}
	printf("\n");

	std::for_each(cbs.begin(), cbs.end(), [packet](std::pair<const void*, can_callback> cb) {
		if ((packet.id & std::get<0>(cb.second)) == std::get<1>(cb.second)) {
			(std::get<2>(cb.second))(packet);
		}
	});
}

void Can::bind(void* id, can_callback callback)
{
	cbs.insert(std::pair<void*, can_callback>(id, callback));
}

void Can::unbind(void* id)
{
	cbs.erase(id);
}
