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
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "item_data.h"
#include "classes.h"
#include "races.h"
//#include "deity.h"


uint32 EQEmu::item::ConvertAugTypeToAugTypeBit(uint8 aug_type)
{
	switch (aug_type) {
	case AugTypeGeneralSingleStat:
		return bit_AugTypeGeneralSingleStat;
	case AugTypeGeneralMultipleStat:
		return bit_AugTypeGeneralMultipleStat;
	case AugTypeGeneralSpellEffect:
		return bit_AugTypeGeneralSpellEffect;
	case AugTypeWeaponGeneral:
		return bit_AugTypeWeaponGeneral;
	case AugTypeWeaponElemDamage:
		return bit_AugTypeWeaponElemDamage;
	case AugTypeWeaponBaseDamage:
		return bit_AugTypeWeaponBaseDamage;
	case AugTypeGeneralGroup:
		return bit_AugTypeGeneralGroup;
	case AugTypeGeneralRaid:
		return bit_AugTypeGeneralRaid;
	case AugTypeGeneralDragonsPoints:
		return bit_AugTypeGeneralDragonsPoints;
	case AugTypeCraftedCommon:
		return bit_AugTypeCraftedCommon;
	case AugTypeCraftedGroup1:
		return bit_AugTypeCraftedGroup1;
	case AugTypeCraftedRaid1:
		return bit_AugTypeCraftedRaid1;
	case AugTypeEnergeiacGroup:
		return bit_AugTypeEnergeiacGroup;
	case AugTypeEnergeiacRaid:
		return bit_AugTypeEnergeiacRaid;
	case AugTypeEmblem:
		return bit_AugTypeEmblem;
	case AugTypeCraftedGroup2:
		return bit_AugTypeCraftedGroup2;
	case AugTypeCraftedRaid2:
		return bit_AugTypeCraftedRaid2;
	case AugTypeUnknown1:
		return bit_AugTypeUnknown1;
	case AugTypeUnknown2:
		return bit_AugTypeUnknown2;
	case AugTypeOrnamentation:
		return bit_AugTypeOrnamentation;
	case AugTypeSpecialOrnamentation:
		return bit_AugTypeSpecialOrnamentation;
	case AugTypeUnknown3:
		return bit_AugTypeUnknown3;
	case AugTypeUnknown4:
		return bit_AugTypeUnknown4;
	case AugTypeUnknown5:
		return bit_AugTypeUnknown5;
	case AugTypeUnknown6:
		return bit_AugTypeUnknown6;
	case AugTypeUnknown7:
		return bit_AugTypeUnknown7;
	case AugTypeUnknown8:
		return bit_AugTypeUnknown8;
	case AugTypeUnknown9:
		return bit_AugTypeUnknown9;
	case AugTypeUnknown10:
		return bit_AugTypeUnknown10;
	case AugTypeEpic2_5:
		return bit_AugTypeEpic2_5;
	case AugTypeTest:
		return bit_AugTypeTest;
	case AugTypeAll:
		return bit_AugTypeAll;
	default:
		return bit_AugTypeNone;
	}
}

