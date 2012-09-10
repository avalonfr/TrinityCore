/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: northrend_beasts
SD%Complete: 90%
SDComment: based on /dev/rsa
SDCategory:
EndScriptData */

// Known bugs:
// Gormok - Not implemented as a vehicle
//        - Snobold Firebomb
//        - Snobolled (creature at back)
// Snakes - miss the 1-hitkill from emerging
//        - visual changes between mobile and stationary models seems not to work sometimes

#include "ScriptPCH.h"
#include "trial_of_the_crusader.h"

enum Yells
{
    //Gormok
    SAY_SNOBOLLED        = -1649000,
    //Acidmaw & Dreadscale
    SAY_SUBMERGE         = -1649010,
    SAY_EMERGE           = -1649011,
    SAY_BERSERK          = -1649012,
    //Icehowl
    SAY_TRAMPLE_STARE    = -1649020,
    SAY_TRAMPLE_FAIL     = -1649021,
    SAY_TRAMPLE_START    = -1649022,
};

enum Equipment
{
    EQUIP_MAIN           = 50760,
    EQUIP_OFFHAND        = 48040,
    EQUIP_RANGED         = 47267,
    EQUIP_DONE           = EQUIP_NO_CHANGE,
};

enum Model
{
    MODEL_ACIDMAW_STATIONARY     = 29815,
    MODEL_ACIDMAW_MOBILE         = 29816,
    MODEL_DREADSCALE_STATIONARY  = 26935,
    MODEL_DREADSCALE_MOBILE      = 24564,
};

enum Summons
{
    NPC_SNOBOLD_VASSAL   = 34800,
    NPC_FIRE_BOMB_TRIGGER = 34854,
    NPC_SLIME_POOL       = 35176,
};

enum BossSpells
{
    //Gormok
    SPELL_IMPALE_0          = 66331,
    SPELL_IMPALE_1          = 67477,
    SPELL_IMPALE_2          = 67478,
    SPELL_IMPALE_3          = 67479,
    SPELL_STAGGERING_STOMP  = 67648,
    SPELL_RISING_ANGER      = 66636,
    //Snobold
	SPELL_SNOBOLLED         = 66406,
    SPELL_BATTER            = 66408,
    SPELL_FIRE_BOMB         = 66313,
    SPELL_FIRE_BOMB_DOT     = 66318,
    SPELL_HEAD_CRACK        = 66407,

    //Acidmaw & Dreadscale
    SPELL_ACID_SPIT         = 66880,
    SPELL_PARALYTIC_SPRAY_0 = 66901,
    SPELL_PARALYTIC_SPRAY_1 = 67615,
    SPELL_PARALYTIC_SPRAY_2 = 67616,
    SPELL_PARALYTIC_SPRAY_3 = 67617,
    SPELL_PARALYTIC_TOXIN_0 = 66823,
    SPELL_PARALYTIC_TOXIN_1 = 67618,
    SPELL_PARALYTIC_TOXIN_2 = 67619,
    SPELL_PARALYTIC_TOXIN_3 = 67620,
    SPELL_ACID_SPEW         = 66818,
    SPELL_PARALYTIC_BITE    = 66824,
    SPELL_SWEEP_0           = 66794,
    SUMMON_SLIME_POOL       = 66883,
    SPELL_FIRE_SPIT         = 66796,
    SPELL_MOLTEN_SPEW       = 66821,
    SPELL_BURNING_BITE      = 66879,
    SPELL_BURNING_SPRAY_0   = 66902,
    SPELL_BURNING_SPRAY_1   = 67627,
    SPELL_BURNING_SPRAY_2   = 67628,
    SPELL_BURNING_SPRAY_3   = 67629,
    SPELL_BURNING_BILE_MAIN = 66869,
    SPELL_BURNING_BILE_0    = 66870,
    SPELL_BURNING_BILE_1    = 67622,
    SPELL_BURNING_BILE_2    = 67621,
    SPELL_BURNING_BILE_3    = 67623,
    SPELL_SWEEP_1           = 67646,
    SPELL_EMERGE_0          = 66947,
    SPELL_SUBMERGE_0        = 53421,
    SPELL_ENRAGE            = 68335,
    SPELL_SLIME_POOL_EFFECT = 66882, //In 60s it diameter grows from 10y to 40y (r=r+0.25 per second)

    //Icehowl
    SPELL_FEROCIOUS_BUTT    = 66770,
    SPELL_MASSIVE_CRASH     = 66683,
    SPELL_WHIRL             = 67345,
    SPELL_ARCTIC_BREATH     = 66689,
    SPELL_TRAMPLE           = 66734,
    SPELL_FROTHING_RAGE     = 66759,
    SPELL_STAGGERED_DAZE    = 66758,
    SPELL_ICEHOWL_ENRAGE    = 26662
};

void HandleAuraOnRaidNorthrendBeasts(Unit* caller, uint32 spellId, bool remove, bool add, uint32 addAmount)
{
    if (spellId <= 0 || (add && addAmount <= 0) || !caller)
        return;

    Map* map = caller->GetMap();
    if (map && map->IsDungeon())
    {
        Map::PlayerList const &PlayerList = map->GetPlayers();

        if (PlayerList.isEmpty())
            return;

        for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
        {
            if (i->getSource())
            {
                if (i->getSource()->isAlive())
                {
                    if (remove)
                        if (i->getSource()->HasAura(spellId))
                            i->getSource()->RemoveAurasDueToSpell(spellId);

                    if (add)
                    {
                        if (addAmount > 0)
                        {
                            if (Aura* aur = caller->AddAura(spellId, i->getSource()))
                            {
                                aur->SetStackAmount(addAmount);
                                aur->SetCharges(addAmount);
                            }
                        }
                    }
                }
            }
       }
    }
}

