/**
 * \file Stats.h
 * \brief RPG stats for characters or objects.
 * \author Ren Davis
 */
#ifndef _STATS_H_
#define _STATS_H_

#include <string>

#include <rapidjson\stringbuffer.h>
#include <rapidjson\prettywriter.h>

class Stats
{
private:
	std::string mName;

	unsigned int mLevel;

	unsigned int mCurExp;
	unsigned int mNextLevelExp;

	unsigned int mCurHP;
	unsigned int mMaxHP;

	unsigned int mStrength;
	unsigned int mMagic;
	unsigned int mSkill;
	unsigned int mSpeed;
	unsigned int mLuck;
	unsigned int mDefense;
	unsigned int mResistance;

public:
	Stats() :
		mName(),
		mLevel(0),
		mCurExp(0),
		mNextLevelExp(0),
		mCurHP(0),
		mMaxHP(0),
		mStrength(0),
		mMagic(0),
		mSkill(0),
		mSpeed(0),
		mLuck(0),
		mDefense(0),
		mResistance(0)
	{}

	Stats(const Stats& obj) = delete;
	Stats& operator=(const Stats& obj) = delete;

	void SetName(const std::string& name)	{ mName = name; }
	void SetLevel(unsigned int level)		{ mLevel = level; }
	
	void SetCurExp(unsigned int curExp)	{ mCurExp = curExp; }
	void SetNextLevelExp(unsigned int nextLevelExp)	{ mNextLevelExp = nextLevelExp; }

	void SetCurHP(unsigned int curHP)		{ mCurHP = curHP; }
	void SetMaxHP(unsigned int maxHP)		{ mMaxHP = maxHP; }

	void SetStrength(unsigned int str)	{ mStrength = str; }
	void SetMagic(unsigned int magic)		{ mMagic = magic; }
	void SetSkill(unsigned int skill)		{ mSkill = skill; }
	void SetSpeed(unsigned int speed)		{ mSpeed = speed; }
	void SetLuck(unsigned int luck)		{ mLuck = luck; }
	void SetDefense(unsigned int def)		{ mDefense = def; }
	void SetResistance(unsigned int res)	{ mResistance = res; }

	const std::string& GetName() const { return mName; }
	unsigned int GetLevel() const { return mLevel; }

	unsigned int GetCurExp() const { return mCurExp; }
	unsigned int GetNextLevelExp() const { return mNextLevelExp; }

	unsigned int GetCurHP() const { return mCurHP; }
	unsigned int GetMaxHP() const { return mMaxHP; }

	unsigned int GetStrength() const { return mStrength; }
	unsigned int GetMagic() const { return mMagic; }
	unsigned int GetSkill() const { return mSkill; }
	unsigned int GetSpeed() const { return mSpeed; }
	unsigned int GetLuck() const { return mLuck; }
	unsigned int GetDefense() const { return mDefense; }
	unsigned int GetResistance() const { return mResistance; }

	std::string ToString() const
	{
		rapidjson::StringBuffer s;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(s);

		writer.StartObject();

		writer.Key("type"); writer.String("Stats");
		writer.Key("name"); writer.String(GetName().c_str());
		writer.Key("level"); writer.Uint(GetLevel());

		writer.Key("curExp"); writer.Uint(GetCurExp());
		writer.Key("nextLevelExp"); writer.Uint(GetNextLevelExp());

		writer.Key("curHp"); writer.Uint(GetCurHP());
		writer.Key("maxHp"); writer.Uint(GetMaxHP());

		writer.Key("strength"); writer.Uint(GetStrength());
		writer.Key("magic"); writer.Uint(GetMagic());
		writer.Key("skill"); writer.Uint(GetSkill());
		writer.Key("speed"); writer.Uint(GetSpeed());
		writer.Key("luck"); writer.Uint(GetLuck());
		writer.Key("defense"); writer.Uint(GetDefense());
		writer.Key("resistance"); writer.Uint(GetResistance());

		writer.EndObject();

		return s.GetString();
	}

	friend std::ostream& operator<<(std::ostream& os, const Stats& obj)
	{
		os << obj.ToString() << std::endl;

		return os;
	}
};

#endif