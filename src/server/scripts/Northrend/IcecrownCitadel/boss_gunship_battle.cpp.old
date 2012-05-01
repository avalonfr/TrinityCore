/*
* Copyright (C) 2008-2010 TrinityCore <http://www.trinitycore.org/>
*
* Copyright (C) Blackcore, Desarrollo completo Blackcore.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
*/

/*
Rewriter : by s@m & the team of
http://wow.avalonserver.org/

Thx ChaosCore & Vlad & DarkSoe
!sry but the real source may be available in 2-3 month if no more leechers

*/

#include "ScriptPCH.h"
#include "icecrown_citadel.h"
#include "MapManager.h"
#include "Transport.h"
#include "Vehicle.h"
#include "Spell.h"
#include "GameObjectAI.h"
#include "SpellAuras.h"

/*
Dedails from :
http://www.wowwiki.com/Gunship_Battle
http://www.youtube.com/watch?v=00FbefuBzF0
http://wowhead.com/
*/

// Fusileros
#define NPC_ZAFOD_BOOMBOX 37184
#define ITEM_GOBLIN_ROCKET_PACK 49278

const Position SpawnZafodAlliance = {0.0f, 0.0f, 0.0f, 0.0f};
const Position SpawnZafodHorde = {0.0f, 0.0f, 0.0f, 0.0f};

#define ZAFOD_BOOMBOX_GOSSIP_GET 1000
#define ZAFOD_BOOMBOX_GOSSIP_CANCEL 1001

// Do_Action
#define DO_ACTION_START_GUNSHIP_BATTLE 0
#define DO_ACTION_FINISH_GUNSHIP_BATTLE 1
#define DO_ACTION_BATTLE_START 2

// tp de fin
const Position DeathbringerRise = { -549.80548f, 2211.19238f, 539.29119f, 6.27718f};

// Spawns de NPCs
#define SPAWN_SERGEANT 0
#define SPAWN_DISTANCE_0 1 // Soldado con Mortero o Lanza Cohetes
#define SPAWN_CAC 2 // Infante de marina o Saqueador
#define SPAWN_CAST 3 // Hechizero o Mago de batalla
#define SPAWN_DISTANCE_1 4 // Fusileros o Lanzadores de hachas

// Zonas de Spawns
const Position SpawnPosAlliance[] =
{
	{0.0f, 0.0f, 0.0f, 0.0f},
	{0.0f, 0.0f, 0.0f, 0.0f},
	{0.0f, 0.0f, 0.0f, 0.0f},
	{0.0f, 0.0f, 0.0f, 0.0f},
	{0.0f, 0.0f, 0.0f, 0.0f}
};
const Position SpawnPosHorde[] =
{
	{0.0f, 0.0f, 0.0f, 0.0f},
	{0.0f, 0.0f, 0.0f, 0.0f},
	{0.0f, 0.0f, 0.0f, 0.0f},
	{0.0f, 0.0f, 0.0f, 0.0f},
	{0.0f, 0.0f, 0.0f, 0.0f}
};

//-----------------------------------------------------------------------
// Alianza :
#define GO_SKYBREAKER 192242
#define GO_GUNSHIP_STAIRS_ALLIANCE 201709 // Acceso innecesario, imposible
#define GO_ALLIANCE_GUNSHIP_PORTAL 195371 // z Útiles ?
#define NPC_GUNSHIP_CANNON_ALLIANCE 36838

#define SKYBREAKER_X 0
#define SKYBREAKER_Y 0
#define SKYBREAKER_Z 0
#define SKYBREAKER_RAYON 0

#define NPC_MURADIN_BRONZEBEARD 36948

// Dialogos
#define ALLIANCE_GOSSIP_ACTION_START 10875
#define ALLIANCE_GOSSIP_ACTION_CANCEL 1001
#define ALLIANCE_GOSSIP_ACTION_TP 1002

const Position PosMuradinBronzebeard[]=
{
	{-472.596f, 2466.8701f, 190.7371f, 6.204f}, // Primera posicion
	{0.0f, 0.0f, 0.0f, 0.0f} // Segunda posicion
};

#define NPC_SKYBREAKER_SERGEANT 36961
#define NPC_SKYBREAKER_MORTAR_SOLDIER 36978
#define NPC_SKYBREAKER_MARINE 36950
#define NPC_SKYBREAKER_SORCERER 37026
#define NPC_SKYBREAKER_SORCERER_CANON 37116
#define NPC_SKYBREAKER_RIFLEMAN 36969

#define AURA_ON_SKYBREAKER 70120

//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// HORDA :
#define GO_OGRIM_HAMMER 192241
#define GO_GUNSHIP_STAIRS_HORDE 202211 // Innecesario.
#define GO_HORDE_GUNSHIP_PORTAL 196413 // z Útil ?
#define NPC_GUNSHIP_CANNON_HORDE 36839

#define OGRIM_HAMMER_X 0
#define OGRIM_HAMMER_Y 0
#define OGRIM_HAMMER_Z 0
#define OGRIM_HAMMER_RAYON 0

#define NPC_HIGH_OVERLORD_VAROK_SAURFANG 36939

// Dialogos
#define HORDE_GOSSIP_ACTION_START 10876
#define HORDE_GOSSIP_ACTION_CANCEL 1004
#define HORDE_GOSSIP_ACTION_TP 1005

const Position PosHighOverlordVarokSaurfang[]=
{
	{0.0f, 0.0f, 0.0f, 0.0f}, // Primera posicion
	{0.0f, 0.0f, 0.0f, 0.0f} // Segunda posicion
};

#define NPC_KORKRON_SERGEANT 36960
#define NPC_KORKRON_ROCKETEER 36982
#define NPC_KORKRON_REAVER 36957
#define NPC_KORKRON_BATTLE_MAGE 37117
#define NPC_KORKRON_AXETHROWER 36968

#define AURA_ON_OGRIM_HAMMER 70121

//-----------------------------------------------------------------------

// CANON_GUNSHIP
#define SPELL_CANNON_BLAST 69400
#define SPELL_INCINERATING_BLAST 70175

// MURADIN BARBABRONCE// ALTO_SENOR_SUPREMO_COLMILLOSAURIO
#define SPELL_CLEAVE 15284
#define SPELL_RENDING_THROW 70309
#define SPELL_TASTE_OF_BLOOD 69634

#define AURA_BATTLE_FURY_10 69638
#define AURA_BATTLE_FURY_25 72307

// TODOS LOS ENEMIGOS TIENEN ESTE HECHIZO EXCEPTO LOS COMANDANTES
#define SPELL_BURNING_PITCH 71335

// HECHICERO // MAGO_DE_BATALLA
#define SPELL_BELOW_ZERO 69705

// SOLDADO_CON_MORTERO // LANZACOHETES
#define SPELL_ROCKET_ARTILLERY 69679
#define SPELL_EXPLOSION 69680 // Efecto, cuando el cohete explosiona en el suelo.
#define SPELL_ROCKET_ARTILLERY_MARKER 71371
#define SPELL_ROCKET_ARTILLERY_TRIGGERED 69679

// SARGENTO
#define SPELL_BLADESTORM 69652
#define SPELL_BLADESTORM_TRIGGER 69653 // Uso innecesario SPELL_BLADESTORM
#define SPELL_DESPERATE_RESOLVE_10_NM 69647
#define SPELL_DESPERATE_RESOLVE_10_HM 72537
#define SPELL_DESPERATE_RESOLVE_25_NM 72536
#define SPELL_DESPERATE_RESOLVE_25_HM 72538
#define SPELL_WOUNDING_STRIKE_10_NM 69651
#define SPELL_WOUNDING_STRIKE_10_HM 72570
#define SPELL_WOUNDING_STRIKE_25_NM 72569
#define SPELL_WOUNDING_STRIKE_25_HM 72571

// FUSILEROS
#define SPELL_SHOOT_10_NM 70162
#define SPELL_SHOOT_10_HM 72567
#define SPELL_SHOOT_25_NM 72566
#define SPELL_SHOOT_25_HM 72568

// LANZADORES_DE_HACHAS
#define SPELL_HURL_AXE_10_NM 70161
#define SPELL_HURL_AXE_10_HM 72540
#define SPELL_HURL_AXE_25_NM 72539
#define SPELL_HURL_AXE_25_HM 72541

// BUFFOS_DE_EXPERIENCIA
#define SPELL_EXPERIENCED 71188
#define SPELL_VETERAN 71193
#define SPELL_ELITE 71195

// Auras
#define SPELL_ON_ORGRIMS_HAMMERS_DECK	70121
#define SPELL_ON_SKYBREAKERS_DECK		70120

#define SPELL_OVERHEAT					69487 // Triggers spell #69488 every 0.25s. It should consume 10 Energy but does not.
//-----------------------------------------------------------------------
//------------------------------ GRITOS ----------------------------------
//-----------------------------------------------------------------------
// HORDA ----------------------------------------------------------------
//-----------------------------------------------------------------------
// PRIMERA_ESCUADRA_ASISTIDA
#define SAY_KORKRON_FIRST_SQUAD_0 "Thank the spirits for you, brothers and sisters. The Skybreaker has already left. Quickly now, to Orgrim's Hammer! If you leave soon, you may be able to catch them."
#define SAY_KORKRON_FIRST_SQUAD_1 "This should be helpin'ya!"

// SEGUNDA_ESCUADRA_ASISTIDA
#define SAY_KORKRON_SECOND_SQUAD_0 "Aka'Magosh, brave warriors. The alliance is in great number here."
#define SAY_KORKRON_SECOND_SQUAD_1 "Captain Saurfang will be pleased to see you aboard Orgrim's Hammer. Make haste, we will secure the area until you are ready for take-off."

// COMIENZA_EL_EVENTO
#define YELL_EVENT_BEGIN_HORDE_HIGH_OVERLORD_SAURFANG_4 "ALL HANDS ON DECK!"
#define YELL_EVENT_BEGIN_HORDE_HIGH_OVERLORD_SAURFANG_0 "Rise up, sons and daughters of the Horde! Today we battle a hated enemy of the Horde! LOK'TAR OGAR! Kor'kron, take us out!"
#define YELL_EVENT_BEGIN_HORDE_HIGH_OVERLORD_SAURFANG_1 "What is that?! Something approaching in the distance!"
#define YELL_EVENT_BEGIN_HORDE_HIGH_OVERLORD_SAURFANG_2 "ALLIANCE GUNSHIP!"
#define YELL_EVENT_BEGIN_HORDE_MURADIN_BRONZEBEARD_0 "Move yer jalopy or we'll blow it out of the sky, orc! The Horde's got no business here!"
#define YELL_EVENT_BEGIN_HORDE_HIGH_OVERLORD_SAURFANG_3 "You will know our business soon! KOR'KRON, ANNIHILATE THEM!"

// EMBARQUE AL MARTILLO ORGRIMMAR
#define YELL_BOARDING_ORGRIM_S_HAMMER_MURADIN_BRONZEBEARD_0 "Marines, Sergeants, attack!"
#define YELL_BOARDING_ORGRIM_S_HAMMER_HIGH_OVERLORD_SAURFANG_0 "You DARE board my ship? Your death will come swiftly."

// NUEVO_FUSILERO
#define YELL_NEW_RIFLEMEN_MURADIN "Riflemen, shoot faster!"
// NUEVO_EQUIPO_DE_MORTERO
#define YELL_NEW_MORTAR_TEAM_MURADIN "Mortar team, reload!"
// NUEVO_MAGO
#define YELL_NEW_MAGE_MURADIN "We're taking hull damage, get a sorcerer out here to shut down those cannons!"
// VICTORIA_HORDA
#define YELL_HORDE_VICTORY_SAURFANG "The Alliance falter. Onward to the Lich King!"
// DERROTA_HORDA
#define YELL_HORDE_DEFEAT_SAURFANG "Damage control! Put those fires out! You haven't seen the last of the Horde!"

// ALIANZA -------------------------------------------------------------
//-----------------------------------------------------------------------
// PRIMERA_ESCUADRA_ASISTIDA
#define SAY_SKYBREAKER_FIRST_SQUAD_0 "Thank goodness you arrived when you did, heroes. Orgrim's Hammer has already left. Quickly now, to The Skybreaker! If you leave soon, you may be able to catch them."
#define SAY_SKYBREAKER_FIRST_SQUAD_1 "This ought to help!"

// SEGUNDA_ESCUADRA_ASISTIDA
#define SAY_SKYBREAKER_SECOND_SQUAD_0 "You have my thanks. We were outnumbered until you arrived."
#define SAY_SKYBREAKER_SECOND_SQUAD_1 "Captain Muradin is waiting aboard The Skybreaker. We'll secure the area until you are ready for take off."
#define SAY_SKYBREAKER_SECOND_SQUAD_2 "Skybreaker infantry, hold position!"

