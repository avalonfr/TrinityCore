/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ScriptPCH.h"
#include "trial_of_the_champion.h"

#define MAX_ENCOUNTER  4

class instance_trial_of_the_champion : public InstanceMapScript
{
public:
    instance_trial_of_the_champion() : InstanceMapScript("instance_trial_of_the_champion", 650) { }

    InstanceScript* GetInstanceScript(InstanceMap* map) const
    {
        return new instance_trial_of_the_champion_InstanceMapScript(map);
    }

    struct instance_trial_of_the_champion_InstanceMapScript : public InstanceScript
    {
        instance_trial_of_the_champion_InstanceMapScript(Map* map) : InstanceScript(map) {Initialize();}

        uint32 m_auiEncounter[MAX_ENCOUNTER];
        uint32 teamInInstance;

        uint16 movementDone;
        uint16 grandChampionsDeaths;
        uint8 argentSoldierDeaths;

        uint64 announcerGUID;
        uint64 mainGateGUID;
        uint64 portcullisGUID;
        uint64 grandChampion1GUID;
        uint64 grandChampion2GUID;
        uint64 grandChampion3GUID;
        uint64 championLootGUID;
        uint64 argentChampionGUID;

        std::list<uint64> vehicleList;
        std::string str_data;

        void Initialize()
        {
            movementDone = 0;
            grandChampionsDeaths = 0;
            argentSoldierDeaths = 0;
            teamInInstance = 0;

            announcerGUID = 0;
            mainGateGUID = 0;
            portcullisGUID = 0;
            grandChampion1GUID = 0;
            grandChampion2GUID = 0;
            grandChampion3GUID = 0;
            championLootGUID = 0;
            argentChampionGUID = 0;

            vehicleList.clear();
            memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
        }

        bool IsEncounterInProgress() const
        {
            for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
            {
                if (m_auiEncounter[i] == IN_PROGRESS)
                    return true;
            }

            return false;
        }

        void OnCreatureCreate(Creature* creature)
        {
            Map::PlayerList const &players = instance->GetPlayers();

            if (!players.isEmpty())
            {
                if (Player* player = players.begin()->getSource())
                    teamInInstance = player->GetTeam();
            }

            switch (creature->GetEntry())
            {
                // Grand Champions
                case NPC_MOKRA:
                    if (teamInInstance == HORDE)
                        creature->UpdateEntry(NPC_JACOB, ALLIANCE);
                    break;
                case NPC_ERESSEA:
                    if (teamInInstance == HORDE)
                        creature->UpdateEntry(NPC_AMBROSE, ALLIANCE);
                    break;
                case NPC_RUNOK:
                    if (teamInInstance == HORDE)
                        creature->UpdateEntry(NPC_COLOSOS, ALLIANCE);
                    break;
                case NPC_ZULTORE:
                    if (teamInInstance == HORDE)
                        creature->UpdateEntry(NPC_JAELYNE, ALLIANCE);
                    break;
                case NPC_VISCERI:
                    if (teamInInstance == HORDE)
                        creature->UpdateEntry(NPC_LANA, ALLIANCE);
                    break;
                // Faction Champios
                case NPC_ORGRIMMAR_CHAMPION:
                    if (teamInInstance == HORDE)
                        creature->UpdateEntry(NPC_STORMWIND_CHAMPION, ALLIANCE);
                    break;
                case NPC_SILVERMOON_CHAMPION:
                    if (teamInInstance == HORDE)
                        creature->UpdateEntry(NPC_GNOMEREGAN_CHAMPION, ALLIANCE);
                    break;
                case NPC_THUNDER_CHAMPION:
                    if (teamInInstance == HORDE)
                        creature->UpdateEntry(NPC_EXODAR_CHAMPION, ALLIANCE);
                    break;
                case NPC_TROLL_CHAMPION:
                    if (teamInInstance == HORDE)
                        creature->UpdateEntry(NPC_DARNASSUS_CHAMPION, ALLIANCE);
                    break;
                case NPC_UNDERCITY_CHAMPION:
                    if (teamInInstance == HORDE)
                        creature->UpdateEntry(NPC_IRONFORGE_CHAMPION, ALLIANCE);
                    break;
                // Coliseum Announcer, Just NPC_JAEREN must be spawned.
                case NPC_JAEREN:
                    announcerGUID = creature->GetGUID();
                    if (teamInInstance == ALLIANCE)
                        creature->UpdateEntry(NPC_ARELAS, ALLIANCE);
                    break;
                case NPC_JAEREN_AN:
                    if (teamInInstance == ALLIANCE)
                        creature->UpdateEntry(NPC_ARELAS_AN, ALLIANCE);
                    break;
                case VEHICLE_ARGENT_WARHORSE:
                case VEHICLE_ARGENT_BATTLEWORG:
                    vehicleList.push_back(creature->GetGUID());
                    break;
                case NPC_EADRIC:
                case NPC_PALETRESS:
                    argentChampionGUID = creature->GetGUID();
                    break;
            }
        }

