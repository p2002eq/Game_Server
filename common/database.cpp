/*	EQEMu: Everquest Server Emulator
	Copyright (C) 2001-2015 EQEMu Development Team (http://eqemulator.net)

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

#include "../common/global_define.h"
#include "../common/rulesys.h"

#include <ctype.h>
#include <iomanip>
#include <iostream>
#include <map>
#include <algorithm>
#include <mysqld_error.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Disgrace: for windows compile
#ifdef _WINDOWS
#include <windows.h>
#define snprintf	_snprintf
#define strncasecmp	_strnicmp
#define strcasecmp	_stricmp
#else
#include "unix.h"
#include <netinet/in.h>
#include <sys/time.h>
#endif

#include "database.h"
#include "eq_packet_structs.h"
#include "extprofile.h"
#include "string_util.h"

extern Client client;

Database::Database () {
}

/*
Establish a connection to a mysql database with the supplied parameters
*/

const std::string Database::BANNED_IPS_TABLE = "Banned_IPs";
const std::string Database::AA_ABILITY_TABLE = "aa_ability";
const std::string Database::AA_RANK_EFFECTS_TABLE = "aa_rank_effects";
const std::string Database::AA_RANK_PREREQS_TABLE = "aa_rank_prereqs";
const std::string Database::AA_RANKS_TABLE = "aa_ranks";
const std::string Database::AA_TIMERS_TABLE = "aa_timers";
const std::string Database::ACCOUNT_TABLE = "account";
const std::string Database::ACCOUNT_FLAGS_TABLE = "account_flags";
const std::string Database::ACCOUNT_IP_TABLE = "account_ip";
const std::string Database::ACCOUNT_REWARDS_TABLE = "account_rewards";
const std::string Database::ACTIVITIES_TABLE = "activities";
const std::string Database::ADVENTURE_DETAILS_TABLE = "adventure_details";
const std::string Database::ADVENTURE_MEMBERS_TABLE = "adventure_members";
const std::string Database::ADVENTURE_STATS_TABLE = "adventure_stats";
const std::string Database::ADVENTURE_TEMPLATE_TABLE = "adventure_template";
const std::string Database::ADVENTURE_TEMPLATE_ENTRY_TABLE = "adventure_template_entry";
const std::string Database::ADVENTURE_TEMPLATE_ENTRY_FLAVOR_TABLE = "adventure_template_entry_flavor";
const std::string Database::ALTADV_VARS_TABLE = "altadv_vars";
const std::string Database::ALTERNATE_CURRENCY_TABLE = "alternate_currency";
const std::string Database::BASE_DATA_TABLE = "base_data";
const std::string Database::BLOCKED_SPELLS_TABLE = "blocked_spells";
const std::string Database::BOOKS_TABLE = "books";
const std::string Database::BUGS_TABLE = "bugs";
const std::string Database::BUYER_TABLE = "buyer";
const std::string Database::CHAR_CREATE_COMBINATIONS_TABLE = "char_create_combinations";
const std::string Database::CHAR_CREATE_POINT_ALLOCATIONS_TABLE = "char_create_point_allocations";
const std::string Database::CHAR_RECIPE_LIST_TABLE = "char_recipe_list";
const std::string Database::CHARACTER_ACTIVITIES_TABLE = "character_activities";
const std::string Database::CHARACTER_ALT_CURRENCY_TABLE = "character_alt_currency";
const std::string Database::CHARACTER_ALTERNATE_ABILITIES_TABLE = "character_alternate_abilities";
const std::string Database::CHARACTER_BACKUP_TABLE = "character_backup";
const std::string Database::CHARACTER_BANDOLIER_TABLE = "character_bandolier";
const std::string Database::CHARACTER_BIND_TABLE = "character_bind";
const std::string Database::CHARACTER_BUFFS_TABLE = "character_buffs";
const std::string Database::CHARACTER_CONSENT_TABLE = "character_consent";
const std::string Database::CHARACTER_CORPSE_ITEMS_TABLE = "character_corpse_items";
const std::string Database::CHARACTER_CORPSE_ITEMS_BACKUP_TABLE = "character_corpse_items_backup";
const std::string Database::CHARACTER_CORPSES_TABLE = "character_corpses";
const std::string Database::CHARACTER_CORPSES_BACKUP_TABLE = "character_corpses_backup";
const std::string Database::CHARACTER_CURRENCY_TABLE = "character_currency";
const std::string Database::CHARACTER_DATA_TABLE = "character_data";
const std::string Database::CHARACTER_DISCIPLINES_TABLE = "character_disciplines";
const std::string Database::CHARACTER_ENABLEDTASKS_TABLE = "character_enabledtasks";
const std::string Database::CHARACTER_INSPECT_MESSAGES_TABLE = "character_inspect_messages";
const std::string Database::CHARACTER_ITEM_RECAST_TABLE = "character_item_recast";
const std::string Database::CHARACTER_KEYRING_TABLE = "character_keyring";
const std::string Database::CHARACTER_LANGUAGES_TABLE = "character_languages";
const std::string Database::CHARACTER_LEADERSHIP_ABILITIES_TABLE = "character_leadership_abilities";
const std::string Database::CHARACTER_LOOKUP_TABLE = "character_lookup";
const std::string Database::CHARACTER_MATERIAL_TABLE = "character_material";
const std::string Database::CHARACTER_MEMMED_SPELLS_TABLE = "character_memmed_spells";
const std::string Database::CHARACTER_PET_BUFFS_TABLE = "character_pet_buffs";
const std::string Database::CHARACTER_PET_INFO_TABLE = "character_pet_info";
const std::string Database::CHARACTER_PET_INVENTORY_TABLE = "character_pet_inventory";
const std::string Database::CHARACTER_POTIONBELT_TABLE = "character_potionbelt";
const std::string Database::CHARACTER_SKILLS_TABLE = "character_skills";
const std::string Database::CHARACTER_SOULMARKS_TABLE = "character_soulmarks";
const std::string Database::CHARACTER_SPELLS_TABLE = "character_spells";
const std::string Database::CHARACTER_TASKS_TABLE = "character_tasks";
const std::string Database::CHARACTER_TRIBUTE_TABLE = "character_tribute";
const std::string Database::CHATCHANNELS_TABLE = "chatchannels";
const std::string Database::CLASS_SKILL_TABLE = "class_skill";
const std::string Database::COMMAND_SETTINGS_TABLE = "command_settings";
const std::string Database::COMMANDS_TABLE = "commands";
const std::string Database::COMMANDS_LOG_TABLE = "commands_log";
const std::string Database::COMMANDS_OLD_TABLE = "commands_old";
const std::string Database::COMPLETED_TASKS_TABLE = "completed_tasks";
const std::string Database::DAMAGESHIELDTYPES_TABLE = "damageshieldtypes";
const std::string Database::DB_STR_TABLE = "db_str";
const std::string Database::DB_VERSION_TABLE = "db_version";
const std::string Database::DISCOVERED_ITEMS_TABLE = "discovered_items";
const std::string Database::DOORS_TABLE = "doors";
const std::string Database::EQTIME_TABLE = "eqtime";
const std::string Database::EVENTLOG_TABLE = "eventlog";
const std::string Database::FACTION_LIST_TABLE = "faction_list";
const std::string Database::FACTION_LIST_MOD_TABLE = "faction_list_mod";
const std::string Database::FACTION_VALUES_TABLE = "faction_values";
const std::string Database::FEAR_HINTS_TABLE = "fear_hints";
const std::string Database::FEEDBACK_TABLE = "feedback";
const std::string Database::FISHING_TABLE = "fishing";
const std::string Database::FORAGE_TABLE = "forage";
const std::string Database::FRIENDS_TABLE = "friends";
const std::string Database::GM_IPS_TABLE = "gm_ips";
const std::string Database::GOALLISTS_TABLE = "goallists";
const std::string Database::GRAVEYARD_TABLE = "graveyard";
const std::string Database::GRID_TABLE = "grid";
const std::string Database::GRID_ENTRIES_TABLE = "grid_entries";
const std::string Database::GROUND_SPAWNS_TABLE = "ground_spawns";
const std::string Database::GROUP_ID_TABLE = "group_id";
const std::string Database::GROUP_LEADERS_TABLE = "group_leaders";
const std::string Database::GUILD_BANK_TABLE = "guild_bank";
const std::string Database::GUILD_MEMBERS_TABLE = "guild_members";
const std::string Database::GUILD_RANKS_TABLE = "guild_ranks";
const std::string Database::GUILD_RELATIONS_TABLE = "guild_relations";
const std::string Database::GUILDS_TABLE = "guilds";
const std::string Database::HACKERS_TABLE = "hackers";
const std::string Database::HORSES_TABLE = "horses";
const std::string Database::INSTANCE_LIST_TABLE = "instance_list";
const std::string Database::INSTANCE_LIST_PLAYER_TABLE = "instance_list_player";
const std::string Database::INVENTORY_TABLE = "inventory";
const std::string Database::INVENTORY_SNAPSHOTS_TABLE = "inventory_snapshots";
const std::string Database::ITEM_TICK_TABLE = "item_tick";
const std::string Database::ITEMS_TABLE = "items";
const std::string Database::KEYRING_TABLE = "keyring";
const std::string Database::LAUNCHER_TABLE = "launcher";
const std::string Database::LAUNCHER_ZONES_TABLE = "launcher_zones";
const std::string Database::LDON_TRAP_ENTRIES_TABLE = "ldon_trap_entries";
const std::string Database::LDON_TRAP_TEMPLATES_TABLE = "ldon_trap_templates";
const std::string Database::LEVEL_EXP_MODS_TABLE = "level_exp_mods";
const std::string Database::LFGUILD_TABLE = "lfguild";
const std::string Database::LOGSYS_CATEGORIES_TABLE = "logsys_categories";
const std::string Database::LOOTDROP_TABLE = "lootdrop";
const std::string Database::LOOTDROP_ENTRIES_TABLE = "lootdrop_entries";
const std::string Database::LOOTTABLE_TABLE = "loottable";
const std::string Database::LOOTTABLE_ENTRIES_TABLE = "loottable_entries";
const std::string Database::MAIL_TABLE = "mail";
const std::string Database::MB_MESSAGES_TABLE = "mb_messages";
const std::string Database::MERC_ARMORINFO_TABLE = "merc_armorinfo";
const std::string Database::MERC_BUFFS_TABLE = "merc_buffs";
const std::string Database::MERC_INVENTORY_TABLE = "merc_inventory";
const std::string Database::MERC_MERCHANT_ENTRIES_TABLE = "merc_merchant_entries";
const std::string Database::MERC_MERCHANT_TEMPLATE_ENTRIES_TABLE = "merc_merchant_template_entries";
const std::string Database::MERC_MERCHANT_TEMPLATES_TABLE = "merc_merchant_templates";
const std::string Database::MERC_NAME_TYPES_TABLE = "merc_name_types";
const std::string Database::MERC_NPC_TYPES_TABLE = "merc_npc_types";
const std::string Database::MERC_SPELL_LIST_ENTRIES_TABLE = "merc_spell_list_entries";
const std::string Database::MERC_SPELL_LISTS_TABLE = "merc_spell_lists";
const std::string Database::MERC_STANCE_ENTRIES_TABLE = "merc_stance_entries";
const std::string Database::MERC_STATS_TABLE = "merc_stats";
const std::string Database::MERC_SUBTYPES_TABLE = "merc_subtypes";
const std::string Database::MERC_TEMPLATES_TABLE = "merc_templates";
const std::string Database::MERC_TYPES_TABLE = "merc_types";
const std::string Database::MERC_WEAPONINFO_TABLE = "merc_weaponinfo";
const std::string Database::MERCHANTLIST_TABLE = "merchantlist";
const std::string Database::MERCHANTLIST_TEMP_TABLE = "merchantlist_temp";
const std::string Database::MERCS_TABLE = "mercs";
const std::string Database::NAME_FILTER_TABLE = "name_filter";
const std::string Database::NPC_EMOTES_TABLE = "npc_emotes";
const std::string Database::NPC_FACTION_TABLE = "npc_faction";
const std::string Database::NPC_FACTION_ENTRIES_TABLE = "npc_faction_entries";
const std::string Database::NPC_SPELLS_TABLE = "npc_spells";
const std::string Database::NPC_SPELLS_EFFECTS_TABLE = "npc_spells_effects";
const std::string Database::NPC_SPELLS_EFFECTS_ENTRIES_TABLE = "npc_spells_effects_entries";
const std::string Database::NPC_SPELLS_ENTRIES_TABLE = "npc_spells_entries";
const std::string Database::NPC_TYPES_TABLE = "npc_types";
const std::string Database::NPC_TYPES_METADATA_TABLE = "npc_types_metadata";
const std::string Database::NPC_TYPES_TINT_TABLE = "npc_types_tint";
const std::string Database::OBJECT_TABLE = "object";
const std::string Database::OBJECT_CONTENTS_TABLE = "object_contents";
const std::string Database::PEQ_ADMIN_TABLE = "peq_admin";
const std::string Database::PERL_EVENT_EXPORT_SETTINGS_TABLE = "perl_event_export_settings";
const std::string Database::PETITIONS_TABLE = "petitions";
const std::string Database::PETS_TABLE = "pets";
const std::string Database::PETS_EQUIPMENTSET_TABLE = "pets_equipmentset";
const std::string Database::PETS_EQUIPMENTSET_ENTRIES_TABLE = "pets_equipmentset_entries";
const std::string Database::PLAYER_TITLESETS_TABLE = "player_titlesets";
const std::string Database::PROXIMITIES_TABLE = "proximities";
const std::string Database::QS_MERCHANT_TRANSACTION_LOG_TABLE = "qs_merchant_transaction_log";
const std::string Database::QS_MERCHANT_TRANSACTION_RECORD_TABLE = "qs_merchant_transaction_record";
const std::string Database::QS_MERCHANT_TRANSACTION_RECORD_ENTRIES_TABLE = "qs_merchant_transaction_record_entries";
const std::string Database::QS_PLAYER_AA_PURCHASE_LOG_TABLE = "qs_player_aa_purchase_log";
const std::string Database::QS_PLAYER_AA_RATE_HOURLY_TABLE = "qs_player_aa_rate_hourly";
const std::string Database::QS_PLAYER_DELETE_RECORD_TABLE = "qs_player_delete_record";
const std::string Database::QS_PLAYER_DELETE_RECORD_ENTRIES_TABLE = "qs_player_delete_record_entries";
const std::string Database::QS_PLAYER_EVENTS_TABLE = "qs_player_events";
const std::string Database::QS_PLAYER_HANDIN_LOG_TABLE = "qs_player_handin_log";
const std::string Database::QS_PLAYER_HANDIN_RECORD_TABLE = "qs_player_handin_record";
const std::string Database::QS_PLAYER_HANDIN_RECORD_ENTRIES_TABLE = "qs_player_handin_record_entries";
const std::string Database::QS_PLAYER_ITEM_DELETE_LOG_TABLE = "qs_player_item_delete_log";
const std::string Database::QS_PLAYER_ITEM_MOVE_LOG_TABLE = "qs_player_item_move_log";
const std::string Database::QS_PLAYER_KILLED_BY_LOG_TABLE = "qs_player_killed_by_log";
const std::string Database::QS_PLAYER_LOOT_RECORDS_LOG_TABLE = "qs_player_loot_records_log";
const std::string Database::QS_PLAYER_MOVE_RECORD_TABLE = "qs_player_move_record";
const std::string Database::QS_PLAYER_MOVE_RECORD_ENTRIES_TABLE = "qs_player_move_record_entries";
const std::string Database::QS_PLAYER_NPC_KILL_LOG_TABLE = "qs_player_npc_kill_log";
const std::string Database::QS_PLAYER_NPC_KILL_RECORD_TABLE = "qs_player_npc_kill_record";
const std::string Database::QS_PLAYER_NPC_KILL_RECORD_ENTRIES_TABLE = "qs_player_npc_kill_record_entries";
const std::string Database::QS_PLAYER_QGLOBAL_UPDATES_LOG_TABLE = "qs_player_qglobal_updates_log";
const std::string Database::QS_PLAYER_SPEECH_TABLE = "qs_player_speech";
const std::string Database::QS_PLAYER_TRADE_LOG_TABLE = "qs_player_trade_log";
const std::string Database::QS_PLAYER_TRADE_RECORD_TABLE = "qs_player_trade_record";
const std::string Database::QS_PLAYER_TRADE_RECORD_ENTRIES_TABLE = "qs_player_trade_record_entries";
const std::string Database::QS_PLAYER_TS_EVENT_LOG_TABLE = "qs_player_ts_event_log";
const std::string Database::QUEST_GLOBALS_TABLE = "quest_globals";
const std::string Database::RACES_TABLE = "races";
const std::string Database::RAID_DETAILS_TABLE = "raid_details";
const std::string Database::RAID_LEADERS_TABLE = "raid_leaders";
const std::string Database::RAID_MEMBERS_TABLE = "raid_members";
const std::string Database::REPORTS_TABLE = "reports";
const std::string Database::RESPAWN_TIMES_TABLE = "respawn_times";
const std::string Database::RULE_SETS_TABLE = "rule_sets";
const std::string Database::RULE_VALUES_TABLE = "rule_values";
const std::string Database::SAYLINK_TABLE = "saylink";
const std::string Database::SHAREDBANK_TABLE = "sharedbank";
const std::string Database::SKILL_CAPS_TABLE = "skill_caps";
const std::string Database::SKILL_DIFFICULTY_TABLE = "skill_difficulty";
const std::string Database::SPAWN2_TABLE = "spawn2";
const std::string Database::SPAWN_CONDITION_VALUES_TABLE = "spawn_condition_values";
const std::string Database::SPAWN_CONDITIONS_TABLE = "spawn_conditions";
const std::string Database::SPAWN_EVENTS_TABLE = "spawn_events";
const std::string Database::SPAWNENTRY_TABLE = "spawnentry";
const std::string Database::SPAWNGROUP_TABLE = "spawngroup";
const std::string Database::SPELL_GLOBALS_TABLE = "spell_globals";
const std::string Database::SPELLS_NEW_TABLE = "spells_new";
const std::string Database::START_ZONES_TABLE = "start_zones";
const std::string Database::STARTING_ITEMS_TABLE = "starting_items";
const std::string Database::TASKS_TABLE = "tasks";
const std::string Database::TASKSETS_TABLE = "tasksets";
const std::string Database::TBLLOGINSERVERACCOUNTS_TABLE = "tblLoginServerAccounts";
const std::string Database::TBLSERVERADMINREGISTRATION_TABLE = "tblServerAdminRegistration";
const std::string Database::TBLSERVERLISTTYPE_TABLE = "tblServerListType";
const std::string Database::TBLWORLDSERVERREGISTRATION_TABLE = "tblWorldServerRegistration";
const std::string Database::TBLACCOUNTACCESSLOG_TABLE = "tblaccountaccesslog";
const std::string Database::TBLLOGINSERVERSETTINGS_TABLE = "tblloginserversettings";
const std::string Database::TIMERS_TABLE = "timers";
const std::string Database::TITLES_TABLE = "titles";
const std::string Database::TRADER_TABLE = "trader";
const std::string Database::TRADER_AUDIT_TABLE = "trader_audit";
const std::string Database::TRADESKILL_RECIPE_TABLE = "tradeskill_recipe";
const std::string Database::TRADESKILL_RECIPE_ENTRIES_TABLE = "tradeskill_recipe_entries";
const std::string Database::TRAPS_TABLE = "traps";
const std::string Database::TRIBUTE_LEVELS_TABLE = "tribute_levels";
const std::string Database::TRIBUTES_TABLE = "tributes";
const std::string Database::VARIABLES_TABLE = "variables";
const std::string Database::VETERAN_REWARD_TEMPLATES_TABLE = "veteran_reward_templates";
const std::string Database::WEBDATA_CHARACTER_TABLE = "webdata_character";
const std::string Database::WEBDATA_SERVERS_TABLE = "webdata_servers";
const std::string Database::ZONE_TABLE = "zone";
const std::string Database::ZONE_FLAGS_TABLE = "zone_flags";
const std::string Database::ZONE_POINTS_TABLE = "zone_points";
const std::string Database::ZONE_SERVER_TABLE = "zone_server";
const std::string Database::ZONE_STATE_DUMP_TABLE = "zone_state_dump";
const std::string Database::ZONESERVER_AUTH_TABLE = "zoneserver_auth";