// COMIENZA_EL_EVENTO
#define YELL_EVENT_BEGIN_ALLIANCE_MURADIN_BRONZEBEARD_0 "Fire up the engines! We got a meetin' with destiny, lads!"
#define YELL_EVENT_BEGIN_ALLIANCE_MURADIN_BRONZEBEARD_1 "Hold on to yer hats!"
#define YELL_EVENT_BEGIN_ALLIANCE_MURADIN_BRONZEBEARD_2 "What in the world is that? Grab me spyglass, crewman!"
#define YELL_EVENT_BEGIN_ALLIANCE_MURADIN_BRONZEBEARD_3 "By me own beard! HORDE SAILIN' IN FAST 'N HOT!"
#define YELL_EVENT_BEGIN_ALLIANCE_MURADIN_BRONZEBEARD_4 "EVASIVE ACTION! MAN THE GUNS!"
#define YELL_EVENT_BEGIN_ALLIANCE_MURADIN_BRONZEBEARD_5 "Cowardly dogs! Ye blindsided us!"
#define YELL_EVENT_BEGIN_ALLIANCE_HIGH_OVERLORD_SAURFANG_0 "This is not your battle, dwarf. Back down or we will be forced to destroy your ship."
#define YELL_EVENT_BEGIN_ALLIANCE_MURADIN_BRONZEBEARD_6 "Not me battle? I dunnae who ye? think ye are, mister, but I got a score to settle with Arthas and yer not gettin' in me way! FIRE ALL GUNS! FIRE! FIRE!"

// EMBARCANDO_EL_ROMPECIELOS
#define YELL_BOARDING_THE_SKYBREAKER_HIGH_OVERLORD_SAURFANG_0 "Reavers, Sergeants, attack!"
#define YELL_BOARDING_THE_SKYBREAKER_MURADIN_BRONZEBEARD_0 "What's this then?! Ye won't be takin' this son o' Ironforge's vessel without a fight!."

// NUEVO_LANZAHACHAS
#define YELL_NEW_AXETHROWERS_HIGH_OVERLORD_SAURFANG_0 "Axethrowers, hurl faster!"

// NUEVO_FUSILEROS
#define YELL_NEW_ROCKETEERS_HIGH_OVERLORD_SAURFANG_0 "Rocketeers, reload!"

// NUEVO_MAGO_DE_BATALLA
#define YELL_NEW_BATTLE_MAGE_HIGH_OVERLORD_SAURFANG_0 "We're taking hull damage, get a battle-mage out here to shut down those cannons!"

// VICTORIA_ALIANZA
#define YELL_ALLIANCE_VICTORY_MURADIN_BRONZEBEARD_0 "Don't say I didn't warn ya, scoundrels! Onward, brothers and sisters!"

// DERROTA_ALIANZA
#define YELL_ALLIANCE_DEFEAT_MURADIN_BRONZEBEARD_0 "Captain Bartlett, get us out of here! We're taken too much damage to stay afloat!"

enum Texts
{
	///--------------------------New Text System
	// Muradin Bronzebeard
	SAY_INTRO_ALLIANCE_0			= 0,
	SAY_INTRO_ALLIANCE_1			= 1,
	SAY_INTRO_ALLIANCE_2			= 2,
	SAY_INTRO_ALLIANCE_3			= 3,
	SAY_INTRO_ALLIANCE_4			= 4,
	SAY_INTRO_ALLIANCE_5			= 5,
	SAY_INTRO_ALLIANCE_7			= 6,
	SAY_INTRO_HORDE_3				= 7,
	SAY_BOARDING_SKYBREAKER_1		= 8,
	SAY_BOARDING_ORGRIMS_HAMMER_0	= 9,
	SAY_NEW_RIFLEMEN_SPAWNED		= 10,
	SAY_NEW_MORTAR_TEAM_SPAWNED		= 11,
	SAY_NEW_MAGE_SPAWNED			= 12,
	SAY_ALLIANCE_VICTORY			= 13,
	SAY_ALLIANCE_DEFEAT				= 14, // How will we handle that case ? Ie. the player loses

	// High Overlord Saurfang
	SAY_INTRO_HORDE_0				= 0,
	SAY_INTRO_HORDE_1				= 1,
	SAY_INTRO_HORDE_2				= 2,
	SAY_INTRO_HORDE_4				= 3,
	SAY_BOARDING_SKYBREAKER_0		= 4,
	SAY_BOARDING_ORGRIMS_HAMMER_1	= 5,
	SAY_NEW_AXETHROWER_SPAWNED		= 6,
	SAY_NEW_ROCKETEERS_SPAWNED		= 7,
	SAY_NEW_BATTLE_MAGE_SPAWNED		= 8,
	SAY_HORDE_VICTORY				= 9,
	SAY_HORDE_DEFEAT				= 10, // How will we handle that case ? Ie. the player loses 
};



//-----------------------------------------------------------------------

#define SPELL_ACHIEVEMENT 72959

enum eAchievements
{
	IM_ON_A_BOAT_10 = 4536,
	IM_ON_A_BOAT_25 = 4612
};

//-----------------------------------------------------------------------
#define PHASE_NOT_STARTED 0
#define PHASE_DISTANCE 1
#define PHASE_BOARDING 2
#define PHASE_END 3

#define EXP_NULL 0
#define EXP_EXPERIENCED 20000
#define EXP_VETERAN 40000
#define EXP_ELITE 80000

//-----------------------------------------------------------------------

void buffExp( ScriptedAI * _script, uint32 timer_Spawn)
{
	if( timer_Spawn < EXP_EXPERIENCED)
		return;
	if( timer_Spawn > EXP_EXPERIENCED && timer_Spawn < EXP_VETERAN && !_script->me->HasAura(SPELL_EXPERIENCED)) {
		_script->DoCast( _script->me, SPELL_EXPERIENCED);
		return;
	}
	if( timer_Spawn > EXP_VETERAN && timer_Spawn < EXP_ELITE /*&& !_script->me->HasAura(SPELL_ELITE)*/ && !_script->me->HasAura(SPELL_VETERAN)) {
		_script->me->RemoveAura(SPELL_EXPERIENCED);
		_script->DoCast( _script->me, SPELL_VETERAN);
		return;
	}
	if( timer_Spawn > EXP_ELITE  && !_script->me->HasAura(SPELL_ELITE)) {
		_script->me->RemoveAura(SPELL_VETERAN);
		_script->DoCast( _script->me, SPELL_ELITE);
		return;
	}
}

// This variable is only used by rocketeers, this is a hack
struct mortarMarksLoc
{
	uint32 durationBeforeRefreshing;
	Position location;
};

class npc_zafod_boombox : public CreatureScript
{
public:
	npc_zafod_boombox() : CreatureScript("npc_zafod_boombox") { }

	struct npc_zafod_boomboxAI : public ScriptedAI
	{
		npc_zafod_boomboxAI(Creature* pCreature) : ScriptedAI(pCreature)
		{
			pInstance = me->GetInstanceScript();
		}

		void UpdateAI( const uint32 diff) 
		{
			if (pInstance && ( pInstance->GetBossState(DATA_GUNSHIP_BATTLE_EVENT) == DONE || pInstance->GetBossState(DATA_GUNSHIP_BATTLE_EVENT) == NOT_STARTED))
				me->DisappearAndDie();
		}

		InstanceScript* pInstance;
	};

	bool OnGossipHello(Player* pPlayer, Creature* pCreature)
	{
		pPlayer->ADD_GOSSIP_ITEM( 2, "Give me one of these machines" , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF +1);
		pPlayer->ADD_GOSSIP_ITEM( 2, "Maybe later" , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF +2);
		pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
		return true;
	}

	void SendDefaultMenu_ACTION(Player* pPlayer, Creature* pCreature, uint32 uiAction)
	{
		switch (uiAction)
		{
		case GOSSIP_ACTION_INFO_DEF +1:
			pPlayer->AddItem(49278,1);
			pPlayer->CLOSE_GOSSIP_MENU();
			break;

		case GOSSIP_ACTION_INFO_DEF +2:
			pPlayer->CLOSE_GOSSIP_MENU();
			break;
		}
	}

	bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
	{
		if (uiSender == GOSSIP_SENDER_MAIN)
		{
			SendDefaultMenu_ACTION(pPlayer, pCreature, uiAction);
		}
		return true;
	}

	CreatureAI* GetAI(Creature* pCreature) const
	{
		return new npc_zafod_boomboxAI(pCreature);
	}
};

class boss_muradin_bronzebeard : public CreatureScript
{
public:
	boss_muradin_bronzebeard() : CreatureScript("boss_muradin_bronzebeard") { }

	struct boss_muradin_bronzebardAI : public BossAI
	{
		boss_muradin_bronzebardAI(Creature* pCreature) : BossAI(pCreature, DATA_GUNSHIP_BATTLE_EVENT)
		{
			//instance->SetBossState( DATA_GUNSHIP_BATTLE_EVENT, NOT_STARTED);

			// instance->SetData( DATA_GUNSHIP_EVENT, PHASE_NOT_STARTED);
			pInstance = me->GetInstanceScript();
			bDialog = false;
			dialogID = 0;
			dialogTimer = 0;
			stack = 0 ;
		}

		void Reset()
		{


			DoCast( me, RAID_MODE (AURA_BATTLE_FURY_10,AURA_BATTLE_FURY_25));
			if (Aura *aura = me->GetAura( RAID_MODE (AURA_BATTLE_FURY_10,AURA_BATTLE_FURY_25)))
				if (stack != 0 )
					aura->SetStackAmount(stack);

			timer_Cleave = urand( 8000, 10000);
			timer_RendingThrow = urand( 10000, 12000);
			timer_TasteOfBlood = 0; // -40%
			timer_BattleFury = 0;

			bDialog = false;
			dialogID = 0;
			dialogTimer = 0;
			DoZoneInCombat();
		}

		void JustReachedHome()
		{
			me->SetReactState(REACT_AGGRESSIVE);
		}

		void DamageTaken(Unit* /*done_by*/, uint32& damage)
		{
		}

		void EnterCombat(Unit* /*pWho*/)
		{
			Unit *target = me->SelectNearestPlayer(100.0f); 
			if (target && target->HasAura(SPELL_ON_SKYBREAKERS_DECK))
				me->AI()->AttackStart(target);
		}

		void UpdateAI( const uint32 diff)
		{


			if (pInstance && pInstance->GetBossState(DATA_GUNSHIP_BATTLE_EVENT) == DONE )
				me->DisappearAndDie();

			if (!UpdateVictim())
				return;

			if (!me->getVictim()->HasAura(SPELL_ON_SKYBREAKERS_DECK))
			{
				Unit *target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, false, SPELL_ON_SKYBREAKERS_DECK);
				if (!target)
				{
					me->SetReactState(REACT_PASSIVE);
					me->getThreatManager().resetAllAggro();
					EnterEvadeMode();
					return;
				}
				else 
				{
					me->AI()->EnterCombat(target);
				}
			}

			if( ( me->GetHealth() / me->GetMaxHealth()) <= 0.4)
			{
				if( timer_TasteOfBlood <= diff) 
				{
					DoCast( me, SPELL_TASTE_OF_BLOOD);
					timer_TasteOfBlood = 120000;
				}else timer_TasteOfBlood -= diff;
			}


			if( timer_Cleave <= diff) {
				DoCastVictim( SPELL_CLEAVE);
				timer_Cleave = urand( 8000, 10000);
			} else timer_Cleave -= diff;

			if( timer_RendingThrow <= diff) {
				DoCastVictim( SPELL_RENDING_THROW);
				timer_RendingThrow = urand( 10000, 12000);
			} else timer_RendingThrow -= diff;

			if (timer_BattleFury <= diff) {
				DoCast (me , RAID_MODE (AURA_BATTLE_FURY_10,AURA_BATTLE_FURY_25));

				uint32 spell = RAID_MODE (AURA_BATTLE_FURY_10,AURA_BATTLE_FURY_25);
				if (Aura* aura_fury = me->GetAuraOfRankedSpell(spell))
				{
					stack = aura_fury->GetStackAmount();
				}



				timer_BattleFury = 3000;
			} else timer_BattleFury -= diff;

			DoMeleeAttackIfReady();
		}

		uint32 health_theSkybreaker;
		uint32 health_ogrimsHammer;

		std::list<Creature*> lSummons;
		Creature* zafodBoombox;

		uint32 timer_Cleave;
		uint32 timer_RendingThrow;
		uint32 timer_TasteOfBlood;
		uint32 timer_BattleFury;

		InstanceScript* pInstance;
		bool bBattleStart;
		bool bDialog;
		uint8 stack;
		uint32 dialogID;
		uint32 dialogTimer;
	};

	CreatureAI* GetAI(Creature* pCreature) const
	{
		return new boss_muradin_bronzebardAI(pCreature);
	}
};

class boss_high_overlord_varok_saurfang : public CreatureScript
{
public:
	boss_high_overlord_varok_saurfang() : CreatureScript("boss_high_overlord_varok_saurfang") { }

	struct boss_high_overlord_varok_saurfangAI : public BossAI
	{
		boss_high_overlord_varok_saurfangAI(Creature* pCreature) : BossAI(pCreature, DATA_GUNSHIP_BATTLE_EVENT)
		{
			//instance->SetBossState( DATA_GUNSHIP_BATTLE_EVENT, NOT_STARTED);
			//instance->SetData( DATA_GUNSHIP_EVENT, PHASE_NOT_STARTED);
			bBattleStart = false;
			bDialog = false;
			dialogID = 0;
			dialogTimer = 0;
		}

