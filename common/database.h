/*	EQEMu: Everquest Server Emulator
	Copyright (C) 2001-2016 EQEMu Development Team (http://eqemulator.net)

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; version 2 of the License.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY except by those people which sell it, which
	are required to give you total support for your newly bought product;
	without even the implied warranty of MERCHANTABILITY or FITNESS FOR
	A PARTICULAR PURPOSE. See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/
#ifndef EQEMU_DATABASE_H
#define EQEMU_DATABASE_H

#define AUTHENTICATION_TIMEOUT	60
#define INVALID_ID				0xFFFFFFFF

#include "global_define.h"
#include "eqemu_logsys.h"

#include "types.h"
#include "dbcore.h"
#include "linked_list.h"
#include "eq_packet_structs.h"

#include <cmath>
#include <string>
#include <vector>
#include <map>

//atoi is not uint32 or uint32 safe!!!!
#define atoul(str) strtoul(str, nullptr, 10)

class MySQLRequestResult;
class Client;

namespace EQEmu
{
	class InventoryProfile;
}

struct EventLogDetails_Struct {
	uint32	id;
	char	accountname[64];
	uint32	account_id;
	int16	status;
	char	charactername[64];
	char	targetname[64];
	char	timestamp[64];
	char	descriptiontype[64];
	char	details[128];
};

struct CharacterEventLog_Struct {
	uint32	count;
	uint8	eventid;
	EventLogDetails_Struct eld[255];
};

struct npcDecayTimes_Struct {
	uint16 minlvl;
	uint16 maxlvl;
	uint32 seconds;
};


struct VarCache_Struct {
	std::map<std::string, std::string> m_cache;
	uint32 last_update;
	VarCache_Struct() : last_update(0) { }
	void Add(const std::string &key, const std::string &value) { m_cache[key] = value; }
	const std::string *Get(const std::string &key) {
		auto it = m_cache.find(key);
		return (it != m_cache.end() ? &it->second : nullptr);
	}
};

class PTimerList;

#ifdef _WINDOWS
#if _MSC_VER > 1700 // greater than 2012 (2013+)
#	define _ISNAN_(a) std::isnan(a)
#else
#	include <float.h>
#	define _ISNAN_(a) _isnan(a)
#endif
#else
#	define _ISNAN_(a) std::isnan(a)
#endif

class Database : public DBcore {
public:
	Database();
	Database(const char* host, const char* user, const char* passwd, const char* database,uint32 port);
	bool Connect(const char* host, const char* user, const char* passwd, const char* database, uint32 port);
	~Database();
	/* Constants */
	static const std::string BANNED_IPS_TABLE;
	static const std::string AA_ABILITY_TABLE;
	static const std::string AA_RANK_EFFECTS_TABLE;
	static const std::string AA_RANK_PREREQS_TABLE;
	static const std::string AA_RANKS_TABLE;
	static const std::string AA_TIMERS_TABLE;
	static const std::string ACCOUNT_TABLE;
	static const std::string ACCOUNT_FLAGS_TABLE;
	static const std::string ACCOUNT_IP_TABLE;
	static const std::string ACCOUNT_REWARDS_TABLE;
	static const std::string ACTIVITIES_TABLE;
	static const std::string ADVENTURE_DETAILS_TABLE;
	static const std::string ADVENTURE_MEMBERS_TABLE;
	static const std::string ADVENTURE_STATS_TABLE;
	static const std::string ADVENTURE_TEMPLATE_TABLE;
	static const std::string ADVENTURE_TEMPLATE_ENTRY_TABLE;
	static const std::string ADVENTURE_TEMPLATE_ENTRY_FLAVOR_TABLE;
	static const std::string ALTADV_VARS_TABLE;
	static const std::string ALTERNATE_CURRENCY_TABLE;
	static const std::string BASE_DATA_TABLE;
	static const std::string BLOCKED_SPELLS_TABLE;
	static const std::string BOOKS_TABLE;
	static const std::string BUGS_TABLE;
	static const std::string BUYER_TABLE;
	static const std::string CHAR_CREATE_COMBINATIONS_TABLE;
	static const std::string CHAR_CREATE_POINT_ALLOCATIONS_TABLE;
	static const std::string CHAR_RECIPE_LIST_TABLE;
	static const std::string CHARACTER_ACTIVITIES_TABLE;
	static const std::string CHARACTER_ALT_CURRENCY_TABLE;
	static const std::string CHARACTER_ALTERNATE_ABILITIES_TABLE;
	static const std::string CHARACTER_BACKUP_TABLE;
	static const std::string CHARACTER_BANDOLIER_TABLE;
	static const std::string CHARACTER_BIND_TABLE;
	static const std::string CHARACTER_BUFFS_TABLE;
	static const std::string CHARACTER_CONSENT_TABLE;
	static const std::string CHARACTER_CORPSE_ITEMS_TABLE;
	static const std::string CHARACTER_CORPSE_ITEMS_BACKUP_TABLE;
	static const std::string CHARACTER_CORPSES_TABLE;
	static const std::string CHARACTER_CORPSES_BACKUP_TABLE;
	static const std::string CHARACTER_CURRENCY_TABLE;
	static const std::string CHARACTER_DATA_TABLE;
	static const std::string CHARACTER_DISCIPLINES_TABLE;
	static const std::string CHARACTER_ENABLEDTASKS_TABLE;
	static const std::string CHARACTER_INSPECT_MESSAGES_TABLE;
	static const std::string CHARACTER_ITEM_RECAST_TABLE;
	static const std::string CHARACTER_KEYRING_TABLE;
	static const std::string CHARACTER_LANGUAGES_TABLE;
	static const std::string CHARACTER_LEADERSHIP_ABILITIES_TABLE;
	static const std::string CHARACTER_LOOKUP_TABLE;
	static const std::string CHARACTER_MATERIAL_TABLE;
	static const std::string CHARACTER_MEMMED_SPELLS_TABLE;
	static const std::string CHARACTER_PET_BUFFS_TABLE;
	static const std::string CHARACTER_PET_INFO_TABLE;
	static const std::string CHARACTER_PET_INVENTORY_TABLE;
	static const std::string CHARACTER_POTIONBELT_TABLE;
	static const std::string CHARACTER_SKILLS_TABLE;
	static const std::string CHARACTER_SOULMARKS_TABLE;
	static const std::string CHARACTER_SPELLS_TABLE;
	static const std::string CHARACTER_TASKS_TABLE;
	static const std::string CHARACTER_TRIBUTE_TABLE;
	static const std::string CHATCHANNELS_TABLE;
	static const std::string CLASS_SKILL_TABLE;
	static const std::string COMMAND_SETTINGS_TABLE;
	static const std::string COMMANDS_TABLE;
	static const std::string COMMANDS_LOG_TABLE;
	static const std::string COMMANDS_OLD_TABLE;
	static const std::string COMPLETED_TASKS_TABLE;
	static const std::string DAMAGESHIELDTYPES_TABLE;
	static const std::string DB_STR_TABLE;
	static const std::string DB_VERSION_TABLE;
	static const std::string DISCOVERED_ITEMS_TABLE;
	static const std::string DOORS_TABLE;
	static const std::string EQTIME_TABLE;
	static const std::string EVENTLOG_TABLE;
	static const std::string FACTION_LIST_TABLE;
	static const std::string FACTION_LIST_MOD_TABLE;
	static const std::string FACTION_VALUES_TABLE;
	static const std::string FEAR_HINTS_TABLE;
	static const std::string FEEDBACK_TABLE;
	static const std::string FISHING_TABLE;
	static const std::string FORAGE_TABLE;
	static const std::string FRIENDS_TABLE;
	static const std::string GM_IPS_TABLE;
	static const std::string GOALLISTS_TABLE;
	static const std::string GRAVEYARD_TABLE;
	static const std::string GRID_TABLE;
	static const std::string GRID_ENTRIES_TABLE;
	static const std::string GROUND_SPAWNS_TABLE;
	static const std::string GROUP_ID_TABLE;
	static const std::string GROUP_LEADERS_TABLE;
	static const std::string GUILD_BANK_TABLE;
	static const std::string GUILD_MEMBERS_TABLE;
	static const std::string GUILD_RANKS_TABLE;
	static const std::string GUILD_RELATIONS_TABLE;
	static const std::string GUILDS_TABLE;
	static const std::string HACKERS_TABLE;
	static const std::string HORSES_TABLE;
	static const std::string INSTANCE_LIST_TABLE;
	static const std::string INSTANCE_LIST_PLAYER_TABLE;
	static const std::string INVENTORY_TABLE;
	static const std::string INVENTORY_SNAPSHOTS_TABLE;
	static const std::string ITEM_TICK_TABLE;
	static const std::string ITEMS_TABLE;
	static const std::string KEYRING_TABLE;
	static const std::string LAUNCHER_TABLE;
	static const std::string LAUNCHER_ZONES_TABLE;
	static const std::string LDON_TRAP_ENTRIES_TABLE;
	static const std::string LDON_TRAP_TEMPLATES_TABLE;
	static const std::string LEVEL_EXP_MODS_TABLE;
	static const std::string LFGUILD_TABLE;
	static const std::string LOGSYS_CATEGORIES_TABLE;
	static const std::string LOOTDROP_TABLE;
	static const std::string LOOTDROP_ENTRIES_TABLE;
	static const std::string LOOTTABLE_TABLE;
	static const std::string LOOTTABLE_ENTRIES_TABLE;
	static const std::string MAIL_TABLE;
	static const std::string MB_MESSAGES_TABLE;
	static const std::string MERC_ARMORINFO_TABLE;
	static const std::string MERC_BUFFS_TABLE;
	static const std::string MERC_INVENTORY_TABLE;
	static const std::string MERC_MERCHANT_ENTRIES_TABLE;
	static const std::string MERC_MERCHANT_TEMPLATE_ENTRIES_TABLE;
	static const std::string MERC_MERCHANT_TEMPLATES_TABLE;
	static const std::string MERC_NAME_TYPES_TABLE;
	static const std::string MERC_NPC_TYPES_TABLE;
	static const std::string MERC_SPELL_LIST_ENTRIES_TABLE;
	static const std::string MERC_SPELL_LISTS_TABLE;
	static const std::string MERC_STANCE_ENTRIES_TABLE;
	static const std::string MERC_STATS_TABLE;
	static const std::string MERC_SUBTYPES_TABLE;
	static const std::string MERC_TEMPLATES_TABLE;
	static const std::string MERC_TYPES_TABLE;
	static const std::string MERC_WEAPONINFO_TABLE;
	static const std::string MERCHANTLIST_TABLE;
	static const std::string MERCHANTLIST_TEMP_TABLE;
	static const std::string MERCS_TABLE;
	static const std::string NAME_FILTER_TABLE;
	static const std::string NPC_EMOTES_TABLE;
	static const std::string NPC_FACTION_TABLE;
	static const std::string NPC_FACTION_ENTRIES_TABLE;
	static const std::string NPC_SPELLS_TABLE;
	static const std::string NPC_SPELLS_EFFECTS_TABLE;
	static const std::string NPC_SPELLS_EFFECTS_ENTRIES_TABLE;
	static const std::string NPC_SPELLS_ENTRIES_TABLE;
	static const std::string NPC_TYPES_TABLE;
	static const std::string NPC_TYPES_METADATA_TABLE;
	static const std::string NPC_TYPES_TINT_TABLE;
	static const std::string OBJECT_TABLE;
	static const std::string OBJECT_CONTENTS_TABLE;
	static const std::string PEQ_ADMIN_TABLE;
	static const std::string PERL_EVENT_EXPORT_SETTINGS_TABLE;
	static const std::string PETITIONS_TABLE;
	static const std::string PETS_TABLE;
	static const std::string PETS_EQUIPMENTSET_TABLE;
	static const std::string PETS_EQUIPMENTSET_ENTRIES_TABLE;
	static const std::string PLAYER_TITLESETS_TABLE;
	static const std::string PROXIMITIES_TABLE;
	static const std::string QS_MERCHANT_TRANSACTION_LOG_TABLE;
	static const std::string QS_MERCHANT_TRANSACTION_RECORD_TABLE;
	static const std::string QS_MERCHANT_TRANSACTION_RECORD_ENTRIES_TABLE;
	static const std::string QS_PLAYER_AA_PURCHASE_LOG_TABLE;
	static const std::string QS_PLAYER_AA_RATE_HOURLY_TABLE;
	static const std::string QS_PLAYER_DELETE_RECORD_TABLE;
	static const std::string QS_PLAYER_DELETE_RECORD_ENTRIES_TABLE;
	static const std::string QS_PLAYER_EVENTS_TABLE;
	static const std::string QS_PLAYER_HANDIN_LOG_TABLE;
	static const std::string QS_PLAYER_HANDIN_RECORD_TABLE;
	static const std::string QS_PLAYER_HANDIN_RECORD_ENTRIES_TABLE;
	static const std::string QS_PLAYER_ITEM_DELETE_LOG_TABLE;
	static const std::string QS_PLAYER_ITEM_MOVE_LOG_TABLE;
	static const std::string QS_PLAYER_KILLED_BY_LOG_TABLE;
	static const std::string QS_PLAYER_LOOT_RECORDS_LOG_TABLE;
	static const std::string QS_PLAYER_MOVE_RECORD_TABLE;
	static const std::string QS_PLAYER_MOVE_RECORD_ENTRIES_TABLE;
	static const std::string QS_PLAYER_NPC_KILL_LOG_TABLE;
	static const std::string QS_PLAYER_NPC_KILL_RECORD_TABLE;
	static const std::string QS_PLAYER_NPC_KILL_RECORD_ENTRIES_TABLE;
	static const std::string QS_PLAYER_QGLOBAL_UPDATES_LOG_TABLE;
	static const std::string QS_PLAYER_SPEECH_TABLE;
	static const std::string QS_PLAYER_TRADE_LOG_TABLE;
	static const std::string QS_PLAYER_TRADE_RECORD_TABLE;
	static const std::string QS_PLAYER_TRADE_RECORD_ENTRIES_TABLE;
	static const std::string QS_PLAYER_TS_EVENT_LOG_TABLE;
	static const std::string QUEST_GLOBALS_TABLE;
	static const std::string RACES_TABLE;
	static const std::string RAID_DETAILS_TABLE;
	static const std::string RAID_LEADERS_TABLE;
	static const std::string RAID_MEMBERS_TABLE;
	static const std::string REPORTS_TABLE;
	static const std::string RESPAWN_TIMES_TABLE;
	static const std::string RULE_SETS_TABLE;
	static const std::string RULE_VALUES_TABLE;
	static const std::string SAYLINK_TABLE;
	static const std::string SHAREDBANK_TABLE;
	static const std::string SKILL_CAPS_TABLE;
	static const std::string SKILL_DIFFICULTY_TABLE;
	static const std::string SPAWN2_TABLE;
	static const std::string SPAWN_CONDITION_VALUES_TABLE;
	static const std::string SPAWN_CONDITIONS_TABLE;
	static const std::string SPAWN_EVENTS_TABLE;
	static const std::string SPAWNENTRY_TABLE;
	static const std::string SPAWNGROUP_TABLE;
	static const std::string SPELL_GLOBALS_TABLE;
	static const std::string SPELLS_NEW_TABLE;
	static const std::string START_ZONES_TABLE;
	static const std::string STARTING_ITEMS_TABLE;
	static const std::string TASKS_TABLE;
	static const std::string TASKSETS_TABLE;
	static const std::string TBLLOGINSERVERACCOUNTS_TABLE;
	static const std::string TBLSERVERADMINREGISTRATION_TABLE;
	static const std::string TBLSERVERLISTTYPE_TABLE;
	static const std::string TBLWORLDSERVERREGISTRATION_TABLE;
	static const std::string TBLACCOUNTACCESSLOG_TABLE;
	static const std::string TBLLOGINSERVERSETTINGS_TABLE;
	static const std::string TIMERS_TABLE;
	static const std::string TITLES_TABLE;
	static const std::string TRADER_TABLE;
	static const std::string TRADER_AUDIT_TABLE;
	static const std::string TRADESKILL_RECIPE_TABLE;
	static const std::string TRADESKILL_RECIPE_ENTRIES_TABLE;
	static const std::string TRAPS_TABLE;
	static const std::string TRIBUTE_LEVELS_TABLE;
	static const std::string TRIBUTES_TABLE;
	static const std::string VARIABLES_TABLE;
	static const std::string VETERAN_REWARD_TEMPLATES_TABLE;
	static const std::string WEBDATA_CHARACTER_TABLE;
	static const std::string WEBDATA_SERVERS_TABLE;
	static const std::string ZONE_TABLE;
	static const std::string ZONE_FLAGS_TABLE;
	static const std::string ZONE_POINTS_TABLE;
	static const std::string ZONE_SERVER_TABLE;
	static const std::string ZONE_STATE_DUMP_TABLE;
	static const std::string ZONESERVER_AUTH_TABLE;

	/* Character Creation */

	bool	AddToNameFilter(const char* name);
	bool	CreateCharacter(uint32 account_id, char* name, uint16 gender, uint16 race, uint16 class_, uint8 str, uint8 sta, uint8 cha, uint8 dex, uint8 int_, uint8 agi, uint8 wis, uint8 face);
	bool	DeleteCharacter(char* name);
	bool	MoveCharacterToZone(const char* charname, const char* zonename);
	bool	MoveCharacterToZone(const char* charname, const char* zonename,uint32 zoneid);
	bool	MoveCharacterToZone(uint32 iCharID, const char* iZonename);
	bool	ReserveName(uint32 account_id, char* name);
	bool	SaveCharacterCreate(uint32 character_id, uint32 account_id, PlayerProfile_Struct* pp);
	bool	SetHackerFlag(const char* accountname, const char* charactername, const char* hacked);
	bool	SetMQDetectionFlag(const char* accountname, const char* charactername, const char* hacked, const char* zone);
	bool	StoreCharacter(uint32 account_id, PlayerProfile_Struct* pp, EQEmu::InventoryProfile* inv);
	bool	UpdateName(const char* oldname, const char* newname);
	bool	SaveCharacterBindPoint(uint32 character_id, const BindStruct &bind);
	bool	MarkCharacterDeleted(char* name);
	bool	UnDeleteCharacter(const char* name);

	/* General Information Queries */

	bool	AddBannedIP(char* bannedIP, const char* notes); //Add IP address to the Banned_IPs table.
	bool	AddGMIP(char* ip_address, char* name);
	bool	CheckBannedIPs(const char* loginIP); //Check incoming connection against banned IP table.
	bool	CheckGMIPs(const char* loginIP, uint32 account_id);
	bool	CheckNameFilter(const char* name, bool surname = false);
	bool	CheckUsedName(const char* name);

	uint32	GetAccountIDByChar(const char* charname, uint32* oCharID = 0);
	uint32	GetAccountIDByChar(uint32 char_id);
	uint32	GetAccountIDByName(const char* accname, int16* status = 0, uint32* lsid = 0);
	uint32	GetCharacterID(const char *name);
	uint32	GetCharacterInfo(const char* iName, uint32* oAccID = 0, uint32* oZoneID = 0, uint32* oInstanceID = 0, float* oX = 0, float* oY = 0, float* oZ = 0);
	uint32	GetGuildIDByCharID(uint32 char_id);
	uint32	GetLevelByChar(const char* charname);

	void	GetAccountName(uint32 accountid, char* name, uint32* oLSAccountID = 0);
	void	GetCharName(uint32 char_id, char* name);
	void	LoginIP(uint32 AccountID, const char* LoginIP);

	/* Instancing */

	bool AddClientToInstance(uint16 instance_id, uint32 char_id);
	bool CharacterInInstanceGroup(uint16 instance_id, uint32 char_id);
	bool CheckInstanceExists(uint16 instance_id);
	bool CheckInstanceExpired(uint16 instance_id);
	bool CreateInstance(uint16 instance_id, uint32 zone_id, uint32 version, uint32 duration);
	bool GetUnusedInstanceID(uint16 &instance_id);
	bool GlobalInstance(uint16 instance_id);
	bool RemoveClientFromInstance(uint16 instance_id, uint32 char_id);
	bool RemoveClientsFromInstance(uint16 instance_id);
	bool VerifyInstanceAlive(uint16 instance_id, uint32 char_id);
	bool VerifyZoneInstance(uint32 zone_id, uint16 instance_id);

	uint16 GetInstanceID(const char* zone, uint32 charid, int16 version);
	uint16 GetInstanceID(uint32 zone, uint32 charid, int16 version);
	uint16 GetInstanceVersion(uint16 instance_id);
	uint32 GetTimeRemainingInstance(uint16 instance_id, bool &is_perma);
	uint32 VersionFromInstanceID(uint16 instance_id);
	uint32 ZoneIDFromInstanceID(uint16 instance_id);

	void AssignGroupToInstance(uint32 gid, uint32 instance_id);
	void AssignRaidToInstance(uint32 rid, uint32 instance_id);
	void BuryCorpsesInInstance(uint16 instance_id);
	void DeleteInstance(uint16 instance_id);
	void FlagInstanceByGroupLeader(uint32 zone, int16 version, uint32 charid, uint32 gid);
	void FlagInstanceByRaidLeader(uint32 zone, int16 version, uint32 charid, uint32 rid);
	void GetCharactersInInstance(uint16 instance_id, std::list<uint32> &charid_list);
	void PurgeExpiredInstances();
	void SetInstanceDuration(uint16 instance_id, uint32 new_duration);

	/* Adventure related. */

	void UpdateAdventureStatsEntry(uint32 char_id, uint8 theme, bool win);
	bool GetAdventureStats(uint32 char_id, AdventureStats_Struct *as);

	/* Account Related */

	bool	DeleteAccount(const char* name);
	bool	GetLiveChar(uint32 account_id, char* cname);
	bool	SetAccountStatus(const char* name, int16 status);
	bool	SetLocalPassword(uint32 accid, const char* password);
	bool	UpdateLiveChar(char* charname, uint32 lsaccount_id);

	int16	CheckStatus(uint32 account_id);

	uint32	CheckLogin(const char* name, const char* password, int16* oStatus = 0);
	uint32	CreateAccount(const char* name, const char* password, int16 status, uint32 lsaccount_id = 0);
	uint32	GetAccountIDFromLSID(uint32 iLSID, char* oAccountName = 0, int16* oStatus = 0);
	uint32	GetMiniLoginAccount(char* ip);
	uint8	GetAgreementFlag(uint32 acctid);

	void	GetAccountFromID(uint32 id, char* oAccountName, int16* oStatus);
	void	SetAgreementFlag(uint32 acctid);
	
	int		GetIPExemption(std::string account_ip);

	int		GetInstanceID(uint32 char_id, uint32 zone_id);


	/* Groups */
	
	char*	GetGroupLeaderForLogin(const char* name,char* leaderbuf);
	char*	GetGroupLeadershipInfo(uint32 gid, char* leaderbuf, char* maintank = nullptr, char* assist = nullptr, char* puller = nullptr, char *marknpc = nullptr, char *mentoree = nullptr, int *mentor_percent = nullptr, GroupLeadershipAA_Struct* GLAA = nullptr);
	
	uint32	GetGroupID(const char* name);
	
	void	ClearGroup(uint32 gid = 0);
	void	ClearGroupLeader(uint32 gid = 0);
	void	SetGroupID(const char* name, uint32 id, uint32 charid, uint32 ismerc = false);
	void	SetGroupLeaderName(uint32 gid, const char* name);

	/* Raids */

	const char *GetRaidLeaderName(uint32 rid);

	uint32	GetRaidID(const char* name);

	void	ClearRaid(uint32 rid = 0);
	void	ClearRaidDetails(uint32 rid = 0);
	void	ClearRaidLeader(uint32 gid = 0xFFFFFFFF, uint32 rid = 0);
	void	GetGroupLeadershipInfo(uint32 gid, uint32 rid, char* maintank = nullptr, char* assist = nullptr, char* puller = nullptr, char *marknpc = nullptr, char *mentoree = nullptr, int *mentor_percent = nullptr, GroupLeadershipAA_Struct* GLAA = nullptr);
	void	GetRaidLeadershipInfo(uint32 rid, char* maintank = nullptr, char* assist = nullptr, char* puller = nullptr, char *marknpc = nullptr, RaidLeadershipAA_Struct* RLAA = nullptr);
	void	SetRaidGroupLeaderInfo(uint32 gid, uint32 rid);

	/* Database Conversions 'database_conversions.cpp' */

	bool	CheckDatabaseConversions();
	bool	CheckDatabaseConvertCorpseDeblob();
	bool	CheckDatabaseConvertPPDeblob();

	/*
	* Database Setup for bootstraps only.
	*/
	bool DBSetup();
	bool DBSetup_PlayerCorpseBackup();

	/* Database Variables */

	bool	GetVariable(std::string varname, std::string &varvalue);
	bool	SetVariable(const std::string varname, const std::string &varvalue);
	bool	LoadVariables();

	/* General Queries */

	bool	GetSafePoints(const char* short_name, uint32 version, float* safe_x = 0, float* safe_y = 0, float* safe_z = 0, int16* minstatus = 0, uint8* minlevel = 0, char *flag_needed = nullptr);
	bool	GetSafePoints(uint32 zoneID, uint32 version, float* safe_x = 0, float* safe_y = 0, float* safe_z = 0, int16* minstatus = 0, uint8* minlevel = 0, char *flag_needed = nullptr) { return GetSafePoints(GetZoneName(zoneID), version, safe_x, safe_y, safe_z, minstatus, minlevel, flag_needed); }
	bool	GetZoneGraveyard(const uint32 graveyard_id, uint32* graveyard_zoneid = 0, float* graveyard_x = 0, float* graveyard_y = 0, float* graveyard_z = 0, float* graveyard_heading = 0);
	bool	GetZoneLongName(const char* short_name, char** long_name, char* file_name = 0, float* safe_x = 0, float* safe_y = 0, float* safe_z = 0, uint32* graveyard_id = 0, uint32* maxclients = 0);
	bool	LoadPTimers(uint32 charid, PTimerList &into);
	bool	LoadZoneNames();

	const char*	GetZoneName(uint32 zoneID, bool ErrorUnknown = false);

	uint32	GetZoneGraveyardID(uint32 zone_id, uint32 version);
	uint32	GetZoneID(const char* zonename);

	uint8	GetPEQZone(uint32 zoneID, uint32 version);
	uint8	GetRaceSkill(uint8 skillid, uint8 in_race);
	uint8	GetServerType();
	uint8	GetSkillCap(uint8 skillid, uint8 in_race, uint8 in_class, uint16 in_level);

	void	AddReport(std::string who, std::string against, std::string lines);
	struct TimeOfDay_Struct		LoadTime(time_t &realtime);
	bool	SaveTime(int8 minute, int8 hour, int8 day, int8 month, int16 year);
	void	ClearMerchantTemp();
	void	ClearPTimers(uint32 charid);
	void	SetFirstLogon(uint32 CharID, uint8 firstlogon);
	void	SetLFG(uint32 CharID, bool LFG);
	void	SetLFP(uint32 CharID, bool LFP);
	void	SetLoginFlags(uint32 CharID, bool LFP, bool LFG, uint8 firstlogon);

	void	ClearInvSnapshots(bool use_rule = true);

	/* EQEmuLogSys */
	void	LoadLogSettings(EQEmuLogSys::LogSettings* log_settings);

private:
	std::map<uint32,std::string>	zonename_array;

	Mutex Mvarcache;
	VarCache_Struct varcache;

	/* Groups, utility methods. */
	void    ClearAllGroupLeaders();
	void    ClearAllGroups();

	/* Raid, utility methods. */
	void ClearAllRaids();
	void ClearAllRaidDetails();
	void ClearAllRaidLeaders();
};

#endif