Database::Database(const char* host, const char* user, const char* passwd, const char* database, uint32 port)
{
	Connect(host, user, passwd, database, port);
}

bool Database::Connect(const char* host, const char* user, const char* passwd, const char* database, uint32 port) {
	uint32 errnum= 0;
	char errbuf[MYSQL_ERRMSG_SIZE];
	if (!Open(host, user, passwd, database, port, &errnum, errbuf)) {
		Log(Logs::General, Logs::Error, "Failed to connect to database: Error: %s", errbuf); 
		return false; 
	}
	else {
		Log(Logs::General, Logs::Status, "Using database '%s' at %s:%d", database, host,port);
		return true;
	}
}

/*
	Close the connection to the database
*/

Database::~Database()
{
}

/*
	Check if there is an account with name "name" and password "password"
	Return the account id or zero if no account matches.
	Zero will also be returned if there is a database error.
*/
uint32 Database::CheckLogin(const char* name, const char* password, int16* oStatus) {

	if(strlen(name) >= 50 || strlen(password) >= 50)
		return(0);

	char tmpUN[100];
	char tmpPW[100];

	DoEscapeString(tmpUN, name, strlen(name));
	DoEscapeString(tmpPW, password, strlen(password));

	std::string query = StringFormat("SELECT id, status FROM %s WHERE name='%s' AND password is not null "
											 "and length(password) > 0 and (password='%s' or password=MD5('%s'))", ACCOUNT_TABLE.c_str(),
		tmpUN, tmpPW, tmpPW);
	auto results = QueryDatabase(query);

	if (!results.Success())
	{
		return 0;
	}

	if(results.RowCount() == 0)
		return 0;

	auto row = results.begin();

	uint32 id = atoi(row[0]);

	if (oStatus)
		*oStatus = atoi(row[1]);

	return id;
}

//Get Banned IP Address List - Only return false if the incoming connection's IP address is not present in the banned_ips table.
bool Database::CheckBannedIPs(const char* loginIP)
{
	std::string query = StringFormat("SELECT ip_address FROM Banned_IPs WHERE ip_address='%s'", loginIP);

	auto results = QueryDatabase(query);

	if (!results.Success())
	{
		return true;
	}

	if (results.RowCount() != 0)
		return true;

	return false;
}