		void Reset()
		{

			timer_Cleave = urand( 8000, 10000);
			timer_RendingThrow = urand( 10000, 12000);
			timer_TasteOfBlood = 0; // -40%
			bBattleStart = false;
			bDialog = false;
			dialogID = 0;
			dialogTimer = 0;
		}

		void DamageTaken(Unit* /*done_by*/, uint32& damage)
		{
			if( !bBattleStart)
				damage = 0;
		}

		void EnterCombat(Unit* /*who*/)
		{
		}

		void Spawn( uint32 code)
		{
			uint8 spawn = floor( (long double) urand( 0, 5));
			switch( code) {
			case SPAWN_SERGEANT :
				lSummons.push_back( DoSpawnCreature( NPC_SKYBREAKER_SERGEANT, SpawnPosAlliance[spawn].GetPositionX(), SpawnPosAlliance[spawn].GetPositionY(), SpawnPosAlliance[spawn].GetPositionZ(), SpawnPosAlliance[spawn].GetOrientation(), TEMPSUMMON_DEAD_DESPAWN,0));
				break;
			case SPAWN_DISTANCE_0 :
				lSummons.push_back( DoSpawnCreature( NPC_SKYBREAKER_MORTAR_SOLDIER, SpawnPosAlliance[spawn].GetPositionX(), SpawnPosAlliance[spawn].GetPositionY(), SpawnPosAlliance[spawn].GetPositionZ(), SpawnPosAlliance[spawn].GetOrientation(), TEMPSUMMON_DEAD_DESPAWN,0));
				break;
			case SPAWN_CAC :
				lSummons.push_back( DoSpawnCreature( NPC_SKYBREAKER_MARINE, SpawnPosAlliance[spawn].GetPositionX(), SpawnPosAlliance[spawn].GetPositionY(), SpawnPosAlliance[spawn].GetPositionZ(), SpawnPosAlliance[spawn].GetOrientation(), TEMPSUMMON_DEAD_DESPAWN,0));
				break;
			case SPAWN_CAST :
				lSummons.push_back( DoSpawnCreature( NPC_SKYBREAKER_SORCERER, SpawnPosAlliance[spawn].GetPositionX(), SpawnPosAlliance[spawn].GetPositionY(), SpawnPosAlliance[spawn].GetPositionZ(), SpawnPosAlliance[spawn].GetOrientation(), TEMPSUMMON_DEAD_DESPAWN,0));
				break;
			case SPAWN_DISTANCE_1 :
				lSummons.push_back( DoSpawnCreature( NPC_SKYBREAKER_RIFLEMAN, SpawnPosAlliance[spawn].GetPositionX(), SpawnPosAlliance[spawn].GetPositionY(), SpawnPosAlliance[spawn].GetPositionZ(), SpawnPosAlliance[spawn].GetOrientation(), TEMPSUMMON_DEAD_DESPAWN,0));
				break;
			};
		}

		void UpdateAI( const uint32 diff)
		{
			if( bDialog)
			{
				if( dialogTimer <= diff)
				{
					switch( dialogID)
					{
					case 0 :
						{
							instance->SetBossState(DATA_GUNSHIP_BATTLE_EVENT, IN_PROGRESS);
							Talk(SAY_INTRO_HORDE_0);
							// me->MonsterYell( YELL_EVENT_BEGIN_HORDE_HIGH_OVERLORD_SAURFANG_0, LANG_UNIVERSAL, 0);
							//if( GameObject* AlliGunShip = GameObject::GetGameObject(*me, instance->GetData64(GO_ALLI_GUNSHIP_2)))
							//{
							/*Position pos;
							pos.m_positionX = AlliGunShip->GetPositionX();
							pos.m_positionY = AlliGunShip->GetPositionY();
							pos.m_positionZ = AlliGunShip->GetPositionZ();

							uint32 dist = AlliGunShip->GetDistance(-436.745209f,1829.303101f,190.945770f);
							uint32 angle = AlliGunShip->GetAngle(-436.745209f,1829.303101f);

							const char *say = "Ich bin das AlliGunship";

							//Position = (-436.745209f,1829.303101f,190.945770f,2.994648f);
							AlliGunShip->MovePosition(pos,dist,angle);
							AlliGunShip->Yell(*say, LANG_UNIVERSAL, 0);

							me->MonsterSay("Gunshis should fly", LANG_UNIVERSAL, 0);*/
							//}

							dialogID = 1;
							dialogTimer = 11000;
							break;
						}
					case 1 :
						{
							Talk(SAY_INTRO_HORDE_1);
							me->GetMotionMaster()->MovePoint(0, -437.100830f, 1987.904297f, 191.234131f);
							dialogID = 2;
							dialogTimer = 3000;
							break;
						}
					case 2 :
						{
							me->GetMotionMaster()->MovePoint(0, -437.477875f, 1948.863892f, 214.638245f);
							dialogID = 3;
							dialogTimer = 6000;
							break;
						}
					case 3 :
						{
							me->GetMotionMaster()->MovePoint(0, -443.602417f, 1935.741455f, 214.005554f);
							dialogID = 4;
							dialogTimer = 2000;
							break;
						}
					case 4 :
						{
							me->GetMotionMaster()->MovePoint(0, -484.726990f, 1915.420776f, 216.699356f);
							dialogID = 5;
							dialogTimer = 7000;
							break;
						}
					case 5 :
						{
							me->GetMotionMaster()->MovePoint(0, -455.1526f, 1884.7131f, 216.4725f);
							dialogID = 6;
							dialogTimer = 1000;
							break;
						}
					case 6 :
						{
							Talk(SAY_INTRO_HORDE_2);
							if( GameObject* AlliGunShip = GameObject::GetGameObject(*me, instance->GetData64(GO_ALLI_GUNSHIP_2)))
							{
								//delete if GameObject move works
								AlliGunShip->Relocate(-505.865143f, 1799.316528f);
								AlliGunShip->DestroyForNearbyPlayers();
								AlliGunShip->UpdateObjectVisibility();
							}
							dialogID = 7;
							dialogTimer = 5000;
							break;
						}
					case 7 :
						{
							Talk(SAY_INTRO_HORDE_4);
							dialogID = 8;
							dialogTimer = 6000;
							break;
						}
					case 8 :
						{
							if( Creature* pMuradinBronzebeard = me->FindNearestCreature(NPC_MURADIN_BRONZEBEARD,600.0f))
							{
								pMuradinBronzebeard->AI()->Talk(SAY_INTRO_HORDE_3);
								dialogID = 9;
								dialogTimer = 2000;
							}
							break;
						}
					case 9 :
						{
							Talk(SAY_BOARDING_ORGRIMS_HAMMER_1);
							dialogID = 10;
							dialogTimer = 3000;
							break;
						}
					case 10 :
						{
							Position tp;
							tp.m_positionX = -72.911972f;
							tp.m_positionY = 2208.592773f;
							tp.m_positionZ = 533.593140f;
							tp.m_orientation = 1.495439f;

							//Dont need teleport anymore when Move works
							Map::PlayerList const &PlayerList = me->GetMap()->GetPlayers();
							if( !PlayerList.isEmpty())
							{
								for(Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
								{
									if( Player *pCurrent = i->getSource())
									{
										if( me->GetDistance(pCurrent) <= 100.0f)
										{
											DoTeleportPlayer( pCurrent, tp.GetPositionX(), tp.GetPositionY(), tp.GetPositionZ(), tp.GetOrientation());
											pCurrent->AddAura(SPELL_ON_ORGRIMS_HAMMERS_DECK,pCurrent);
										}
									}
								}
							}

							me->NearTeleportTo(tp.GetPositionX(), tp.GetPositionY(), tp.GetPositionZ(), tp.GetOrientation());
							me->AddAura(SPELL_ON_ORGRIMS_HAMMERS_DECK,me);
							me->SummonCreature(NPC_ZAFOD_BOOMBOX,(tp.GetPositionX()+10.0f), (tp.GetPositionY()+5.0f), tp.GetPositionZ(), tp.GetOrientation());
							dialogID = 11;
							dialogTimer = 10000;
							break;
						}
					case 11 :
						{
							if( Creature* pMuradinBronzebeard = me->FindNearestCreature(NPC_MURADIN_BRONZEBEARD,600.0f))
							{
								pMuradinBronzebeard->AI()->Talk(SAY_BOARDING_ORGRIMS_HAMMER_0);
								dialogID = 12;
								dialogTimer = 2000;
							}
							break;
						}
					case 12 :
						{
							Talk(SAY_BOARDING_SKYBREAKER_0);
							// me->MonsterYell( YELL_BOARDING_ORGRIM_S_HAMMER_HIGH_OVERLORD_SAURFANG_0, LANG_UNIVERSAL, 0);
							dialogID = 13;
							dialogTimer = 2000;
							break;
						}
					case 13 :
						{
							bBattleStart = true;
							bDialog = false;
							DoAction(DO_ACTION_BATTLE_START);
							break;
						}
					}
				} else dialogTimer -= diff;
			}

			if( bBattleStart)
			{
				if (!UpdateVictim())
					return;

				if( ( me->GetHealth() / me->GetMaxHealth()) <= 0.4) {
					if( timer_TasteOfBlood <= diff) {
						DoCast( me, SPELL_TASTE_OF_BLOOD);
						timer_TasteOfBlood = 120000;
					} else timer_TasteOfBlood -= diff;
				}

				if( timer_Cleave <= diff) {
					DoCastVictim( SPELL_CLEAVE);
					timer_Cleave = urand( 8000, 10000);
				} else timer_Cleave -= diff;

				if( timer_RendingThrow <= diff) {
					DoCastVictim( SPELL_RENDING_THROW);
					timer_RendingThrow = urand( 10000, 12000);
				} else timer_RendingThrow -= diff;

				DoMeleeAttackIfReady();
			}
		}

		void DoAction(const int32 param)
		{
			switch( param)
			{
			case DO_ACTION_BATTLE_START :
				{
					uint32 m_ALLIGSTRIGGER = RAID_MODE(NPC_ALLIGSTRIGGER_10, NPC_ALLIGSTRIGGER_25, NPC_ALLIGSTRIGGER_10, NPC_ALLIGSTRIGGER_25);
					uint32 m_HORDEGSTRIGGER = RAID_MODE(NPC_HORDEGSTRIGGER_10, NPC_HORDEGSTRIGGER_25, NPC_HORDEGSTRIGGER_10, NPC_HORDEGSTRIGGER_25);

					if (Creature *m_AlliGS = me->SummonCreature(m_ALLIGSTRIGGER, 18.637676f, 2247.624023f, 527.277039f))
					{
						instance->SendEncounterUnit(ENCOUNTER_FRAME_ADD, m_AlliGS);
					}

					if (Creature *m_HordeGS = me->SummonCreature(m_HORDEGSTRIGGER, -71.189644f, 2226.092285f, 527.117432f))
					{
						instance->SendEncounterUnit(ENCOUNTER_FRAME_ADD, m_HordeGS);
					}

					uint32 Count = RAID_MODE(2, 4, 2, 4);

					if (Count == 4)
					{
						if (Creature *canon = me->SummonCreature(NPC_GUNSHIP_CANNON_HORDE, -38.841675f, 2258.511963f, 526.378174f))
							canon->SetOrientation(6.169659f);
						if (Creature *canon = me->SummonCreature(NPC_GUNSHIP_CANNON_HORDE, -38.193577f, 2253.306885f, 526.427917f))
							canon->SetOrientation(6.169659f);
						if (Creature *canon = me->SummonCreature(NPC_GUNSHIP_CANNON_HORDE, -39.107193f, 2236.982422f, 526.979919f))
							canon->SetOrientation(6.169659f);
						if (Creature *canon = me->SummonCreature(NPC_GUNSHIP_CANNON_HORDE, -38.604717f, 2241.389404f, 526.657288f))
							canon->SetOrientation(6.169659f);
					}else{
						if (Creature *canon = me->SummonCreature(NPC_GUNSHIP_CANNON_HORDE, -38.841675f, 2258.511963f, 526.378174f))
							canon->SetOrientation(6.169659f);
						if (Creature *canon = me->SummonCreature(NPC_GUNSHIP_CANNON_HORDE, -38.604717f, 2241.389404f, 526.657288f))
							canon->SetOrientation(6.169659f);
					}

					//me->SummonCreature(NPC_CANONTRIGGER, 39.714436f, 2230.390625f, 528.113281f);
					//me->SummonCreature(NPC_CANONTRIGGER, 43.005005f, 2256.538574f, 528.131104f);

					me->SummonCreature(NPC_FIGHTTRIGGER, 26.312359f, 2246.370850f, 527.267151f);
				}
			case DO_ACTION_START_GUNSHIP_BATTLE :
				{
					if (!bBattleStart)
					{
						instance->SetBossState(DATA_GUNSHIP_BATTLE_EVENT, IN_PROGRESS);

						//instance->SetBossState( DATA_GUNSHIP_BATTLE_EVENT, NOT_STARTED);
						//instance->SetData( DATA_GUNSHIP_EVENT, PHASE_NOT_STARTED);
						bDialog = true;
						dialogID = 0;
						dialogTimer = 1000;
					}
					break;
				}
			case DO_ACTION_FINISH_GUNSHIP_BATTLE :
				{
					Position tp = DeathbringerRise;
					Map::PlayerList const &PlayerList = me->GetMap()->GetPlayers();
					if( !PlayerList.isEmpty())
					{
						for(Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
						{
							if( Player *pCurrent = i->getSource())
								if( me->GetDistance(pCurrent) <= 100.0f)
									DoTeleportPlayer( pCurrent, tp.GetPositionX(), tp.GetPositionY(), tp.GetPositionZ(), tp.GetOrientation());
						}
					}
					break;
				}
			}
		}

		uint32 health_theSkybreaker;
		uint32 health_ogrimsHammer;

		std::list<Creature*> lSummons;

		uint32 timer_Cleave;
		uint32 timer_RendingThrow;
		uint32 timer_TasteOfBlood;

		bool bBattleStart;
		bool bDialog;
		uint32 dialogID;
		uint32 dialogTimer;
	};

	bool OnGossipHello(Player* pPlayer, Creature* pCreature)
	{
		//if ( pPlayer->GetTeamId() == TEAM_ALLIANCE)
		//    return false;

		InstanceScript* pInstance = pCreature->GetInstanceScript();
		if (pInstance && (pInstance->GetBossState(DATA_GUNSHIP_BATTLE_EVENT) == NOT_STARTED  || pInstance->GetBossState(DATA_GUNSHIP_BATTLE_EVENT) == TO_BE_DECIDED))
		{
			pPlayer->ADD_GOSSIP_ITEM(0, "Ready for battle", 631, HORDE_GOSSIP_ACTION_START);
			pPlayer->ADD_GOSSIP_ITEM(0, "Return Later", 631, HORDE_GOSSIP_ACTION_CANCEL);
			pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());

			return true;
		}

		if (pInstance && pInstance->GetBossState(DATA_GUNSHIP_BATTLE_EVENT) == DONE)
		{
			pPlayer->ADD_GOSSIP_ITEM(0, "Go to the top of the Deathbringer", 631, HORDE_GOSSIP_ACTION_TP);
			pPlayer->ADD_GOSSIP_ITEM(0, "Return Later", 631, HORDE_GOSSIP_ACTION_CANCEL);
			pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());

			return true;
		}
		return false;
	}

