/*######
## Arena Watcher
######*/

#include "Player.h"
#include "BattlegroundMgr.h"
#include "Battleground.h"
#include "ArenaTeamMgr.h"
#include "ArenaTeam.h"


enum WatcherData
{
    GOSSIP_OFFSET = GOSSIP_ACTION_INFO_DEF + 10,
};

class npc_arena_watcher : public CreatureScript
{
public:
    npc_arena_watcher() : CreatureScript("npc_arena_watcher") {}
    
    bool OnGossipHello(Player* player, Creature* creature)
    {
        BattlegroundSet arenasSet = sBattlegroundMgr->GetAllBattlegroundsWithTypeId(BATTLEGROUND_AA);
        uint32 arenasQty[3] = {0, 0, 0};
        for (BattlegroundSet::const_iterator itr = arenasSet.begin(); itr != arenasSet.end(); ++itr)
            if (Battleground* bg = itr->second)
                switch (bg->GetArenaType())
                {
                    case ARENA_TYPE_2v2:  arenasQty[0]++; break;
                    case ARENA_TYPE_3v3:  arenasQty[1]++; break;
                    case ARENA_TYPE_5v5:  arenasQty[2]++; break;
                }

        std::stringstream gossip2;
        std::stringstream gossip3;
        std::stringstream gossip5;
        gossip2 << "Observez un match 2v2 . (" << arenasQty[0] << " match(s) en cours)";
        gossip3 << "Observez un match 3v3 . (" << arenasQty[1] << " match(s) en cours)";
        gossip5 << "Observez un match 5v5 . (" << arenasQty[2] << " match(s) en cours)";

        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, gossip2.str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, gossip3.str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, gossip5.str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
        player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_CHAT, "Observez un Joueur.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4, "", 0, true);
        
        player->PlayerTalkClass->SendGossipMenu(player->GetGossipTextId(creature), creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();

        uint8 mode = ARENA_TYPE_2v2;
        if (action == (GOSSIP_ACTION_INFO_DEF + 3))
            mode = ARENA_TYPE_3v3;
        if (action == (GOSSIP_ACTION_INFO_DEF + 5))
            mode = ARENA_TYPE_5v5;

        if (action <= GOSSIP_OFFSET)
        {
            BattlegroundSet arenasSet = sBattlegroundMgr->GetAllBattlegroundsWithTypeId(BATTLEGROUND_AA);

            bool bracketMatchs = false;
            for (BattlegroundSet::const_iterator itr = arenasSet.begin(); itr != arenasSet.end(); ++itr)
            {
                if (Battleground* bg = itr->second)
                {
                    if (bg->GetArenaType() == mode)
                    {
                        bracketMatchs = true;
                        break;
                    }
                }
            }

            if (!bracketMatchs)
            {
                std::stringstream errMsg;
                errMsg << "Désolée " << player->GetName() << ", Aucun Match en cour.";
                creature->MonsterWhisper(errMsg.str().c_str(), player->GetGUID());
                player->PlayerTalkClass->ClearMenus();
                player->CLOSE_GOSSIP_MENU();
            }
            else
            {
                for (BattlegroundSet::const_iterator itr = arenasSet.begin(); itr != arenasSet.end(); ++itr)
                {
                    if (Battleground* bg = itr->second)
                    {
                        ArenaTeam* teamOne = sArenaTeamMgr->GetArenaTeamById(bg->GetArenaTeamIdByIndex(0));
                        ArenaTeam* teamTwo = sArenaTeamMgr->GetArenaTeamById(bg->GetArenaTeamIdByIndex(1));

                        if (teamOne && teamTwo)
                        {
                            std::stringstream gossipItem;
                            gossipItem << teamOne->GetName() << " (";
                            gossipItem << teamOne->GetRating() << ") VS ";
                            gossipItem << teamTwo->GetName() << " (";
                            gossipItem << teamTwo->GetRating() << ")";
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, gossipItem.str(), GOSSIP_SENDER_MAIN + 1, itr->first + GOSSIP_OFFSET);
                        }
                    }
                }
                player->PlayerTalkClass->SendGossipMenu(player->GetGossipTextId(creature), creature->GetGUID());
            }
        }
        else
        {
            uint32 arenaId = action - GOSSIP_OFFSET;
            BattlegroundSet arenasSet = sBattlegroundMgr->GetAllBattlegroundsWithTypeId(BATTLEGROUND_AA);

            if (arenasSet[arenaId] != NULL)
            {
                Battleground* arenaChosen = arenasSet[arenaId];

                if (arenaChosen->GetStatus() != STATUS_NONE && arenaChosen->GetStatus() != STATUS_IN_PROGRESS)
                {
                    std::stringstream errMsg;
                    errMsg << "Désolée " << player->GetName() << ", l'arène choisie est terminée";
                    creature->MonsterWhisper(errMsg.str().c_str(), player->GetGUID());
                    player->PlayerTalkClass->ClearMenus();
                    player->CLOSE_GOSSIP_MENU();
                    return false;
                }

                player->SetBattlegroundId(arenaChosen->GetInstanceID(), arenaChosen->GetTypeID());
                player->SetBattlegroundEntryPoint();
                float x, y, z;
                switch (arenaChosen->GetMapId())
                {
                    case 617: x = 1299.046f;    y = 784.825f;     z = 9.338f;     break;
                    case 618: x = 763.5f;       y = -284;         z = 28.276f;    break;
                    case 572: x = 1285.810547f; y = 1667.896851f; z = 39.957642f; break;
                    case 562: x = 6238.930176f; y = 262.963470f;  z = 0.889519f;  break;
                    case 559: x = 4055.504395f; y = 2919.660645f; z = 13.611241f; break;
                }
                player->SetGMVisible(false);
                player->TeleportTo(arenaChosen->GetMapId(), x, y, z, player->GetOrientation());
            }
        }
        return true;
    }

    bool OnGossipSelectCode(Player* player, Creature* creature, uint32 uiSender, uint32 uiAction, const char* code)
    {
        player->PlayerTalkClass->ClearMenus();
        player->CLOSE_GOSSIP_MENU();
        if (uiSender == GOSSIP_SENDER_MAIN)
        {
            switch (uiAction)
            {
                case GOSSIP_ACTION_INFO_DEF + 4:

                    const char* plrName = code;
                    if (Player* target = sObjectAccessor->FindPlayerByName(plrName))
                    {
                        if (!target->IsInWorld())
                        {
                            creature->MonsterWhisper("Désolé, je ne peux pas vous permettre de vous téléporter à ce joueur", player->GetGUID());
                            return true;
                        }

                        if (!target->InArena())
                        {
                            creature->MonsterWhisper("Désolé, ce joueur n'est pas dans l'arène. Bien essayé!", player->GetGUID());
                            return true;
                        }

                        if (target->isGameMaster())
                        {
                            creature->MonsterWhisper("Owned, alors comme ça, on veut ce tp sur un Mj !", player->GetGUID());
                            return true;
                        }

                        player->SetBattlegroundId(target->GetBattleground()->GetInstanceID(), target->GetBattleground()->GetTypeID());
                        player->SetBattlegroundEntryPoint();
                        float x, y, z;
                        target->GetContactPoint(player, x, y, z);
                        player->TeleportTo(target->GetMapId(), x, y, z, player->GetAngle(target));
                        player->SetGMVisible(false);
                    }
                    return true;
            }
        }

        return false;
    }
};

void AddSC_ArenaWatcher()
{
	new npc_arena_watcher; //UPDATE `creature_template` SET `npcflag` = 1, `ScriptName` = 'npc_arena_watcher' WHERE `entry` = 32743; (not blizz)
}