bool Database::AddBannedIP(char* bannedIP, const char* notes) {
	std::string query = StringFormat("INSERT into %s SET ip_address='%s', notes='%s'", BANNED_IPS_TABLE.c_str(), bannedIP, notes);
	auto results = QueryDatabase(query); 
	if (!results.Success()) {
		return false;
	} 
	return true;
}

 bool Database::CheckGMIPs(const char* ip_address, uint32 account_id) {
	 std::string query = StringFormat("SELECT * FROM %s WHERE `ip_address` = '%s' AND `account_id` = %i", GM_IPS_TABLE.c_str(), ip_address, account_id);
	auto results = QueryDatabase(query);

	if (!results.Success())
		return false;

	if (results.RowCount() == 1)
		return true;

	return false;
}

bool Database::AddGMIP(char* ip_address, char* name) {
	std::string query = StringFormat("INSERT into `%s` SET `ip_address` = '%s', `name` = '%s'", GM_IPS_TABLE.c_str(), ip_address, name);
	auto results = QueryDatabase(query); 
	return results.Success();
}

void Database::LoginIP(uint32 AccountID, const char* LoginIP) {
	std::string query = StringFormat("INSERT INTO %s SET accid=%i, ip='%s' ON DUPLICATE KEY UPDATE count=count+1, lastused=now()", ACCOUNT_IP_TABLE.c_str(), AccountID, LoginIP);
	QueryDatabase(query); 
}

int16 Database::CheckStatus(uint32 account_id) {
	std::string query = StringFormat("SELECT `status`, UNIX_TIMESTAMP(`suspendeduntil`) as `suspendeduntil`, UNIX_TIMESTAMP() as `current`"
											 " FROM `%s` WHERE `id` = %i", ACCOUNT_TABLE.c_str(), account_id);

	auto results = QueryDatabase(query); 
	if (!results.Success()) {
		return 0;
	}

	if (results.RowCount() != 1)
		return 0;
	
	auto row = results.begin(); 
	int16 status = atoi(row[0]); 
	int32 suspendeduntil = 0;

	// MariaDB initalizes with NULL if unix_timestamp() is out of range
	if (row[1] != nullptr) {
		suspendeduntil = atoi(row[1]);
	}

	int32 current = atoi(row[2]);

	if(suspendeduntil > current)
		return -1;

	return status;
}

uint32 Database::CreateAccount(const char* name, const char* password, int16 status, uint32 lsaccount_id) {
	std::string query;

	if (password)
		query = StringFormat("INSERT INTO %s SET name='%s', password='%s', status=%i, lsaccount_id=%i, time_creation=UNIX_TIMESTAMP();", ACCOUNT_TABLE.c_str(), name,password,status, lsaccount_id);
	else
		query = StringFormat("INSERT INTO %s SET name='%s', status=%i, lsaccount_id=%i, time_creation=UNIX_TIMESTAMP();", ACCOUNT_TABLE.c_str(),name, status, lsaccount_id);

	Log(Logs::General, Logs::World_Server, "Account Attempting to be created: '%s' status: %i", name, status);
	auto results = QueryDatabase(query);

	if (!results.Success()) {
		return 0;
	}

	if (results.LastInsertedID() == 0)
	{
		return 0;
	}

	return results.LastInsertedID();
}

bool Database::DeleteAccount(const char* name) {
	std::string query = StringFormat("DELETE FROM %s WHERE name='%s';", ACCOUNT_TABLE.c_str(), name);
	Log(Logs::General, Logs::World_Server, "Account Attempting to be deleted:'%s'", name);

	auto results = QueryDatabase(query); 
	if (!results.Success()) {
		return false;
	}

	return results.RowsAffected() == 1;
}

bool Database::SetLocalPassword(uint32 accid, const char* password) {
	std::string query = StringFormat("UPDATE %s SET password=MD5('%s') where id=%i;", ACCOUNT_TABLE.c_str(), EscapeString(password).c_str(), accid);

	auto results = QueryDatabase(query);

	if (!results.Success()) {
		return false;
	}

	return true;
}

bool Database::SetAccountStatus(const char* name, int16 status) {
	std::string query = StringFormat("UPDATE %s SET status=%i WHERE name='%s';", ACCOUNT_TABLE.c_str(), status, name);

	std::cout << "Account being GM Flagged:" << name << ", Level: " << (int16) status << std::endl;

	auto results = QueryDatabase(query);

	if (!results.Success())
		return false;

	if (results.RowsAffected() == 0)
	{
		std::cout << "Account: " << name << " does not exist, therefore it cannot be flagged\n";
		return false; 
	}

	return true;
}

/* This initially creates the character during character create */
bool Database::ReserveName(uint32 account_id, char* name) {
	std::string query = StringFormat("SELECT `account_id`, `name` FROM `%s` WHERE `name` = '%s'", CHARACTER_DATA_TABLE.c_str(), name);
	auto results = QueryDatabase(query);
	for (auto row = results.begin(); row != results.end(); ++row) {
		if (row[0] && atoi(row[0]) > 0){
			Log(Logs::General, Logs::World_Server, "Account: %i tried to request name: %s, but it is already taken...", account_id, name);
			return false;
		}
	}

	query = StringFormat("INSERT INTO `%s` SET `account_id` = %i, `name` = '%s'", CHARACTER_DATA_TABLE.c_str(), account_id, name);
	results = QueryDatabase(query);
	if (!results.Success() || results.ErrorMessage() != ""){ return false; } 
	return true;
}

/*
	Delete the character with the name "name"
	returns false on failure, true otherwise
*/
bool Database::DeleteCharacter(char *name) {
	uint32 charid = 0;
	if(!name ||	!strlen(name)) {
		Log(Logs::General, Logs::World_Server, "DeleteCharacter: request to delete without a name (empty char slot)");
		return false;
	}
	Log(Logs::General, Logs::World_Server, "Database::DeleteCharacter name : '%s'", name);

	/* Get id from character_data before deleting record so we can clean up the rest of the tables */
	std::string query = StringFormat("SELECT `id` from `%s` WHERE `name` = '%s'", CHARACTER_DATA_TABLE.c_str(), name);
	auto results = QueryDatabase(query);
	for (auto row = results.begin(); row != results.end(); ++row) { charid = atoi(row[0]); }
	if (charid <= 0){ 
		Log(Logs::General, Logs::Error, "Database::DeleteCharacter :: Character (%s) not found, stopping delete...", name);
		return false; 
	}

	query = StringFormat("DELETE FROM `%s` WHERE `charid` = '%d'", QUEST_GLOBALS_TABLE.c_str(), charid); QueryDatabase(query);
	query = StringFormat("DELETE FROM `%s` WHERE `charid` = '%d'", CHARACTER_ACTIVITIES_TABLE.c_str(), charid); QueryDatabase(query);
	query = StringFormat("DELETE FROM `%s` WHERE `charid` = '%d'", FRIENDS_TABLE.c_str(), charid); QueryDatabase(query);
	query = StringFormat("DELETE FROM `%s` WHERE `charid` = '%d'", MAIL_TABLE.c_str(), charid); QueryDatabase(query);
	query = StringFormat("DELETE FROM `%s` WHERE `char_id` = '%d'", TIMERS_TABLE.c_str(), charid); QueryDatabase(query);
	query = StringFormat("DELETE FROM `%s` WHERE `charid` = '%d'", INVENTORY_TABLE.c_str(), charid); QueryDatabase(query);
	query = StringFormat("DELETE FROM `%s` WHERE `char_id` = '%d'", CHAR_RECIPE_LIST_TABLE.c_str(), charid); QueryDatabase(query);
	query = StringFormat("DELETE FROM `%s` WHERE `charID` = '%d'", ZONE_FLAGS_TABLE.c_str(), charid); QueryDatabase(query);
	query = StringFormat("DELETE FROM `%s` WHERE `char_id` = '%d'", TITLES_TABLE.c_str(), charid); QueryDatabase(query);
	query = StringFormat("DELETE FROM `%s` WHERE `char_id` = '%d'", PLAYER_TITLESETS_TABLE.c_str(), charid); QueryDatabase(query);
	query = StringFormat("DELETE FROM `%s` WHERE `char_id` = '%d'", KEYRING_TABLE.c_str(), charid); QueryDatabase(query);
	query = StringFormat("DELETE FROM `%s` WHERE `char_id` = '%d'", FACTION_VALUES_TABLE.c_str(), charid); QueryDatabase(query);
	query = StringFormat("DELETE FROM `%s` WHERE `charid` = '%d'", INSTANCE_LIST_PLAYER_TABLE.c_str(), charid); QueryDatabase(query);
	query = StringFormat("DELETE FROM `%s` WHERE `id` = '%d'", CHARACTER_DATA_TABLE.c_str(), charid); QueryDatabase(query);
	query = StringFormat("DELETE FROM `%s` WHERE `id` = %u", CHARACTER_SKILLS_TABLE.c_str(), charid); QueryDatabase(query);
	query = StringFormat("DELETE FROM `%s` WHERE `id` = %u", CHARACTER_LANGUAGES_TABLE.c_str(), charid); QueryDatabase(query);
	query = StringFormat("DELETE FROM `%s` WHERE `id` = %u", CHARACTER_BIND_TABLE.c_str(), charid); QueryDatabase(query);
	query = StringFormat("DELETE FROM `%s` WHERE `id` = %u", CHARACTER_ALTERNATE_ABILITIES_TABLE.c_str(), charid); QueryDatabase(query);
	query = StringFormat("DELETE FROM `%s` WHERE `id` = %u", CHARACTER_CURRENCY_TABLE.c_str(), charid); QueryDatabase(query);
	// Left off db constant conversion
	query = StringFormat("DELETE FROM `character_data` WHERE `id` = %u", charid); QueryDatabase(query);
	query = StringFormat("DELETE FROM `character_spells` WHERE `id` = %u", charid); QueryDatabase(query);
	query = StringFormat("DELETE FROM `character_memmed_spells` WHERE `id` = %u", charid); QueryDatabase(query);
	query = StringFormat("DELETE FROM `character_disciplines` WHERE `id` = %u", charid); QueryDatabase(query);
	query = StringFormat("DELETE FROM `character_material` WHERE `id` = %u", charid); QueryDatabase(query);
	query = StringFormat("DELETE FROM `character_tribute` WHERE `id` = %u", charid); QueryDatabase(query);
	query = StringFormat("DELETE FROM `character_bandolier` WHERE `id` = %u", charid); QueryDatabase(query);
	query = StringFormat("DELETE FROM `character_potionbelt` WHERE `id` = %u", charid); QueryDatabase(query);
	query = StringFormat("DELETE FROM `character_inspect_messages` WHERE `id` = %u", charid); QueryDatabase(query);
	query = StringFormat("DELETE FROM `character_leadership_abilities` WHERE `id` = %u", charid); QueryDatabase(query);
	query = StringFormat("DELETE FROM `character_alt_currency` WHERE `char_id` = '%d'", charid); QueryDatabase(query);
#ifdef BOTS
	query = StringFormat("DELETE FROM `guild_members` WHERE `char_id` = '%d' AND GetMobTypeById(%i) = 'C'", charid); // note: only use of GetMobTypeById()
#else
	query = StringFormat("DELETE FROM `guild_members` WHERE `char_id` = '%d'", charid);
#endif
	QueryDatabase(query);
	
	return true;
}

