/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

/* ScriptData
SDName: Dragonblight
SD%Complete: 100
SDComment:
SDCategory: Dragonblight
EndScriptData */

/* ContentData
npc_alexstrasza_wr_gate
EndContentData */

#include "ScriptPCH.h"
#include "ScriptedEscortAI.h"

enum eEnums
{
    QUEST_RETURN_TO_AG_A    = 12499,
    QUEST_RETURN_TO_AG_H    = 12500,
    MOVIE_ID_GATES          = 14
};

#define GOSSIP_ITEM_WHAT_HAPPENED   "Alexstrasza, can you show me what happened here?"

class npc_alexstrasza_wr_gate : public CreatureScript
{
public:
    npc_alexstrasza_wr_gate() : CreatureScript("npc_alexstrasza_wr_gate") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if (creature->isQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        if (player->GetQuestRewardStatus(QUEST_RETURN_TO_AG_A) || player->GetQuestRewardStatus(QUEST_RETURN_TO_AG_H))
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_WHAT_HAPPENED, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

        player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* /*creature*/, uint32 /*uiSender*/, uint32 uiAction)
    {
        player->PlayerTalkClass->ClearMenus();
        if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
        {
            player->CLOSE_GOSSIP_MENU();
            player->SendMovieStart(MOVIE_ID_GATES);
        }

        return true;
    }
};

/*######
## No Mercy for the Captured
## http://www.wowhead.com/quest=12245
## UPDATE `creature_template`SET `ScriptName` = 'npc_no_mercy_for_the_captured' WHERE `entry` IN (27376,27378,27379,27381);
######*/

enum NoMercyForTheCaptured
{
    NPC_DEATHGUARD_SCHNEIDER = 27376,
    NPC_SENIOR_SCRIVENER_BARRIGA = 27378,
    NPC_ENGINEER_BURKE = 27379,
    NPC_CHANCELLOR_AMAI = 27381,
    QUEST_NO_MERCY_FOR_THE_CAPTURED = 12245,
};

#define GOSSIP_PRISIONEROS "What do you mean my time has come? I'll kill you where you stand!"

class npc_no_mercy_for_the_captured : public CreatureScript
{
public:
    npc_no_mercy_for_the_captured() : CreatureScript("npc_no_mercy_for_the_captured") { }

    struct npc_no_mercy_for_the_capturedAI : public ScriptedAI
    {
        npc_no_mercy_for_the_capturedAI(Creature* creature) : ScriptedAI(creature)
        {
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_UNK_15);
        }

        void Reset()
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_UNK_15);
            me->RestoreFaction();
        }
    };

    CreatureAI *GetAI(Creature* creature) const
    {
        return new npc_no_mercy_for_the_capturedAI(creature);
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if (player->GetQuestStatus(QUEST_NO_MERCY_FOR_THE_CAPTURED) == QUEST_STATUS_INCOMPLETE)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_PRISIONEROS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());

        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*uiSender*/, uint32 uiAction)
    {
        player->PlayerTalkClass->ClearMenus();
        if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
        {
            player->CLOSE_GOSSIP_MENU();
            creature->setFaction(14);
            creature->AI()->AttackStart(player);
        }

        return true;
    }
};

/*######
## npc_devout_bodyguard
######*/

float DevoutBodyguardWay[1][21][3] =
{
    {
        {2790.324f, -490.081f, 119.616f},
        {2792.576f, -483.441f, 119.616f},
        {2801.183f, -478.036f, 119.616f},
        {2814.852f, -465.822f, 119.613f},
        {2818.68f, -469.324f, 119.614f},
        {2821.951f, -471.869f, 123.61f},
        {2828.52f, -472.507f, 129.532f},
        {2834.976f, -469.976f, 135.162f},
        {2840.456f, -460.922f, 135.362f},
        {2838.088f, -453.457f, 135.362f},
        {2831.782f, -448.045f, 135.362f},
        {2823.282f, -447.724f, 135.362f},
        {2816.99f, -452.967f, 135.362f},
        {2814.536f, -460.418f, 135.362f},
        {2816.803f, -468.241f, 135.361f},
        {2818.536f, -469.547f, 135.355f},
        {2823.803f, -472.822f, 141.208f},
        {2831.668f, -471.995f, 148.345f},
        {2834.233f, -470.502f, 150.673f},
        {2837.822f, -467.322f, 150.836f},
        {2832.710f, -462.756f, 150.835f}
    }
};

