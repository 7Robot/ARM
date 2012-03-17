#include "Robot.h"

Robot::Robot(int canbus, const char * basedir):
	m_queue(), m_can(canbus, &m_queue), m_mh(basedir, &m_can, &m_queue) {}

Module * Robot::load(const char * name)
{
	m_mh.load(name);
}