class boss_gormok : public CreatureScript
{
public:
    boss_gormok() : CreatureScript("boss_gormok") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_gormokAI(creature);
    }

    struct boss_gormokAI : public ScriptedAI
    {
        boss_gormokAI(Creature* creature) : ScriptedAI(creature)
        {
            m_instance = (InstanceScript*)creature->GetInstanceScript();
        }

        InstanceScript* m_instance;

        uint32 m_uiImpaleTimer;
        uint32 m_uiStaggeringStompTimer;
        uint32 m_uiSummonTimer;
        uint32 m_uiSummonCount;
        uint32 m_summonNextBossHeroicTimer;

        bool nextBossSpawnInitiated;

        void Reset()
        {
            m_uiImpaleTimer = urand(8*IN_MILLISECONDS, 10*IN_MILLISECONDS);
            m_uiStaggeringStompTimer = 10*IN_MILLISECONDS;
            m_uiSummonTimer = urand(15*IN_MILLISECONDS, 20*IN_MILLISECONDS);;
            m_summonNextBossHeroicTimer = 150000;
            nextBossSpawnInitiated = false;

            if (GetDifficulty() == RAID_DIFFICULTY_25MAN_NORMAL ||
                GetDifficulty() == RAID_DIFFICULTY_25MAN_HEROIC)
                m_uiSummonCount = 5;
            else
                m_uiSummonCount = 4;

            HandleAuraOnRaidNorthrendBeasts(me, RAID_MODE(SPELL_IMPALE_0, SPELL_IMPALE_1, SPELL_IMPALE_2, SPELL_IMPALE_3), true, false, 0);
        }

        void JustDied(Unit* /*killer*/)
        {
            HandleAuraOnRaidNorthrendBeasts(me, RAID_MODE(SPELL_IMPALE_0, SPELL_IMPALE_1, SPELL_IMPALE_2, SPELL_IMPALE_3), true, false, 0);

            if (m_instance)
                m_instance->SetData(TYPE_NORTHREND_BEASTS, GORMOK_DONE);
        }

        void JustReachedHome()
        {
            if (m_instance)
            {
                if (m_instance->GetData(TYPE_NORTHREND_BEASTS) != FAIL)
                {
                    m_instance->SetData(TYPE_NORTHREND_BEASTS, FAIL);
                    m_instance->SetData(SPAWNED_NEXT_BOSS_1, 0);
                }
            }
            me->DespawnOrUnsummon();
        }

        void MovementInform(uint32 type, uint32 pointId)
        {
            if (type != POINT_MOTION_TYPE)
                return;

            if (pointId == 1)
            {
                if (Unit* target = me->SelectNearestTarget(150))
                    AttackStart(target);

                DoZoneInCombat();
            }
        }

        void EnterCombat(Unit* /*who*/)
        {
            DoZoneInCombat();
            me->SetHomePosition(ToCCommonLoc[5].GetPositionX(), ToCCommonLoc[5].GetPositionY(), ToCCommonLoc[5].GetPositionZ(), 0);
            m_instance->SetData(TYPE_NORTHREND_BEASTS, GORMOK_IN_PROGRESS);
        }

        void JustSummoned(Creature* summon)
        {
            switch (summon->GetEntry())
            {
                case NPC_SNOBOLD_VASSAL:

                    DoZoneInCombat(summon);

                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1))
                    {
                        if (summon->AI())
                            summon->AI()->AttackStart(target);
                    }

                    if (me->HasAura(SPELL_RISING_ANGER))
                    {
                        if (Aura* angerAura = me->GetAura(SPELL_RISING_ANGER))
                        {
                            angerAura->SetStackAmount(angerAura->GetStackAmount() + 1);
                        }
                    }
                    else
                        me->AddAura(SPELL_RISING_ANGER, me);

                    --m_uiSummonCount;
                    break;
            }
        }

        void SummonedCreatureDespawn(Creature* summon)
        {
            switch (summon->GetEntry())
            {
                case NPC_SNOBOLD_VASSAL:
                    ++m_uiSummonCount;
                    break;
            }
        }

        void UpdateAI(uint32 const diff)
        {
            if (!UpdateVictim())
                return;

            if (IsHeroic())
            {
                if (!nextBossSpawnInitiated)
                {
                    if (m_summonNextBossHeroicTimer <= diff)
                    {
                        if (m_instance)
                        {
                            nextBossSpawnInitiated = true;
                            m_instance->SetData(TYPE_EVENT, 200);
                            m_instance->SetData(SPAWNED_NEXT_BOSS_1, 1);
                            m_instance->SetData(TYPE_NORTHREND_BEASTS, IN_PROGRESS);
                            m_instance->SetData(TYPE_BEASTS, IN_PROGRESS);
                        }
                    } else m_summonNextBossHeroicTimer -= diff;
                }
            }

            if (m_uiImpaleTimer <= diff)
            {
                if (!me->HasAuraType(SPELL_AURA_MOD_DISARM))
                    DoCastVictim(SPELL_IMPALE_0);

                m_uiImpaleTimer = urand(8*IN_MILLISECONDS, 10*IN_MILLISECONDS);
            } else m_uiImpaleTimer -= diff;

            if (m_uiStaggeringStompTimer <= diff)
            {
                DoCastVictim(SPELL_STAGGERING_STOMP);
                m_uiStaggeringStompTimer = urand(9*IN_MILLISECONDS, 13*IN_MILLISECONDS);
            } else m_uiStaggeringStompTimer -= diff;

            if (m_uiSummonTimer <= diff)
            {
                if (m_uiSummonCount > 0)
                {
                    me->SummonCreature(NPC_SNOBOLD_VASSAL, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0, TEMPSUMMON_CORPSE_DESPAWN);
                    DoScriptText(SAY_SNOBOLLED, me);
                }
                m_uiSummonTimer = urand(15*IN_MILLISECONDS, 20*IN_MILLISECONDS);
            } else m_uiSummonTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };

};