class npc_devout_bodyguard : public CreatureScript
{
public:
    npc_devout_bodyguard() : CreatureScript("npc_devout_bodyguard") { }

    CreatureAI* GetAI(Creature *_Creature) const
    {
        return new npc_devout_bodyguardAI(_Creature);
    }

    struct npc_devout_bodyguardAI: public ScriptedAI
    {
            npc_devout_bodyguardAI(Creature *c) : ScriptedAI(c) {}

            uint32 waypoint;
            uint32 WaitTimer;
            bool isMoving;

            void Reset()
            {
                me->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);
                isMoving = false;
                waypoint = 0;
                WaitTimer = 1;
            }

            void StartMove()
            {
                me->AddUnitMovementFlag(MOVEMENTFLAG_WALKING);
                WaitTimer = 1;
                isMoving = true;
            }

            void MovementInform(uint32, uint32)
            {
                if (waypoint == 20)
                {
                    isMoving = false;
                    me->DisappearAndDie();
                }
                WaitTimer = 1;
            }

            void UpdateAI(const uint32 diff)
            {

                if (WaitTimer == 1)
                {
                    me->GetMotionMaster()->Clear();
                    if (isMoving)
                        me->GetMotionMaster()->MovePoint(0, DevoutBodyguardWay[0][waypoint][0], DevoutBodyguardWay[0][waypoint][1], DevoutBodyguardWay[0][waypoint][2]);
                    ++waypoint;
                    WaitTimer = 0;
                }

                if (!UpdateVictim())
                    return;

                DoMeleeAttackIfReady();
            }

            void EnterCombat(Unit* /*who*/) {}
    };

};

/*######
## npc_high_abbot_landgren
######*/

enum eHighAbbotLandgren
{
    QUEST_A_FALL_FROM_GRACE                     = 12274,

    SPELL_SCARLET_RAVEN_PRIEST_IMAGE_FEMALE     = 48761,
    SPELL_SCARLET_RAVEN_PRIEST_IMAGE_MALE       = 48763,

    TEXT_1                                      = 12848,
    TEXT_2                                      = 12849,

    NPC_HIGH_ABBOT_LANDGREN_ENTRY               = 27245,
    NPC_HIGH_ABBOT_LANDGREN_ESCORTEE_ENTRY      = 27439,
    NPC_DEVOUT_BODYGUARD                        = 27247
};

float HighAbbotLandgrenSpawnPos[1][1][4] = {{ 2828.981934f, -439.187012f, 119.624001f, 1.647600f }};
float HighAbbotLandgrenJumpPos[1][1][3] = {{ 2719.657f, -554.755f, 23.988f }};

static const char* HighAbbotText[] =
{
        "I am ready, your grace. <kiss the ring>",
        "I know a place nearby where we could speak in private, my child. Follow me.",
        "Did you think that i could not see through your flimsy disguise, $N.",
        "There is much that you do not understand, $R. The Master sees all.",
        "He told me that you would come for me. I won't die by your hand, through. I have seen what you have done to my compatriots.",
        "No i will leave this world in a manner of my own choosing. And i will return, the grand admiral's permitting.",
        "AAAEEEEIIIiiiiiiiiiiiiiiiiiiiiiiiiiiii........................................"
};

class npc_high_abbot_landgren : public CreatureScript
{
public:
    npc_high_abbot_landgren() : CreatureScript("npc_high_abbot_landgren") { }

    struct npc_high_abbot_landgrenAI : public npc_escortAI
    {
        npc_high_abbot_landgrenAI(Creature* pCreature) : npc_escortAI(pCreature) { }

        bool BodyGuardStart;
        bool CheckPlayerDist;
        bool EventStarted;
        bool EndSequence;
        uint8 GuardCount;
        uint8 EndSayCount;
        uint32 AuraCheckTimer;
        uint32 BodyGuardMoveTimer;
        uint32 EndSequenceTimer;
        float Range;
        uint64 PlayerGUID;