        void OnGameObjectCreate(GameObject* go)
        {
            switch (go->GetEntry())
            {
                case GO_MAIN_GATE:
                    mainGateGUID = go->GetGUID();
                    break;
                case GO_NORTH_PORTCULLIS:
                    portcullisGUID = go->GetGUID();
                    break;
                case GO_CHAMPIONS_LOOT:
                case GO_CHAMPIONS_LOOT_H:
                    championLootGUID = go->GetGUID();
                    break;
            }
        }

        void SetData(uint32 type, uint32 data)
        {
            switch (type)
            {
                case DATA_MOVEMENT_DONE:
                    movementDone = data;
                    if (movementDone == 3)
                    {
                        if (Creature* announcer = instance->GetCreature(announcerGUID))
                            announcer->AI()->SetData(DATA_IN_POSITION, 0);
                    }
                    break;
                case BOSS_GRAND_CHAMPIONS:
                    m_auiEncounter[0] = data;
                    if (data == IN_PROGRESS)
                    {
                        for (std::list<uint64>::const_iterator itr = vehicleList.begin(); itr != vehicleList.end(); ++itr)
                            if (Creature* summon = instance->GetCreature(*itr))
                                summon->RemoveFromWorld();
                    }
                    else if (data == DONE)
                    {
                        ++grandChampionsDeaths;
                        if (grandChampionsDeaths >= 3)
                        {
                            for (uint8 i = 0; i < 3; ++i)
                                if (Creature* GrandChampion = instance->GetCreature(GetData64(DATA_GRAND_CHAMPION_1 + i)))
                                    GrandChampion->AI()->SetData(11, 0);

                            if (Creature* announcer = instance->GetCreature(announcerGUID))
                            {
                                m_auiEncounter[0] = data;
                                announcer->GetMotionMaster()->MovePoint(0, 748.309f, 619.487f, 411.171f);
                                announcer->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                                announcer->SummonGameObject(instance->IsHeroic() ? GO_CHAMPIONS_LOOT_H : GO_CHAMPIONS_LOOT, 746.59f, 618.49f, 411.09f, 1.42f, 0, 0, 0, 0, 90000000);
                                BindPlayersToInstance(announcer);
                            }
                        }
                    }
                    break;
                case DATA_ARGENT_SOLDIER_DEFEATED:
                    argentSoldierDeaths = data;
                    if (argentSoldierDeaths == 9)
                    {
                        if (Creature* boss = instance->GetCreature(argentChampionGUID))
                        {
                            boss->GetMotionMaster()->MovePoint(0, 746.88f, 618.74f, 411.06f);
                            boss->RemoveFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_NON_ATTACKABLE);
                            boss->SetReactState(REACT_AGGRESSIVE);
                            boss->setFaction(16);
                        }
                    }
                    break;
                case BOSS_ARGENT_CHALLENGE_E:
                    m_auiEncounter[1] = data;
                    if (data == IN_PROGRESS)
                    {
                        for (std::list<uint64>::const_iterator itr = vehicleList.begin(); itr != vehicleList.end(); ++itr)
                            if (Creature* summon = instance->GetCreature(*itr))
                                summon->RemoveFromWorld();
                    }
                    else if (data == DONE)
                    {
                        if (Creature* announcer = instance->GetCreature(announcerGUID))
                        {
                            announcer->GetMotionMaster()->MovePoint(0, 748.309f, 619.487f, 411.171f);
                            announcer->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                            announcer->SummonGameObject(instance->IsHeroic() ? GO_EADRIC_LOOT_H : GO_EADRIC_LOOT, 746.59f, 618.49f, 411.09f, 1.42f, 0, 0, 0, 0, 90000000);
                            BindPlayersToInstance(announcer);
                        }
                    }
                    break;
                case BOSS_ARGENT_CHALLENGE_P:
                    m_auiEncounter[2] = data;
                    if (data == IN_PROGRESS)
                    {
                        for (std::list<uint64>::const_iterator itr = vehicleList.begin(); itr != vehicleList.end(); ++itr)
                            if (Creature* summon = instance->GetCreature(*itr))
                                summon->RemoveFromWorld();
                    }
                    else if (data == DONE)
                    {
                        if (Creature* announcer = instance->GetCreature(announcerGUID))
                        {
                            announcer->GetMotionMaster()->MovePoint(0, 748.309f, 619.487f, 411.171f);
                            announcer->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                            announcer->SummonGameObject(instance->IsHeroic() ? GO_PALETRESS_LOOT_H : GO_PALETRESS_LOOT, 746.59f, 618.49f, 411.09f, 1.42f, 0, 0, 0, 0, 90000000);
                            BindPlayersToInstance(announcer);
                        }
                    }
                    break;
                case BOSS_BLACK_KNIGHT:
                    m_auiEncounter[3] = data;
                    break;
            }