class mob_snobold_vassal : public CreatureScript
{
public:
    mob_snobold_vassal() : CreatureScript("mob_snobold_vassal") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_snobold_vassalAI(creature);
    }

    struct mob_snobold_vassalAI : public ScriptedAI
    {
        mob_snobold_vassalAI(Creature* creature) : ScriptedAI(creature), Summons(me)
        {
            m_instance = (InstanceScript*)creature->GetInstanceScript();
            if (m_instance)
                m_instance->SetData(DATA_SNOBOLD_COUNT, INCREASE);
        }

        InstanceScript* m_instance;
        uint32 m_uiFireBombTimer;
        uint32 m_uiBatterTimer;
        uint32 m_uiHeadCrackTimer;
        uint64 m_uiBossGUID;
        uint64 m_uiTargetGUID;
        bool fireBombHitTarget;

        SummonList Summons;

        void Reset()
        {
            m_uiFireBombTimer = 15000;
            m_uiBatterTimer = 5000;
            m_uiHeadCrackTimer = 25000;

            fireBombHitTarget = false;

            m_uiTargetGUID = 0;

            if (m_instance)
                m_uiBossGUID = m_instance->GetData64(NPC_GORMOK);
            //Workaround for Snobold
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);

            Summons.DespawnAll();
        }

        void SpellHitTarget(Unit* target, const SpellInfo* spell)
        {
            if (!target)
                return;

            if (!spell)
                return;

            if (spell->Id == SPELL_FIRE_BOMB && !fireBombHitTarget)
            {
                fireBombHitTarget = true;
                if (Creature* trigger = me->SummonCreature(NPC_FIRE_BOMB_TRIGGER, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 60000))
                {
                    trigger->CastSpell(trigger, SPELL_FIRE_BOMB_DOT, true);
                }
            }
        }

        void JustSummoned(Creature* summon)
        {
            Summons.Summon(summon);
        }

        void SummonedCreatureDespawn(Creature* summon)
        {
            Summons.Despawn(summon);
        }

        void AttackStart(Unit* who)
        {
            if (!who)
                return;

            UnitAI::AttackStart(who);
            m_uiTargetGUID = who->GetGUID();
            me->AddThreat(who, 5000000.0f);
			DoCast(who, SPELL_SNOBOLLED);
        }

        void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
        {
            if (pDoneBy->GetGUID() == m_uiTargetGUID)
                uiDamage = 0;
        }

        void JustReachedHome()
        {
            me->Kill(me);
        }

        void JustDied(Unit* killer)
        {
            Summons.DespawnAll();
            if (Unit* target = Unit::GetPlayer(*me, m_uiTargetGUID))
                if (target->isAlive())
                    target->RemoveAurasDueToSpell(SPELL_SNOBOLLED);
            if (m_instance)
                m_instance->SetData(DATA_SNOBOLD_COUNT, DECREASE);
        }

        void UpdateAI(uint32 const diff)
        {
            if (!UpdateVictim())
                return;

            if (Unit* target = Unit::GetUnit(*me, m_uiTargetGUID))
                if (!target->isAlive() || me->getVictim() != target || target->GetTypeId() != TYPEID_PLAYER)
                    if (Unit* pNewTarget = SelectTarget(SELECT_TARGET_RANDOM, 0))
                        AttackStart(pNewTarget);

            if (m_uiFireBombTimer < diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                {
                    fireBombHitTarget = false;
                    DoCast(target, SPELL_FIRE_BOMB);
                }
                m_uiFireBombTimer = 20000;
            }
            else m_uiFireBombTimer -= diff;

            if (m_uiBatterTimer < diff)
            {
                if (Unit* target = Unit::GetUnit(*me, m_uiTargetGUID))
                    DoCast(target, SPELL_BATTER);
                m_uiBatterTimer = 10000;
            }
            else m_uiBatterTimer -= diff;

            if (m_uiHeadCrackTimer < diff)
            {
                if (Unit* target = Unit::GetUnit(*me, m_uiTargetGUID))
                    me->AddAura(SPELL_HEAD_CRACK, target);
                m_uiHeadCrackTimer = 35000;
            }
            else m_uiHeadCrackTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };

};


struct boss_jormungarAI : public ScriptedAI
{
    boss_jormungarAI(Creature* creature) : ScriptedAI(creature), Summons(me)
    {
        m_instance = (InstanceScript*)creature->GetInstanceScript();
        m_wasCombat = false;
    }

    InstanceScript* m_instance;

    uint32 m_uiSisterID;

    SummonList Summons;

    uint32 m_uiModelStationary;
    uint32 m_uiModelMobile;

    uint32 m_uiBiteSpell;
    uint32 m_uiSpewSpell;
    uint32 m_uiSpitSpell;
    uint32 m_uiSpraySpell;

