/*	EQEMu: Everquest Server Emulator
Copyright (C) 2001-2002 EQEMu Development Team (http://eqemu.org)

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

#include "client.h"
#include "entity.h"
#include "groups.h"
#include "mob.h"
#include "raids.h"

#include "../common/rulesys.h"
#include "../common/data_verification.h"

#include "hate_list.h"
#include "quest_parser_collection.h"
#include "zone.h"
#include "water_map.h"

#include <stdlib.h>
#include <list>

extern Zone *zone;

HateList::HateList()
{
	hate_owner = nullptr;
}

HateList::~HateList()
{
}

// added for frenzy support
// checks if target still is in frenzy mode
void HateList::IsEntityInFrenzyMode()
{
	auto iterator = list.begin();
	while (iterator != list.end())
	{
		if ((*iterator)->entity_on_hatelist->GetHPRatio() >= 20)
			(*iterator)->is_entity_frenzy = false;
		++iterator;
	}
}

void HateList::WipeHateList(bool npc_only)
{
	auto iterator = list.begin();

	while (iterator != list.end())
	{
		Mob* m = (*iterator)->entity_on_hatelist;
		if (m && (m->IsClient() || (m->IsPet() && m->GetOwner()->IsClient())) && npc_only)
			iterator++;
		else {
			if (m)
			{
				parse->EventNPC(EVENT_HATE_LIST, hate_owner->CastToNPC(), m, "0", 0);
				if (m->IsClient()) {
					m->CastToClient()->DecrementAggroCount();
					// P2002 doesn't use XTarget
					//m->CastToClient()->RemoveXTarget(hate_owner, true);
				}
			}
			delete (*iterator);
			iterator = list.erase(iterator);
		}
	}
}

bool HateList::IsEntOnHateList(Mob *mob)
{
	if (Find(mob))
		return true;
	return false;
}

struct_HateList *HateList::Find(Mob *in_entity)
{
	auto iterator = list.begin();
	while (iterator != list.end())
	{
		if ((*iterator)->entity_on_hatelist == in_entity)
			return (*iterator);
		++iterator;
	}
	return nullptr;
}

void HateList::SetHateAmountOnEnt(Mob* other, uint32 in_hate, uint32 in_damage)
{
	struct_HateList *entity = Find(other);
	if (entity)
	{
		if (in_damage > 0) {
			entity->hatelist_damage = in_damage;
		}

		if (in_hate > 0) {
			entity->stored_hate_amount = in_hate;
		}
		entity->last_modified = Timer::GetCurrentTime();
	}
}

Mob* HateList::GetDamageTopOnHateList(Mob* hater)
{
	Mob* current = nullptr;
	Group* grp = nullptr;
	Raid* r = nullptr;
	uint32 dmg_amt = 0;

	auto iterator = list.begin();
	while (iterator != list.end())
	{
		grp = nullptr;
		r = nullptr;

		if ((*iterator)->entity_on_hatelist && (*iterator)->entity_on_hatelist->IsClient()){
			r = entity_list.GetRaidByClient((*iterator)->entity_on_hatelist->CastToClient());
		}

		grp = entity_list.GetGroupByMob((*iterator)->entity_on_hatelist);

		if ((*iterator)->entity_on_hatelist && r){
			if (r->GetTotalRaidDamage(hater) >= dmg_amt)
			{
				current = (*iterator)->entity_on_hatelist;
				dmg_amt = r->GetTotalRaidDamage(hater);
			}
		}
		else if ((*iterator)->entity_on_hatelist != nullptr && grp != nullptr)
		{
			if (grp->GetTotalGroupDamage(hater) >= dmg_amt)
			{
				current = (*iterator)->entity_on_hatelist;
				dmg_amt = grp->GetTotalGroupDamage(hater);
			}
		}
		else if ((*iterator)->entity_on_hatelist != nullptr && (uint32)(*iterator)->hatelist_damage >= dmg_amt)
		{
			current = (*iterator)->entity_on_hatelist;
			dmg_amt = (*iterator)->hatelist_damage;
		}
		++iterator;
	}
	return current;
}

Mob* HateList::GetClosestEntOnHateList(Mob *hater, bool clients_first) {
	Mob* close_entity = nullptr;
	float close_distance = 99999.9f;
	float hatelist_distance;

	auto iterator = list.begin();
	while (iterator != list.end()) {
		hatelist_distance = DistanceSquaredNoZ((*iterator)->entity_on_hatelist->GetPosition(), hater->GetPosition());
		Mob* hatelist_entity = (*iterator)->entity_on_hatelist;
		if (hatelist_entity != nullptr) {
			if (close_entity == nullptr
			|| (hatelist_entity->IsClient() && close_entity->IsNPC() && hatelist_entity->CombatRange(hater))
			|| (hatelist_entity->IsNPC() && close_entity->IsClient() && !close_entity->CombatRange(hater))
			|| (hatelist_distance <= close_distance && ( (close_entity->IsClient() && hatelist_entity->IsClient()) 
														|| (close_entity->IsNPC() && hatelist_entity->IsNPC())))) {
				close_distance = hatelist_distance;
				close_entity = hatelist_entity;
			}
		}

		++iterator;
	}

	if ((!close_entity && hater->IsNPC()) || (close_entity && close_entity->DivineAura()))
		close_entity = hater->CastToNPC()->GetHateTop();

	return close_entity;
}

//Count how many players (non-pet) are on aggro list
int HateList::GetClientAggroCount()
{
	int clientCount = 0;
	auto iterator = list.begin();
	while (iterator != list.end())
	{
		if ((*iterator)->entity_on_hatelist && (*iterator)->entity_on_hatelist->IsClient()) clientCount++;
		++iterator;
	}
	return clientCount;
}

void HateList::AddEntToHateList(Mob *in_entity, int32 in_hate, int32 in_damage, bool in_is_entity_frenzied, bool iAddIfNotExist)
{
	if (!in_entity) {
		return;
	}

	if (in_entity->IsCorpse()) {
		return;
	}

	if (in_entity->IsClient() && in_entity->CastToClient()->IsDead()) {
		return;
	}

	struct_HateList *entity = Find(in_entity);
	if (entity)
	{
		entity->hatelist_damage += (in_damage >= 0) ? in_damage : 0;
		entity->stored_hate_amount += in_hate;
		entity->is_entity_frenzy = in_is_entity_frenzied;
	  entity->last_modified = Timer::GetCurrentTime();
	}
	else if (iAddIfNotExist) {
		entity = new struct_HateList;
		entity->entity_on_hatelist = in_entity;
		entity->hatelist_damage = (in_damage >= 0) ? in_damage : 0;
		entity->stored_hate_amount = in_hate;
		entity->is_entity_frenzy = in_is_entity_frenzied;
	  entity->last_modified = Timer::GetCurrentTime();
		list.push_back(entity);
		parse->EventNPC(EVENT_HATE_LIST, hate_owner->CastToNPC(), in_entity, "1", 0);

		if (in_entity->IsClient()) {
			if (hate_owner->CastToNPC()->IsRaidTarget()) {
				in_entity->CastToClient()->SetEngagedRaidTarget(true);
			}
			in_entity->CastToClient()->IncrementAggroCount();
		}
	}
}

bool HateList::RemoveEntFromHateList(Mob *in_entity)
{
	if (!in_entity)
		return false;

	bool is_found = false;
	auto iterator = list.begin();

	while (iterator != list.end())
	{
		if ((*iterator)->entity_on_hatelist == in_entity)
		{
			is_found = true;

			if (in_entity && in_entity->IsClient())
				in_entity->CastToClient()->DecrementAggroCount();

			delete (*iterator);
			iterator = list.erase(iterator);

			if (in_entity)
				parse->EventNPC(EVENT_HATE_LIST, hate_owner->CastToNPC(), in_entity, "0", 0);

		}
		else
			++iterator;
	}
	return is_found;
}

void HateList::DoFactionHits(int32 npc_faction_level_id) {
	if (npc_faction_level_id <= 0)
		return;
	auto iterator = list.begin();
	while (iterator != list.end())
	{
		Client *client;

		if ((*iterator)->entity_on_hatelist && (*iterator)->entity_on_hatelist->IsClient())
			client = (*iterator)->entity_on_hatelist->CastToClient();
		else
			client = nullptr;

		if (client)
			client->SetFactionLevel(client->CharacterID(), npc_faction_level_id, client->GetBaseClass(), client->GetBaseRace(), client->GetDeity());
		++iterator;
	}
}

int HateList::GetSummonedPetCountOnHateList(Mob *hater) {

	//Function to get number of 'Summoned' pets on a targets hate list to allow calculations for certian spell effects.
	//Unclear from description that pets are required to be 'summoned body type'. Will not require at this time.
	int pet_count = 0;
	auto iterator = list.begin();
	while (iterator != list.end()) {

		if ((*iterator)->entity_on_hatelist != nullptr && (*iterator)->entity_on_hatelist->IsNPC() && ((*iterator)->entity_on_hatelist->CastToNPC()->IsPet() || ((*iterator)->entity_on_hatelist->CastToNPC()->GetSwarmOwner() > 0)))
		{
			++pet_count;
		}

		++iterator;
	}

	return pet_count;
}

int HateList::GetHateRatio(Mob *top, Mob *other)
{
	auto other_entry = Find(other);

	if (!other_entry || other_entry->stored_hate_amount < 1)
		return 0;

	auto top_entry = Find(top);

	if (!top_entry || top_entry->stored_hate_amount < 1)
		return 999; // shouldn't happen if you call it right :P

	return EQEmu::Clamp(static_cast<int>((other_entry->stored_hate_amount * 100) / top_entry->stored_hate_amount), 1, 999);
}

// skip is used to ignore a certain mob on the list
// Currently used for getting 2nd on list for aggro meter
Mob *HateList::GetEntWithMostHateOnList(Mob *center, Mob *skip)
{
	// hack fix for zone shutdown crashes on some servers
	if (!zone->IsLoaded())
		return nullptr;

	Mob* top_hate = nullptr;
	int64 hate = -1;

	if (center == nullptr)
		return nullptr;

	if (RuleB(Aggro, SmartAggroList)) {
		Mob *top_client_type_in_range = nullptr;
		int64 hate_client_type_in_range = -1;
		int skipped_count = 0;

		auto iterator = list.begin();
		while (iterator != list.end()) {
			struct_HateList *cur = (*iterator);
			int16 aggro_mod = 0;
			int16 hate_mod = 0;

			if (!cur) {
				++iterator;
				continue;
			}

			if (!cur->entity_on_hatelist) {
				++iterator;
				continue;
			}

			if (cur->entity_on_hatelist == skip) {
				++iterator;
				continue;
			}

			auto hateEntryPosition = glm::vec3(cur->entity_on_hatelist->GetX(), cur->entity_on_hatelist->GetY(),
											   cur->entity_on_hatelist->GetZ());
			if (center->IsNPC() && center->CastToNPC()->IsUnderwaterOnly() && zone->HasWaterMap()) {
				if (!zone->watermap->InLiquid(hateEntryPosition)) {
					skipped_count++;
					++iterator;
					continue;
				}
			}

			if (cur->entity_on_hatelist->Sanctuary()) {
				if (hate == -1) {
					top_hate = cur->entity_on_hatelist;
					hate = 1;
				}
				++iterator;
				continue;
			}

			if (cur->entity_on_hatelist->DivineAura() || cur->entity_on_hatelist->IsMezzed() ||
				cur->entity_on_hatelist->IsFeared()) {
				if (hate == -1) {
					top_hate = cur->entity_on_hatelist;
					hate = 0;
				}
				++iterator;
				continue;
			}

			int64 current_hate = cur->stored_hate_amount;

#ifdef BOTS
            if (cur->entity_on_hatelist->IsClient() || cur->entity_on_hatelist->IsBot()){

                if (cur->entity_on_hatelist->IsClient() && cur->entity_on_hatelist->CastToClient()->IsSitting()){
                    aggro_mod += RuleI(Aggro, SittingAggroMod);
                }
#else
			if (cur->entity_on_hatelist->IsClient()) {

				if (cur->entity_on_hatelist->CastToClient()->IsSitting()) {
					aggro_mod += RuleI(Aggro, SittingAggroMod);
				}
#endif
				if (center) {
					if (center->GetTarget() == cur->entity_on_hatelist)
						aggro_mod += RuleI(Aggro, CurrentTargetAggroMod);

					if (center->CombatRange(cur->entity_on_hatelist)) {
						int32 max_hp = center->GetMaxHP();
						if (max_hp < 4000)
							hate_mod = 100;
						else if (max_hp >= 4000 && max_hp <= 60000)
							hate_mod = ((max_hp - 4000) / 75) + 100;
						else
							hate_mod = (max_hp / 100) + 250;
						if (hate_mod > 2250)
							hate_mod = 2250;

						if (current_hate > hate_client_type_in_range || cur->is_entity_frenzy) {
							hate_client_type_in_range = current_hate;
							top_client_type_in_range = cur->entity_on_hatelist;
						}
					}
				}
			}
			else {
				if (center) {
					if (center->GetTarget() == cur->entity_on_hatelist)
						aggro_mod += RuleI(Aggro, CurrentTargetAggroMod);
					if (RuleI(Aggro, MeleeRangeAggroMod) != 0) {
						if (center->CombatRange(cur->entity_on_hatelist) ||
						((cur->entity_on_hatelist->IsCharmed() && center->CombatRange(cur->entity_on_hatelist, RuleR(Aggro, CharmedPetAggroRadiusMod))))) {
							aggro_mod += RuleI(Aggro, MeleeRangeAggroMod);
						}
					}
				}
			}

			if (cur->entity_on_hatelist->GetMaxHP() != 0 &&
				((cur->entity_on_hatelist->GetHP() * 100 / cur->entity_on_hatelist->GetMaxHP()) < 20)) {
				aggro_mod += RuleI(Aggro, CriticallyWoundedAggroMod);
			}

			if (aggro_mod) {
				current_hate += (current_hate * aggro_mod / 100) + hate_mod;
			}

			if (current_hate > hate || cur->is_entity_frenzy) {
				hate = current_hate;
				top_hate = cur->entity_on_hatelist;
			}

			++iterator;
		}

		if (top_client_type_in_range != nullptr && top_hate != nullptr) {
			bool isTopClientType = top_hate->IsClient();
#ifdef BOTS
            if (!isTopClientType) {
                if (top_hate->IsBot()) {
                    isTopClientType = true;
                    top_client_type_in_range = top_hate;
                }
            }
#endif //BOTS

			if (!isTopClientType) {
				if (top_hate->IsMerc()) {
					isTopClientType = true;
					top_client_type_in_range = top_hate;
				}
			}

			if (!isTopClientType) {
				if (top_hate->GetSpecialAbility(ALLOW_TO_TANK)) {
					isTopClientType = true;
					top_client_type_in_range = top_hate;
				}
			}

			if (!isTopClientType)
				return top_client_type_in_range ? top_client_type_in_range : nullptr;

			return top_hate ? top_hate : nullptr;
		} else {
			if (top_hate == nullptr && skipped_count > 0) {
				if (center->GetTarget() && center->GetTarget()->GetHP() > 0) {
					return center->GetTarget();
				} else {
					return nullptr;
				}
			}
			return top_hate ? top_hate : nullptr;
		}
	} else {
		auto iterator = list.begin();
		int skipped_count = 0;
		while (iterator != list.end()) {
			struct_HateList *cur = (*iterator);
			if (cur->entity_on_hatelist == skip) {
				++iterator;
				continue;
			}

			if (center->IsNPC() && center->CastToNPC()->IsUnderwaterOnly() && zone->HasWaterMap()) {
				if (!zone->watermap->InLiquid(glm::vec3(cur->entity_on_hatelist->GetPosition()))) {
					skipped_count++;
					++iterator;
					continue;
				}
			}

			if (cur->entity_on_hatelist != nullptr && ((cur->stored_hate_amount > hate) || cur->is_entity_frenzy)) {
				top_hate = cur->entity_on_hatelist;
				hate = cur->stored_hate_amount;
			}
			++iterator;
		}
		if (top_hate == nullptr && skipped_count > 0) {
			if (center->GetTarget() && center->GetTarget()->GetHP() > 0) {
				return center->GetTarget();
			} else {
				return nullptr;
			}
		}
		return top_hate ? top_hate : nullptr;
	}
	return nullptr;
}

Mob *HateList::GetEntOnHateListByID(uint16 mobId) {
	Mob* toReturn = nullptr;
	auto iterator = list.begin();
	while (iterator != list.end())
	{
		struct_HateList *cur = (*iterator);
		if (cur->entity_on_hatelist->GetID() == mobId)
			return cur->entity_on_hatelist;
		++iterator;
	}
	return toReturn;
}

Mob *HateList::GetEntWithMostHateOnList(){
	Mob* top = nullptr;
	int64 hate = -1;

	auto iterator = list.begin();
	while (iterator != list.end())
	{
		struct_HateList *cur = (*iterator);
		if (cur && cur->entity_on_hatelist != nullptr && (cur->stored_hate_amount > hate))
		{
			top = cur->entity_on_hatelist;
			hate = cur->stored_hate_amount;
		}
		++iterator;
	}
	return top;
}


Mob *HateList::GetRandomEntOnHateList()
{
	int count = list.size();
	if (count == 0) //If we don't have any entries it'll crash getting a random 0, -1 position.
		return NULL;

	if (count == 1) //No need to do all that extra work if we only have one hate entry
	{
		if (*list.begin()) // Just in case tHateEntry is invalidated somehow...
			return (*list.begin())->entity_on_hatelist;

		return NULL;
	}

	auto iterator = list.begin();
	int random = zone->random.Int(0, count - 1);
	for (int i = 0; i < random; i++)
		++iterator;

	return (*iterator)->entity_on_hatelist;
}

int32 HateList::GetEntHateAmount(Mob *in_entity, bool damage)
{
	struct_HateList *entity;

	entity = Find(in_entity);

	if (entity && damage)
		return entity->hatelist_damage;
	else if (entity)
		return entity->stored_hate_amount;
	else
		return 0;
}

bool HateList::IsHateListEmpty() {
	return(list.size() == 0);
}

void HateList::PrintHateListToClient(Client *c)
{
	auto iterator = list.begin();
	while (iterator != list.end())
	{
		struct_HateList *e = (*iterator);
		c->Message(0, "- name: %s, damage: %d, hate: %d",
			(e->entity_on_hatelist && e->entity_on_hatelist->GetName()) ? e->entity_on_hatelist->GetName() : "(null)",
			e->hatelist_damage, e->stored_hate_amount);

		++iterator;
	}
}

int HateList::AreaRampage(Mob *caster, Mob *target, int count, ExtraAttackOptions *opts)
{
	if (!target || !caster)
		return 0;

	int hit_count = 0;
	// This should prevent crashes if something dies (or mainly more than 1 thing goes away)
	// This is a temp solution until the hate lists can be rewritten to not have that issue
	std::vector<uint16> id_list;
	for (auto &h : list) {
		if (h->entity_on_hatelist && h->entity_on_hatelist != caster &&
		    caster->CombatRange(h->entity_on_hatelist, 1.0, true)) {
			if(caster->GetHateTop()->GetID() != h->entity_on_hatelist->GetID() || caster->GetTarget()->GetID() != h->entity_on_hatelist->GetID()) {
				id_list.push_back(h->entity_on_hatelist->GetID());
			} else {
				caster->Shout("Excludingi: %s", h->entity_on_hatelist->GetName());
			}
		}

		if (count != -1 && id_list.size() > count) {
			break;
		}
	}

	for (auto &id : id_list) {
		auto mob = entity_list.GetMobID(id);
    bool tank = caster->GetHateTop()->GetID() == mob->GetID();
		if (mob && !tank) {
			++hit_count;
			caster->ProcessAttackRounds(mob, opts);
			//caster->Shout("Hitting: %s",mob->CastToNPC()->GetName());
		}
	}

	return hit_count;
}

/* int HateList::AreaRampage(Mob *caster, Mob *target, int count, ExtraAttackOptions *opts)
{
	if(!target || !caster)
		return 0;

	int ret = 0;
	std::list<uint32> id_list;
	auto iterator = list.begin();
	while (iterator != list.end())
	{
		struct_HateList *h = (*iterator);
		++iterator;
		if(h && h->entity_on_hatelist && h->entity_on_hatelist != caster)
		{
			if(caster->CombatRange(h->entity_on_hatelist))
			{
				id_list.push_back(h->entity_on_hatelist->GetID());
				++ret;
			}
		}
	}

	std::list<uint32>::iterator iter = id_list.begin();
	Mob *hate_top = target->GetHateMost();
	while(iter != id_list.end())
	{
		Mob *cur = entity_list.GetMobID((*iter));
		if(cur)
		{
			for(int i = 0; i < count; ++i) {
				if(!hate_top) {
					caster->ProcessAttackRounds(cur, opts);
				}
			}
		}
		iter++;
	}

	return ret;
} */