	bool OnGossipSelect(Player* player, Creature* pCreature, uint32 /*sender*/, uint32 action)
	{
		//player->PlayerTalkClass->ClearMenus();
		player->CLOSE_GOSSIP_MENU();

		switch( action) {
		case HORDE_GOSSIP_ACTION_START :
			pCreature->AI()->DoAction( DO_ACTION_START_GUNSHIP_BATTLE); // Comienzo !
			return true;
		case HORDE_GOSSIP_ACTION_TP :
			pCreature->AI()->DoAction( DO_ACTION_FINISH_GUNSHIP_BATTLE); // Fin !
			return true;
		case HORDE_GOSSIP_ACTION_CANCEL :
			return true;
		};

		return false;
	}

	CreatureAI* GetAI(Creature* pCreature) const
	{
		return new boss_high_overlord_varok_saurfangAI(pCreature);
	}
};

class npc_sergeant : public CreatureScript
{
public:
	npc_sergeant() : CreatureScript("npc_sergeant") { }

	struct npc_sergeantAI : public ScriptedAI
	{
		npc_sergeantAI(Creature* pCreature) : ScriptedAI(pCreature)
		{ pInstance = me->GetInstanceScript(); }

		void Reset()
		{	
			Unit *target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, false, SPELL_ON_ORGRIMS_HAMMERS_DECK);
			if (target)
			{
				me->SetReactState(REACT_AGGRESSIVE);
				me->AI()->EnterCombat(target);
			}

			timer_Bladestorm = 7000;
			timer_WoundingStrike = urand( 10000, 12000);
			timer_BurningPitch = 60000;
			timer_DesperateResolve = 0; // -40% de Vida
			timer_Spawn = 0 ;
			DoZoneInCombat();
		}

		void EnterCombat(Unit* /*pWho*/)
		{
			Unit *target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, false, SPELL_ON_ORGRIMS_HAMMERS_DECK);
			//DoZoneInCombat();

		}

		void JustReachedHome()
		{
			me->SetReactState(REACT_AGGRESSIVE);
		}
		void UpdateAI( const uint32 diff)
		{
			timer_Spawn += diff;
			buffExp( this, timer_Spawn);

			if (pInstance && ( pInstance->GetBossState(DATA_GUNSHIP_BATTLE_EVENT) == DONE || pInstance->GetBossState(DATA_GUNSHIP_BATTLE_EVENT) == NOT_STARTED))
				me->DisappearAndDie();

			if (!UpdateVictim())
				return;
			// Need a Check , creature should not leaf the Bout.

			if (!me->getVictim()->HasAura(SPELL_ON_ORGRIMS_HAMMERS_DECK))
			{
				Unit *target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, false, SPELL_ON_ORGRIMS_HAMMERS_DECK);
				if (!target)
				{
					me->SetReactState(REACT_PASSIVE);
					me->getThreatManager().resetAllAggro();
					EnterEvadeMode();								
					return;
					//me->GetMotionMaster()->MovePoint(0,me->GetHomePosition().GetPositionX(),me->GetHomePosition().GetPositionY(),me->GetHomePosition().GetPositionZ());
				}
				else 
				{
					me->AI()->EnterCombat(target);
				}
			}


			if( ( me->GetHealth() / me->GetMaxHealth()) <= 0.4) {
				if( timer_DesperateResolve <= diff) {
					DoCast( me, RAID_MODE( SPELL_DESPERATE_RESOLVE_10_NM, SPELL_DESPERATE_RESOLVE_25_NM, SPELL_DESPERATE_RESOLVE_10_HM, SPELL_DESPERATE_RESOLVE_25_HM));
					timer_DesperateResolve = 120000;
				} else timer_DesperateResolve -= diff;
			}

			if( timer_Bladestorm <= diff) {
				DoCast( me, SPELL_BLADESTORM);

				timer_Bladestorm = 7000;
			} else timer_Bladestorm -= diff;

			if( timer_BurningPitch <= diff) {
				if( Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 10000, true)) {
					DoCast( pTarget, SPELL_BURNING_PITCH);
					timer_BurningPitch = 60000;
				}
			} else timer_BurningPitch -= diff;

			if( timer_WoundingStrike <= diff) {
				DoCastVictim( RAID_MODE( SPELL_WOUNDING_STRIKE_10_NM, SPELL_WOUNDING_STRIKE_25_NM, SPELL_WOUNDING_STRIKE_10_HM, SPELL_WOUNDING_STRIKE_25_HM));
				timer_WoundingStrike = urand( 10000, 12000);
			} else timer_WoundingStrike -= diff;

			DoMeleeAttackIfReady();
		}

		uint32 timer_Bladestorm;
		uint32 timer_WoundingStrike;
		uint32 timer_DesperateResolve;
		uint32 timer_BurningPitch;

		uint32 timer_Spawn;
		InstanceScript* pInstance;
	};

	CreatureAI* GetAI(Creature* pCreature) const
	{
		return new npc_sergeantAI(pCreature);
	}
};

/* transport script */
class transport_gunship : public GameObjectScript
{
public:
	transport_gunship() : GameObjectScript("transport_gunship") { }

	struct transport_gunshipAI : public GameObjectAI
	{
		transport_gunshipAI(GameObject* pGO) : GameObjectAI(pGO)
		{ /*pInstance = me->GetInstanceScript();*/ }

		/*void OnAddPassenger(Transport* transport, Player* player)
		{
		//*if (InstanceScript* instance = transport->GetInstanceScript())
		//{
		switch (transport->GetEntry())
		{
		case 1915003:
		player->AddAura(SPELL_ON_ORGRIMS_HAMMERS_DECK, player);
		break;
		case ALLIANCE:
		player->AddAura(SPELL_ON_SKYBREAKERS_DECK, player);
		break;
		}
		//}
		}

		void OnRemovePassenger(Transport* transport, Player* player)
		{
		player->RemoveAurasDueToSpell(SPELL_ON_ORGRIMS_HAMMERS_DECK);
		player->RemoveAurasDueToSpell(SPELL_ON_SKYBREAKERS_DECK);
		}

		void OnGameObjectUpdate(GameObject* go, uint32 diff)
		{
		sLog->outError("ds gameobjectupdate *******************************************");

		}*/
	};

	GameObjectAI* GetAI(GameObject* pGO) const
	{
		return new transport_gunshipAI(pGO);

	};

};

class npc_skybreaker_sorcerer : public CreatureScript
{
public:
	npc_skybreaker_sorcerer() : CreatureScript("npc_skybreaker_sorcerer") { }

	struct npc_skybreaker_sorcererAI : public ScriptedAI
	{
		npc_skybreaker_sorcererAI(Creature* pCreature) : ScriptedAI(pCreature)
		{pInstance = me->GetInstanceScript();}

		void Reset()
		{
			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
		}
		void UpdateAI( const uint32 diff)
		{
			
			if (pInstance && ( pInstance->GetBossState(DATA_GUNSHIP_BATTLE_EVENT) == DONE || pInstance->GetBossState(DATA_GUNSHIP_BATTLE_EVENT) == NOT_STARTED))
				me->DisappearAndDie();
		}

		InstanceScript* pInstance;
	};

	CreatureAI* GetAI(Creature* pCreature) const
	{
		return new npc_skybreaker_sorcererAI(pCreature);
	}
};

class npc_skybreaker_rifleman : public CreatureScript
{
public:
	npc_skybreaker_rifleman() : CreatureScript("npc_skybreaker_rifleman") { }

	struct npc_skybreaker_riflemanAI : public ScriptedAI
	{
		npc_skybreaker_riflemanAI(Creature* pCreature) : ScriptedAI(pCreature)
		{ pInstance = me->GetInstanceScript(); }

		void Reset()
		{
			Unit *target = SelectTarget(SELECT_TARGET_RANDOM, 0, 400.0f, false, SPELL_ON_ORGRIMS_HAMMERS_DECK);
			if (target)
			{
				me->SetReactState(REACT_AGGRESSIVE);
				me->AI()->EnterCombat(target);
			}

			timer_Shoot = urand( 2000, 4000);
			timer_BurningPitch = 60000;
			timer_Spawn =0 ;
			DoZoneInCombat();
			//me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
		}

		void EnterCombat(Unit* /*who*/)
		{
			/*Unit *target = SelectTarget(SELECT_TARGET_RANDOM, 0, 400.0f, false, SPELL_ON_ORGRIMS_HAMMERS_DECK);
			if (target)
			{
			me->SetReactState(REACT_AGGRESSIVE);
			me->AI()->EnterCombat(target);
			}*/
			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
			//SetCombatMovement(false);
		}

		void UpdateAI( const uint32 diff)
		{
			timer_Spawn += diff;
			buffExp( this, timer_Spawn);

			if (pInstance && ( pInstance->GetBossState(DATA_GUNSHIP_BATTLE_EVENT) == DONE || pInstance->GetBossState(DATA_GUNSHIP_BATTLE_EVENT) == NOT_STARTED))
				me->DisappearAndDie();

			Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 400.0f, true, SPELL_ON_ORGRIMS_HAMMERS_DECK);
			if (!target)
				return;

			if (!UpdateVictim())
				return;

			if( timer_BurningPitch <= diff) 
			{
				if( Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 10000, true, SPELL_ON_ORGRIMS_HAMMERS_DECK))
				{
					std::list<Creature*> CanonList;
					me->GetCreatureListWithEntryInGrid(CanonList, NPC_GUNSHIP_CANNON_HORDE, 200.0f);
					bool OnCanon = false;
					if (!CanonList.empty())
					{
						for (std::list<Creature*>::const_iterator itr = CanonList.begin(); itr != CanonList.end(); ++itr)
						{
							Unit *Canon = *itr;
							if (pTarget->IsOnVehicle(Canon))
								OnCanon = true;

						}
					}

					if (OnCanon)
						return;   // double check si ds canon

					DoCast( pTarget, SPELL_BURNING_PITCH);
					timer_BurningPitch = 60000;
				}
			} else timer_BurningPitch -= diff;