    uint32 m_uiBiteTimer;
    uint32 m_uiSpewTimer;
    uint32 m_uiSlimePoolTimer;
    uint32 m_uiSprayTimer;
    uint32 m_uiSweepTimer;
    uint32 m_uiSubmergeTimer;
    uint8  m_uiStage;
    bool   m_bEnraged;
    bool   m_wasCombat;

    uint32 m_summonNextBossHeroicTimer;
    bool nextBossSpawnInitiated;

    void Reset()
    {
        Summons.DespawnAll();

        if (m_wasCombat && me->GetMotionMaster())
        {
            me->GetMotionMaster()->Clear();
            me->RemoveAurasDueToSpell(SPELL_SUBMERGE_0);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_DISABLE_MOVE);
            me->GetMotionMaster()->MoveTargetedHome();
        }

        m_bEnraged = false;
        m_uiBiteTimer = 20000;
        m_uiSpewTimer = urand(9*1000, 14*1000);
        m_uiSlimePoolTimer = urand(6*1000, 10*1000);
        m_uiSprayTimer = 20000;
        m_uiSweepTimer = urand(8*1000, 12*1000);

        m_summonNextBossHeroicTimer = 180000;
        nextBossSpawnInitiated = false;
    }

    void MovementInform(uint32 type, uint32 pointId)
    {
        if (type != POINT_MOTION_TYPE)
            return;

        if (pointId == 1)
        {
            if (Unit* target = me->SelectNearestTarget(150))
                AttackStart(target);

            DoZoneInCombat();
        }
    }

    void SpellHitTarget(Unit* target, const SpellInfo* spell)
    {
        switch (spell->Id)
        {
            case SPELL_PARALYTIC_SPRAY_0:
                me->AddAura(SPELL_PARALYTIC_TOXIN_0, target);
                break;
            case SPELL_PARALYTIC_SPRAY_1:
                me->AddAura(SPELL_PARALYTIC_TOXIN_1, target);
                break;
            case SPELL_PARALYTIC_SPRAY_2:
                me->AddAura(SPELL_PARALYTIC_TOXIN_2, target);
                break;
            case SPELL_PARALYTIC_SPRAY_3:
                me->AddAura(SPELL_PARALYTIC_TOXIN_3, target);
                break;
            case SPELL_BURNING_SPRAY_0:
                target->RemoveAurasDueToSpell(SPELL_PARALYTIC_TOXIN_0);
                break;
            case SPELL_BURNING_SPRAY_1:
                target->RemoveAurasDueToSpell(SPELL_PARALYTIC_TOXIN_1);
                break;
            case SPELL_BURNING_SPRAY_2:
                target->RemoveAurasDueToSpell(SPELL_PARALYTIC_TOXIN_2);
                break;
            case SPELL_BURNING_SPRAY_3:
                target->RemoveAurasDueToSpell(SPELL_PARALYTIC_TOXIN_3);
                break;
        }
    }

    void JustDied(Unit* /*killer*/)
    {
        Summons.DespawnAll();

        if (m_instance)
        {
            if (Creature* pSister = Unit::GetCreature((*me), m_instance->GetData64(m_uiSisterID)))
            {
                if (!pSister->isAlive())
                {
                    m_instance->SetData(TYPE_NORTHREND_BEASTS, SNAKES_DONE);
                    pSister->DespawnOrUnsummon();
                    me->DespawnOrUnsummon();
                }
                else
                {
                    m_instance->SetData(TYPE_NORTHREND_BEASTS, SNAKES_SPECIAL);
                }
            }
            else // should never happen though!
            {
                me->DespawnOrUnsummon();
                m_instance->SetData(TYPE_NORTHREND_BEASTS, SNAKES_DONE);
            }
        }
    }

    void JustReachedHome()
    {
        if (m_instance)
        {
            if (m_instance->GetData(TYPE_NORTHREND_BEASTS) != FAIL)
            {
               m_instance->SetData(TYPE_NORTHREND_BEASTS, FAIL);
            }
		}

        if (m_instance)
        {
            if (Creature* pSister = Unit::GetCreature((*me), m_instance->GetData64(m_uiSisterID)))
                pSister->DespawnOrUnsummon();

            m_instance->SetData(SPAWNED_NEXT_BOSS_2, 0);
        }

        me->DespawnOrUnsummon();
    }

    void KilledUnit(Unit* who)
    {
        if (who->GetTypeId() == TYPEID_PLAYER)
        {
            if (m_instance)
                m_instance->SetData(DATA_TRIBUTE_TO_IMMORTALITY_ELEGIBLE, 0);
        }
    }

    void EnterCombat(Unit* /*who*/)
    {
        m_wasCombat = true;
        DoZoneInCombat();

        if (m_uiSisterID == NPC_ACIDMAW)
        {
            me->SetHomePosition(ToCCommonLoc[6].GetPositionX(), ToCCommonLoc[6].GetPositionY(), ToCCommonLoc[6].GetPositionZ(), 0);
        }
        else
        {
            me->SetHomePosition(ToCCommonLoc[7].GetPositionX(), ToCCommonLoc[7].GetPositionY(), ToCCommonLoc[7].GetPositionZ(), 0);
        }

        if (m_instance)
            if (Creature* pSister = Unit::GetCreature((*me), m_instance->GetData64(m_uiSisterID)))
                DoZoneInCombat(pSister);

        if (m_instance)
            m_instance->SetData(TYPE_NORTHREND_BEASTS, SNAKES_IN_PROGRESS);
    }

    void JustSummoned(Creature* summon)
    {
        Summons.Summon(summon);
    }

    void SummonedCreatureDespawn(Creature* summon)
    {
        Summons.Despawn(summon);
    }

    void UpdateAI(uint32 const diff);
};