void HateList::SpellCast(Mob *caster, uint32 spell_id, float range, Mob* ae_center)
{
	if (!caster)
		return;

	Mob* center = caster;

	if (ae_center)
		center = ae_center;

	//this is slower than just iterating through the list but avoids
	//crashes when people kick the bucket in the middle of this call
	//that invalidates our iterator but there's no way to know sadly
	//So keep a list of entity ids and look up after
	std::list<uint32> id_list;
	range = range * range;
	float min_range2 = spells[spell_id].min_range * spells[spell_id].min_range;
	float dist_targ = 0;
	auto iterator = list.begin();
	while (iterator != list.end())
	{
		struct_HateList *h = (*iterator);
		if (range > 0)
		{
			dist_targ = DistanceSquared(center->GetPosition(), h->entity_on_hatelist->GetPosition());
			if (dist_targ <= range && dist_targ >= min_range2)
			{
				id_list.push_back(h->entity_on_hatelist->GetID());
				h->entity_on_hatelist->CalcSpellPowerDistanceMod(spell_id, dist_targ);
			}
		}
		else
		{
			id_list.push_back(h->entity_on_hatelist->GetID());
			h->entity_on_hatelist->CalcSpellPowerDistanceMod(spell_id, 0, caster);
		}
		++iterator;
	}

	auto iter = id_list.begin();
	while (iter != id_list.end())
	{
		Mob *cur = entity_list.GetMobID((*iter));
		if (cur)
		{
			caster->SpellOnTarget(spell_id, cur);
		}
		iter++;
	}
}