uint8 EQEmu::item::ConvertAugTypeBitToAugType(uint32 aug_type_bit)
{
	switch (aug_type_bit) {
	case bit_AugTypeGeneralSingleStat:
		return AugTypeGeneralSingleStat;
	case bit_AugTypeGeneralMultipleStat:
		return AugTypeGeneralMultipleStat;
	case bit_AugTypeGeneralSpellEffect:
		return AugTypeGeneralSpellEffect;
	case bit_AugTypeWeaponGeneral:
		return AugTypeWeaponGeneral;
	case bit_AugTypeWeaponElemDamage:
		return AugTypeWeaponElemDamage;
	case bit_AugTypeWeaponBaseDamage:
		return AugTypeWeaponBaseDamage;
	case bit_AugTypeGeneralGroup:
		return AugTypeGeneralGroup;
	case bit_AugTypeGeneralRaid:
		return AugTypeGeneralRaid;
	case bit_AugTypeGeneralDragonsPoints:
		return AugTypeGeneralDragonsPoints;
	case bit_AugTypeCraftedCommon:
		return AugTypeCraftedCommon;
	case bit_AugTypeCraftedGroup1:
		return AugTypeCraftedGroup1;
	case bit_AugTypeCraftedRaid1:
		return AugTypeCraftedRaid1;
	case bit_AugTypeEnergeiacGroup:
		return AugTypeEnergeiacGroup;
	case bit_AugTypeEnergeiacRaid:
		return AugTypeEnergeiacRaid;
	case bit_AugTypeEmblem:
		return AugTypeEmblem;
	case bit_AugTypeCraftedGroup2:
		return AugTypeCraftedGroup2;
	case bit_AugTypeCraftedRaid2:
		return AugTypeCraftedRaid2;
	case bit_AugTypeUnknown1:
		return AugTypeUnknown1;
	case bit_AugTypeUnknown2:
		return AugTypeUnknown2;
	case bit_AugTypeOrnamentation:
		return AugTypeOrnamentation;
	case bit_AugTypeSpecialOrnamentation:
		return AugTypeSpecialOrnamentation;
	case bit_AugTypeUnknown3:
		return AugTypeUnknown3;
	case bit_AugTypeUnknown4:
		return AugTypeUnknown4;
	case bit_AugTypeUnknown5:
		return AugTypeUnknown5;
	case bit_AugTypeUnknown6:
		return AugTypeUnknown6;
	case bit_AugTypeUnknown7:
		return AugTypeUnknown7;
	case bit_AugTypeUnknown8:
		return AugTypeUnknown8;
	case bit_AugTypeUnknown9:
		return AugTypeUnknown9;
	case bit_AugTypeUnknown10:
		return AugTypeUnknown10;
	case bit_AugTypeEpic2_5:
		return AugTypeEpic2_5;
	case bit_AugTypeTest:
		return AugTypeTest;
	case bit_AugTypeAll:
		return AugTypeAll;
	default:
		return AugTypeNone;
	}
}

bool EQEmu::ItemData::IsEquipable(uint16 race_id, uint16 class_id) const
{
	if (!(Races & GetPlayerRaceBit(race_id)))
		return false;

	if (!(Classes & GetPlayerClassBit(GetPlayerClassValue(class_id))))
		return false;

	return true;
}

bool EQEmu::ItemData::IsClassCommon() const
{
	return (ItemClass == item::ItemClassCommon);
}

bool EQEmu::ItemData::IsClassBag() const
{
	return (ItemClass == item::ItemClassBag);
}

bool EQEmu::ItemData::IsClassBook() const
{
	return (ItemClass == item::ItemClassBook);
}

bool EQEmu::ItemData::IsType1HWeapon() const
{
	return ((ItemType == item::ItemType1HBlunt) || (ItemType == item::ItemType1HSlash) || (ItemType == item::ItemType1HPiercing) || (ItemType == item::ItemTypeMartial));
}

bool EQEmu::ItemData::IsType2HWeapon() const
{
	return ((ItemType == item::ItemType2HBlunt) || (ItemType == item::ItemType2HSlash) || (ItemType == item::ItemType2HPiercing));
}

bool EQEmu::ItemData::IsTypeShield() const
{
	return (ItemType == item::ItemTypeShield);
}

bool EQEmu::ItemData::CheckLoreConflict(const ItemData* l_item, const ItemData* r_item)
{
	if (!l_item || !r_item)
		return false;

	if (!l_item->LoreGroup || !r_item->LoreGroup)
		return false;

	if (l_item->LoreGroup == r_item->LoreGroup) {
		if ((l_item->LoreGroup == -1) && (l_item->ID != r_item->ID))
			return false;

		return true;
	}

	return false;
}