class boss_acidmaw : public CreatureScript
{
    public:
    boss_acidmaw() : CreatureScript("boss_acidmaw") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_acidmawAI(creature);
    }

    struct boss_acidmawAI : public boss_jormungarAI
    {
        boss_acidmawAI(Creature* creature) : boss_jormungarAI(creature) { }

        void Reset()
        {
            m_uiBiteSpell = SPELL_PARALYTIC_BITE;
            m_uiSpewSpell = SPELL_ACID_SPEW;
            m_uiSpitSpell = SPELL_ACID_SPIT;
            m_uiSpraySpell = SPELL_PARALYTIC_SPRAY_0;
            m_uiModelStationary = MODEL_ACIDMAW_STATIONARY;
            m_uiModelMobile = MODEL_ACIDMAW_MOBILE;
            m_uiSisterID = NPC_DREADSCALE;

            m_uiSubmergeTimer = 0;

            if (!m_wasCombat)
                DoCast(me, SPELL_SUBMERGE_0);

            HandleAuraOnRaidNorthrendBeasts(me, RAID_MODE(SPELL_PARALYTIC_TOXIN_0, SPELL_PARALYTIC_TOXIN_1, SPELL_PARALYTIC_TOXIN_2, SPELL_PARALYTIC_TOXIN_3), true, false, 0);

            m_uiStage = 2;

            boss_jormungarAI::Reset();
        }

        void JustDied(Unit* killer)
        {
            if (!killer)
                return;

            HandleAuraOnRaidNorthrendBeasts(me, RAID_MODE(SPELL_PARALYTIC_TOXIN_0, SPELL_PARALYTIC_TOXIN_1, SPELL_PARALYTIC_TOXIN_2, SPELL_PARALYTIC_TOXIN_3), true, false, 0);

            boss_jormungarAI::JustDied(killer);
        }
    };
};


class boss_dreadscale : public CreatureScript
{
public:
    boss_dreadscale() : CreatureScript("boss_dreadscale") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_dreadscaleAI(creature);
    }

    struct boss_dreadscaleAI : public boss_jormungarAI
    {
        boss_dreadscaleAI(Creature* creature) : boss_jormungarAI(creature) { }

        void Reset()
        {
            boss_jormungarAI::Reset();
            m_uiBiteSpell = SPELL_BURNING_BITE;
            m_uiSpewSpell = SPELL_MOLTEN_SPEW;
            m_uiSpitSpell = SPELL_FIRE_SPIT;
            m_uiSpraySpell = SPELL_BURNING_SPRAY_0;
            m_uiModelStationary = MODEL_DREADSCALE_STATIONARY;
            m_uiModelMobile = MODEL_DREADSCALE_MOBILE;
            m_uiSisterID = NPC_ACIDMAW;

            HandleAuraOnRaidNorthrendBeasts(me, RAID_MODE(SPELL_BURNING_BILE_0, SPELL_BURNING_BILE_1, SPELL_BURNING_BILE_2, SPELL_BURNING_BILE_3), true, false, 0);
            HandleAuraOnRaidNorthrendBeasts(me, SPELL_BURNING_BILE_MAIN, true, false, 0);

            m_uiSubmergeTimer = 45*1000;
            m_uiStage = 0;
        }

        void JustDied(Unit* killer)
        {
            if (!killer)
                return;

            HandleAuraOnRaidNorthrendBeasts(me, RAID_MODE(SPELL_BURNING_BILE_0, SPELL_BURNING_BILE_1, SPELL_BURNING_BILE_2, SPELL_BURNING_BILE_3), true, false, 0);
            HandleAuraOnRaidNorthrendBeasts(me, SPELL_BURNING_BILE_MAIN, true, false, 0);

            boss_jormungarAI::JustDied(killer);
        }
    };

};