            if (data == DONE)
                SaveToDB();
        }

        uint32 GetData(uint32 data)
        {
            switch (data)
            {
                case BOSS_GRAND_CHAMPIONS:  return m_auiEncounter[0];
                case BOSS_ARGENT_CHALLENGE_E: return m_auiEncounter[1];
                case BOSS_ARGENT_CHALLENGE_P: return m_auiEncounter[2];
                case BOSS_BLACK_KNIGHT: return m_auiEncounter[3];

                case DATA_MOVEMENT_DONE: return movementDone;
                case DATA_ARGENT_SOLDIER_DEFEATED: return argentSoldierDeaths;
                case DATA_TEAM_IN_INSTANCE: return teamInInstance;
            }

            return 0;
        }

        uint64 GetData64(uint32 data)
        {
            switch (data)
            {
                case DATA_ANNOUNCER: return announcerGUID;
                case DATA_MAIN_GATE: return mainGateGUID;
                case DATA_PORTCULLIS: return portcullisGUID;

                case DATA_GRAND_CHAMPION_1: return grandChampion1GUID;
                case DATA_GRAND_CHAMPION_2: return grandChampion2GUID;
                case DATA_GRAND_CHAMPION_3: return grandChampion3GUID;
            }

            return 0;
        }

        void SetData64(uint32 type, uint64 data)
        {
            switch (type)
            {
                case DATA_GRAND_CHAMPION_1:
                    grandChampion1GUID = data;
                    break;
                case DATA_GRAND_CHAMPION_2:
                    grandChampion2GUID = data;
                    break;
                case DATA_GRAND_CHAMPION_3:
                    grandChampion3GUID = data;
                    break;
            }
        }

        std::string GetSaveData()
        {
            OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;

            saveStream << "T C " << m_auiEncounter[0]
                << ' ' << m_auiEncounter[1]
                << ' ' << m_auiEncounter[2]
                << ' ' << m_auiEncounter[3]
                << ' ' << grandChampionsDeaths
                << ' ' << movementDone;

            str_data = saveStream.str();

            OUT_SAVE_INST_DATA_COMPLETE;
            return str_data;
        }

        void Load(const char* in)
        {
            if (!in)
            {
                OUT_LOAD_INST_DATA_FAIL;
                return;
            }

            OUT_LOAD_INST_DATA(in);

            char dataHead1, dataHead2;
            uint16 data0, data1, data2, data3, data4, data5;

            std::istringstream loadStream(in);
            loadStream >> dataHead1 >> dataHead2 >> data0 >> data1 >> data2 >> data3 >> data4 >> data5;

            if (dataHead1 == 'T' && dataHead2 == 'C')
            {
                m_auiEncounter[0] = data0;
                m_auiEncounter[1] = data1;
                m_auiEncounter[2] = data2;
                m_auiEncounter[3] = data3;

                for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                    if (m_auiEncounter[i] == IN_PROGRESS)
                        m_auiEncounter[i] = NOT_STARTED;

                grandChampionsDeaths = data4;
                movementDone = data5;
            }
            else
                OUT_LOAD_INST_DATA_FAIL;

            OUT_LOAD_INST_DATA_COMPLETE;
        }
    };
};

void AddSC_instance_trial_of_the_champion()
{
    new instance_trial_of_the_champion();
}