int EQEmu::ItemData::GetItemScore() const
{
	int itemScore = 0;
	auto item = this;
	if (item->AAgi > 0) {
		itemScore += item->AAgi * 30;
		//if (IsFighterClass(myClass)) classItemScore += item->AAgi * 40;
		//else if (IsCasterClass(myClass)) classItemScore += item->AAgi * 10;
	}
	if (item->AC > 0) itemScore += item->AC * 50;
	if (item->Accuracy > 0) itemScore += item->Accuracy * 80;
	if (item->ACha > 0) itemScore += item->ACha * 10;
	if (item->ADex > 0) itemScore += item->ACha * 20;
	if (item->AInt > 0) itemScore += item->AInt * 30;
	if (item->ASta > 0) itemScore += item->ASta * 30;
	if (item->AStr > 0) itemScore += item->AStr * 10;
	if (item->Attack > 0) itemScore += item->Attack * 30;
	if (item->Avoidance > 0) itemScore += item->Avoidance * 40;
	if (item->AWis > 0) itemScore += item->AWis * 30;
	if (item->BackstabDmg > 0) itemScore += item->BackstabDmg * 10;
	if (item->BaneDmgAmt > 0) itemScore += item->BaneDmgAmt * 90;
	if (item->BaneDmgRaceAmt > 0) itemScore += item->BaneDmgRaceAmt * 90;
	if (item->CombatEffects > 0) itemScore += item->CombatEffects * 10;
	if (item->Damage > 0) itemScore += item->Damage * 80;
	if (item->DamageShield > 0) itemScore += item->DamageShield * 20;
	if (item->DotShielding > 0) itemScore += item->DotShielding * 20;
	if (item->DR > 0) itemScore += item->DR * 30;
	if (item->DSMitigation > 0) itemScore += item->DSMitigation * 40;
	if (item->ElemDmgAmt > 0) itemScore += item->ElemDmgAmt * 90;
	if (item->Endur > 0) itemScore += item->Endur * 20;
	if (item->EnduranceRegen > 0) itemScore += item->EnduranceRegen * 10;
	if (item->ExtraDmgAmt > 0) itemScore += item->ExtraDmgAmt * 70;
	if (item->Focus.Effect > 0) itemScore += 30;
	if (item->FR > 0)
		itemScore += item->FR * 40;
	if (item->Haste > 0)
		itemScore += item->Haste * 90;
	if (item->HealAmt > 0)
		itemScore += item->HealAmt * 90;
	if (item->HeroicAgi > 0)
		itemScore += item->HeroicAgi * 50;
	if (item->HeroicCha > 0)
		itemScore += item->HeroicCha * 20;

	if (item->HeroicCR > 0)
		itemScore += item->HeroicCR * 20;

	if (item->HeroicDex > 0)
		itemScore += item->HeroicDex * 20;

	if (item->HeroicDR > 0)
		itemScore += item->HeroicDR * 20;

	if (item->HeroicFR > 0)
		itemScore += item->HeroicFR * 20;

	if (item->HeroicInt > 0)
		itemScore += item->HeroicInt * 20;
	if (item->HeroicMR > 0)
		itemScore += item->HeroicMR * 20;
	if (item->HeroicPR > 0)
		itemScore += item->HeroicPR * 20;
	if (item->HeroicSta > 0)
		itemScore += item->HeroicSta * 20;
	if (item->HeroicStr > 0)
		itemScore += item->HeroicStr * 20;
	if (item->HeroicSVCorrup > 0)
		itemScore += item->HeroicSVCorrup * 20;
	if (item->HeroicWis > 0)
		itemScore += item->HeroicWis * 20;
	if (item->HP > 0)
		itemScore += item->HP * 90;
	if (item->Mana > 0)
		itemScore += item->Mana * 90;
	if (item->ManaRegen > 0)
		itemScore += item->ManaRegen * 120;
	if (item->MR > 0)
		itemScore += item->MR * 40;
	if (item->PR > 0)
		itemScore += item->PR * 40;
	if (item->Proc.Effect > 0)
		itemScore += 30;
	if (item->Purity > 0)
		itemScore += item->Purity * 30;
	if (item->Regen > 0)
		itemScore += item->Regen * 30;
	if (item->Shielding > 0)
		itemScore += item->Shielding * 50;
	if (item->SpellDmg > 0)
		itemScore += item->SpellDmg * 50;
	if (item->SpellShield > 0)
		itemScore += item->SpellShield * 40;
	if (item->StrikeThrough > 0)
		itemScore += item->StrikeThrough * 80;
	if (item->StunResist > 0) {
		itemScore += item->StunResist * 40;
	}
	if (item->SVCorruption > 0) {
		itemScore += item->SVCorruption * 40;
	}
	if (item->Worn.Effect > 0) {
		itemScore += 40;
	}
	return itemScore;
}