bool Database::SaveCharacterCreate(uint32 character_id, uint32 account_id, PlayerProfile_Struct* pp){
	std::string query = StringFormat(
		"REPLACE INTO `%s` ("
		"id,"
		"account_id,"
		"`name`,"
		"last_name,"
		"gender,"
		"race,"
		"class,"
		"`level`,"
		"deity,"
		"birthday,"
		"last_login,"
		"time_played,"
		"pvp_status,"
		"level2,"
		"anon,"
		"gm,"
		"intoxication,"
		"hair_color,"
		"beard_color,"
		"eye_color_1,"
		"eye_color_2,"
		"hair_style,"
		"beard,"
		"ability_time_seconds,"
		"ability_number,"
		"ability_time_minutes,"
		"ability_time_hours,"
		"title,"
		"suffix,"
		"exp,"
		"points,"
		"mana,"
		"cur_hp,"
		"str,"
		"sta,"
		"cha,"
		"dex,"
		"`int`,"
		"agi,"
		"wis,"
		"face,"
		"y,"
		"x,"
		"z,"
		"heading,"
		"pvp2,"
		"pvp_type,"
		"autosplit_enabled,"
		"zone_change_count,"
		"drakkin_heritage,"
		"drakkin_tattoo,"
		"drakkin_details,"
		"toxicity,"
		"hunger_level,"
		"thirst_level,"
		"ability_up,"
		"zone_id,"
		"zone_instance,"
		"leadership_exp_on,"
		"ldon_points_guk,"
		"ldon_points_mir,"
		"ldon_points_mmc,"
		"ldon_points_ruj,"
		"ldon_points_tak,"
		"ldon_points_available,"
		"tribute_time_remaining,"
		"show_helm,"
		"career_tribute_points,"
		"tribute_points,"
		"tribute_active,"
		"endurance,"
		"group_leadership_exp,"
		"raid_leadership_exp,"
		"group_leadership_points,"
		"raid_leadership_points,"
		"air_remaining,"
		"pvp_kills,"
		"pvp_deaths,"
		"pvp_current_points,"
		"pvp_career_points,"
		"pvp_best_kill_streak,"
		"pvp_worst_death_streak,"
		"pvp_current_kill_streak,"
		"aa_points_spent,"
		"aa_exp,"
		"aa_points,"
		"group_auto_consent,"
		"raid_auto_consent,"
		"guild_auto_consent,"
		"RestTimer) "
		"VALUES ("
		"%u,"  // id					
		"%u,"  // account_id			
		"'%s',"  // `name`				
		"'%s',"  // last_name			
		"%u,"  // gender				
		"%u,"  // race					
		"%u,"  // class					
		"%u,"  // `level`				
		"%u,"  // deity					
		"%u,"  // birthday				
		"%u,"  // last_login			
		"%u,"  // time_played			
		"%u,"  // pvp_status			
		"%u,"  // level2				
		"%u,"  // anon					
		"%u,"  // gm					
		"%u,"  // intoxication			
		"%u,"  // hair_color			
		"%u,"  // beard_color			
		"%u,"  // eye_color_1			
		"%u,"  // eye_color_2			
		"%u,"  // hair_style			
		"%u,"  // beard					
		"%u,"  // ability_time_seconds	
		"%u,"  // ability_number		
		"%u,"  // ability_time_minutes	
		"%u,"  // ability_time_hours	
		"'%s',"  // title				
		"'%s',"  // suffix				
		"%u,"  // exp					
		"%u,"  // points				
		"%u,"  // mana					
		"%u,"  // cur_hp				
		"%u,"  // str					
		"%u,"  // sta					
		"%u,"  // cha					
		"%u,"  // dex					
		"%u,"  // `int`					
		"%u,"  // agi					
		"%u,"  // wis					
		"%u,"  // face					
		"%f,"  // y						
		"%f,"  // x						
		"%f,"  // z						
		"%f,"  // heading				
		"%u,"  // pvp2					
		"%u,"  // pvp_type				
		"%u,"  // autosplit_enabled		
		"%u,"  // zone_change_count		
		"%u,"  // drakkin_heritage		
		"%u,"  // drakkin_tattoo		
		"%u,"  // drakkin_details		
		"%i,"  // toxicity				
		"%i,"  // hunger_level			
		"%i,"  // thirst_level			
		"%u,"  // ability_up			
		"%u,"  // zone_id				
		"%u,"  // zone_instance			
		"%u,"  // leadership_exp_on		
		"%u,"  // ldon_points_guk		
		"%u,"  // ldon_points_mir		
		"%u,"  // ldon_points_mmc		
		"%u,"  // ldon_points_ruj		
		"%u,"  // ldon_points_tak		
		"%u,"  // ldon_points_available	
		"%u,"  // tribute_time_remaining
		"%u,"  // show_helm				
		"%u,"  // career_tribute_points	
		"%u,"  // tribute_points		
		"%u,"  // tribute_active		
		"%u,"  // endurance				
		"%u,"  // group_leadership_exp	
		"%u,"  // raid_leadership_exp	
		"%u,"  // group_leadership_point
		"%u,"  // raid_leadership_points
		"%u,"  // air_remaining			
		"%u,"  // pvp_kills				
		"%u,"  // pvp_deaths			
		"%u,"  // pvp_current_points	
		"%u,"  // pvp_career_points		
		"%u,"  // pvp_best_kill_streak	
		"%u,"  // pvp_worst_death_streak
		"%u,"  // pvp_current_kill_strea
		"%u,"  // aa_points_spent		
		"%u,"  // aa_exp				
		"%u,"  // aa_points				
		"%u,"  // group_auto_consent	
		"%u,"  // raid_auto_consent		
		"%u,"  // guild_auto_consent	
		"%u"  // RestTimer				
		")",
		CHARACTER_DATA_TABLE.c_str(),
		character_id,					  // " id,                        "
		account_id,						  // " account_id,                "
		EscapeString(pp->name).c_str(),	  // " `name`,                    "
		EscapeString(pp->last_name).c_str(), // " last_name,              "
		pp->gender,						  // " gender,                    "
		pp->race,						  // " race,                      "
		pp->class_,						  // " class,                     "
		pp->level,						  // " `level`,                   "
		pp->deity,						  // " deity,                     "
		pp->birthday,					  // " birthday,                  "
		pp->lastlogin,					  // " last_login,                "
		pp->timePlayedMin,				  // " time_played,               "
		pp->pvp,						  // " pvp_status,                "
		pp->level2,						  // " level2,                    "
		pp->anon,						  // " anon,                      "
		pp->gm,							  // " gm,                        "
		pp->intoxication,				  // " intoxication,              "
		pp->haircolor,					  // " hair_color,                "
		pp->beardcolor,					  // " beard_color,               "
		pp->eyecolor1,					  // " eye_color_1,               "
		pp->eyecolor2,					  // " eye_color_2,               "
		pp->hairstyle,					  // " hair_style,                "
		pp->beard,						  // " beard,                     "
		pp->ability_time_seconds,		  // " ability_time_seconds,      "
		pp->ability_number,				  // " ability_number,            "
		pp->ability_time_minutes,		  // " ability_time_minutes,      "
		pp->ability_time_hours,			  // " ability_time_hours,        "
		EscapeString(pp->title).c_str(),  // " title,                     "
		EscapeString(pp->suffix).c_str(), // " suffix,                    "
		pp->exp,						  // " exp,                       "
		pp->points,						  // " points,                    "
		pp->mana,						  // " mana,                      "
		pp->cur_hp,						  // " cur_hp,                    "
		pp->STR,						  // " str,                       "
		pp->STA,						  // " sta,                       "
		pp->CHA,						  // " cha,                       "
		pp->DEX,						  // " dex,                       "
		pp->INT,						  // " `int`,                     "
		pp->AGI,						  // " agi,                       "
		pp->WIS,						  // " wis,                       "
		pp->face,						  // " face,                      "
		pp->y,							  // " y,                         "
		pp->x,							  // " x,                         "
		pp->z,							  // " z,                         "
		pp->heading,					  // " heading,                   "
		pp->pvp2,						  // " pvp2,                      "
		pp->pvptype,					  // " pvp_type,                  "
		pp->autosplit,					  // " autosplit_enabled,         "
		pp->zone_change_count,			  // " zone_change_count,         "
		pp->drakkin_heritage,			  // " drakkin_heritage,          "
		pp->drakkin_tattoo,				  // " drakkin_tattoo,            "
		pp->drakkin_details,			  // " drakkin_details,           "
		pp->toxicity,					  // " toxicity,                  "
		pp->hunger_level,				  // " hunger_level,              "
		pp->thirst_level,				  // " thirst_level,              "
		pp->ability_up,					  // " ability_up,                "
		pp->zone_id,					  // " zone_id,                   "
		pp->zoneInstance,				  // " zone_instance,             "
		pp->leadAAActive,				  // " leadership_exp_on,         "
		pp->ldon_points_guk,			  // " ldon_points_guk,           "
		pp->ldon_points_mir,			  // " ldon_points_mir,           "
		pp->ldon_points_mmc,			  // " ldon_points_mmc,           "
		pp->ldon_points_ruj,			  // " ldon_points_ruj,           "
		pp->ldon_points_tak,			  // " ldon_points_tak,           "
		pp->ldon_points_available,		  // " ldon_points_available,     "
		pp->tribute_time_remaining,		  // " tribute_time_remaining,    "
		pp->showhelm,					  // " show_helm,                 "
		pp->career_tribute_points,		  // " career_tribute_points,     "
		pp->tribute_points,				  // " tribute_points,            "
		pp->tribute_active,				  // " tribute_active,            "
		pp->endurance,					  // " endurance,                 "
		pp->group_leadership_exp,		  // " group_leadership_exp,      "
		pp->raid_leadership_exp,		  // " raid_leadership_exp,       "
		pp->group_leadership_points,	  // " group_leadership_points,   "
		pp->raid_leadership_points,		  // " raid_leadership_points,    "
		pp->air_remaining,				  // " air_remaining,             "
		pp->PVPKills,					  // " pvp_kills,                 "
		pp->PVPDeaths,					  // " pvp_deaths,                "
		pp->PVPCurrentPoints,			  // " pvp_current_points,        "
		pp->PVPCareerPoints,			  // " pvp_career_points,         "
		pp->PVPBestKillStreak,			  // " pvp_best_kill_streak,      "
		pp->PVPWorstDeathStreak,		  // " pvp_worst_death_streak,    "
		pp->PVPCurrentKillStreak,		  // " pvp_current_kill_streak,   "
		pp->aapoints_spent,				  // " aa_points_spent,           "
		pp->expAA,						  // " aa_exp,                    "
		pp->aapoints,					  // " aa_points,                 "
		pp->groupAutoconsent,			  // " group_auto_consent,        "
		pp->raidAutoconsent,			  // " raid_auto_consent,         "
		pp->guildAutoconsent,			  // " guild_auto_consent,        "
		pp->RestTimer					  // " RestTimer)                 "
	);
	auto results = QueryDatabase(query);
	/* Save Bind Points */
	query = StringFormat("REPLACE INTO `%s` (id, zone_id, instance_id, x, y, z, heading) VALUES (%u, %u, %u, %f, %f, %f, %f, %i)",
						 CHARACTER_BIND_TABLE.c_str(), character_id, pp->binds[0].zoneId, 0, pp->binds[0].x, pp->binds[0].y, pp->binds[0].z, pp->binds[0].heading
	); results = QueryDatabase(query);

        /* HoTT Ability */
        if(RuleB(Character, GrantHoTTOnCreate))
        {
                query = StringFormat("INSERT INTO `character_leadership_abilities` (id, slot, rank) VALUES (%u, %i, %i)", character_id, 14, 1);
                results = QueryDatabase(query);
        }

	/* Save Skills */
	int firstquery = 0;
	for (int i = 0; i < MAX_PP_SKILL; i++){
		if (pp->skills[i] > 0){
			if (firstquery != 1){
				firstquery = 1;
				query = StringFormat("REPLACE INTO `character_skills` (id, skill_id, value) VALUES (%u, %u, %u)", character_id, i, pp->skills[i]);
			}
			else{
				query = query + StringFormat(", (%u, %u, %u)", character_id, i, pp->skills[i]);
			}
		}
	}
	results = QueryDatabase(query); 

	/* Save Language */
	firstquery = 0;
	for (int i = 0; i < MAX_PP_LANGUAGE; i++){
		if (pp->languages[i] > 0){
			if (firstquery != 1){
				firstquery = 1;
				query = StringFormat("REPLACE INTO `character_languages` (id, lang_id, value) VALUES (%u, %u, %u)", character_id, i, pp->languages[i]);
			}
			else{
				query = query + StringFormat(", (%u, %u, %u)", character_id, i, pp->languages[i]);
			}
		}
	}
	results = QueryDatabase(query); 

	return true;
}

