#ifndef _IDENTIFIER_H_
#define _IDENTIFIER_H_

#include <string>

class Identifier
{
public:
	static const Identifier INVALID;

	Identifier() : mId(-1) {}
	Identifier(const std::string& id) { mId = std::hash<std::string>{}(id); }
	Identifier(const char* id) { mId = std::hash<std::string>{}(id); }
	Identifier(size_t id) : mId(id) {}

	Identifier(const Identifier& id) = default;
	Identifier& operator=(const Identifier& id) = default;

	friend bool operator==(const Identifier& id1, const Identifier& id2)
	{
		return id1.mId == id2.mId;
	}

	friend bool operator!=(const Identifier& id1, const Identifier& id2)
	{
		return id1.mId != id2.mId;
	}

	friend bool operator<(const Identifier& id1, const Identifier& id2)
	{
		return id1.mId < id2.mId;
	}

private:
	size_t mId;
};

#endif // !_IDENTIFIER_H_