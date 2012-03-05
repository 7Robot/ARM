#ifndef _MISSION_H_
#define _MISSION_H_

#include <pthread.h>

#include "Can.h"
#include "MissionHandler.h"

class MissionHandler;

class Mission
{
	public:
		void setup(Can * can, const char * name, Mission * owner);

		int getState() const;
		const char * getName() const;
		Mission * getOwner() const;


		virtual void start();

		virtual bool missionLoaded(Mission * mission, bool ownMission);
		virtual bool missionDone(Mission * mission, bool ownMission, bool completed);
		virtual bool microswitchEvent(int id, bool status);
		virtual bool asservDone(int error);
		virtual bool sonarEvent(int id, bool edge, bool nearby, int distance);
		virtual bool odoEvent(int x, int y, int theta);
		virtual bool canEvent(struct libcan::can_t packet);

		virtual void stop();

	protected:
		Can * can;
		int state;

		virtual bool missionLoaded(Mission * mission);
		virtual bool missionDone(Mission * mission);

		void load(const char * mission);
		void unload(Mission * mission);
		void end();
		void sleep(int secondes);
		void msleep(int microsecondes);

	private:
		int m_id;
		char * name;
		Mission * owner;
};

#endif
