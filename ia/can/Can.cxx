#include "Can.h"
#include "Queue.h"
#include "Task.h"

#include <unistd.h> // fsync
#include <string.h> // memcpy
#include <stdio.h> // perror
#include <stdarg.h> // va_list

// Constructeur, enregistre le fd du bus can
Can::Can(int canbus): m_canbus(canbus) {}

// Construit un packet et l'ajoute à la file d'envois
void Can::push(int id, int length, ...)
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

	push(packet);
}

// Ajoute un packet à la file de tâche
void Can::push(can_packet packet)
{
	std::function<void (void)> callback = [this, packet]() {
		this->send(packet);
	};
	Task<std::function<void (void)>> * t = new Task<std::function<void (void)>>(callback);
}

// Non réentrante !
void Can::send(can_packet packet)
{
	static uint8_t raw[12] = { 0xFD };
	raw[1] = (packet.id >> 8) + (packet.length << 4);
	raw[2] = packet.id & 0xFF;
	memcpy(raw+3, packet.b, packet.length);
	raw[packet.length+3] = 0xBF;
	printf("%d %d\n", raw[1], raw[2]);
	if (write(m_canbus, raw, packet.length+4) == -1) {
		perror("write");
	}
	fsync(m_canbus);
}