			if( timer_Shoot <= diff) 
			{
				if( Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 10000, true,SPELL_ON_ORGRIMS_HAMMERS_DECK))
				{
					std::list<Creature*> CanonList;
					me->GetCreatureListWithEntryInGrid(CanonList, NPC_GUNSHIP_CANNON_HORDE, 200.0f);
					bool OnCanon = false;
					if (!CanonList.empty())
					{
						for (std::list<Creature*>::const_iterator itr = CanonList.begin(); itr != CanonList.end(); ++itr)
						{
							Unit *Canon = *itr;
							if (pTarget->IsOnVehicle(Canon))
								OnCanon = true;
						}
					}

					if (OnCanon)
						return;   // double check si ds canon

					DoCast(pTarget, RAID_MODE( SPELL_SHOOT_10_NM, SPELL_SHOOT_25_NM, SPELL_SHOOT_10_HM, SPELL_SHOOT_25_HM));
					timer_Shoot = urand( 2000, 4000);
				}
			} else timer_Shoot -= diff;
		}

		uint32 timer_Shoot;
		uint32 timer_BurningPitch;

		uint32 timer_Spawn;
		InstanceScript* pInstance;
	};

	CreatureAI* GetAI(Creature* pCreature) const
	{
		return new npc_skybreaker_riflemanAI(pCreature);
	}
};

class npc_korkron_axethrower : public CreatureScript
{
public:
	npc_korkron_axethrower() : CreatureScript("npc_korkron_axethrower") { }

	struct npc_korkron_axethrowerAI : public ScriptedAI
	{
		npc_korkron_axethrowerAI(Creature* pCreature) : ScriptedAI(pCreature)
		{ pInstance = me->GetInstanceScript(); }

		void Reset()
		{
			Player *target = me->SelectNearestPlayer(400.0f);
			me->AI()->AttackStart(target);

			timer_HurlAxe = urand( 2000, 4000);
			timer_BurningPitch = 60000;
			timer_Spawn =0 ;
			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
		}

		void EnterCombat(Unit* /*who*/)
		{
			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
			SetCombatMovement(false);
		}

		void UpdateAI( const uint32 diff)
		{
			timer_Spawn += diff;
			buffExp( this, timer_Spawn);

			if (pInstance && pInstance->GetBossState(DATA_GUNSHIP_BATTLE_EVENT) == DONE)
				me->Kill(me);

			if (!UpdateVictim())
				return;

			if( timer_BurningPitch <= diff) {
				if( Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 10000, true)) {
					DoCast( pTarget, SPELL_BURNING_PITCH);
					timer_BurningPitch = 60000;
				}
			} else timer_BurningPitch -= diff;

			if( timer_HurlAxe <= diff) {
				DoCastVictim( RAID_MODE( SPELL_HURL_AXE_10_NM, SPELL_HURL_AXE_25_NM, SPELL_HURL_AXE_10_HM, SPELL_HURL_AXE_25_HM));
				timer_HurlAxe = urand( 2000, 4000);
			} else timer_HurlAxe -= diff;
		}

		uint32 timer_HurlAxe;
		uint32 timer_BurningPitch;

		uint32 timer_Spawn;
		InstanceScript* pInstance;
	};

	CreatureAI* GetAI(Creature* pCreature) const
	{
		return new npc_korkron_axethrowerAI(pCreature);
	}
};

class npc_marine_or_reaver : public CreatureScript
{
public:
	npc_marine_or_reaver() : CreatureScript("npc_marine_or_reaver") { }

	struct npc_marine_or_reaverAI : public ScriptedAI
	{
		npc_marine_or_reaverAI(Creature* pCreature) : ScriptedAI(pCreature)
		{ pInstance = me->GetInstanceScript(); }

		void Reset()
		{
			Unit *target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, false, SPELL_ON_ORGRIMS_HAMMERS_DECK);
			if (target)
			{
				me->SetReactState(REACT_AGGRESSIVE);
				me->AI()->EnterCombat(target);
			}
			timer_Spawn = 0 ;
			timer_BurningPitch = 60000;
			timer_DesperateResolve = 0;
		}

		void EnterCombat(Unit* /*pWho*/)
		{
			Unit *target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true, SPELL_ON_ORGRIMS_HAMMERS_DECK);
			DoZoneInCombat();

		}

		void JustReachedHome()
		{
			me->SetReactState(REACT_AGGRESSIVE);
		}
		void UpdateAI( const uint32 diff)
		{
			timer_Spawn += diff;
			buffExp( this, timer_Spawn);

			if (pInstance && ( pInstance->GetBossState(DATA_GUNSHIP_BATTLE_EVENT) == DONE || pInstance->GetBossState(DATA_GUNSHIP_BATTLE_EVENT) == NOT_STARTED))
				me->DisappearAndDie();

			if (!UpdateVictim())
				return;

			// Need a Check , creature should not leaf the Bout.
			if (!me->SelectVictim()->HasAura(SPELL_ON_ORGRIMS_HAMMERS_DECK))
			{
				Unit *target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, false, SPELL_ON_ORGRIMS_HAMMERS_DECK);
				if (!target)
				{
					me->SetReactState(REACT_PASSIVE);
					me->getThreatManager().resetAllAggro();
					EnterEvadeMode();
					return;
				}
				else 
				{
					me->AI()->EnterCombat(target);
				}
			}

			if(( me->GetHealth() / me->GetMaxHealth()) <= 0.4) {
				if( timer_DesperateResolve <= diff) {
					DoCast( me, RAID_MODE( SPELL_DESPERATE_RESOLVE_10_NM, SPELL_DESPERATE_RESOLVE_25_NM, SPELL_DESPERATE_RESOLVE_10_HM, SPELL_DESPERATE_RESOLVE_25_HM));
					timer_DesperateResolve = 120000;
				} else timer_DesperateResolve -= diff;
			}

			if( timer_BurningPitch <= diff) {
				if( Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 10000, true)) {
					DoCast( pTarget, SPELL_BURNING_PITCH);
					timer_BurningPitch = 60000;
				}
			} else timer_BurningPitch -= diff;

			DoMeleeAttackIfReady();
		}

		uint32 timer_DesperateResolve;
		uint32 timer_BurningPitch;

		uint32 timer_Spawn;
		InstanceScript* pInstance;
	};

	CreatureAI* GetAI(Creature* pCreature) const
	{
		return new npc_marine_or_reaverAI(pCreature);
	}
};

class npc_mortar_soldier_or_rocketeer : public CreatureScript
{
public:
	npc_mortar_soldier_or_rocketeer() : CreatureScript("npc_mortar_soldier_or_rocketeer") { }

	struct npc_mortar_soldier_or_rocketeerAI : public ScriptedAI
	{
		npc_mortar_soldier_or_rocketeerAI(Creature* pCreature) : ScriptedAI(pCreature)
		{pInstance = me->GetInstanceScript();}

		void Reset()
		{
			Unit *target = SelectTarget(SELECT_TARGET_RANDOM, 0, 400.0f, true, SPELL_ON_ORGRIMS_HAMMERS_DECK);
			if (target)
			{
				me->SetReactState(REACT_AGGRESSIVE);
				me->AI()->EnterCombat(target);
			}
			DoZoneInCombat();
			timer_RocketArtillery = urand( 10000, 15000);
			timer_BurningPitch = 60000;
			timer_Spawn = 0 ;
			//me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
		}

		void EnterCombat(Unit* /*who*/)
		{
			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
			//SetCombatMovement(false);
		}

		void UpdateAI( const uint32 diff)
		{
			timer_Spawn += diff;
			buffExp( this, timer_Spawn);

			if (pInstance && (pInstance->GetBossState(DATA_GUNSHIP_BATTLE_EVENT) == DONE || pInstance->GetBossState(DATA_GUNSHIP_BATTLE_EVENT) == NOT_STARTED))
				me->DisappearAndDie();

			Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 400.0f, true, SPELL_ON_ORGRIMS_HAMMERS_DECK);
			if (!target)
				return;


			if (!UpdateVictim())
				return;

			if( timer_BurningPitch <= diff)
			{
				if( Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 10000, true,SPELL_ON_ORGRIMS_HAMMERS_DECK))
				{
					std::list<Creature*> CanonList;
					me->GetCreatureListWithEntryInGrid(CanonList, NPC_GUNSHIP_CANNON_HORDE, 200.0f);
					bool OnCanon = false;
					if (!CanonList.empty())
					{
						for (std::list<Creature*>::const_iterator itr = CanonList.begin(); itr != CanonList.end(); ++itr)
						{
							Unit *Canon = *itr;
							if (pTarget->IsOnVehicle(Canon))
								OnCanon = true;

						}
					}

					if (OnCanon)
						return;   // double check si ds canon

					DoCast( pTarget, SPELL_BURNING_PITCH);
					timer_BurningPitch = 60000;
				}
			} else timer_BurningPitch -= diff;

			if( timer_RocketArtillery <= diff) {
				Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 400.0f, true, SPELL_ON_ORGRIMS_HAMMERS_DECK);
				if (pTarget)
				{

					std::list<Creature*> CanonList;
					me->GetCreatureListWithEntryInGrid(CanonList, NPC_GUNSHIP_CANNON_HORDE, 200.0f);
					bool OnCanon = false;
					if (!CanonList.empty())
					{
						for (std::list<Creature*>::const_iterator itr = CanonList.begin(); itr != CanonList.end(); ++itr)
						{
							Unit *Canon = *itr;
							if (pTarget->IsOnVehicle(Canon))
								OnCanon = true;

						}
					}

					if (OnCanon)
						return;   // double check si ds canon

					Position pos;
					pTarget->GetPosition(&pos);
					mortarMarksLoc markPos;
					markPos.durationBeforeRefreshing = 5000;
					markPos.location = pos;
					marks.push_back(markPos);
					me->CastSpell(pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), SPELL_ROCKET_ARTILLERY_TRIGGERED, true);
					me->CastSpell(markPos.location.GetPositionX(), markPos.location.GetPositionY(), markPos.location.GetPositionZ(), SPELL_ROCKET_ARTILLERY_MARKER, true);
				}
				timer_RocketArtillery   =       urand( 10000, 15000);
			} else timer_RocketArtillery -= diff;
		}



		uint32 timer_RocketArtillery;
		uint32 timer_BurningPitch;

		uint32 timer_Spawn;
		InstanceScript* pInstance;
		std::list<mortarMarksLoc> marks;
		std::list<mortarMarksLoc> getMarksPositions() { return marks; }
		bool setMarks(std::list<mortarMarksLoc> newMarks) { marks = newMarks; }
	};

	CreatureAI* GetAI(Creature* pCreature) const
	{
		return new npc_mortar_soldier_or_rocketeerAI(pCreature);
	}
};

class npc_sorcerer_or_battle_mage : public CreatureScript
{
public:
	npc_sorcerer_or_battle_mage() : CreatureScript("npc_sorcerer_or_battle_mage") { }

	struct npc_sorcerer_or_battle_mageAI : public ScriptedAI
	{
		npc_sorcerer_or_battle_mageAI(Creature* pCreature) : ScriptedAI(pCreature)
		{ pInstance = me->GetInstanceScript(); }

		void Reset()
		{
			timer_BurningPitch = 60000;
			timer_BelowZero = urand( 10000, 15000);
			timer_Spawn = 0 ;
			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
			DoZoneInCombat();
		}


		void JustDied(Unit* /*killer*/)
		{
			std::list<Creature*> CanonList;
			me->GetCreatureListWithEntryInGrid(CanonList, NPC_GUNSHIP_CANNON_HORDE, 200.0f);
			if (!CanonList.empty())
			{
				for (std::list<Creature*>::const_iterator itr = CanonList.begin(); itr != CanonList.end(); ++itr)
				{
					if (Creature* pTarget = *itr)
						pTarget->RemoveAura(SPELL_BELOW_ZERO);
					// DoCast( pTarget, SPELL_BELOW_ZERO);
				}
			}
		}

		void UpdateAI( const uint32 diff)
		{
			timer_Spawn += diff;
			buffExp( this, timer_Spawn);

			if (pInstance && (pInstance->GetBossState(DATA_GUNSHIP_BATTLE_EVENT) == DONE || pInstance->GetBossState(DATA_GUNSHIP_BATTLE_EVENT) == NOT_STARTED))
				me->DisappearAndDie();

			if (!UpdateVictim())
				return;

			if( timer_BurningPitch <= diff) {
				if( Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 10000, true)) {
					DoCast( pTarget, SPELL_BURNING_PITCH);
					timer_BurningPitch = 60000;
				}
			} else timer_BurningPitch -= diff;

			if( timer_BelowZero <= diff) {
				/*std::CreatureList const &pCreature = *///me->GetCreatureListWithEntryInGrid(;

				std::list<Creature*> CanonList;
				me->GetCreatureListWithEntryInGrid(CanonList, NPC_GUNSHIP_CANNON_HORDE, 200.0f);
				if (!CanonList.empty())
				{
					for (std::list<Creature*>::const_iterator itr = CanonList.begin(); itr != CanonList.end(); ++itr)
					{
						if (Creature* pTarget = *itr)
							DoCast( pTarget, SPELL_BELOW_ZERO);
					}
				}
				timer_BelowZero = urand( 10000, 15000);
			} else timer_BelowZero -= diff;
		}

		uint32 timer_BelowZero;
		uint32 timer_BurningPitch;

		uint32 timer_Spawn;
		InstanceScript* pInstance;
	};

	CreatureAI* GetAI(Creature* pCreature) const
	{
		return new npc_sorcerer_or_battle_mageAI(pCreature);
	}
};

class npc_combattrigger : public CreatureScript
{
public:
	npc_combattrigger() : CreatureScript("npc_combattrigger") { }

