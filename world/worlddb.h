/*	EQEMu: Everquest Server Emulator
	Copyright (C) 2001-2006 EQEMu Development Team (http://eqemulator.net)

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
#ifndef WORLDDB_H_
#define WORLDDB_H_

#include "../common/shareddb.h"
#include "../common/zone_numbers.h"
#include "../common/eq_packet.h"
#include "../common/database.h"

struct PlayerProfile_Struct;
struct CharCreate_Struct;
struct CharacterSelect_Struct;

//Used by ItemScore system
struct CharData_Struct {
	explicit CharData_Struct(int pID, int pClassID) : ID(pID), classID(pClassID) {};
	int ID;
	int classID;
};

class WorldDatabase : public SharedDatabase {
public:
	bool GetStartZone(PlayerProfile_Struct* in_pp, CharCreate_Struct* in_cc, bool isTitanium);
	void GetCharSelectInfo(uint32 accountID, EQApplicationPacket **outApp, uint32 clientVersionBit);
	int MoveCharacterToBind(int CharID, uint8 bindnum = 0);

	void GetLauncherList(std::vector<std::string> &result);
	bool GetCharacterLevel(const char *name, int &level);

	bool LoadCharacterCreateAllocations();
	bool LoadCharacterCreateCombos();
	bool GetAllCharacters(std::vector<CharData_Struct*> &chars);

private:
	void SetTitaniumDefaultStartZone(PlayerProfile_Struct* in_pp, CharCreate_Struct* in_cc);
	void SetSoFDefaultStartZone(PlayerProfile_Struct* in_pp, CharCreate_Struct* in_cc);
};

extern WorldDatabase database;

#endif /*WORLDDB_H_*/