void boss_jormungarAI::UpdateAI(const uint32 diff)
{
    if (!UpdateVictim()) return;

    if (IsHeroic())
    {
        if (!nextBossSpawnInitiated)
        {
            if (m_summonNextBossHeroicTimer <= diff)
            {
                if (m_instance)
                {
                    nextBossSpawnInitiated = true;

                    if (Creature* pSister = Unit::GetCreature((*me), m_instance->GetData64(m_uiSisterID)))
                    {
                        bool sisterSpawnedNextBoss = false;

                        if (pSister->AI())
                        {
                            if (m_uiSisterID == NPC_ACIDMAW)
                            {
                                sisterSpawnedNextBoss = CAST_AI(boss_acidmaw::boss_acidmawAI, pSister->AI())->nextBossSpawnInitiated;
                            }
                            else
                            {
                                sisterSpawnedNextBoss = CAST_AI(boss_dreadscale::boss_dreadscaleAI, pSister->AI())->nextBossSpawnInitiated;
                            }
                        }

                        if (sisterSpawnedNextBoss)
                            return;
                    }

                    m_instance->SetData(TYPE_EVENT, 300);
                    m_instance->SetData(SPAWNED_NEXT_BOSS_2, 1);
                    m_instance->SetData(TYPE_NORTHREND_BEASTS, IN_PROGRESS);
                    m_instance->SetData(TYPE_BEASTS, IN_PROGRESS);
                }
            } else m_summonNextBossHeroicTimer -= diff;
        }
    }

    if (m_instance && m_instance->GetData(TYPE_NORTHREND_BEASTS) == SNAKES_SPECIAL && !m_bEnraged)
    {
        DoScriptText(SAY_EMERGE, me);
        me->RemoveAurasDueToSpell(SPELL_SUBMERGE_0);
        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE /*| UNIT_FLAG_IMMUNE_TO_PC */| UNIT_FLAG_NOT_SELECTABLE);
        DoCast(SPELL_ENRAGE);
        m_bEnraged = true;
        DoScriptText(SAY_BERSERK, me);
        switch (m_uiStage)
        {
            case 0: break;
            case 4:
                m_uiStage = 5;
                m_uiSubmergeTimer = 5*IN_MILLISECONDS;
            default:
                m_uiStage = 7;
        }
    }

    switch (m_uiStage)
    {
        case 0: // Mobile
            if (m_uiBiteTimer <= diff)
            {
                me->InterruptNonMeleeSpells(false);
                DoCastVictim(m_uiBiteSpell);
                m_uiBiteTimer = 20000;
            } else m_uiBiteTimer -= diff;

            if (m_uiSpewTimer <= diff)
            {
                DoCastAOE(m_uiSpewSpell);
                m_uiSpewTimer = urand(10*1000, 20*1000);
            } else m_uiSpewTimer -= diff;

            if (m_uiSlimePoolTimer <= diff)
            {
                DoCast(me, SUMMON_SLIME_POOL, true);
                m_uiSlimePoolTimer = urand(15*1000, 20*1000);
            } else m_uiSlimePoolTimer -= diff;

            if (m_uiSubmergeTimer <= diff && !m_bEnraged)
            {
                m_uiStage = 1;
                m_uiSubmergeTimer = 5*IN_MILLISECONDS;
            } else m_uiSubmergeTimer -= diff;

            DoMeleeAttackIfReady();
            break;
        case 1: // Submerge
            me->SetDisplayId(11686);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE /*| UNIT_FLAG_IMMUNE_TO_PC*/ | UNIT_FLAG_NOT_SELECTABLE);
            DoCast(me, SPELL_SUBMERGE_0);
            DoScriptText(SAY_SUBMERGE, me);
            me->GetMotionMaster()->MovePoint(0, ToCCommonLoc[1].GetPositionX()+urand(0, 80)-40, ToCCommonLoc[1].GetPositionY()+urand(0, 80)-40, ToCCommonLoc[1].GetPositionZ());
            m_uiStage = 2;
        case 2: // Wait til emerge
            if (m_uiSubmergeTimer <= diff)
            {
                m_uiStage = 3;
                m_uiSubmergeTimer = 45*1000;
            } else m_uiSubmergeTimer -= diff;
            break;
        case 3: // Emerge
            DoScriptText(SAY_EMERGE, me);
            me->RemoveAurasDueToSpell(SPELL_SUBMERGE_0);
            DoCast(me, SPELL_EMERGE_0);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE /*| UNIT_FLAG_IMMUNE_TO_PC*/ | UNIT_FLAG_NOT_SELECTABLE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
            SetCombatMovement(false);
            me->StopMoving();
            me->GetMotionMaster()->Clear();
            me->GetMotionMaster()->MoveIdle();
            me->SetDisplayId(m_uiModelStationary);

            m_uiSprayTimer = 20000;

            m_uiStage = 4;
            break;
        case 4: // Stationary
            if (m_uiSprayTimer <= diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                {
                    me->InterruptNonMeleeSpells(false);
                    DoCast(target, m_uiSpraySpell);
                }
                m_uiSprayTimer = 20000;
            } else m_uiSprayTimer -= diff;

            if (m_uiSweepTimer <= diff)
            {
                DoCastAOE(SPELL_SWEEP_0);
                m_uiSweepTimer = urand(8*1000, 12*1000);
            } else m_uiSweepTimer -= diff;

            if (m_uiSubmergeTimer <= diff)
            {
                m_uiStage = 5;
                m_uiSubmergeTimer = 5*1000;
            } else m_uiSubmergeTimer -= diff;

            DoSpellAttackIfReady(m_uiSpitSpell);
            break;
        case 5: // Submerge
            me->SetDisplayId(11686);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE/* | UNIT_FLAG_IMMUNE_TO_PC*/ | UNIT_FLAG_NOT_SELECTABLE);
            DoCast(me, SPELL_SUBMERGE_0);
            DoScriptText(SAY_SUBMERGE, me);
            me->GetMotionMaster()->MovePoint(0, ToCCommonLoc[1].GetPositionX()+urand(0, 80)-40, ToCCommonLoc[1].GetPositionY()+urand(0, 80)-40, ToCCommonLoc[1].GetPositionZ());
            m_uiStage = 6;
        case 6: // Wait til emerge
            if (m_uiSubmergeTimer <= diff)
            {
                m_uiStage = 7;
                m_uiSubmergeTimer = 45*IN_MILLISECONDS;
            } else m_uiSubmergeTimer -= diff;
            break;
        case 7: // Emerge
            DoScriptText(SAY_EMERGE, me);
            me->RemoveAurasDueToSpell(SPELL_SUBMERGE_0);
            DoCast(me, SPELL_EMERGE_0);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE/* | UNIT_FLAG_IMMUNE_TO_PC */| UNIT_FLAG_NOT_SELECTABLE);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
            SetCombatMovement(true);
            me->GetMotionMaster()->MoveChase(me->getVictim());
            me->SetDisplayId(m_uiModelMobile);

            m_uiBiteTimer = 20000;

            m_uiStage = 0;
            break;
    }
}