bool Database::SaveCharacterBindPoint(uint32 character_id, const BindStruct &bind)
{
	/* Save Home Bind Point */
	std::string query =
			StringFormat("REPLACE INTO `%s` (id, zone_id, instance_id, x, y, z, heading)"
								 "VALUES (%u, %u, %u, %f, %f, %f, %f)",
						 CHARACTER_BIND_TABLE.c_str(), character_id, bind.zoneId, bind.instance_id, bind.x, bind.y, bind.z, bind.heading);

	Log(Logs::General, Logs::None, "ZoneDatabase::SaveCharacterBindPoint for character ID: %i zone_id: %u instance_id: %u position: %f %f %f %f",
			character_id, bind.zoneId, bind.instance_id, bind.x, bind.y, bind.z, bind.heading);

	auto results = QueryDatabase(query);
	if (!results.RowsAffected())
		Log(Logs::General, Logs::None, "ERROR Bind Home Save: %s. %s", results.ErrorMessage().c_str(),
				query.c_str());

	return true;
}

/* This only for new Character creation storing */
bool Database::StoreCharacter(uint32 account_id, PlayerProfile_Struct* pp, EQEmu::InventoryProfile* inv) {
	uint32 charid = 0; 
	char zone[50]; 
	float x, y, z; 
	charid = GetCharacterID(pp->name);

	if(!charid) {
		Log(Logs::General, Logs::Error, "StoreCharacter: no character id");
		return false;
	}

	const char *zname = GetZoneName(pp->zone_id);
	if(zname == nullptr) {
		/* Zone not in the DB, something to prevent crash... */
		strn0cpy(zone, "qeynos", 49);
		pp->zone_id = 1;
	}
	else{ strn0cpy(zone, zname, 49); }

	x = pp->x;
	y = pp->y;
	z = pp->z;

	/* Saves Player Profile Data */
	SaveCharacterCreate(charid, account_id, pp); 

	/* Insert starting inventory... */
	std::string invquery;
	for (int16 i = EQEmu::legacy::EQUIPMENT_BEGIN; i <= EQEmu::legacy::BANK_BAGS_END;) {
		const EQEmu::ItemInstance* newinv = inv->GetItem(i);
		if (newinv) {
			invquery = StringFormat("INSERT INTO `%s` (charid, slotid, itemid, charges, color) VALUES (%u, %i, %u, %i, %u)",
									INVENTORY_TABLE.c_str(), charid, i, newinv->GetItem()->ID, newinv->GetCharges(), newinv->GetColor());

			auto results = QueryDatabase(invquery); 
		}

		if (i == EQEmu::inventory::slotCursor) {
			i = EQEmu::legacy::GENERAL_BAGS_BEGIN; 
			continue;
		}
		else if (i == EQEmu::legacy::CURSOR_BAG_END) { 
			i = EQEmu::legacy::BANK_BEGIN; 
			continue; 
		}
		else if (i == EQEmu::legacy::BANK_END) { 
			i = EQEmu::legacy::BANK_BAGS_BEGIN; 
			continue; 
		} 
		i++;
	}
	return true;
}

uint32 Database::GetCharacterID(const char *name) {
	std::string query = StringFormat("SELECT `id` FROM `character_data` WHERE `name` = '%s'", name);
	auto results = QueryDatabase(query);
	auto row = results.begin();
	if (results.RowCount() == 1)
	{
		return atoi(row[0]);
	}
	return 0; 
}

/*
	This function returns the account_id that owns the character with
	the name "name" or zero if no character with that name was found
	Zero will also be returned if there is a database error.
*/
uint32 Database::GetAccountIDByChar(const char* charname, uint32* oCharID) {
	std::string query = StringFormat("SELECT `account_id`, `id` FROM `character_data` WHERE name='%s'", EscapeString(charname).c_str());

	auto results = QueryDatabase(query);

	if (!results.Success())
	{
		return 0;
	}

	if (results.RowCount() != 1)
		return 0; 

	auto row = results.begin();

	uint32 accountId = atoi(row[0]);

	if (oCharID)
		*oCharID = atoi(row[1]);

	return accountId;
}

// Retrieve account_id for a given char_id
uint32 Database::GetAccountIDByChar(uint32 char_id) {
	std::string query = StringFormat("SELECT `account_id` FROM `character_data` WHERE `id` = %i LIMIT 1", char_id); 
	auto results = QueryDatabase(query); 
	if (!results.Success()) {
		return 0;
	}

	if (results.RowCount() != 1)
		return 0;

	auto row = results.begin(); 
	return atoi(row[0]);
}

uint32 Database::GetAccountIDByName(const char* accname, int16* status, uint32* lsid) {
	if (!isAlphaNumeric(accname))
		return 0;

	std::string query = StringFormat("SELECT `id`, `status`, `lsaccount_id` FROM `account` WHERE `name` = '%s' LIMIT 1", accname);
	auto results = QueryDatabase(query);

	if (!results.Success()) {
		return 0;
	}

	if (results.RowCount() != 1)
		return 0;

	auto row = results.begin();

	uint32 id = atoi(row[0]);

	if (status)
		*status = atoi(row[1]);

	if (lsid) {
		if (row[2])
			*lsid = atoi(row[2]);
		else
			*lsid = 0;
	}

	return id;
}

void Database::GetAccountName(uint32 accountid, char* name, uint32* oLSAccountID) {
	std::string query = StringFormat("SELECT `name`, `lsaccount_id` FROM `account` WHERE `id` = '%i'", accountid); 
	auto results = QueryDatabase(query);

	if (!results.Success()) {
		return;
	}

	if (results.RowCount() != 1)
		return;

	auto row = results.begin();

	strcpy(name, row[0]);
	if (row[1] && oLSAccountID) {
		*oLSAccountID = atoi(row[1]);
	}

}

void Database::GetCharName(uint32 char_id, char* name) {
	std::string query = StringFormat("SELECT `name` FROM `character_data` WHERE id='%i'", char_id);
	auto results = QueryDatabase(query);

	if (!results.Success()) {
		return; 
	}

	auto row = results.begin();
	for (auto row = results.begin(); row != results.end(); ++row) {
		strcpy(name, row[0]);
	}
}

bool Database::LoadVariables() {
	auto results = QueryDatabase(StringFormat("SELECT varname, value, unix_timestamp() FROM variables where unix_timestamp(ts) >= %d", varcache.last_update));

	if (!results.Success())
		return false;

	if (results.RowCount() == 0)
		return true;

	LockMutex lock(&Mvarcache);

	std::string key, value;
	for (auto row = results.begin(); row != results.end(); ++row) {
		varcache.last_update = atoi(row[2]); // ahh should we be comparing if this is newer?
		key = row[0];
		value = row[1];
		std::transform(std::begin(key), std::end(key), std::begin(key), ::tolower); // keys are lower case, DB doesn't have to be
		varcache.Add(key, value);
	}

	return true;
}

// Gets variable from 'variables' table
bool Database::GetVariable(std::string varname, std::string &varvalue)
{
	varvalue.clear();

	LockMutex lock(&Mvarcache);

	if (varname.empty())
		return false;

	std::transform(std::begin(varname), std::end(varname), std::begin(varname), ::tolower); // all keys are lower case
	auto tmp = varcache.Get(varname);
	if (tmp) {
		varvalue = *tmp;
		return true;
	}
	return false;
}

bool Database::SetVariable(const std::string varname, const std::string &varvalue)
{
	std::string escaped_name = EscapeString(varname);
	std::string escaped_value = EscapeString(varvalue);
	std::string query = StringFormat("Update variables set value='%s' WHERE varname like '%s'", escaped_value.c_str(), escaped_name.c_str());
	auto results = QueryDatabase(query);

	if (!results.Success())
		return false;

	if (results.RowsAffected() == 1)
	{
		LoadVariables(); // refresh cache
		return true;
	}

	query = StringFormat("Insert Into variables (varname, value) values ('%s', '%s')", escaped_name.c_str(), escaped_value.c_str());
	results = QueryDatabase(query);

	if (results.RowsAffected() != 1)
		return false;

	LoadVariables(); // refresh cache
	return true;
}

uint32 Database::GetMiniLoginAccount(char* ip)
{
	std::string query = StringFormat("SELECT `id` FROM `account` WHERE `minilogin_ip` = '%s'", ip);
	auto results = QueryDatabase(query);

	if (!results.Success())
		return 0;

	auto row = results.begin(); 
	return atoi(row[0]);
}

// Get zone starting points from DB
bool Database::GetSafePoints(const char* short_name, uint32 version, float* safe_x, float* safe_y, float* safe_z, int16* minstatus, uint8* minlevel, char *flag_needed) {
	
	std::string query = StringFormat("SELECT safe_x, safe_y, safe_z, min_status, min_level, flag_needed FROM zone "
		" WHERE short_name='%s' AND (version=%i OR version=0) ORDER BY version DESC", short_name, version);
	auto results = QueryDatabase(query);

	if (!results.Success())
		return false;

	if (results.RowCount() == 0)
		return false;

	auto row = results.begin();

	if (safe_x != nullptr)
		*safe_x = atof(row[0]);
	if (safe_y != nullptr)
		*safe_y = atof(row[1]);
	if (safe_z != nullptr)
		*safe_z = atof(row[2]);
	if (minstatus != nullptr)
		*minstatus = atoi(row[3]);
	if (minlevel != nullptr)
		*minlevel = atoi(row[4]);
	if (flag_needed != nullptr)
		strcpy(flag_needed, row[5]);

	return true;
}

bool Database::GetZoneLongName(const char* short_name, char** long_name, char* file_name, float* safe_x, float* safe_y, float* safe_z, uint32* graveyard_id, uint32* maxclients) {
	
	std::string query = StringFormat("SELECT long_name, file_name, safe_x, safe_y, safe_z, graveyard_id, maxclients FROM zone WHERE short_name='%s' AND version=0", short_name);
	auto results = QueryDatabase(query);

	if (!results.Success()) {
		return false;
	}

	if (results.RowCount() == 0)
		return false;

	auto row = results.begin();

	if (long_name != nullptr)
		*long_name = strcpy(new char[strlen(row[0])+1], row[0]);

	if (file_name != nullptr) {
		if (row[1] == nullptr)
			strcpy(file_name, short_name);
		else
			strcpy(file_name, row[1]);
	}

	if (safe_x != nullptr)
		*safe_x = atof(row[2]);
	if (safe_y != nullptr)
		*safe_y = atof(row[3]);
	if (safe_z != nullptr)
		*safe_z = atof(row[4]);
	if (graveyard_id != nullptr)
		*graveyard_id = atoi(row[5]);
	if (maxclients != nullptr)
		*maxclients = atoi(row[6]);

	return true;
}