	struct npc_combattriggerAI : public ScriptedAI
	{
		npc_combattriggerAI(Creature* pCreature) : ScriptedAI(pCreature)
		{pInstance = me->GetInstanceScript();}

		void Reset()
		{
			CaseType = 1;
			CaseTimer = 10000;
			CheckPlayerTimer = 1000;
			bWype = false;

			//Creature_Pool For big reset on whipe or other
			//Skybreaker creature
			Creature_Pool.push_back(NPC_SKYBREAKER_MORTAR_SOLDIER);
			Creature_Pool.push_back(NPC_SKYBREAKER_RIFLEMAN);
			Creature_Pool.push_back(NPC_SKYBREAKER_SORCERER);
			Creature_Pool.push_back(NPC_SKYBREAKER_SORCERER_CANON);

			//Orgrim Hamer creature
			Creature_Pool.push_back(NPC_PORTAL);
			Creature_Pool.push_back(NPC_SKYBREAKER_SERGEANT);
			Creature_Pool.push_back(NPC_SKYBREAKER_MARINE);
			Creature_Pool.push_back(NPC_GUNSHIP_CANNON_HORDE);
			Creature_Pool.push_back(NPC_ZAFOD_BOOMBOX);

			//Trigger
			Creature_Pool.push_back(RAID_MODE(NPC_ALLIGSTRIGGER_10, NPC_ALLIGSTRIGGER_25, NPC_ALLIGSTRIGGER_10, NPC_ALLIGSTRIGGER_25));
			Creature_Pool.push_back(RAID_MODE(NPC_HORDEGSTRIGGER_10, NPC_HORDEGSTRIGGER_25, NPC_HORDEGSTRIGGER_10, NPC_HORDEGSTRIGGER_25));

			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_DISABLE_MOVE);

			Map::PlayerList const &players = me->GetMap()->GetPlayers();
			for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
			{
				player = itr->getSource();
				if (!player->isGameMaster()) 
				{
					me->AI()->AttackStart(player);
					break;
				}
			}
		}

		void EnterCombat(Unit* /*who*/)
		{
			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_DISABLE_MOVE);
			SetCombatMovement(false);
			uint32 Count = RAID_MODE(2, 4, 2, 4);

			// Summon Deffer
			if (Count == 2)
			{
				me->SummonCreature(NPC_SKYBREAKER_MORTAR_SOLDIER, 19.905600f, 2229.406738f, 526.922729f);
				me->SummonCreature(NPC_SKYBREAKER_MORTAR_SOLDIER, 21.892221f, 2244.537354f, 526.904419f);
			}
			else
			{
				me->SummonCreature(NPC_SKYBREAKER_MORTAR_SOLDIER, 19.905600f, 2229.406738f, 526.922729f);
				me->SummonCreature(NPC_SKYBREAKER_MORTAR_SOLDIER, 30.509739f, 2229.745361f, 526.906372f);
				me->SummonCreature(NPC_SKYBREAKER_MORTAR_SOLDIER, 21.892221f, 2244.537354f, 526.904419f);
				me->SummonCreature(NPC_SKYBREAKER_MORTAR_SOLDIER, 30.448759f, 2239.410156f, 526.872314f);
			}

			//me->SummonCreature(NPC_SKYBREAKER_MORTAR_SOLDIER, 16.182817f, 2250.926025f, 526.981812f);

			me->SummonCreature(NPC_SKYBREAKER_RIFLEMAN, -6.278732f, 2239.198975f, 528.106934f);
			me->SummonCreature(NPC_SKYBREAKER_RIFLEMAN, -5.815511f, 2244.536865f, 528.089905f);
			me->SummonCreature(NPC_SKYBREAKER_RIFLEMAN, -4.562603f, 2250.770508f, 528.068665f);
			me->SummonCreature(NPC_SKYBREAKER_RIFLEMAN, -3.562603f, 2257.770508f, 528.068665f);

			me->SummonCreature(NPC_MURADIN_BRONZEBEARD,13.676f,2216.89f,527.26f,1.29f);
			me->SummonCreature(NPC_SKYBREAKER_SORCERER,20.09f,2216.051f,527.26f,1.29f);
			me->SummonCreature(NPC_SKYBREAKER_SORCERER,7.85f,2217.491f,527.26f,1.29f);

		}

		void Despawn_Creature()
		{
			for (std::list<uint32>::iterator itr = Creature_Pool.begin(); itr != Creature_Pool.end(); ++itr)
			{
				std::list<Creature*> CreatureList;
				uint32 Id;
				Id = *itr;
				me->GetCreatureListWithEntryInGrid(CreatureList, Id, 400.0f);
				for (std::list<Creature*>::iterator itrbis = CreatureList.begin(); itrbis != CreatureList.end(); ++itrbis)
				{
					Creature *Summoned = *itrbis;
					Summoned->DisappearAndDie();
				}

			}
		}



		void UpdateAI( const uint32 diff)
		{
			if (pInstance && (pInstance->GetBossState(DATA_GUNSHIP_BATTLE_EVENT) == DONE || pInstance->GetBossState(DATA_GUNSHIP_BATTLE_EVENT) == NOT_STARTED))
				me->DisappearAndDie();

			if (CheckPlayerTimer <= diff)
			{
				GameObject * GobGSH =   GameObject::GetGameObject(*me, pInstance->GetData64(GO_HORDE_GUNSHIP_BATTLE)) ;
				GameObject * GobGSA =   GameObject::GetGameObject(*me, pInstance->GetData64(GO_ALLI_GUNSHIP_BATTLE)) ;

				bWype = true;
				Map::PlayerList const &players = me->GetMap()->GetPlayers();
				for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
				{
					player = itr->getSource();
					if (!player)
					{
						bWype = true;
						break;
					}

					if (/*player->isGameMaster() || */!player->isAlive())
					{
						bWype = true;
						continue;
					}

					if (player->isAlive())
						bWype = false;

					// AI of canon disable when mounted so need to check here
					bool OnCanon = false;

					std::list<Creature*> CanonList;
					me->GetCreatureListWithEntryInGrid(CanonList, NPC_GUNSHIP_CANNON_HORDE, 200.0f);
					if (!CanonList.empty())
					{
						for (std::list<Creature*>::const_iterator itr = CanonList.begin(); itr != CanonList.end(); ++itr)
						{
							Unit *Canon = *itr;

							if (player->IsOnVehicle(Canon))
								OnCanon = true;

							Canon->SetFullHealth(); 

							if (Canon->GetPower(Canon->getPowerType()) == 100)
							{
								if (!Canon->HasAura(69487))
								{
									Canon->AddAura(SPELL_OVERHEAT,Canon);
									Canon->CastSpell(Canon,SPELL_OVERHEAT,false);
									Canon->SetPower(Canon->getPowerType(),90);
								}
							}

						}
					}
					if (OnCanon)
					{
						player->RemoveAura(SPELL_ON_ORGRIMS_HAMMERS_DECK);
						player->RemoveAura(SPELL_ON_SKYBREAKERS_DECK);
						continue;
					}

					if((player->GetDistance2d(GobGSH) - GobGSH->GetObjectSize()) < 36.0f)
					{
						if(!player->HasAura(SPELL_ON_ORGRIMS_HAMMERS_DECK))
						{
							player->AddAura(SPELL_ON_ORGRIMS_HAMMERS_DECK,player);
							continue;
						}
					}else player->RemoveAura(SPELL_ON_ORGRIMS_HAMMERS_DECK);

					if((player->GetDistance2d(GobGSA) - GobGSA->GetObjectSize()) < 33.0f)
					{
						if(!player->HasAura(SPELL_ON_SKYBREAKERS_DECK))
						{


							player->AddAura(SPELL_ON_SKYBREAKERS_DECK,player);
							me->FindNearestCreature(36948,200.0f)->AI()->AttackStart(player);
							continue;
						}
					}else player->RemoveAura(SPELL_ON_SKYBREAKERS_DECK);

				}

				CheckPlayerTimer = 1000;
			}

			else CheckPlayerTimer -= diff;

			////////////////////////////////// WHYPE
			if (bWype == true )
			{
				if ( Creature* cToReplace = me->FindNearestCreature(NPC_SAUFRANG_G,200.0f))
				{
					cToReplace->NearTeleportTo(-436.92f,1998.66f,191.24f,4.67f);
					cToReplace->AI()->Reset();
				}
				if (Creature* cToReplace = me->FindNearestCreature(NPC_MURADIN_BRONZEBEARD,200.0f))
				{				
					cToReplace->CombatStop();
					cToReplace->DisappearAndDie();
				}

				uint32 m_ALLIGSTRIGGER = RAID_MODE(NPC_ALLIGSTRIGGER_10, NPC_ALLIGSTRIGGER_25, NPC_ALLIGSTRIGGER_10, NPC_ALLIGSTRIGGER_25);
				uint32 m_HORDEGSTRIGGER = RAID_MODE(NPC_HORDEGSTRIGGER_10, NPC_HORDEGSTRIGGER_25, NPC_HORDEGSTRIGGER_10, NPC_HORDEGSTRIGGER_25);

				if (Creature *m_AlliGS = me->FindNearestCreature(m_ALLIGSTRIGGER,400.0f,false))
				{
					pInstance->SendEncounterUnit(ENCOUNTER_FRAME_REMOVE, m_AlliGS);
					m_AlliGS->DisappearAndDie();
				}

				if (Creature *m_HordeGS = me->FindNearestCreature(m_HORDEGSTRIGGER,400.0f,false))
				{
					pInstance->SendEncounterUnit(ENCOUNTER_FRAME_REMOVE, m_HordeGS);
					m_HordeGS->DisappearAndDie();
				}

				//Despawn_Creature();
				pInstance->SetBossState(DATA_GUNSHIP_BATTLE_EVENT, NOT_STARTED);
				//me->DisappearAndDie();
				return;
			}



			if (CaseTimer <= diff)
			{
				switch (CaseType)
				{
				case 1 :
					{
						// Sumon Portal
						if  (me->FindNearestCreature(NPC_SKYBREAKER_SORCERER,100.0f,true) )
							me->SummonCreature(NPC_PORTAL, -70.393806f, 2235.044678f, 526.761536f);

						CaseTimer = 4000;
						CaseType = 2;
						break;
					}
				case 2:
					{

						std::list<Creature*> MortarList;
						me->GetCreatureListWithEntryInGrid(MortarList, NPC_SKYBREAKER_MORTAR_SOLDIER, 100.0f);
						if (!MortarList.empty())
						{
							for (std::list<Creature*>::const_iterator itr = MortarList.begin(); itr != MortarList.end(); ++itr)
							{
								if (Creature* pTarget = *itr)
									if ( pTarget->isAlive() )
										continue;
									else
										pTarget->Respawn();
							}
						}

						std::list<Creature*> RiflemanList;
						me->GetCreatureListWithEntryInGrid(RiflemanList, NPC_SKYBREAKER_RIFLEMAN, 100.0f);
						if (!RiflemanList.empty())
						{
							for (std::list<Creature*>::const_iterator itr = RiflemanList.begin(); itr != RiflemanList.end(); ++itr)
							{
								if (Creature* pTarget = *itr)
									if ( pTarget->isAlive() )
										continue;
									else
										pTarget->Respawn();
							}
						}

						std::list<Creature*> SorcererList;
						me->GetCreatureListWithEntryInGrid(SorcererList, NPC_SKYBREAKER_SORCERER, 100.0f);
						if (!RiflemanList.empty())
						{
							for (std::list<Creature*>::const_iterator itr = SorcererList.begin(); itr != SorcererList.end(); ++itr)
							{
								if (Creature* pTarget = *itr)
									if ( pTarget->isAlive() )
										continue;
									else
										pTarget->Respawn();
							}
						}

						CaseTimer = 40000;
						CaseType = 3;
						break;
					}

				case 3:
					{
						if (Creature * pMuradin = me->FindNearestCreature(NPC_MURADIN_BRONZEBEARD ,100.0f))
							pMuradin->AI()->Talk(SAY_NEW_MAGE_SPAWNED);

						// Summon BattleMage
						if (Creature *pCreature = me->FindNearestCreature(NPC_SKYBREAKER_SORCERER_CANON,100.0f))
						{
							if (!pCreature->isAlive() )
								pCreature->Respawn();
						}
						else
						{
							me->SummonCreature(NPC_SKYBREAKER_SORCERER_CANON, 18.254408f, 2242.593018f, 527.132568f);
						}
						CaseTimer = 10000;
						CaseType = 1;
						break;
					}
				}
			}else{
				CaseTimer -= diff;
			}
		}

		bool bWype;
		Player* player;
		uint32 CaseType;
		uint32 CaseTimer;
		uint32 CheckPlayerTimer;
		std::list<uint32> Creature_Pool;
		InstanceScript* pInstance;
	};

	CreatureAI* GetAI(Creature* pCreature) const
	{
		return new npc_combattriggerAI(pCreature);
	}
};


class npc_GunShip_healthtrigger : public CreatureScript
{
public:
	npc_GunShip_healthtrigger() : CreatureScript("npc_GunShip_healthtrigger") { }

