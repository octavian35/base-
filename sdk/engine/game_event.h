#pragma once

class IGameEventManager
{
public:
    virtual int  LoadEventsFromFile(const char* filename) = 0;
    virtual void Reset() = 0;
    virtual bool AddListener(IGameEventListener2* listener, const char* name, bool serverSide) = 0;
    virtual bool FindListener(IGameEventListener2* listener, const char* name) = 0;
    virtual int  RemoveListener(IGameEventListener2* listener) = 0;
    virtual IGameEvent* CreateEvents(const char* name, bool force) = 0;
};

class IGameEvent
{
public:
    virtual const char* GetName() const = 0;

    virtual bool  GetBool(const char* name, bool def = false) const = 0;
    virtual int   GetInt(const char* name, int def = 0) const = 0;
    virtual float GetFloat(const char* name, float def = 0.f) const = 0;
    virtual const char* GetString(const char* name, const char* def = "") const = 0;
};

class IGameEventListener2
{
public:
    virtual ~IGameEventListener2() {}
    virtual void FireGameEvent(IGameEvent* event) = 0;
    virtual int  GetEventDebugID(void) { return 42; }
};