uint32 Database::GetZoneGraveyardID(uint32 zone_id, uint32 version) {

	std::string query = StringFormat("SELECT graveyard_id FROM zone WHERE zoneidnumber='%u' AND (version=%i OR version=0) ORDER BY version DESC", zone_id, version);
	auto results = QueryDatabase(query);

	if (!results.Success())
		return 0;

	if (results.RowCount() == 0)
		return 0;

	auto row = results.begin();
	return atoi(row[0]);
}

bool Database::GetZoneGraveyard(const uint32 graveyard_id, uint32* graveyard_zoneid, float* graveyard_x, float* graveyard_y, float* graveyard_z, float* graveyard_heading) {
	
	std::string query = StringFormat("SELECT zone_id, x, y, z, heading FROM graveyard WHERE id=%i", graveyard_id);
	auto results = QueryDatabase(query);

	if (!results.Success()){
		return false;
	}

	if (results.RowCount() != 1)
		return false;

	auto row = results.begin();

	if(graveyard_zoneid != nullptr)
		*graveyard_zoneid = atoi(row[0]);
	if(graveyard_x != nullptr)
		*graveyard_x = atof(row[1]);
	if(graveyard_y != nullptr)
		*graveyard_y = atof(row[2]);
	if(graveyard_z != nullptr)
		*graveyard_z = atof(row[3]);
	if(graveyard_heading != nullptr)
		*graveyard_heading = atof(row[4]);

	return true;
}

bool Database::LoadZoneNames() {
	std::string query("SELECT zoneidnumber, short_name FROM zone");

	auto results = QueryDatabase(query);

	if (!results.Success())
	{
		return false;
	}

	for (auto row= results.begin();row != results.end();++row)
	{
		uint32 zoneid = atoi(row[0]);
		std::string zonename = row[1];
		zonename_array.insert(std::pair<uint32,std::string>(zoneid,zonename));
	}

	return true;
}

uint32 Database::GetZoneID(const char* zonename) {

	if (zonename == nullptr)
		return 0;

	for (auto iter = zonename_array.begin(); iter != zonename_array.end(); ++iter)
		if (strcasecmp(iter->second.c_str(), zonename) == 0)
			return iter->first;

	return 0;
}

const char* Database::GetZoneName(uint32 zoneID, bool ErrorUnknown) {
	auto iter = zonename_array.find(zoneID);

	if (iter != zonename_array.end())
		return iter->second.c_str();

	if (ErrorUnknown)
		return "UNKNOWN";

	return 0;
}

uint8 Database::GetPEQZone(uint32 zoneID, uint32 version){
	
	std::string query = StringFormat("SELECT peqzone from zone where zoneidnumber='%i' AND (version=%i OR version=0) ORDER BY version DESC", zoneID, version);
	auto results = QueryDatabase(query);

	if (!results.Success()) {
		return 0;
	}

	if (results.RowCount() == 0)
		return 0;

	auto row = results.begin();

	return atoi(row[0]);
}

bool Database::CheckNameFilter(const char* name, bool surname)
{
	std::string str_name = name;

	// the minimum 4 is enforced by the client too
	if (!name || strlen(name) < 4)
	{
		return false;
	}

	// Given name length is enforced by the client too
	if (!surname && strlen(name) > 15)
	{
		return false;
	}

	for (size_t i = 0; i < str_name.size(); i++)
	{
		if(!isalpha(str_name[i]))
		{
			return false;
		}
	}

	for(size_t x = 0; x < str_name.size(); ++x)
	{
		str_name[x] = tolower(str_name[x]);
	}

	char c = '\0';
	uint8 num_c = 0;
	for(size_t x = 0; x < str_name.size(); ++x)
	{
		if(str_name[x] == c)
		{
			num_c++;
		}
		else
		{
			num_c = 1;
			c = str_name[x];
		}
		if(num_c > 2)
		{
			return false;
		}
	}

	
	std::string query("SELECT name FROM name_filter");
	auto results = QueryDatabase(query);

	if (!results.Success())
	{
		// false through to true? shouldn't it be falls through to false?
		return true;
	}

	for (auto row = results.begin();row != results.end();++row)
	{
		std::string current_row = row[0];

		for(size_t x = 0; x < current_row.size(); ++x)
			current_row[x] = tolower(current_row[x]);

		if(str_name.find(current_row) != std::string::npos)
			return false;
	}

	return true;
}

bool Database::AddToNameFilter(const char* name) {
	
	std::string query = StringFormat("INSERT INTO name_filter (name) values ('%s')", name);
	auto results = QueryDatabase(query);

	if (!results.Success())
	{
		return false;
	}

	if (results.RowsAffected() == 0)
		return false;

	return true;
}

uint32 Database::GetAccountIDFromLSID(uint32 iLSID, char* oAccountName, int16* oStatus) {
	uint32 account_id = 0;
	std::string query = StringFormat("SELECT id, name, status FROM account WHERE lsaccount_id=%i", iLSID);
	auto results = QueryDatabase(query);

	if (!results.Success()) {
		return 0;
	}

	if (results.RowCount() != 1)
		return 0;

	for (auto row = results.begin(); row != results.end(); ++row) {
		account_id = atoi(row[0]);

		if (oAccountName)
			strcpy(oAccountName, row[1]);
		if (oStatus)
			*oStatus = atoi(row[2]);
	}

	return account_id;
}

void Database::GetAccountFromID(uint32 id, char* oAccountName, int16* oStatus) {
	
	std::string query = StringFormat("SELECT name, status FROM account WHERE id=%i", id);
	auto results = QueryDatabase(query);

	if (!results.Success()){
		return;
	}

	if (results.RowCount() != 1)
		return;

	auto row = results.begin();

	if (oAccountName)
		strcpy(oAccountName, row[0]);
	if (oStatus)
		*oStatus = atoi(row[1]);
}

void Database::ClearMerchantTemp(){
	QueryDatabase("DELETE FROM merchantlist_temp");
}

bool Database::UpdateName(const char* oldname, const char* newname) { 
	std::cout << "Renaming " << oldname << " to " << newname << "..." << std::endl; 
	std::string query = StringFormat("UPDATE `character_data` SET `name` = '%s' WHERE `name` = '%s';", newname, oldname);
	auto results = QueryDatabase(query);

	if (!results.Success())
		return false;

	if (results.RowsAffected() == 0)
		return false;

	return true;
}

// If the name is used or an error occurs, it returns false, otherwise it returns true
bool Database::CheckUsedName(const char* name) {
	std::string query = StringFormat("SELECT `id` FROM `character_data` WHERE `name` = '%s'", name);
	auto results = QueryDatabase(query); 
	if (!results.Success()) {
		return false;
	}

	if (results.RowCount() > 0)
		return false;

	return true;
}

uint8 Database::GetServerType() {
	std::string query("SELECT `value` FROM `variables` WHERE `varname` = 'ServerType' LIMIT 1");
	auto results = QueryDatabase(query); 
	if (!results.Success()) {
		return 0;
	}

	if (results.RowCount() != 1)
		return 0;

	auto row = results.begin();
	return atoi(row[0]);
}

bool Database::MoveCharacterToZone(const char* charname, const char* zonename, uint32 zoneid) {
	if(zonename == nullptr || strlen(zonename) == 0)
		return false;

	std::string query = StringFormat("UPDATE `character_data` SET `zone_id` = %i, `x` = -1, `y` = -1, `z` = -1 WHERE `name` = '%s'", zoneid, charname);
	auto results = QueryDatabase(query);

	if (!results.Success()) {
		return false;
	}

	if (results.RowsAffected() == 0)
		return false;

	return true;
}

bool Database::MoveCharacterToZone(const char* charname, const char* zonename) {
	return MoveCharacterToZone(charname, zonename, GetZoneID(zonename));
}

bool Database::MoveCharacterToZone(uint32 iCharID, const char* iZonename) { 
	std::string query = StringFormat("UPDATE `character_data` SET `zone_id` = %i, `x` = -1, `y` = -1, `z` = -1 WHERE `id` = %i", GetZoneID(iZonename), iCharID);
	auto results = QueryDatabase(query);

	if (!results.Success()) {
		return false;
	}

	return results.RowsAffected() != 0;
}

bool Database::SetHackerFlag(const char* accountname, const char* charactername, const char* hacked) { 
	std::string query = StringFormat("INSERT INTO `hackers` (account, name, hacked) values('%s','%s','%s')", accountname, charactername, hacked);
	auto results = QueryDatabase(query);

	if (!results.Success()) {
		return false;
	}

	return results.RowsAffected() != 0;
}

bool Database::SetMQDetectionFlag(const char* accountname, const char* charactername, const char* hacked, const char* zone) { 
	//Utilize the "hacker" table, but also give zone information.
	std::string query = StringFormat("INSERT INTO hackers(account,name,hacked,zone) values('%s','%s','%s','%s')", accountname, charactername, hacked, zone);
	auto results = QueryDatabase(query);

	if (!results.Success())
	{
		return false;
	}

	return results.RowsAffected() != 0;
}

uint8 Database::GetRaceSkill(uint8 skillid, uint8 in_race)
{
	uint16 race_cap = 0;
	
	//Check for a racial cap!
	std::string query = StringFormat("SELECT skillcap from race_skillcaps where skill = %i && race = %i", skillid, in_race);
	auto results = QueryDatabase(query);

	if (!results.Success())
		return 0;

	if (results.RowCount() == 0)
		return 0;

	auto row = results.begin();
	return atoi(row[0]);
}

uint8 Database::GetSkillCap(uint8 skillid, uint8 in_race, uint8 in_class, uint16 in_level)
{
	uint8 skill_level = 0, skill_formula = 0;
	uint16 base_cap = 0, skill_cap = 0, skill_cap2 = 0, skill_cap3 = 0;
	

	//Fetch the data from DB.
	std::string query = StringFormat("SELECT level, formula, pre50cap, post50cap, post60cap from skillcaps where skill = %i && class = %i", skillid, in_class);
	auto results = QueryDatabase(query);

	if (results.Success() && results.RowsAffected() != 0)
	{
		auto row = results.begin();
		skill_level = atoi(row[0]);
		skill_formula = atoi(row[1]);
		skill_cap = atoi(row[2]);
		if (atoi(row[3]) > skill_cap)
			skill_cap2 = (atoi(row[3])-skill_cap)/10; //Split the post-50 skill cap into difference between pre-50 cap and post-50 cap / 10 to determine amount of points per level.
		skill_cap3 = atoi(row[4]);
	}

	int race_skill = GetRaceSkill(skillid,in_race);

	if (race_skill > 0 && (race_skill > skill_cap || skill_cap == 0 || in_level < skill_level))
		return race_skill;

	if (skill_cap == 0) //Can't train this skill at all.
		return 255; //Untrainable

	if (in_level < skill_level)
		return 254; //Untrained

	//Determine pre-51 level-based cap
	if (skill_formula > 0)
		base_cap = in_level*skill_formula+skill_formula;
	if (base_cap > skill_cap || skill_formula == 0)
		base_cap = skill_cap;

	//If post 50, add post 50 cap to base cap.
	if (in_level > 50 && skill_cap2 > 0)
		base_cap += skill_cap2*(in_level-50);

	//No cap should ever go above its post50cap
	if (skill_cap3 > 0 && base_cap > skill_cap3)
		base_cap = skill_cap3;

	//Base cap is now the max value at the person's level, return it!
	return base_cap;
}

