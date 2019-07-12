
#ifndef _IUPDATEABLE_H_
#define _IUPDATEABLE_H_

class IUpdateable
{
public:
	virtual ~IUpdateable() {}

	virtual void Update() = 0;
};

#endif // !_IUPDATEABLE_H_