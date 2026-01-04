#pragma once
class CBaseHandle;
// ripped from the hl2src leak
class IHandleEntity {
public:
	virtual ~IHandleEntity() {}
	virtual void                 set_ref_e_handle(const CBaseHandle& handle) = 0;
	virtual const CBaseHandle& get_ref_e_handle() const = 0;
};