        void WaypointReached(uint32 i)
        {
            if (me->GetEntry() != NPC_HIGH_ABBOT_LANDGREN_ESCORTEE_ENTRY)
                return;

            if(PlayerGUID)
                if(Player* pPlayer = Unit::GetPlayer(*me,PlayerGUID))
                {
                    switch (i)
                    {
                        case 0:
                            me->MonsterSay(HighAbbotText[1], LANG_UNIVERSAL, 0);
                            break;
                        case 12:
                            EndSequence = true;
                            break;
                    }
                }
        }

        void Reset()
        {
            BodyGuardStart = true;
            CheckPlayerDist = false;
            EventStarted = false;
            EndSequence = false;
            GuardCount = 0;
            EndSayCount = 2;
            AuraCheckTimer = 300;
            BodyGuardMoveTimer = 2000;
            EndSequenceTimer = 500;
            Range = 3.0f;
            PlayerGUID = NULL;
        }

        void StartEvent()
        {
            CheckPlayerDist = true;
        }

        void StartMove()
        {
            if ( Creature* tmp = me->SummonCreature(NPC_HIGH_ABBOT_LANDGREN_ESCORTEE_ENTRY, HighAbbotLandgrenSpawnPos[0][0][0], HighAbbotLandgrenSpawnPos[0][0][1], HighAbbotLandgrenSpawnPos[0][0][2], HighAbbotLandgrenSpawnPos[0][0][3]))
            {
                CAST_AI(npc_high_abbot_landgren::npc_high_abbot_landgrenAI, tmp->AI())->PlayerGUID = PlayerGUID;
                me->DespawnOrUnsummon();
            }
        }