//When a death happens, this trigger causes entities on the hate list a trigger
void HateList::OnDeathTrigger()
{

	// uint32 rank;
	auto iterator = list.begin();
	while (iterator != list.end())
	{
		struct_HateList *h = (*iterator);
		Mob *mobHated = h->entity_on_hatelist;

		// Custom non-implemented feature
		/*if (mobHated->IsClient()) {
			uint8 rank = mobHated->CastToClient()->GetBuildRank(SHADOWKNIGHT, RB_SHD_ROTTENCORE);
			if (rank > 0) {
				uint32 counters = mobHated->CastToClient()->GetCoreCounter();
				mobHated->CastToClient()->AddCoreCounter(1);
				if (counters < mobHated->CastToClient()->GetCoreCounter()) {
					mobHated->Message(MT_FocusEffect, "Rotten Core %u increased to %u counters.", rank, mobHated->CastToClient()->GetCoreCounter());
				}
			}
			rank = mobHated->CastToClient()->GetBuildRank(ROGUE, RB_ROG_KILLINGSPREE);
			if (rank > 0) {
				uint32 counters = mobHated->CastToClient()->GetCoreCounter();
				mobHated->CastToClient()->AddCoreCounter(1);
				if (counters < mobHated->CastToClient()->GetCoreCounter()) {
					mobHated->Message(MT_FocusEffect, "Killing Spree %u increased to %u counters.", rank, mobHated->CastToClient()->GetCoreCounter());
				}
			}
		} */
		++iterator;
	}
}

void HateList::RemoveStaleEntries(int time_ms, float dist)
{
	auto it = list.begin();

	auto cur_time = Timer::GetCurrentTime();

	while (it != list.end()) {
		auto m = (*it)->entity_on_hatelist;
		if (m) {
			// 10 mins or distance
			if ((cur_time - (*it)->last_modified > time_ms) || hate_owner->CalculateDistance(m->GetX(), m->GetY(), m->GetZ()) > dist) {
				parse->EventNPC(EVENT_HATE_LIST, hate_owner->CastToNPC(), m, "0", 0);

				if (m->IsClient()) {
					m->CastToClient()->DecrementAggroCount();
					m->CastToClient()->RemoveXTarget(hate_owner, true);
				}

				delete (*it);
				it = list.erase(it);
				continue;
			}
		}
		++it;
	}
}