uint32 Database::GetCharacterInfo(const char* iName, uint32* oAccID, uint32* oZoneID, uint32* oInstanceID, float* oX, float* oY, float* oZ) { 
	std::string query = StringFormat("SELECT `id`, `account_id`, `zone_id`, `zone_instance`, `x`, `y`, `z` FROM `character_data` WHERE `name` = '%s'", iName);
	auto results = QueryDatabase(query);

	if (!results.Success()) {
		return 0;
	}

	if (results.RowCount() != 1)
		return 0;

	auto row = results.begin();
	uint32 charid = atoi(row[0]);
	if (oAccID){ *oAccID = atoi(row[1]); }
	if (oZoneID){ *oZoneID = atoi(row[2]); }
	if (oInstanceID){ *oInstanceID = atoi(row[3]); }
	if (oX){ *oX = atof(row[4]); }
	if (oY){ *oY = atof(row[5]); }
	if (oZ){ *oZ = atof(row[6]); }

	return charid;
}

bool Database::UpdateLiveChar(char* charname,uint32 lsaccount_id) {

	std::string query = StringFormat("UPDATE account SET charname='%s' WHERE id=%i;",charname, lsaccount_id);
	auto results = QueryDatabase(query);

	if (!results.Success()){
		return false;
	}

	return true;
}

bool Database::GetLiveChar(uint32 account_id, char* cname) {

	std::string query = StringFormat("SELECT charname FROM account WHERE id=%i", account_id);
	auto results = QueryDatabase(query);

	if (!results.Success())
	{
		return false;
	}

	if (results.RowCount() != 1)
		return false;

	auto row = results.begin();
	strcpy(cname,row[0]);

	return true;
}

void Database::SetLFP(uint32 CharID, bool LFP) { 
	std::string query = StringFormat("UPDATE `character_data` SET `lfp` = %i WHERE `id` = %i",LFP, CharID);
	QueryDatabase(query); 
}

void Database::SetLoginFlags(uint32 CharID, bool LFP, bool LFG, uint8 firstlogon) { 
	std::string query = StringFormat("update `character_data` SET `lfp` = %i, `lfg` = %i, `firstlogon` = %i WHERE `id` = %i",LFP, LFG, firstlogon, CharID);
	QueryDatabase(query); 
}

void Database::SetLFG(uint32 CharID, bool LFG) { 
	std::string query = StringFormat("update `character_data` SET `lfg` = %i WHERE `id` = %i",LFG, CharID);
	QueryDatabase(query); 
}

void Database::SetFirstLogon(uint32 CharID, uint8 firstlogon) { 
	std::string query = StringFormat( "UPDATE `character_data` SET `firstlogon` = %i WHERE `id` = %i",firstlogon, CharID);
	QueryDatabase(query); 
}

void Database::AddReport(std::string who, std::string against, std::string lines)
{
	auto escape_str = new char[lines.size() * 2 + 1];
	DoEscapeString(escape_str, lines.c_str(), lines.size());

	std::string query = StringFormat("INSERT INTO reports (name, reported, reported_text) VALUES('%s', '%s', '%s')", EscapeString(who).c_str(), EscapeString(against).c_str(), escape_str);
	QueryDatabase(query);
	safe_delete_array(escape_str);
}

void Database::SetGroupID(const char* name, uint32 id, uint32 charid, uint32 ismerc) {
	std::string query;
	if (id == 0) {
		// removing from group
		query = StringFormat("delete from group_id where charid=%i and name='%s' and ismerc=%i",charid, name, ismerc);
		auto results = QueryDatabase(query);

		if (!results.Success())
			Log(Logs::General, Logs::Error, "Error deleting character from group id: %s", results.ErrorMessage().c_str());

		return;
	}

	/* Add to the Group */
	query = StringFormat("REPLACE INTO  `group_id` SET `charid` = %i, `groupid` = %i, `name` = '%s', `ismerc` = '%i'", charid, id, name, ismerc);
	QueryDatabase(query);
}

void Database::ClearAllGroups(void)
{
	std::string query("DELETE FROM `group_id`");
	QueryDatabase(query);
	return;
}

void Database::ClearGroup(uint32 gid) {
	ClearGroupLeader(gid);
	
	if(gid == 0)
	{
		//clear all groups
		ClearAllGroups();
		return;
	}

	//clear a specific group
	std::string query = StringFormat("delete from group_id where groupid = %lu", (unsigned long)gid);
	QueryDatabase(query);
}

uint32 Database::GetGroupID(const char* name){ 
	std::string query = StringFormat("SELECT groupid from group_id where name='%s'", name);
	auto results = QueryDatabase(query);

	if (!results.Success()) {
		return 0;
	}

	if (results.RowCount() == 0)
	{
		// Commenting this out until logging levels can prevent this from going to console
		//Log(Logs::General, Logs::None,, "Character not in a group: %s", name);
		return 0;
	}

	auto row = results.begin();

	return atoi(row[0]);
}

/* Is this really getting used properly... A half implementation ? Akkadius */
char* Database::GetGroupLeaderForLogin(const char* name, char* leaderbuf) {
	strcpy(leaderbuf, "");
	uint32 group_id = 0;

	std::string query = StringFormat("SELECT `groupid` FROM `group_id` WHERE `name` = '%s'", name);
	auto results = QueryDatabase(query);

	for (auto row = results.begin(); row != results.end(); ++row)
		if (row[0])
			group_id = atoi(row[0]);

	if (group_id == 0)
		return leaderbuf;

	query = StringFormat("SELECT `leadername` FROM `group_leaders` WHERE `gid` = '%u' LIMIT 1", group_id);
	results = QueryDatabase(query);

	for (auto row = results.begin(); row != results.end(); ++row)
		if (row[0])
			strcpy(leaderbuf, row[0]);

	return leaderbuf;
}

void Database::SetGroupLeaderName(uint32 gid, const char* name) { 
	std::string query = StringFormat("UPDATE group_leaders SET leadername = '%s' WHERE gid = %u", EscapeString(name).c_str(), gid);
	auto result = QueryDatabase(query);

	if(result.RowsAffected() != 0) {
		return;
	}

	query = StringFormat("REPLACE INTO group_leaders(gid, leadername, marknpc, leadershipaa, maintank, assist, puller, mentoree, mentor_percent) VALUES(%u, '%s', '', '', '', '', '', '', '0')",
						 gid, EscapeString(name).c_str());
	result = QueryDatabase(query);

	if(!result.Success()) {
		Log(Logs::General, Logs::None, "Error in Database::SetGroupLeaderName: %s", result.ErrorMessage().c_str());
	}
}

char *Database::GetGroupLeadershipInfo(uint32 gid, char* leaderbuf, char* maintank, char* assist, char* puller, char *marknpc, char *mentoree, int *mentor_percent, GroupLeadershipAA_Struct* GLAA)
{
	std::string query = StringFormat("SELECT `leadername`, `maintank`, `assist`, `puller`, `marknpc`, `mentoree`, `mentor_percent`, `leadershipaa` FROM `group_leaders` WHERE `gid` = %lu",(unsigned long)gid);
	auto results = QueryDatabase(query);

	if (!results.Success() || results.RowCount() == 0) {
		if(leaderbuf)
			strcpy(leaderbuf, "UNKNOWN");

		if(maintank)
			maintank[0] = '\0';

		if(assist)
			assist[0] = '\0';

		if(puller)
			puller[0] = '\0';

		if(marknpc)
			marknpc[0] = '\0';

		if (mentoree)
			mentoree[0] = '\0';

		if (mentor_percent)
			*mentor_percent = 0;

		return leaderbuf;
	}

	auto row = results.begin();

	if(leaderbuf)
		strcpy(leaderbuf, row[0]);

	if(maintank)
		strcpy(maintank, row[1]);

	if(assist)
		strcpy(assist, row[2]);

	if(puller)
		strcpy(puller, row[3]);

	if(marknpc)
		strcpy(marknpc, row[4]);

	if (mentoree)
		strcpy(mentoree, row[5]);

	if (mentor_percent)
		*mentor_percent = atoi(row[6]);

	if(GLAA && results.LengthOfColumn(7) == sizeof(GroupLeadershipAA_Struct))
		memcpy(GLAA, row[7], sizeof(GroupLeadershipAA_Struct));

	return leaderbuf;
}

// Clearing all group leaders
void Database::ClearAllGroupLeaders(void) {
	std::string query("DELETE from group_leaders");
	auto results = QueryDatabase(query);

	if (!results.Success())
		std::cout << "Unable to clear group leaders: " << results.ErrorMessage() << std::endl;

	return;
}

void Database::ClearGroupLeader(uint32 gid) {
	
	if(gid == 0)
	{
		ClearAllGroupLeaders();
		return;
	}

	std::string query = StringFormat("DELETE from group_leaders where gid = %lu", (unsigned long)gid);
	auto results = QueryDatabase(query);

	if (!results.Success())
		std::cout << "Unable to clear group leader: " << results.ErrorMessage() << std::endl;
}

uint8 Database::GetAgreementFlag(uint32 acctid) {

	std::string query = StringFormat("SELECT rulesflag FROM account WHERE id=%i",acctid);
	auto results = QueryDatabase(query);

	if (!results.Success())
		return 0;

	if (results.RowCount() != 1)
		return 0;

	auto row = results.begin();

	return atoi(row[0]);
}

void Database::SetAgreementFlag(uint32 acctid) {
	std::string query = StringFormat("UPDATE account SET rulesflag=1 where id=%i", acctid);
	QueryDatabase(query);
}

void Database::ClearRaid(uint32 rid) {
	if(rid == 0)
	{
		//clear all raids
		ClearAllRaids();
		return;
	}

	//clear a specific group
	std::string query = StringFormat("delete from raid_members where raidid = %lu", (unsigned long)rid);
	auto results = QueryDatabase(query);

	if (!results.Success())
		std::cout << "Unable to clear raids: " << results.ErrorMessage() << std::endl;
}

void Database::ClearAllRaids(void) {

	std::string query("delete from raid_members");
	auto results = QueryDatabase(query);

	if (!results.Success())
		std::cout << "Unable to clear raids: " << results.ErrorMessage() << std::endl;
}

void Database::ClearAllRaidDetails(void)
{

	std::string query("delete from raid_details");
	auto results = QueryDatabase(query);

	if (!results.Success())
		std::cout << "Unable to clear raid details: " << results.ErrorMessage() << std::endl;
}

void Database::ClearRaidDetails(uint32 rid) {
	
	if(rid == 0)
	{
		//clear all raids
		ClearAllRaidDetails();
		return;
	}

	//clear a specific group
	std::string query = StringFormat("delete from raid_details where raidid = %lu", (unsigned long)rid);
	auto results = QueryDatabase(query);

	if (!results.Success())
		std::cout << "Unable to clear raid details: " << results.ErrorMessage() << std::endl;
}

// returns 0 on error or no raid for that character, or
// the raid id that the character is a member of.
uint32 Database::GetRaidID(const char* name)
{ 
	std::string query = StringFormat("SELECT `raidid` FROM `raid_members` WHERE `name` = '%s'", name);
	auto results = QueryDatabase(query);

	if (!results.Success()) {
		return 0;
	}

	auto row = results.begin(); 
	if (row == results.end()) {
		return 0;
	}

	if (row[0]) // would it ever be possible to have a null here?
		return atoi(row[0]);

	return 0;
}

const char* Database::GetRaidLeaderName(uint32 raid_id)
{
	// Would be a good idea to fix this to be a passed in variable and
	// make the caller responsible. static local variables like this are
	// not guaranteed to be thread safe (nor is the internal guard
	// variable). C++0x standard states this should be thread safe
	// but may not be fully supported in some compilers.
	static char name[128];
	
	std::string query = StringFormat("SELECT `name` FROM `raid_members` WHERE `raidid` = %u AND `israidleader` = 1", raid_id);
	auto results = QueryDatabase(query);

	if (!results.Success()) {
		Log(Logs::General, Logs::Debug, "Unable to get Raid Leader Name for Raid ID: %u", raid_id);
		return "UNKNOWN";
	}

	auto row = results.begin();

	if (row == results.end()) {
		return "UNKNOWN";
	}

	memset(name, 0, sizeof(name));
	strcpy(name, row[0]);

	return name;
}

