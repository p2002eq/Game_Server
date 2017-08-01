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

#include "database.h"
#include "string_util.h"

#pragma warning( disable : 4267 4715 )

/*	This section past here is for setting up the database in bootstrap ONLY.
	Please put regular server changes and additions above this.
	Don't use this for the login server. It should never have access to the game database. */

bool Database::DBSetup() {
	Log(Logs::Detail, Logs::Debug, "Database setup started..");
	DBSetup_PlayerCorpseBackup();
	DBSetup_Logs();
	return true;
}

bool Database::DBSetup_PlayerCorpseBackup(){

	std::string check_query = StringFormat("SHOW TABLES LIKE 'character_corpse_items_backup'");
	auto check_results = QueryDatabase(check_query);
	if (check_results.RowCount() == 0){
		std::string create_query = StringFormat(
				"CREATE TABLE `character_corpse_items_backup` (	  "
						"`corpse_id` int(11) unsigned NOT NULL,		  "
						"`equip_slot` int(11) unsigned NOT NULL,	  "
						"`item_id` int(11) unsigned DEFAULT NULL,	  "
						"`charges` int(11) unsigned DEFAULT NULL,	  "
						"`attuned` smallint(5) NOT NULL DEFAULT '0',  "
						"PRIMARY KEY(`corpse_id`, `equip_slot`)		  "
						") ENGINE = InnoDB DEFAULT CHARSET = latin1;  "
		);
		Log(Logs::Detail, Logs::Debug, "Attempting to create table character_corpse_items_backup..");
		auto create_results = QueryDatabase(create_query);
		if (!create_results.Success()){
			Log(Logs::Detail, Logs::Error, "Error creating character_corpse_items_backup table.");
			return false;
		}
		Log(Logs::Detail, Logs::Debug, "character_corpse_items_backup table created.");
	}

	std::string cb_check_query = StringFormat("SHOW TABLES LIKE 'character_corpses_backup'");
	auto cb_check_results = QueryDatabase(cb_check_query);
	if (cb_check_results.RowCount() == 0){
		std::string cb_create_query = StringFormat(
				"CREATE TABLE `character_corpses_backup` (	  "
						"`id` int(11) unsigned NOT NULL default 0,"
						"`charid` int(11) unsigned NOT NULL default '0',"
						"`charname` varchar(64) NOT NULL default '',"
						"`zone_id` smallint(5) NOT NULL default '0',"
						"`instance_id` smallint(5) unsigned NOT NULL default '0',"
						"`x` float NOT NULL default '0',"
						"`y` float NOT NULL default '0',"
						"`z` float NOT NULL default '0',"
						"`heading` float NOT NULL default '0',"
						"`time_of_death` datetime NOT NULL default '0000-00-00 00:00:00',"
						"`is_rezzed` tinyint(3) unsigned default '0',"
						"`is_buried` tinyint(3) NOT NULL default '0',"
						"`was_at_graveyard` tinyint(3) NOT NULL default '0',"
						"`is_locked` tinyint(11) default '0',"
						"`exp` int(11) unsigned default '0',"
						"`gmexp` int(11) NOT NULL default '0',"
						"`size` int(11) unsigned default '0',"
						"`level` int(11) unsigned default '0',"
						"`race` int(11) unsigned default '0',"
						"`gender` int(11) unsigned default '0',"
						"`class` int(11) unsigned default '0',"
						"`deity` int(11) unsigned default '0',"
						" `texture` int(11) unsigned default '0',"
						" `helm_texture` int(11) unsigned default '0',"
						" `copper` int(11) unsigned default '0',"
						" `silver` int(11) unsigned default '0',"
						" `gold` int(11) unsigned default '0',"
						" `platinum` int(11) unsigned default '0',"
						" `hair_color` int(11) unsigned default '0',"
						" `beard_color` int(11) unsigned default '0',"
						" `eye_color_1` int(11) unsigned default '0',"
						" `eye_color_2` int(11) unsigned default '0',"
						" `hair_style` int(11) unsigned default '0',"
						" `face` int(11) unsigned default '0',"
						" `beard` int(11) unsigned default '0',"
						" `drakkin_heritage` int(11) unsigned default '0',"
						" `drakkin_tattoo` int(11) unsigned default '0',"
						" `drakkin_details` int(11) unsigned default '0',"
						" `wc_1` int(11) unsigned default '0',"
						" `wc_2` int(11) unsigned default '0',"
						" `wc_3` int(11) unsigned default '0',"
						" `wc_4` int(11) unsigned default '0',"
						" `wc_5` int(11) unsigned default '0',"
						" `wc_6` int(11) unsigned default '0',"
						" `wc_7` int(11) unsigned default '0',"
						" `wc_8` int(11) unsigned default '0',"
						" `wc_9` int(11) unsigned default '0',"
						" `killedby` tinyint(11) NOT NULL default '0',"
						" `rezzable` tinyint(11) NOT NULL default '0',"
						"PRIMARY KEY(`id`)		  "
						") ENGINE=MyISAM DEFAULT CHARSET=latin1;"
		);
		Log(Logs::Detail, Logs::Debug, "Attepting to create table character_corpses_backup..");
		auto cb_create_results = QueryDatabase(cb_create_query);
		if (!cb_create_results.Success()){
			Log(Logs::Detail, Logs::Error, "Error creating character_corpses_backup table.");
			return false;
		}
		Log(Logs::Detail, Logs::Debug, "character_corpses_backup table created.");
	}

	if(cb_check_results.RowCount() == 0 || check_results.RowCount() == 0)
	{
		std::string cbp_query = StringFormat("INSERT INTO `character_corpses_backup` SELECT * from `character_corpses`");
		auto cbp_results = QueryDatabase(cbp_query);
		if (!cbp_results.Success()){
			Log(Logs::Detail, Logs::Error, "Error populating character_corpses_backup table.");
			return false;
		}
		std::string cip_query = StringFormat("INSERT INTO `character_corpse_items_backup` SELECT * from `character_corpse_items`");
		auto cip_results = QueryDatabase(cip_query);
		if (!cip_results.Success()){
			Log(Logs::Detail, Logs::Error, "Error populating character_corpse_items_backup table.");
			return false;
		}

		Log(Logs::Detail, Logs::Debug, "Corpse backup tables populated.");

		std::string delcheck_query = StringFormat(
				"SELECT id FROM `character_corpses_backup`");
		auto delcheck_results = QueryDatabase(delcheck_query);
		for (auto row = delcheck_results.begin(); row != delcheck_results.end(); ++row) {
			uint32 corpse_id = atoi(row[0]);
			std::string del_query = StringFormat(
					"DELETE from character_corpses_backup where id = %d AND ( "
							"SELECT COUNT(*) from character_corpse_items_backup where corpse_id = %d) "
							" = 0", corpse_id, corpse_id);
			auto del_results = QueryDatabase(del_query);
			if(!del_results.Success())
				return false;
		}

		Log(Logs::Detail, Logs::Debug, "Corpse backup tables cleaned of empty corpses.");
	}

	return true;
}