	struct npc_GunShip_healthtriggerAI : public ScriptedAI
	{
		npc_GunShip_healthtriggerAI(Creature* pCreature) : ScriptedAI(pCreature)
		{pInstance = me->GetInstanceScript();}

		void Reset()
		{
			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
			//me->SetVisible(false); // Must see the NPC in Debug ;)

			Map::PlayerList const &players = me->GetMap()->GetPlayers();
			for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
			{
				player = itr->getSource();
				//if (!player->isGameMaster()) // Not needet in Debug
				//{
				me->AI()->AttackStart(player);
				break;
				//}
			}
		}


		void JustDied(Unit* killer)
		{

			if(me->GetEntry() == RAID_MODE(NPC_HORDEGSTRIGGER_10, NPC_HORDEGSTRIGGER_25, NPC_HORDEGSTRIGGER_10, NPC_HORDEGSTRIGGER_25))
			{
				Map::PlayerList const &players = me->GetMap()->GetPlayers();
				for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
				{
					player = itr->getSource();
					if (player = itr->getSource()) 
					{
						me->DealDamage(player,1000000);

					}
				}


				//me->SummonGameObject(goEntry,

				//pInstance->SetBossState(DATA_GUNSHIP_BATTLE_EVENT, NOT_STARTED);
			}

			if(me->GetEntry() == RAID_MODE(NPC_ALLIGSTRIGGER_10, NPC_ALLIGSTRIGGER_25, NPC_ALLIGSTRIGGER_10, NPC_ALLIGSTRIGGER_25))
			{

				uint32 goEntry = RAID_MODE(GO_GUNSHIP_CACHE_10N,GO_GUNSHIP_CACHE_25N,GO_GUNSHIP_CACHE_10H,GO_GUNSHIP_CACHE_25H);
				me->SummonGameObject(goEntry,-83.502563f,2254.417236f,525.978760f,0.0f,3.259687f,0.0f,0.0f,0.0f,3600*24*7*1000);
				pInstance->SetBossState(DATA_GUNSHIP_BATTLE_EVENT, DONE);
				//pInstance->DoCompleteAchievement(RAID_MODE(IM_ON_A_BOAT_10,IM_ON_A_BOAT_25));
				pInstance->DoUpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET,72959);

				Map::PlayerList const &players = me->GetMap()->GetPlayers();
				for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
				{
					
					if (player = itr->getSource()) 
					{
						player->CombatStop();

					}
				}
			}




		}

		void EnterCombat(Unit* /*who*/)
		{
			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
			SetCombatMovement(false);
		}

		void UpdateAI( const uint32 diff)
		{
			if (pInstance && (pInstance->GetBossState(DATA_GUNSHIP_BATTLE_EVENT) == DONE || pInstance->GetBossState(DATA_GUNSHIP_BATTLE_EVENT) == NOT_STARTED))
				me->DisappearAndDie();
		}

		uint32 m_me;
		Player* player;
		InstanceScript* pInstance;
	};

	CreatureAI* GetAI(Creature* pCreature) const
	{
		return new npc_GunShip_healthtriggerAI(pCreature);
	}
};

class npc_portal_icc : public CreatureScript
{
public:
	npc_portal_icc() : CreatureScript("npc_portal_icc") { }

	struct npc_portal_iccAI : public ScriptedAI
	{
		npc_portal_iccAI(Creature* pCreature) : ScriptedAI(pCreature)
		{pInstance = me->GetInstanceScript();}

		void Reset()
		{
			SummonTimer = urand(2000, 4000);
			SummonCount = RAID_MODE(4, 7, 5, 8);
			CurrentCount = 0;

			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_SELECTABLE);

			Map::PlayerList const &players = me->GetMap()->GetPlayers();
			for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
			{
				pPlayer = itr->getSource();
				//if (!pPlayer->isGameMaster()) // Not needet in Debug
				//{
				me->AI()->AttackStart(pPlayer);
				break;
				//}
			}
		}

		void EnterCombat(Unit* /*who*/)
		{
			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_SELECTABLE);
			SetCombatMovement(false);
		}

		void SummonOffer(uint32 data)
		{
			if(data == 1)
				me->SummonCreature(NPC_SKYBREAKER_SERGEANT, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ());
			else if(data == 2)
				me->SummonCreature(NPC_SKYBREAKER_MARINE, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ());
		}

		void UpdateAI( const uint32 diff)
		{
			if (pInstance && (pInstance->GetBossState(DATA_GUNSHIP_BATTLE_EVENT) == DONE || pInstance->GetBossState(DATA_GUNSHIP_BATTLE_EVENT) == NOT_STARTED))
				me->DisappearAndDie();

			if (SummonTimer <= diff)
			{
				if (CurrentCount < SummonCount)
				{
					SummonTimer = urand(500, 2000);
					SummonOffer(urand(1,2));
					CurrentCount++;
				}else{
					//me->SetVisible(false);
					me->DisappearAndDie();
				}
			}else{
				SummonTimer -= diff;
			}

		}

		Player* pPlayer;
		uint32 CurrentCount;
		uint32 SummonCount;
		uint32 SummonTimer;
		InstanceScript* pInstance;
	};

	CreatureAI* GetAI(Creature* pCreature) const
	{
		return new npc_portal_iccAI(pCreature);
	}
};

class npc_gunship_canon : public CreatureScript
{
public:
	npc_gunship_canon() : CreatureScript("npc_gunship_canon") {}

	struct npc_gunship_canonAI : public ScriptedAI
	{
		npc_gunship_canonAI(Creature *pCreature) : ScriptedAI(pCreature)
		{
			pInstance = me->GetInstanceScript();
		}




		void SpellHit(Unit* /*caster*/, SpellInfo const* spell)
		{

			if (spell->Id == SPELL_BELOW_ZERO)
				me->GetVehicleKit()->RemoveAllPassengers();
		}

		void UpdateAI(const uint32 diff)
		{

			if (pInstance && ( pInstance->GetBossState(DATA_GUNSHIP_BATTLE_EVENT) == DONE || pInstance->GetBossState(DATA_GUNSHIP_BATTLE_EVENT) == NOT_STARTED))
				me->DisappearAndDie();

		}

		InstanceScript* pInstance;
	};

	CreatureAI* GetAI(Creature* pCreature) const
	{
		return new npc_gunship_canonAI(pCreature);
	}
};


/* ----------------------------------- Spells ----------------------------------- */

/* Overheat - 69487 */
class spell_icc_overheat : public SpellScriptLoader
{
public:
	spell_icc_overheat() : SpellScriptLoader("spell_icc_overheat") { }

	class spell_icc_overheat_AuraScript : public AuraScript
	{
		PrepareAuraScript(spell_icc_overheat_AuraScript);

		void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
		{
			Unit* passenger = GetUnitOwner()->GetVehicle()->GetPassenger(1);
			if (passenger->GetTypeId() == TYPEID_PLAYER)
			{
				passenger->ToPlayer()->AddSpellCooldown(69399, 0, time(NULL) + 3000);
				passenger->ToPlayer()->AddSpellCooldown(SPELL_INCINERATING_BLAST, 0, time(NULL) + 3000);
			}
		}

		void Register()
		{
			OnEffectApply += AuraEffectApplyFn(spell_icc_overheat_AuraScript::OnApply, EFFECT_0, SPELL_EFFECT_APPLY_AURA, AURA_EFFECT_HANDLE_DEFAULT);
		}
	};

	AuraScript* GetAuraScript() const
	{
		return new spell_icc_overheat_AuraScript();
	}
};

/* Rocket Pack - 69188 */
/* 68721 is a big red ball */
/* 69193 is the damage when landing, it does not include the visual (which is 69192) */
class spell_icc_rocket_pack : public SpellScriptLoader
{
public:
	spell_icc_rocket_pack() : SpellScriptLoader("spell_icc_rocket_pack") { }

	class spell_icc_rocket_pack_SpellScript : public SpellScript
	{
		PrepareSpellScript(spell_icc_rocket_pack_SpellScript);

		SpellCastResult CheckRequirement()
		{
			Unit* caster = GetOriginalCaster();
			if (caster->GetTypeId() != TYPEID_PLAYER)
				return SPELL_FAILED_TARGET_NOT_PLAYER;

			// The aura checks if the player has the aura that Zafod Boombox uses. (SPELL_EFFECT_APPLY_AREA_AURA_FRIEND)
			if (!caster->ToPlayer()->HasAura(70348)) // Rocket Pack Useable
			{
				// May have a custom error message.
				Spell::SendCastResult(caster->ToPlayer(), GetSpellInfo(), 0, SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW);
				return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;
			}

			return SPELL_CAST_OK;
		}

		void Register()
		{
			OnCheckCast += SpellCheckCastFn(spell_icc_rocket_pack_SpellScript::CheckRequirement);
		}
	};

	class spell_icc_rocket_pack_AuraScript : public AuraScript
	{
		PrepareAuraScript(spell_icc_rocket_pack_AuraScript);

		void AfterRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
		{
			Unit* caster = GetCaster();
			caster->CastSpell(caster, 69193, true);
			// 69193 does trigger the visual AoE effect (69192) through DB
			caster->RemoveAurasDueToSpell(69188); // spell_linked_spell
			caster->RemoveAurasDueToSpell(68721); // spell_linked_spell
		}

		void Register()
		{
			AfterEffectRemove += AuraEffectRemoveFn(spell_icc_rocket_pack_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
		}
	};

	SpellScript* GetSpellScript() const
	{
		return new spell_icc_rocket_pack_SpellScript();
	}

	AuraScript* GetAuraScript() const
	{
		return new spell_icc_rocket_pack_AuraScript();
	}
};

/* Remove Rocket Pack - 70713 */
class spell_icc_remove_rocket_pack : public SpellScriptLoader
{
public:
	spell_icc_remove_rocket_pack() : SpellScriptLoader("spell_icc_remove_rocket_pack") { }

	class spell_icc_remove_rocket_pack_SpellScript : public SpellScript
	{
		PrepareSpellScript(spell_icc_remove_rocket_pack_SpellScript);

		void HandleEffect(SpellEffIndex /*effIndex*/)
		{
			Player* hitPlr = GetHitPlayer();
			if (!hitPlr) // If player is offline
				return;

			int32 itemId = GetEffectValue();
			int32 itemCount = hitPlr->GetItemCount(itemId, false); // Should be 1, but just in case.
			hitPlr->DestroyItemCount(itemId, -itemCount, true, false);
		}

		void Register()
		{
			OnEffectHit += SpellEffectFn(spell_icc_remove_rocket_pack_SpellScript::HandleEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
		}
	};

	SpellScript* GetSpellScript() const
	{
		return new spell_icc_remove_rocket_pack_SpellScript();
	}
};


/* Rocket Artillery - 69679 */
class spell_icc_rocket_artillery_triggered : public SpellScriptLoader
{
public:
	spell_icc_rocket_artillery_triggered() : SpellScriptLoader("spell_icc_rocket_artillery_triggered") { }

	class spell_icc_rocket_artillery_triggered_SpellScript : public SpellScript
	{
		PrepareSpellScript(spell_icc_rocket_artillery_triggered_SpellScript);

		void HandleEffect(SpellEffIndex /*effIndex*/)
		{
			Unit* caster = GetCaster();
			//WorldLocation* targetedArea = GetTargetDest();
			WorldLocation const* const targetedArea = GetTargetDest();

			if (!targetedArea || !caster || caster->GetTypeId() != TYPEID_UNIT)
				return;

			Position targetedPosition;
			targetedArea->GetPosition(&targetedPosition);

			std::list<mortarMarksLoc> marks = CAST_AI(npc_mortar_soldier_or_rocketeer::npc_mortar_soldier_or_rocketeerAI, caster->ToCreature()->AI())->getMarksPositions();
			for (std::list<mortarMarksLoc>::iterator itr = marks.begin(); itr != marks.end(); ++itr)
			{
				// I love dirty code
				Position positionInList = (*itr).location;
				if (positionInList.m_positionX == targetedPosition.m_positionX)
					if (positionInList.m_positionY == targetedPosition.m_positionY)
						if (positionInList.m_positionZ == targetedPosition.m_positionZ)
						{
							// Don't give a fuck about orientation
							//marks.remove(*itr);
						}
			}

			CAST_AI(npc_mortar_soldier_or_rocketeer::npc_mortar_soldier_or_rocketeerAI, caster->ToCreature()->AI())->setMarks(marks);
		}

		void Register()
		{
			OnEffectHit += SpellEffectFn(spell_icc_rocket_artillery_triggered_SpellScript::HandleEffect, EFFECT_0, SPELL_EFFECT_TRIGGER_SPELL);
		}
	};

	SpellScript* GetSpellScript() const
	{
		return new spell_icc_rocket_artillery_triggered_SpellScript();
	}
};


class spell_deal_damage_to_horde_gunship : public SpellScriptLoader
{
public:
	spell_deal_damage_to_horde_gunship() : SpellScriptLoader("spell_deal_damage_to_horde_gunship") { }