// maintank, assist, puller, marknpc currently unused
void Database::GetGroupLeadershipInfo(uint32 gid, uint32 rid, char *maintank,
		char *assist, char *puller, char *marknpc, char *mentoree, int *mentor_percent, GroupLeadershipAA_Struct *GLAA)
{
	std::string query = StringFormat(
			"SELECT maintank, assist, puller, marknpc, mentoree, mentor_percent, leadershipaa FROM raid_leaders WHERE gid = %lu AND rid = %lu",
			(unsigned long)gid, (unsigned long)rid);
	auto results = QueryDatabase(query);

	if (!results.Success() || results.RowCount() == 0) {
		if (maintank)
			maintank[0] = '\0';

		if (assist)
			assist[0] = '\0';

		if (puller)
			puller[0] = '\0';

		if (marknpc)
			marknpc[0] = '\0';

		if (mentoree)
			mentoree[0] = '\0';

		if (mentor_percent)
			*mentor_percent = 0;

		return;
	}

	auto row = results.begin();

	if (maintank)
		strcpy(maintank, row[0]);

	if (assist)
		strcpy(assist, row[1]);

	if (puller)
		strcpy(puller, row[2]);

	if (marknpc)
		strcpy(marknpc, row[3]);

	if (mentoree)
		strcpy(mentoree, row[4]);

	if (mentor_percent)
		*mentor_percent = atoi(row[5]);

	if (GLAA && results.LengthOfColumn(6) == sizeof(GroupLeadershipAA_Struct))
		memcpy(GLAA, row[6], sizeof(GroupLeadershipAA_Struct));

	return;
}

// maintank, assist, puller, marknpc currently unused
void Database::GetRaidLeadershipInfo(uint32 rid, char *maintank,
		char *assist, char *puller, char *marknpc, RaidLeadershipAA_Struct *RLAA)
{
	std::string query = StringFormat(
			"SELECT maintank, assist, puller, marknpc, leadershipaa FROM raid_leaders WHERE gid = %lu AND rid = %lu",
			(unsigned long)0xFFFFFFFF, (unsigned long)rid);
	auto results = QueryDatabase(query);

	if (!results.Success() || results.RowCount() == 0) {
		if (maintank)
			maintank[0] = '\0';

		if (assist)
			assist[0] = '\0';

		if (puller)
			puller[0] = '\0';

		if (marknpc)
			marknpc[0] = '\0';

		return;
	}

	auto row = results.begin();

	if (maintank)
		strcpy(maintank, row[0]);

	if (assist)
		strcpy(assist, row[1]);

	if (puller)
		strcpy(puller, row[2]);

	if (marknpc)
		strcpy(marknpc, row[3]);

	if (RLAA && results.LengthOfColumn(4) == sizeof(RaidLeadershipAA_Struct))
		memcpy(RLAA, row[4], sizeof(RaidLeadershipAA_Struct));

	return;
}

void Database::SetRaidGroupLeaderInfo(uint32 gid, uint32 rid)
{
	std::string query = StringFormat("UPDATE raid_leaders SET leadershipaa = '' WHERE gid = %lu AND rid = %lu",
			(unsigned long)gid, (unsigned long)rid);
	auto results = QueryDatabase(query);

	if (results.RowsAffected() != 0)
		return;

	query = StringFormat("REPLACE INTO raid_leaders(gid, rid, marknpc, leadershipaa, maintank, assist, puller, mentoree, mentor_percent) VALUES(%lu, %lu, '', '', '', '', '', '', 0)",
			(unsigned long)gid, (unsigned long)rid);
	results = QueryDatabase(query);

	return;
}

// Clearing all raid leaders
void Database::ClearAllRaidLeaders(void)
{
	std::string query("DELETE from raid_leaders");
	QueryDatabase(query);
	return;
}

void Database::ClearRaidLeader(uint32 gid, uint32 rid)
{
	if (rid == 0) {
		ClearAllRaidLeaders();
		return;
	}

	std::string query = StringFormat("DELETE from raid_leaders where gid = %lu and rid = %lu", (unsigned long)gid, (unsigned long)rid);
	QueryDatabase(query);
}

void Database::UpdateAdventureStatsEntry(uint32 char_id, uint8 theme, bool win)
{

	std::string field;

	switch(theme)
	{
		case 1:
		{
			field = "guk_";
			break;
		}
		case 2:
		{
			field = "mir_";
			break;
		}
		case 3:
		{
			field = "mmc_";
			break;
		}
		case 4:
		{
			field = "ruj_";
			break;
		}
		case 5:
		{
			field = "tak_";
			break;
		}
		default:
		{
			return;
		}
	}

	if (win)
		field += "wins";
	else
		field += "losses";

	std::string query = StringFormat("UPDATE `adventure_stats` SET %s=%s+1 WHERE player_id=%u",field.c_str(), field.c_str(), char_id);
	auto results = QueryDatabase(query);

	if (results.RowsAffected() != 0)
		return;

	query = StringFormat("INSERT INTO `adventure_stats` SET %s=1, player_id=%u", field.c_str(), char_id);
	QueryDatabase(query);
}

bool Database::GetAdventureStats(uint32 char_id, AdventureStats_Struct *as)
{
	std::string query = StringFormat(
		"SELECT "
		"`guk_wins`, "
		"`mir_wins`, "
		"`mmc_wins`, "
		"`ruj_wins`, "
		"`tak_wins`, "
		"`guk_losses`, "
		"`mir_losses`, "
		"`mmc_losses`, "
		"`ruj_losses`, "
		"`tak_losses` "
		"FROM "
		"`adventure_stats` "
		"WHERE "
		"player_id = %u ", 
		char_id
	);
	auto results = QueryDatabase(query);

	if (!results.Success())
		return false;

	if (results.RowCount() == 0)
		return false;

	auto row = results.begin();

	as->success.guk = atoi(row[0]);
	as->success.mir = atoi(row[1]);
	as->success.mmc = atoi(row[2]);
	as->success.ruj = atoi(row[3]);
	as->success.tak = atoi(row[4]);
	as->failure.guk = atoi(row[5]);
	as->failure.mir = atoi(row[6]);
	as->failure.mmc = atoi(row[7]);
	as->failure.ruj = atoi(row[8]);
	as->failure.tak = atoi(row[9]);
	as->failure.total = as->failure.guk + as->failure.mir + as->failure.mmc + as->failure.ruj + as->failure.tak;
	as->success.total = as->success.guk + as->success.mir + as->success.mmc + as->success.ruj + as->success.tak;

	return true;
}

uint32 Database::GetGuildIDByCharID(uint32 character_id) 
{
	std::string query = StringFormat("SELECT guild_id FROM guild_members WHERE char_id='%i'", character_id);
	auto results = QueryDatabase(query);

	if (!results.Success())
		return 0;

	if (results.RowCount() == 0)
		return 0;

	auto row = results.begin();
	return atoi(row[0]);
}

void Database::LoadLogSettings(EQEmuLogSys::LogSettings* log_settings)
{
	// log_settings previously initialized to '0' by EQEmuLogSys::LoadLogSettingsDefaults()
	
	std::string query = 
		"SELECT "
		"log_category_id, "
		"log_category_description, "
		"log_to_console, "
		"log_to_file, "
		"log_to_gmsay "
		"FROM "
		"logsys_categories "
		"ORDER BY log_category_id";
	auto results = QueryDatabase(query);

	int log_category = 0;
	LogSys.file_logs_enabled = false;

	for (auto row = results.begin(); row != results.end(); ++row) {
		log_category = atoi(row[0]);
		if (log_category <= Logs::None || log_category >= Logs::MaxCategoryID)
			continue;

		log_settings[log_category].log_to_console = atoi(row[2]);
		log_settings[log_category].log_to_file = atoi(row[3]);
		log_settings[log_category].log_to_gmsay = atoi(row[4]);

		/* Determine if any output method is enabled for the category 
			and set it to 1 so it can used to check if category is enabled */
		const bool log_to_console = log_settings[log_category].log_to_console > 0;
		const bool log_to_file = log_settings[log_category].log_to_file > 0;
		const bool log_to_gmsay = log_settings[log_category].log_to_gmsay > 0;
		const bool is_category_enabled = log_to_console || log_to_file || log_to_gmsay;

		if (is_category_enabled)
			log_settings[log_category].is_category_enabled = 1;

		/* 
			This determines whether or not the process needs to actually file log anything.
			If we go through this whole loop and nothing is set to any debug level, there is no point to create a file or keep anything open
		*/
		if (log_settings[log_category].log_to_file > 0){
			LogSys.file_logs_enabled = true;
		}
	}
}

void Database::ClearInvSnapshots(bool use_rule)
{
	uint32 del_time = time(nullptr);
	if (use_rule) { del_time -= RuleI(Character, InvSnapshotHistoryD) * 86400; }

	std::string query = StringFormat("DELETE FROM inventory_snapshots WHERE time_index <= %lu", (unsigned long)del_time);
	QueryDatabase(query);
}

struct TimeOfDay_Struct Database::LoadTime(time_t &realtime)
{

	TimeOfDay_Struct eqTime;
	std::string query = StringFormat("SELECT minute,hour,day,month,year,realtime FROM eqtime limit 1");
	auto results = QueryDatabase(query);

	if (!results.Success() || results.RowCount() == 0){
		Log(Logs::Detail, Logs::World_Server, "Loading EQ time of day failed. Using defaults.");
		eqTime.minute = 0;
		eqTime.hour = 9;
		eqTime.day = 1;
		eqTime.month = 1;
		eqTime.year = 3100;
		realtime = time(0);
	}
	else{
		auto row = results.begin();

		eqTime.minute = atoi(row[0]);
		eqTime.hour = atoi(row[1]);
		eqTime.day = atoi(row[2]);
		eqTime.month = atoi(row[3]);
		eqTime.year = atoi(row[4]);
		realtime = atoi(row[5]);
	}

	return eqTime;
}

bool Database::SaveTime(int8 minute, int8 hour, int8 day, int8 month, int16 year)
{
	std::string query = StringFormat("UPDATE eqtime set minute = %d, hour = %d, day = %d, month = %d, year = %d, realtime = %d limit 1", minute, hour, day, month, year, time(0));
	auto results = QueryDatabase(query);

	return results.Success();

}

int Database::GetIPExemption(std::string account_ip) {
	std::string query = StringFormat("SELECT `exemption_amount` FROM `ip_exemptions` WHERE `exemption_ip` = '%s'", account_ip.c_str());
	auto results = QueryDatabase(query);
	
	if (results.Success() && results.RowCount() > 0) {
		auto row = results.begin();
		return atoi(row[0]);
	}
	
	return RuleI(World, MaxClientsPerIP);
}

int Database::GetInstanceID(uint32 char_id, uint32 zone_id) {
	std::string query = StringFormat("SELECT instance_list.id FROM instance_list INNER JOIN instance_list_player ON instance_list.id = instance_list_player.id WHERE instance_list.zone = '%i' AND instance_list_player.charid = '%i'", zone_id, char_id);
	auto results = QueryDatabase(query);

	if (results.Success() && results.RowCount() > 0) {
		auto row = results.begin();
		return atoi(row[0]);;
	}

	return 0;
}