bool Database::DBSetup_Logs()
{
	// Placing ALL Rule creations in here, initial rule check as number in query and results, then increment by letter.
	std::string check_query1 = StringFormat("SELECT * FROM `logsys_categories` WHERE `log_category_description`='Group'");
	auto results1 = QueryDatabase(check_query1);
	if (results1.RowCount() == 0)
	{
		std::string check_query1a = StringFormat("INSERT INTO `logsys_categories` (`log_category_id`, `log_category_description`, `log_to_console`, `log_to_file`, `log_to_gmsay`) VALUES ('46', 'Group', '0', '0', '0')");
		auto results1a = QueryDatabase(check_query1a);
		if (!results1a.Success())
		{
			Log(Logs::Detail, Logs::Error, "Error creating logsys category `group`.");
			return false;
		}
	}
	std::string check_query2 = StringFormat("SELECT * FROM `logsys_categories` WHERE `log_category_description`='Corpse'");
	auto results2 = QueryDatabase(check_query2);
	if (results2.RowCount() == 0)
	{
		std::string check_query2a = StringFormat("INSERT INTO `logsys_categories` (`log_category_id`, `log_category_description`, `log_to_console`, `log_to_file`, `log_to_gmsay`) VALUES ('47', 'Corpse', '0', '0', '0')");
		auto results2a = QueryDatabase(check_query2a);
		if (!results2a.Success())
		{
			Log(Logs::Detail, Logs::Error, "Error creating logsys category `corpse`.");
			return false;
		}
	}
	std::string check_query3 = StringFormat("SELECT * FROM `logsys_categories` WHERE `log_category_description`='Bazaar'");
	auto results3 = QueryDatabase(check_query3);
	if (results3.RowCount() == 0)
	{
		std::string check_query3a = StringFormat("INSERT INTO `logsys_categories` (`log_category_id`, `log_category_description`, `log_to_console`, `log_to_file`, `log_to_gmsay`) VALUES ('48', 'Bazaar', '0', '0', '0')");
		auto results3a = QueryDatabase(check_query3a);
		if (!results3a.Success())
		{
			Log(Logs::Detail, Logs::Error, "Error creating logsys category `bazaar`.");
			return false;
		}
	}
	std::string check_query4 = StringFormat("SELECT * FROM `logsys_categories` WHERE `log_category_description`='Discs'");
	auto results4 = QueryDatabase(check_query4);
	if (results4.RowCount() == 0)
	{
		std::string check_query4a = StringFormat("INSERT INTO `logsys_categories` (`log_category_id`, `log_category_description`, `log_to_console`, `log_to_file`, `log_to_gmsay`) VALUES ('49', 'Discs', '0', '0', '0')");
		auto results4a = QueryDatabase(check_query4a);
		if (!results4a.Success())
		{
			Log(Logs::Detail, Logs::Error, "Error creating logsys category `disc`.");
			return false;
		}
	}
	std::string check_query5 = StringFormat("SELECT * FROM `logsys_categories` WHERE `log_category_description`='Boats'");
	auto results5 = QueryDatabase(check_query5);
	if (results5.RowCount() == 0)
	{
		std::string check_query5a = StringFormat("INSERT INTO `logsys_categories` (`log_category_id`, `log_category_description`, `log_to_console`, `log_to_file`, `log_to_gmsay`) VALUES ('50', 'Boats', '0', '0', '0')");
		auto results5a = QueryDatabase(check_query5a);
		if (!results5a.Success())
		{
			Log(Logs::Detail, Logs::Error, "Error creating logsys category `boats`.");
			return false;
		}
	}
	std::string check_query6 = StringFormat("SELECT * FROM `logsys_categories` WHERE `log_category_description`='Traps'");
	auto results6 = QueryDatabase(check_query6);
	if (results6.RowCount() == 0)
	{
		std::string check_query6a = StringFormat("INSERT INTO `logsys_categories` (`log_category_id`, `log_category_description`, `log_to_console`, `log_to_file`, `log_to_gmsay`) VALUES ('51', 'Traps', '0', '0', '0')");
		auto results6a = QueryDatabase(check_query6a);
		if (!results6a.Success())
		{
			Log(Logs::Detail, Logs::Error, "Error creating logsys category `traps`.");
			return false;
		}
	}
}