        void UpdateAI(const uint32 diff)
        {
            npc_escortAI::UpdateAI(diff);

            if (PlayerGUID)
            {
                Player* AuraPlayer = Unit::GetPlayer(*me,PlayerGUID);

                if (!AuraPlayer)
                {
                    me->DisappearAndDie();
                    return;
                }
                else
                {
                    if (AuraPlayer->isDead())
                    {
                        AuraPlayer->FailQuest(QUEST_A_FALL_FROM_GRACE);
                        me->DisappearAndDie();
                        return;
                    }

                    if (EndSequence)
                    {
                        if (EndSequenceTimer <= diff)
                        {
                            EndSequenceTimer = 12000;
                            if (EndSayCount == 6)
                            {
                                me->GetMotionMaster()->MoveJump(HighAbbotLandgrenJumpPos[0][0][0], HighAbbotLandgrenJumpPos[0][0][1], HighAbbotLandgrenJumpPos[0][0][2], 0.5f, 8.0f);
                                EndSequenceTimer = 2000;
                            }
                            if (EndSayCount == 7)
                            {
                                me->MonsterSay(HighAbbotText[6], LANG_UNIVERSAL, 0);
                                EndSequenceTimer = 2000;
                            }
                            if (EndSayCount == 8)
                            {
                                AuraPlayer->KilledMonsterCredit(27444, me->GetGUID());
                                if (AuraPlayer->HasAura(SPELL_SCARLET_RAVEN_PRIEST_IMAGE_FEMALE))
                                    AuraPlayer->RemoveAura(SPELL_SCARLET_RAVEN_PRIEST_IMAGE_FEMALE);
                                if (AuraPlayer->HasAura(SPELL_SCARLET_RAVEN_PRIEST_IMAGE_MALE))
                                    AuraPlayer->RemoveAura(SPELL_SCARLET_RAVEN_PRIEST_IMAGE_MALE);
                                EndSequence = false;
                            }

                            if (EndSayCount < 6)
                                me->MonsterSay(HighAbbotText[EndSayCount], LANG_UNIVERSAL, PlayerGUID);

                            EndSayCount++;
                        } else
                            EndSequenceTimer -= diff;
                    }

                    if (!EventStarted && me->GetEntry() == NPC_HIGH_ABBOT_LANDGREN_ESCORTEE_ENTRY)
                    {
                        Start(false, false, PlayerGUID, 0, false);
                        EventStarted = true;
                    }

                    if (CheckPlayerDist)
                        if (AuraPlayer->GetDistance(2827.796f, -420.191f, 118.196f) < 4)
                            StartMove();

                    if (AuraCheckTimer <= diff) {
                        if (AuraPlayer && AuraPlayer->GetQuestStatus(QUEST_A_FALL_FROM_GRACE) == QUEST_STATUS_INCOMPLETE && AuraPlayer->getQuestStatusMap()[QUEST_A_FALL_FROM_GRACE].m_creatureOrGOcount[0] == 1 && !AuraPlayer->HasAura(SPELL_SCARLET_RAVEN_PRIEST_IMAGE_MALE) && !AuraPlayer->HasAura(SPELL_SCARLET_RAVEN_PRIEST_IMAGE_FEMALE)) {
                            switch(AuraPlayer->getGender())
                            {
                            case GENDER_FEMALE:
                                AuraPlayer->CastSpell(AuraPlayer, SPELL_SCARLET_RAVEN_PRIEST_IMAGE_FEMALE, false);
                                break;
                            case GENDER_MALE:
                                AuraPlayer->CastSpell(AuraPlayer, SPELL_SCARLET_RAVEN_PRIEST_IMAGE_MALE, false);
                                break;
                            }
                        }
                        AuraCheckTimer = 300;
                    } else
                        AuraCheckTimer -= diff;

                    if (me->GetEntry() != NPC_HIGH_ABBOT_LANDGREN_ESCORTEE_ENTRY)
                    {
                        if (BodyGuardStart && AuraPlayer->GetQuestStatus(QUEST_A_FALL_FROM_GRACE) == QUEST_STATUS_INCOMPLETE && AuraPlayer->getQuestStatusMap()[QUEST_A_FALL_FROM_GRACE].m_creatureOrGOcount[0] == 1) {
                            if (Creature* tmp = me->FindNearestCreature(NPC_DEVOUT_BODYGUARD, Range, true))
                            {
                                if (BodyGuardMoveTimer <= diff)
                                {
                                    CAST_AI(npc_devout_bodyguard::npc_devout_bodyguardAI, tmp->AI())->StartMove();
                                    BodyGuardMoveTimer = 6000;
                                    Range = 4.0f;
                                    if (GuardCount == 1)
                                        BodyGuardStart = false;
                                    GuardCount++;
                                } else
                                    BodyGuardMoveTimer -= diff;
                            }
                        }
                    }
                }
            }
        }
    };

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction)
    {
        switch (uiAction)
        {
            case GOSSIP_ACTION_INFO_DEF + 1:
                pPlayer->SEND_GOSSIP_MENU(TEXT_2, pCreature->GetGUID());
                CAST_AI(npc_high_abbot_landgren::npc_high_abbot_landgrenAI, pCreature->AI())->StartEvent();
                break;
        }
        return true;
    }

    bool OnGossipHello(Player *player, Creature *pCreature)
    {
        if (pCreature->isQuestGiver())
            player->PrepareQuestMenu(pCreature->GetGUID());

        if (pCreature->GetEntry() == NPC_HIGH_ABBOT_LANDGREN_ENTRY && player->GetQuestStatus(QUEST_A_FALL_FROM_GRACE) == QUEST_STATUS_INCOMPLETE && player->getQuestStatusMap()[QUEST_A_FALL_FROM_GRACE].m_creatureOrGOcount[0] == 1 && (player->HasAura(SPELL_SCARLET_RAVEN_PRIEST_IMAGE_MALE) || player->HasAura(SPELL_SCARLET_RAVEN_PRIEST_IMAGE_FEMALE)))
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, HighAbbotText[0], GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            player->SEND_GOSSIP_MENU(TEXT_1, pCreature->GetGUID());
            return true;
        }

        player->SEND_GOSSIP_MENU(player->GetGossipTextId(pCreature), pCreature->GetGUID());
        return true;
    }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_high_abbot_landgrenAI(pCreature);
    }

};

void AddSC_dragonblight()
{
    new npc_alexstrasza_wr_gate;
	new npc_no_mercy_for_the_captured;
	new npc_devout_bodyguard;
	new npc_high_abbot_landgren;
}