	class spell_deal_damage_to_horde_gunship_SpellScript : public SpellScript
	{
		PrepareSpellScript(spell_deal_damage_to_horde_gunship_SpellScript);

		SpellCastResult DealDamage()
		{

			Unit* pCaster = GetCaster();
			if (pCaster)
			{
				InstanceScript* pInstance = pCaster->GetInstanceScript();
				if (pInstance)
				{
					Creature* pTarget;
					pTarget = pCaster->FindNearestCreature(NPC_HORDEGSTRIGGER_10,200.0f,true);
					if (!pTarget)
					{
						pTarget = pCaster->FindNearestCreature(NPC_HORDEGSTRIGGER_25,200.0f,true);
						if(!pTarget)
							return SPELL_FAILED_NO_VALID_TARGETS;
						else {
							uint32 dealdamage = pCaster->CalculateSpellDamage(GetTargetUnit(),this->GetSpellInfo(),1/*this->GetEffectValue()*/);
							pCaster->DealDamage(pTarget,dealdamage);
							return SPELL_CAST_OK;
						}
					}
					else {  
						uint32 dealdamage = pCaster->CalculateSpellDamage(GetTargetUnit(),this->GetSpellInfo(),this->GetEffectValue());
						if (this->GetSpellInfo()->Id == 71335)  //dirty hack
						{
							dealdamage = dealdamage / 3;
						}
						pCaster->DealDamage(pTarget,dealdamage);
						return SPELL_CAST_OK;
					}
				}
				else return SPELL_FAILED_NO_VALID_TARGETS;
			}else  return SPELL_FAILED_NO_VALID_TARGETS;
		}

		void Register()
		{
			OnCheckCast += SpellCheckCastFn(spell_deal_damage_to_horde_gunship_SpellScript::DealDamage);
		}
	};

	SpellScript* GetSpellScript() const
	{
		return new spell_deal_damage_to_horde_gunship_SpellScript();
	}
};

class spell_deal_damage_to_alli_gunship : public SpellScriptLoader
{
public:
	spell_deal_damage_to_alli_gunship() : SpellScriptLoader("spell_deal_damage_to_alli_gunship") { }

	class spell_deal_damage_to_alli_gunship_SpellScript : public SpellScript
	{
		PrepareSpellScript(spell_deal_damage_to_alli_gunship_SpellScript);

		SpellCastResult DealDamage()
		{

			Unit* pCaster = GetCaster();
			if (pCaster)
			{
				InstanceScript* pInstance = pCaster->GetInstanceScript();
				if (pInstance)
				{
					Creature* pTarget;
					pTarget = pCaster->FindNearestCreature(NPC_ALLIGSTRIGGER_10,200.0f,true);
					if (!pTarget)
					{
						pTarget = pCaster->FindNearestCreature(NPC_ALLIGSTRIGGER_25,200.0f,true);
						if(!pTarget)
							return SPELL_FAILED_NO_VALID_TARGETS;
						else
						{
							pCaster->DealDamage(pTarget,1000);
							return SPELL_CAST_OK;
						}
					}
					else
					{ 
						pCaster->DealDamage(pTarget,1000);
						return SPELL_CAST_OK;
					}
				}
				else return SPELL_FAILED_NO_VALID_TARGETS;
			}else  return SPELL_FAILED_NO_VALID_TARGETS;
		}

		void Register()
		{
			OnCheckCast += SpellCheckCastFn(spell_deal_damage_to_alli_gunship_SpellScript::DealDamage);
		}
	};

	SpellScript* GetSpellScript() const
	{
		return new spell_deal_damage_to_alli_gunship_SpellScript();
	}
};

/* ---------------------------------- AreaTrigger Scripts ------------------------------------- */
/*class at_icc_land_frostwyrm : public AreaTriggerScript
{
public:
at_icc_land_frostwyrm() : AreaTriggerScript("at_icc_land_frostwyrm") { }

bool OnTrigger(Player* player, AreaTriggerEntry const* areaTrigger)
{
if (InstanceScript* instance = player->GetInstanceScript())
{
if (instance->GetData(DATA_SPIRE_FROSTWYRM_STATE) == NOT_STARTED)
{
instance->SetData(DATA_SPIRE_FROSTWYRM_STATE, IN_PROGRESS);
// Emote on landing seems to be 447
// The frostwyrm needs a script.
// There are 2 wyrms in fact, both land, they get in combat with the NPCs
// "A screeching cry pierces the air above" when she lands.
// The one from the opposite side does not land, however.
}
}

return true;
}
};*/


class spell_cannon_blast : public SpellScriptLoader
{
public:
	spell_cannon_blast() : SpellScriptLoader("spell_cannon_blast") {}

	class spell_cannon_blast_SpellScript : public SpellScript
	{
		PrepareSpellScript(spell_cannon_blast_SpellScript);


		SpellCastResult TriggerSpell()
		{
			const WorldLocation* TarDest = this->GetTargetDest();
			Unit *Canon = this->GetCaster();
			Canon->CastSpell(TarDest->GetPositionX(),TarDest->GetPositionY(),TarDest->GetPositionZ(),SPELL_CANNON_BLAST,false);

			Unit* pCaster = GetCaster();
			if (pCaster)
			{
				InstanceScript* pInstance = pCaster->GetInstanceScript();
				if (pInstance)
				{
					Creature* pTarget;
					pTarget = pCaster->FindNearestCreature(NPC_ALLIGSTRIGGER_10,200.0f,true);
					if (!pTarget)
					{
						pTarget = pCaster->FindNearestCreature(NPC_ALLIGSTRIGGER_25,200.0f,true);
						if(!pTarget)
							return SPELL_FAILED_NO_VALID_TARGETS;
						else
						{
							pCaster->DealDamage(pTarget,1000);
							return SPELL_CAST_OK;
						}
					}
					else
					{ 
						pCaster->DealDamage(pTarget,1000);
						return SPELL_CAST_OK;
					}
				}
				else return SPELL_FAILED_NO_VALID_TARGETS;
			}else  return SPELL_FAILED_NO_VALID_TARGETS;
		}

		bool Validate(SpellEntry const * /*spellEntry*/)
		{
			if (!sSpellStore.LookupEntry(SPELL_OVERHEAT))
				return false;
			return true;
		}

		//        void HandleScript(SpellEffIndex /*effIndex*/)
		/*        {
		Unit* caster = GetCaster();

		PreventHitDefaultEffect(EFFECT_1);
		if (caster->getPowerType() == POWER_ENERGY)
		{
		uint32 random = urand(6, 10);
		uint32 currentEnergy = caster->GetPower(POWER_ENERGY);
		if ((currentEnergy + random) < caster->GetMaxPower(POWER_ENERGY))
		caster->SetPower(POWER_ENERGY, (currentEnergy + random));
		else
		{
		caster->SetPower(POWER_ENERGY, caster->GetMaxPower(POWER_ENERGY));
		caster->CastSpell(caster, SPELL_OVERHEAT, true);
		}
		}
		}
		*/
		void Register()
		{
			OnCheckCast += SpellCheckCastFn(spell_cannon_blast_SpellScript::TriggerSpell);
			//            OnEffect += SpellEffectFn(spell_cannon_blast_SpellScript::HandleScript, EFFECT_1, SPELL_EFFECT_ENERGIZE);
		}
	};

	SpellScript* GetSpellScript() const
	{
		return new spell_cannon_blast_SpellScript();
	}
};

class spell_incinerating_blast : public SpellScriptLoader
{
public:
	spell_incinerating_blast() : SpellScriptLoader("spell_incinerating_blast") {}

	class spell_incinerating_blast_SpellScript : public SpellScript
	{
		PrepareSpellScript(spell_incinerating_blast_SpellScript);


		SpellCastResult TriggerSpell()
		{
			const WorldLocation* TarDest = this->GetTargetDest();
			Unit *Canon = this->GetCaster();
			Canon->CastSpell(TarDest->GetPositionX(),TarDest->GetPositionY(),TarDest->GetPositionZ(),SPELL_INCINERATING_BLAST,false);
			return SPELL_CAST_OK;
		}

		void Register()
		{
			OnCheckCast += SpellCheckCastFn(spell_incinerating_blast_SpellScript::TriggerSpell);
		}

		uint32 hitDamage;
	};

	SpellScript* GetSpellScript() const
	{
		return new spell_incinerating_blast_SpellScript();
	}
};

class spell_overheat_test : public SpellScriptLoader
{
public:
	spell_overheat_test() : SpellScriptLoader("spell_overheat_test") {}

	class spell_overheat_test_SpellScript : public SpellScript
	{
		PrepareSpellScript(spell_overheat_test_SpellScript);

		SpellCastResult applycd()
		{
			Creature* pTarget = this->GetCaster()->ToCreature();
			if (pTarget)
			{
				InstanceScript* pInstance = pTarget->GetInstanceScript();
				if (pInstance)
				{
					Player* passenger = pTarget->SelectNearestPlayer(10.0f);
					if (passenger)
					{
						passenger->AddSpellCooldown(69399, 0, time(NULL) + 3000);
						passenger->AddSpellCooldown(SPELL_INCINERATING_BLAST, 0, time(NULL) + 3000);
					}

				}
			}

			return SPELL_CAST_OK;
		}

		void HandleEffect(SpellEffIndex /*effIndex*/)
		{
		}


		void Register()
		{
			OnCheckCast += SpellCheckCastFn(spell_overheat_test_SpellScript::applycd);
			OnEffectHit += SpellEffectFn(spell_overheat_test_SpellScript::HandleEffect, EFFECT_2, SPELL_EFFECT_SCRIPT_EFFECT);
		}
	};

	SpellScript* GetSpellScript() const
	{
		return new spell_overheat_test_SpellScript();
	}
};

class spell_incinerating_blast_damage : public SpellScriptLoader
{
public:
	spell_incinerating_blast_damage() : SpellScriptLoader("spell_incinerating_blast_damage") {}

	class spell_incinerating_blast_damage_SpellScript : public SpellScript
	{
		PrepareSpellScript(spell_incinerating_blast_damage_SpellScript);


		bool Load()
		{
			if (GetCaster()->getPowerType() == POWER_ENERGY)
			{
				hitDamage = GetCaster()->GetPower(POWER_ENERGY)*100;
				return true;
			}
			else
				return false;
		}

		void SetDamage(SpellEffIndex /*effIndex*/)
		{
			SetHitDamage(GetHitDamage() + hitDamage);
		}


		SpellCastResult DealDamage()
		{

			Unit* pCaster = GetCaster();
			if (pCaster)
			{
				InstanceScript* pInstance = pCaster->GetInstanceScript();
				if (pInstance)
				{
					Creature* pTarget;
					pTarget = pCaster->FindNearestCreature(NPC_ALLIGSTRIGGER_10,200.0f,true);
					if (!pTarget)
					{
						pTarget = pCaster->FindNearestCreature(NPC_ALLIGSTRIGGER_25,200.0f,true);
						if(!pTarget)
						{
							return SPELL_CAST_OK;
						}
					}

					pCaster->DealDamage(pTarget,(GetHitDamage() + hitDamage));

				}
			}

			return SPELL_CAST_OK;

		}

		void HandleScriptEffect(SpellEffIndex /*effIndex*/)
		{
			GetCaster()->SetPower(POWER_ENERGY, 0);
		}

		void Register()
		{
			OnEffectHit += SpellEffectFn(spell_incinerating_blast_damage_SpellScript::SetDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
			OnEffectHit += SpellEffectFn(spell_incinerating_blast_damage_SpellScript::SetDamage, EFFECT_1, SPELL_EFFECT_SCHOOL_DAMAGE);
			OnCheckCast += SpellCheckCastFn(spell_incinerating_blast_damage_SpellScript::DealDamage);
			OnEffectHit += SpellEffectFn(spell_incinerating_blast_damage_SpellScript::HandleScriptEffect, EFFECT_2, SPELL_EFFECT_SCRIPT_EFFECT);
		}

		uint32 hitDamage;
	};

	SpellScript* GetSpellScript() const
	{
		return new spell_incinerating_blast_damage_SpellScript();
	}
};

void AddSC_boss_gunship_battle()
{
	new npc_sergeant();
	new npc_zafod_boombox();
	new npc_skybreaker_sorcerer();
	new npc_skybreaker_rifleman();
	new npc_korkron_axethrower();
	new npc_marine_or_reaver();
	new npc_mortar_soldier_or_rocketeer();
	new npc_sorcerer_or_battle_mage();
	new boss_muradin_bronzebeard();
	new boss_high_overlord_varok_saurfang();
	new npc_combattrigger();
	new npc_GunShip_healthtrigger();
	new npc_portal_icc();
	new npc_gunship_canon();
	// AreaTriggers
	//new at_icc_land_frostwyrm();

	// Transports
	//new transport_gunship();
	// Spells
	new spell_icc_overheat();
	new spell_overheat_test();


	new spell_icc_rocket_pack();
	new spell_icc_remove_rocket_pack();

	new spell_deal_damage_to_horde_gunship();
	new spell_incinerating_blast();
	new spell_incinerating_blast_damage();
	new spell_cannon_blast();
	new spell_deal_damage_to_alli_gunship();

}