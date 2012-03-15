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

		// Callback from mission handler
		virtual bool missionLoaded(Mission * mission, bool ownMission);
		virtual bool missionLoaded(Mission * mission);
		virtual bool missionDone(Mission * mission, bool ownMission, bool completed);
		virtual bool missionDone(Mission * mission);


		// Callback from can bus
		virtual bool microswitchEvent(int id, bool status) { return true; };
		virtual bool sonarAnswer(int id, bool edge, bool nearby, int distance) { return true; };
		virtual bool odoRequest() { return true; };
		virtual bool odoBroadcast(bool broadcast) { return true; };
		virtual bool odoSet(int x, int y, int theta) { return true; };
		virtual bool odoAnswer(int x, int y, int theta) { return true; };
		virtual bool asservLineOrder(int distance) { return true; };
		virtual bool asservRotOrder(int angle) { return true; };
		virtual bool asservDone(int error) { return true; };
		virtual bool asservSpeedOrder(int left, int right, bool withRamp) { return true; };
		virtual bool asservUp(bool up) { return true; };
		virtual bool asservStop() { return true; };
		virtual bool globalReset() { return true; };
		virtual bool canEvent(struct libcan::can_t packet) { return true; };

		virtual void stop();

	protected:
		Can * can;
		int state;

		void load(const char * mission);
		void unload(Mission * mission);
		void end();
		//void msleep(int microsecondes);

	private:
		int m_id;
		char * name;
		Mission * owner;
};

#endif
