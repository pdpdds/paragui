#include "pgtimerobject.h"

Uint32 PG_TimerObject::objectcount = 0;
PG_TimerID PG_TimerObject::globalTimerID = 0;
std::map<PG_TimerID, PG_TimerObject*> PG_TimerObject::timermap;

PG_TimerObject::PG_TimerObject() {
	if(objectcount == 0) {
		SDL_InitSubSystem(SDL_INIT_TIMER);
	}
	
	objectcount++;
}

PG_TimerObject::~PG_TimerObject() {
	
	// remove all timers of this object
	std::map<PG_TimerID, SDL_TimerID>::iterator i;
	
	for(i = my_timermap.begin(); i != my_timermap.end(); i++) {
		RemoveTimer((*i).first);
	}
	
	objectcount--;

	if(objectcount == 0) {
		SDL_QuitSubSystem(SDL_INIT_TIMER);
	}
}
	
PG_TimerID PG_TimerObject::AddTimer(Uint32 interval) {
	SDL_TimerID id = SDL_AddTimer(interval, &PG_TimerObject::callbackTimer, (void*)(globalTimerID+1));

	if(id == 0) {
		return 0;
	}
	
	PG_TimerID pgid = ++globalTimerID;
	my_timermap[pgid] = id;
	timermap[pgid] = this;
	
	return pgid;
}
	
bool PG_TimerObject::RemoveTimer(PG_TimerID id) {
	if(my_timermap[id] == 0) {
		return false;
	}
	
	return SDL_RemoveTimer(my_timermap[id]);
}
	
Uint32 PG_TimerObject::eventTimer(PG_TimerID id, Uint32 interval) {
	return interval;
}

Uint32 PG_TimerObject::callbackTimer(Uint32 interval, void* data) {
	PG_TimerID id = reinterpret_cast<PG_TimerID>(data);
	return timermap[id]->eventTimer(id, interval);
}