class mob_slime_pool : public CreatureScript
{
public:
    mob_slime_pool() : CreatureScript("mob_slime_pool") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_slime_poolAI(creature);
    }

    struct mob_slime_poolAI : public ScriptedAI
    {
        mob_slime_poolAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        bool casted;

        void Reset()
        {
            casted = false;
            me->SetReactState(REACT_PASSIVE);
            me->ForcedDespawn(60*IN_MILLISECONDS);
        }

        void UpdateAI(const uint32 /*diff*/)
        {
            if (!casted)
            {
                casted = true;
                DoCast(me, SPELL_SLIME_POOL_EFFECT, true);
            }
        }
    };

};

class boss_icehowl : public CreatureScript
{
public:
    boss_icehowl() : CreatureScript("boss_icehowl") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_icehowlAI(creature);
    }

    struct boss_icehowlAI : public ScriptedAI
    {
        boss_icehowlAI(Creature* creature) : ScriptedAI(creature)
        {
            m_instance = (InstanceScript*)creature->GetInstanceScript();
        }

        InstanceScript* m_instance;

        uint32 m_uiFerociousButtTimer;
        uint32 m_uiArticBreathTimer;
        uint32 m_uiWhirlTimer;
        uint32 m_uiMassiveCrashTimer;
        uint32 m_uiTrampleTimer;
		//uint32 m_uiChargeTimer;
        float  m_fTrampleTargetX, m_fTrampleTargetY, m_fTrampleTargetZ;
        uint64 m_uiTrampleTargetGUID;
        bool   m_bMovementStarted;
        bool   m_bMovementFinish;
        bool   m_bTrampleCasted;
        uint8  m_uiStage;
        Unit*  target;

        void Reset()
        {
            m_uiFerociousButtTimer = urand(15*IN_MILLISECONDS, 30*IN_MILLISECONDS);
            m_uiArticBreathTimer = urand(25*IN_MILLISECONDS, 40*IN_MILLISECONDS);
            m_uiWhirlTimer = urand(15*IN_MILLISECONDS, 30*IN_MILLISECONDS);
            m_uiMassiveCrashTimer = 30*IN_MILLISECONDS;
            m_uiTrampleTimer = IN_MILLISECONDS;
            m_bMovementStarted = false;
            m_bMovementFinish = false;
            m_bTrampleCasted = false;
            m_uiTrampleTargetGUID = 0;
            m_fTrampleTargetX = 0;
            m_fTrampleTargetY = 0;
            m_fTrampleTargetZ = 0;
            m_uiStage = 0;
			//m_uiChargeTimer = 2*IN_MILLISECONDS;
        }

        void JustDied(Unit* /*killer*/)
        {
            if (m_instance)
                m_instance->SetData(TYPE_NORTHREND_BEASTS, ICEHOWL_DONE);
        }

        void MovementInform(uint32 type, uint32 pointId)
        {
            if (type != POINT_MOTION_TYPE && type != EFFECT_MOTION_TYPE)
                return;

            switch (pointId)
            {
                case 0:
                    if (me->GetDistance2d(ToCCommonLoc[1].GetPositionX(), ToCCommonLoc[1].GetPositionY()) < 6.0f)
                    {
                        // Middle of the room
                        m_uiStage = 1;
                    }
                    else
                    {
                        // Landed from Hop backwards (start trample)
                        if (Unit::GetPlayer(*me, m_uiTrampleTargetGUID))
                        {
							
							//m_uiChargeTimer = 4*IN_MILLISECONDS;
                            m_uiStage = 4;
                        }
                        else
                            m_uiStage = 6;
                    }
                    break;
                case 1: // Finish trample
                    m_bMovementFinish = true;
                    break;
                case 2:
                    m_instance->DoUseDoorOrButton(m_instance->GetData64(GO_MAIN_GATE_DOOR));
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                    me->SetReactState(REACT_AGGRESSIVE);
                    me->SetInCombatWithZone();
                    break;
            }
        }

        void EnterEvadeMode()
        {
            m_instance->DoUseDoorOrButton(m_instance->GetData64(GO_MAIN_GATE_DOOR));
            ScriptedAI::EnterEvadeMode();
        }

        void JustReachedHome()
        {
            if (m_instance)
            {
                m_instance->DoUseDoorOrButton(m_instance->GetData64(GO_MAIN_GATE_DOOR));
                m_instance->SetData(TYPE_NORTHREND_BEASTS, FAIL);
            }
            me->DespawnOrUnsummon();
        }

        void KilledUnit(Unit* who)
        {
            if (who->GetTypeId() == TYPEID_PLAYER)
            {
                if (m_instance)
                    m_instance->SetData(DATA_TRIBUTE_TO_IMMORTALITY_ELEGIBLE, 0);
            }
        }

        void EnterCombat(Unit* /*who*/)
        {
            if (m_instance)
                m_instance->SetData(TYPE_NORTHREND_BEASTS, ICEHOWL_IN_PROGRESS);
            me->SetInCombatWithZone();
        }

        void SpellHitTarget(Unit* target, SpellInfo const* spell)
        {
            if (spell->Id == SPELL_TRAMPLE && target->GetTypeId() == TYPEID_PLAYER)
            {
                if (!m_bTrampleCasted)
                {
                    DoCast(me, SPELL_FROTHING_RAGE, true);
                    m_bTrampleCasted = true;
                }
            }
        }

        void UpdateAI(uint32 const diff)
        {
            if (!UpdateVictim())
                return;

            switch (m_uiStage)
            {
                case 0:
                    if (m_uiFerociousButtTimer <= diff)
                    {
                        DoCastVictim(SPELL_FEROCIOUS_BUTT);
                        m_uiFerociousButtTimer = urand(15*IN_MILLISECONDS, 30*IN_MILLISECONDS);
                    } else m_uiFerociousButtTimer -= diff;

                    if (m_uiArticBreathTimer <= diff)
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                            DoCast(target, SPELL_ARCTIC_BREATH);
                        m_uiArticBreathTimer = urand(25*IN_MILLISECONDS, 40*IN_MILLISECONDS);
                    } else m_uiArticBreathTimer -= diff;

                    if (m_uiWhirlTimer <= diff)
                    {
                        DoCastAOE(SPELL_WHIRL);
                        m_uiWhirlTimer = urand(15*IN_MILLISECONDS, 30*IN_MILLISECONDS);
                    } else m_uiWhirlTimer -= diff;

                    if (m_uiMassiveCrashTimer <= diff)
                    {
                        me->GetMotionMaster()->MoveJump(ToCCommonLoc[1].GetPositionX(), ToCCommonLoc[1].GetPositionY(), ToCCommonLoc[1].GetPositionZ(), 20.0f, 20.0f); // 1: Middle of the room
                        m_uiStage = 7; //Invalid (Do nothing more than move)
                        m_uiMassiveCrashTimer = 30*IN_MILLISECONDS;
                    } else m_uiMassiveCrashTimer -= diff;

                    DoMeleeAttackIfReady();
                    break;
                case 1:
					me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
					SetCombatMovement(false);
					me->StopMoving();
					me->GetMotionMaster()->Clear();
					me->GetMotionMaster()->MoveIdle();
                    DoCastAOE(SPELL_MASSIVE_CRASH);
                    m_uiStage = 2;
                    break;
                case 2:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0, true))
                    {
                        m_uiTrampleTargetGUID = target->GetGUID();
                        me->SetTarget(m_uiTrampleTargetGUID);
                        DoScriptText(SAY_TRAMPLE_STARE, me, target);
                        m_bTrampleCasted = false;
                       // SetCombatMovement(false);
                        //me->GetMotionMaster()->MoveIdle();
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        m_uiTrampleTimer = 4*IN_MILLISECONDS;
                        m_uiStage = 3;
                    } else m_uiStage = 6;
                    break;
                case 3:
                    if (m_uiTrampleTimer <= diff)
                    {
                        if (Unit* target = Unit::GetPlayer(*me, m_uiTrampleTargetGUID))
                        {
                            m_bTrampleCasted = false;
                            m_bMovementStarted = true;
                            m_fTrampleTargetX = target->GetPositionX();
                            m_fTrampleTargetY = target->GetPositionY();
                            m_fTrampleTargetZ = target->GetPositionZ();
                            me->GetMotionMaster()->MoveJump(2*me->GetPositionX()-m_fTrampleTargetX,
                                2*me->GetPositionY()-m_fTrampleTargetY,
                                me->GetPositionZ(),
                                20.0f, 20.0f); // 2: Hop Backwards
                            m_uiStage = 7; //Invalid (Do nothing more than move)
                        } else m_uiStage = 6;
                    } else m_uiTrampleTimer -= diff;
                    break;
                case 4:
						DoScriptText(SAY_TRAMPLE_START, me);
						me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
						SetCombatMovement(true);
						me->GetMotionMaster()->MoveCharge(m_fTrampleTargetX, m_fTrampleTargetY, m_fTrampleTargetZ+2, 42, 1);
						me->SetTarget(0);
						m_uiStage = 5;
                    break;
                case 5:
                    if (m_bMovementFinish)
                    {
                        if (m_uiTrampleTimer <= diff) DoCastAOE(SPELL_TRAMPLE);
                        m_bMovementFinish = false;
                        m_uiStage = 6;
                        return;
                    }
                    if (m_uiTrampleTimer <= diff)
                    {
                        Map::PlayerList const &lPlayers = me->GetMap()->GetPlayers();
                        for (Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
                        {
                            if (Unit* player = itr->getSource())
                                if (player->isAlive() && player->IsWithinDistInMap(me, 6.0f))
                                {
                                    DoCastAOE(SPELL_TRAMPLE);
                                    m_uiTrampleTimer = IN_MILLISECONDS;
                                    break;
                                }
                        }
                    } else m_uiTrampleTimer -= diff;
                    break;
                case 6:
                    if (!m_bTrampleCasted)
                    {
                        DoCast(me, SPELL_STAGGERED_DAZE);
                        DoScriptText(SAY_TRAMPLE_FAIL, me);
                    }
                    m_bMovementStarted = false;
                    me->GetMotionMaster()->MovementExpired();
                    me->GetMotionMaster()->MoveChase(me->getVictim());
                    SetCombatMovement(true);
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    m_uiStage = 0;
                    break;
				default :
					break;
            }
        }
    };

};

void AddSC_boss_northrend_beasts()
{
    new boss_gormok();
    new mob_snobold_vassal();
    new boss_acidmaw();
    new boss_dreadscale();
    new mob_slime_pool();
    new boss_icehowl();
}
