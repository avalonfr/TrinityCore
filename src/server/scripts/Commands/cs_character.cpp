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

/* ScriptData
Name: character_commandscript
%Complete: 100
Comment: All character related commands
Category: commandscripts
EndScriptData */

#include "ScriptMgr.h"
#include "Chat.h"
#include "AccountMgr.h"
#include "ObjectMgr.h"
#include "PlayerDump.h"

class character_commandscript : public CommandScript
{
public:
    character_commandscript() : CommandScript("character_commandscript") { }

    ChatCommand* GetCommands() const
    {
        static ChatCommand pdumpCommandTable[] =
        {
            { "load",           SEC_ADMINISTRATOR,  true,  &HandlePDumpLoadCommand,                 "", NULL },
            { "write",          SEC_ADMINISTRATOR,  true,  &HandlePDumpWriteCommand,                "", NULL },
            { NULL,             0,                  false, NULL,                                    "", NULL }
        };
        static ChatCommand characterDeletedCommandTable[] =
        {
            { "delete",         SEC_CONSOLE,        true,  &HandleCharacterDeletedDeleteCommand,   "", NULL },
            { "list",           SEC_ADMINISTRATOR,  true,  &HandleCharacterDeletedListCommand,     "", NULL },
            { "restore",        SEC_ADMINISTRATOR,  true,  &HandleCharacterDeletedRestoreCommand,  "", NULL },
            { "old",            SEC_CONSOLE,        true,  &HandleCharacterDeletedOldCommand,      "", NULL },
            { NULL,             0,                  false, NULL,                                   "", NULL }
        };

        static ChatCommand characterCommandTable[] =
        {
            { "customize",      SEC_GAMEMASTER,     true,  &HandleCharacterCustomizeCommand,       "", NULL },
            { "changefaction",  SEC_GAMEMASTER,     true,  &HandleCharacterChangeFactionCommand,   "", NULL },
            { "changerace",     SEC_GAMEMASTER,     true,  &HandleCharacterChangeRaceCommand,      "", NULL },
            { "deleted",        SEC_GAMEMASTER,     true,  NULL,                                   "", characterDeletedCommandTable },
            { "erase",          SEC_CONSOLE,        true,  &HandleCharacterEraseCommand,           "", NULL },
            { "level",          SEC_ADMINISTRATOR,  true,  &HandleCharacterLevelCommand,           "", NULL },
            { "rename",         SEC_GAMEMASTER,     true,  &HandleCharacterRenameCommand,          "", NULL },
            { "reputation",     SEC_GAMEMASTER,     true,  &HandleCharacterReputationCommand,      "", NULL },
            { "titles",         SEC_GAMEMASTER,     true,  &HandleCharacterTitlesCommand,          "", NULL },
            { NULL,             0,                  false, NULL,                                   "", NULL }
        };

/*		static ChatCommand boutiqueCommandTable[] =
		{
			{ "infos",		SEC_PLAYER, 			true, 	&BoutiqueInfos, 					"", NULL },
			{ "item", 		SEC_PLAYER, 			true, 	&BoutiqueAdditem, 					"", NULL },
			{ "level", 		SEC_PLAYER, 			true, 	&BoutiqueLevel, 					"", NULL },
			{ "rename", 	SEC_PLAYER, 			true, 	&BoutiqueRename, 					"", NULL },
			{ "custom", 	SEC_PLAYER, 			true, 	&BoutiqueCustomize, 				"", NULL },
			{ "race", 		SEC_PLAYER, 			true, 	&BoutiqueRace, 						"", NULL },
			{ "faction", 	SEC_PLAYER, 			true, 	&BoutiqueFaction, 					"", NULL },
			{ "metier", 	SEC_PLAYER, 			true, 	&BoutiqueMetier, 					"", NULL },
			{ NULL, 		0, 						false, 	NULL, 								"", NULL }
		};
*/
        static ChatCommand commandTable[] =
        {
            { "character",      SEC_GAMEMASTER,     true,  NULL,                                    "", characterCommandTable },
            { "levelup",        SEC_ADMINISTRATOR,  false, &HandleLevelUpCommand,                   "", NULL },
            { "pdump",          SEC_ADMINISTRATOR,  true,  NULL,                                    "", pdumpCommandTable },
/*			{ "boutique",		SEC_PLAYER,			true,  NULL,									"", boutiqueCommandTable  },*/
            { NULL,             0,                  false, NULL,                                    "", NULL }
        };
        return commandTable;
    }

    // Stores informations about a deleted character
    struct DeletedInfo
    {
        uint32      lowGuid;                            ///< the low GUID from the character
        std::string name;                               ///< the character name
        uint32      accountId;                          ///< the account id
        std::string accountName;                        ///< the account name
        time_t      deleteDate;                         ///< the date at which the character has been deleted
    };

    typedef std::list<DeletedInfo> DeletedInfoList;

    /**
    * Collects all GUIDs (and related info) from deleted characters which are still in the database.
    *
    * @param foundList    a reference to an std::list which will be filled with info data
    * @param searchString the search string which either contains a player GUID or a part fo the character-name
    * @return             returns false if there was a problem while selecting the characters (e.g. player name not normalizeable)
    */
    static bool GetDeletedCharacterInfoList(DeletedInfoList& foundList, std::string searchString)
    {
        PreparedQueryResult result;
        PreparedStatement* stmt;
        if (!searchString.empty())
        {
            // search by GUID
            if (isNumeric(searchString.c_str()))
            {
                stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHAR_DEL_INFO_BY_GUID);
                stmt->setUInt32(0, uint32(atoi(searchString.c_str())));
                result = CharacterDatabase.Query(stmt);
            }
            // search by name
            else
            {
                if (!normalizePlayerName(searchString))
                    return false;

                stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHAR_DEL_INFO_BY_NAME);
                stmt->setString(0, searchString);
                result = CharacterDatabase.Query(stmt);
            }
        }
        else
        {
            stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHAR_DEL_INFO);
            result = CharacterDatabase.Query(stmt);
        }

        if (result)
        {
            do
            {
                Field* fields = result->Fetch();

                DeletedInfo info;

                info.lowGuid    = fields[0].GetUInt32();
                info.name       = fields[1].GetString();
                info.accountId  = fields[2].GetUInt32();

                // account name will be empty for not existed account
                AccountMgr::GetName(info.accountId, info.accountName);
                info.deleteDate = time_t(fields[3].GetUInt32());
                foundList.push_back(info);
            }
            while (result->NextRow());
        }

        return true;
    }

    /**
    * Shows all deleted characters which matches the given search string, expected non empty list
    *
    * @see HandleCharacterDeletedListCommand
    * @see HandleCharacterDeletedRestoreCommand
    * @see HandleCharacterDeletedDeleteCommand
    * @see DeletedInfoList
    *
    * @param foundList contains a list with all found deleted characters
    */
    static void HandleCharacterDeletedListHelper(DeletedInfoList const& foundList, ChatHandler* handler)
    {
        if (!handler->GetSession())
        {
            handler->SendSysMessage(LANG_CHARACTER_DELETED_LIST_BAR);
            handler->SendSysMessage(LANG_CHARACTER_DELETED_LIST_HEADER);
            handler->SendSysMessage(LANG_CHARACTER_DELETED_LIST_BAR);
        }

        for (DeletedInfoList::const_iterator itr = foundList.begin(); itr != foundList.end(); ++itr)
        {
            std::string dateStr = TimeToTimestampStr(itr->deleteDate);

            if (!handler->GetSession())
                handler->PSendSysMessage(LANG_CHARACTER_DELETED_LIST_LINE_CONSOLE,
                    itr->lowGuid, itr->name.c_str(), itr->accountName.empty() ? "<Not existed>" : itr->accountName.c_str(),
                    itr->accountId, dateStr.c_str());
            else
                handler->PSendSysMessage(LANG_CHARACTER_DELETED_LIST_LINE_CHAT,
                    itr->lowGuid, itr->name.c_str(), itr->accountName.empty() ? "<Not existed>" : itr->accountName.c_str(),
                    itr->accountId, dateStr.c_str());
        }

        if (!handler->GetSession())
            handler->SendSysMessage(LANG_CHARACTER_DELETED_LIST_BAR);
    }

    /**
    * Restore a previously deleted character
    *
    * @see HandleCharacterDeletedListHelper
    * @see HandleCharacterDeletedRestoreCommand
    * @see HandleCharacterDeletedDeleteCommand
    * @see DeletedInfoList
    *
    * @param delInfo the informations about the character which will be restored
    */
    static void HandleCharacterDeletedRestoreHelper(DeletedInfo const& delInfo, ChatHandler* handler)
    {
        if (delInfo.accountName.empty())                    // account not exist
        {
            handler->PSendSysMessage(LANG_CHARACTER_DELETED_SKIP_ACCOUNT, delInfo.name.c_str(), delInfo.lowGuid, delInfo.accountId);
            return;
        }

        // check character count
        uint32 charcount = AccountMgr::GetCharactersCount(delInfo.accountId);
        if (charcount >= 10)
        {
            handler->PSendSysMessage(LANG_CHARACTER_DELETED_SKIP_FULL, delInfo.name.c_str(), delInfo.lowGuid, delInfo.accountId);
            return;
        }

        if (sObjectMgr->GetPlayerGUIDByName(delInfo.name))
        {
            handler->PSendSysMessage(LANG_CHARACTER_DELETED_SKIP_NAME, delInfo.name.c_str(), delInfo.lowGuid, delInfo.accountId);
            return;
        }

        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UDP_RESTORE_DELETE_INFO);
        stmt->setString(0, delInfo.name);
        stmt->setUInt32(1, delInfo.accountId);
        stmt->setUInt32(2, delInfo.lowGuid);
        CharacterDatabase.Execute(stmt);

        stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_NAME_DATA);
        stmt->setUInt32(0, delInfo.lowGuid);
        if (PreparedQueryResult result = CharacterDatabase.Query(stmt))
            sWorld->AddCharacterNameData(delInfo.lowGuid, delInfo.name, (*result)[2].GetUInt8(), (*result)[0].GetUInt8(), (*result)[1].GetUInt8());
    }

    static void HandleCharacterLevel(Player* player, uint64 playerGuid, uint32 oldLevel, uint32 newLevel, ChatHandler* handler)
    {
        if (player)
        {
            player->GiveLevel(newLevel);
            player->InitTalentForLevel();
            player->SetUInt32Value(PLAYER_XP, 0);

            if (handler->needReportToTarget(player))
            {
                if (oldLevel == newLevel)
                    ChatHandler(player).PSendSysMessage(LANG_YOURS_LEVEL_PROGRESS_RESET, handler->GetNameLink().c_str());
                else if (oldLevel < newLevel)
                    ChatHandler(player).PSendSysMessage(LANG_YOURS_LEVEL_UP, handler->GetNameLink().c_str(), newLevel);
                else                                                // if (oldlevel > newlevel)
                    ChatHandler(player).PSendSysMessage(LANG_YOURS_LEVEL_DOWN, handler->GetNameLink().c_str(), newLevel);
            }
        }
        else
        {
            // Update level and reset XP, everything else will be updated at login
            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_LEVEL);
            stmt->setUInt8(0, uint8(newLevel));
            stmt->setUInt32(1, GUID_LOPART(playerGuid));
            CharacterDatabase.Execute(stmt);
        }
    }

    static bool HandleCharacterTitlesCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        Player* target;
        if (!handler->extractPlayerTarget((char*)args, &target))
            return false;

        LocaleConstant loc = handler->GetSessionDbcLocale();
        char const* targetName = target->GetName();
        char const* knownStr = handler->GetTrinityString(LANG_KNOWN);

        // Search in CharTitles.dbc
        for (uint32 id = 0; id < sCharTitlesStore.GetNumRows(); id++)
        {
            CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(id);

            if (titleInfo && target->HasTitle(titleInfo))
            {
                std::string name = titleInfo->name[loc];
                if (name.empty())
                    continue;

                char const* activeStr = target && target->GetUInt32Value(PLAYER_CHOSEN_TITLE) == titleInfo->bit_index
                ? handler->GetTrinityString(LANG_ACTIVE)
                : "";

                char titleNameStr[80];
                snprintf(titleNameStr, 80, name.c_str(), targetName);

                // send title in "id (idx:idx) - [namedlink locale]" format
                if (handler->GetSession())
                    handler->PSendSysMessage(LANG_TITLE_LIST_CHAT, id, titleInfo->bit_index, id, titleNameStr, localeNames[loc], knownStr, activeStr);
                else
                    handler->PSendSysMessage(LANG_TITLE_LIST_CONSOLE, id, titleInfo->bit_index, name.c_str(), localeNames[loc], knownStr, activeStr);
            }
        }

        return true;
    }

    //rename characters
    static bool HandleCharacterRenameCommand(ChatHandler* handler, char const* args)
    {
        Player* target;
        uint64 targetGuid;
        std::string targetName;
        if (!handler->extractPlayerTarget((char*)args, &target, &targetGuid, &targetName))
            return false;

        if (target)
        {
            // check online security
            if (handler->HasLowerSecurity(target, 0))
                return false;

            handler->PSendSysMessage(LANG_RENAME_PLAYER, handler->GetNameLink(target).c_str());
            target->SetAtLoginFlag(AT_LOGIN_RENAME);
        }
        else
        {
            // check offline security
            if (handler->HasLowerSecurity(NULL, targetGuid))
                return false;

            std::string oldNameLink = handler->playerLink(targetName);
            handler->PSendSysMessage(LANG_RENAME_PLAYER_GUID, oldNameLink.c_str(), GUID_LOPART(targetGuid));

            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_ADD_AT_LOGIN_FLAG);
            stmt->setUInt16(0, uint16(AT_LOGIN_RENAME));
            stmt->setUInt32(1, GUID_LOPART(targetGuid));
            CharacterDatabase.Execute(stmt);
        }

        return true;
    }

    static bool HandleCharacterLevelCommand(ChatHandler* handler, char const* args)
    {
        char* nameStr;
        char* levelStr;
        handler->extractOptFirstArg((char*)args, &nameStr, &levelStr);
        if (!levelStr)
            return false;

        // exception opt second arg: .character level $name
        if (isalpha(levelStr[0]))
        {
            nameStr = levelStr;
            levelStr = NULL;                                    // current level will used
        }

        Player* target;
        uint64 targetGuid;
        std::string targetName;
        if (!handler->extractPlayerTarget(nameStr, &target, &targetGuid, &targetName))
            return false;

        int32 oldlevel = target ? target->getLevel() : Player::GetLevelFromDB(targetGuid);
        int32 newlevel = levelStr ? atoi(levelStr) : oldlevel;

        if (newlevel < 1)
            return false;                                       // invalid level

        if (newlevel > STRONG_MAX_LEVEL)                         // hardcoded maximum level
            newlevel = STRONG_MAX_LEVEL;

        HandleCharacterLevel(target, targetGuid, oldlevel, newlevel, handler);
        if (!handler->GetSession() || handler->GetSession()->GetPlayer() != target)      // including player == NULL
        {
            std::string nameLink = handler->playerLink(targetName);
            handler->PSendSysMessage(LANG_YOU_CHANGE_LVL, nameLink.c_str(), newlevel);
        }

        return true;
    }

    // customize characters
    static bool HandleCharacterCustomizeCommand(ChatHandler* handler, char const* args)
    {
        Player* target;
        uint64 targetGuid;
        std::string targetName;
        if (!handler->extractPlayerTarget((char*)args, &target, &targetGuid, &targetName))
            return false;

        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_ADD_AT_LOGIN_FLAG);
        stmt->setUInt16(0, uint16(AT_LOGIN_CUSTOMIZE));
        if (target)
        {
            handler->PSendSysMessage(LANG_CUSTOMIZE_PLAYER, handler->GetNameLink(target).c_str());
            target->SetAtLoginFlag(AT_LOGIN_CUSTOMIZE);
            stmt->setUInt32(1, target->GetGUIDLow());
        }
        else
        {
            std::string oldNameLink = handler->playerLink(targetName);
            stmt->setUInt32(1, GUID_LOPART(targetGuid));
            handler->PSendSysMessage(LANG_CUSTOMIZE_PLAYER_GUID, oldNameLink.c_str(), GUID_LOPART(targetGuid));
        }
        CharacterDatabase.Execute(stmt);

        return true;
    }

    static bool HandleCharacterChangeFactionCommand(ChatHandler* handler, char const* args)
    {
        Player* target;
        uint64 targetGuid;
        std::string targetName;

        if (!handler->extractPlayerTarget((char*)args, &target, &targetGuid, &targetName))
            return false;

        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_ADD_AT_LOGIN_FLAG);
        stmt->setUInt16(0, uint16(AT_LOGIN_CHANGE_FACTION));
        if (target)
        {
            handler->PSendSysMessage(LANG_CUSTOMIZE_PLAYER, handler->GetNameLink(target).c_str());
            target->SetAtLoginFlag(AT_LOGIN_CHANGE_FACTION);
            stmt->setUInt32(1, target->GetGUIDLow());
        }
        else
        {
            std::string oldNameLink = handler->playerLink(targetName);
            handler->PSendSysMessage(LANG_CUSTOMIZE_PLAYER_GUID, oldNameLink.c_str(), GUID_LOPART(targetGuid));
            stmt->setUInt32(1, GUID_LOPART(targetGuid));
        }
        CharacterDatabase.Execute(stmt);

        return true;
    }

    static bool HandleCharacterChangeRaceCommand(ChatHandler* handler, char const* args)
    {
        Player* target;
        uint64 targetGuid;
        std::string targetName;
        if (!handler->extractPlayerTarget((char*)args, &target, &targetGuid, &targetName))
            return false;

        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_ADD_AT_LOGIN_FLAG);
        stmt->setUInt16(0, uint16(AT_LOGIN_CHANGE_RACE));
        if (target)
        {
            // TODO : add text into database
            handler->PSendSysMessage(LANG_CUSTOMIZE_PLAYER, handler->GetNameLink(target).c_str());
            target->SetAtLoginFlag(AT_LOGIN_CHANGE_RACE);
            stmt->setUInt32(1, target->GetGUIDLow());
        }
        else
        {
            std::string oldNameLink = handler->playerLink(targetName);
            // TODO : add text into database
            handler->PSendSysMessage(LANG_CUSTOMIZE_PLAYER_GUID, oldNameLink.c_str(), GUID_LOPART(targetGuid));
            stmt->setUInt32(1, GUID_LOPART(targetGuid));
        }
        CharacterDatabase.Execute(stmt);

        return true;
    }

    static bool HandleCharacterReputationCommand(ChatHandler* handler, char const* args)
    {
        Player* target;
        if (!handler->extractPlayerTarget((char*)args, &target))
            return false;

        LocaleConstant loc = handler->GetSessionDbcLocale();

        FactionStateList const& targetFSL = target->GetReputationMgr().GetStateList();
        for (FactionStateList::const_iterator itr = targetFSL.begin(); itr != targetFSL.end(); ++itr)
        {
            FactionState const& faction = itr->second;
            FactionEntry const* factionEntry = sFactionStore.LookupEntry(faction.ID);
            char const* factionName = factionEntry ? factionEntry->name[loc] : "#Not found#";
            ReputationRank rank = target->GetReputationMgr().GetRank(factionEntry);
            std::string rankName = handler->GetTrinityString(ReputationRankStrIndex[rank]);
            std::ostringstream ss;
            if (handler->GetSession())
                ss << faction.ID << " - |cffffffff|Hfaction:" << faction.ID << "|h[" << factionName << ' ' << localeNames[loc] << "]|h|r";
            else
                ss << faction.ID << " - " << factionName << ' ' << localeNames[loc];

            ss << ' ' << rankName << " (" << target->GetReputationMgr().GetReputation(factionEntry) << ')';

            if (faction.Flags & FACTION_FLAG_VISIBLE)
                ss << handler->GetTrinityString(LANG_FACTION_VISIBLE);
            if (faction.Flags & FACTION_FLAG_AT_WAR)
                ss << handler->GetTrinityString(LANG_FACTION_ATWAR);
            if (faction.Flags & FACTION_FLAG_PEACE_FORCED)
                ss << handler->GetTrinityString(LANG_FACTION_PEACE_FORCED);
            if (faction.Flags & FACTION_FLAG_HIDDEN)
                ss << handler->GetTrinityString(LANG_FACTION_HIDDEN);
            if (faction.Flags & FACTION_FLAG_INVISIBLE_FORCED)
                ss << handler->GetTrinityString(LANG_FACTION_INVISIBLE_FORCED);
            if (faction.Flags & FACTION_FLAG_INACTIVE)
                ss << handler->GetTrinityString(LANG_FACTION_INACTIVE);

            handler->SendSysMessage(ss.str().c_str());
        }

        return true;
    }

   /**
    * Handles the '.character deleted list' command, which shows all deleted characters which matches the given search string
    *
    * @see HandleCharacterDeletedListHelper
    * @see HandleCharacterDeletedRestoreCommand
    * @see HandleCharacterDeletedDeleteCommand
    * @see DeletedInfoList
    *
    * @param args the search string which either contains a player GUID or a part fo the character-name
    */
    static bool HandleCharacterDeletedListCommand(ChatHandler* handler, char const* args)
    {
        DeletedInfoList foundList;
        if (!GetDeletedCharacterInfoList(foundList, args))
            return false;

        // if no characters have been found, output a warning
        if (foundList.empty())
        {
            handler->SendSysMessage(LANG_CHARACTER_DELETED_LIST_EMPTY);
            return false;
        }

        HandleCharacterDeletedListHelper(foundList, handler);

        return true;
    }

    /**
     * Handles the '.character deleted restore' command, which restores all deleted characters which matches the given search string
     *
     * The command automatically calls '.character deleted list' command with the search string to show all restored characters.
     *
     * @see HandleCharacterDeletedRestoreHelper
     * @see HandleCharacterDeletedListCommand
     * @see HandleCharacterDeletedDeleteCommand
     *
     * @param args the search string which either contains a player GUID or a part of the character-name
     */
    static bool HandleCharacterDeletedRestoreCommand(ChatHandler* handler, char const* args)
    {
        // It is required to submit at least one argument
        if (!*args)
            return false;

        std::string searchString;
        std::string newCharName;
        uint32 newAccount = 0;

        // GCC by some strange reason fail build code without temporary variable
        std::istringstream params(args);
        params >> searchString >> newCharName >> newAccount;

        DeletedInfoList foundList;
        if (!GetDeletedCharacterInfoList(foundList, searchString))
            return false;

        if (foundList.empty())
        {
            handler->SendSysMessage(LANG_CHARACTER_DELETED_LIST_EMPTY);
            return false;
        }

        handler->SendSysMessage(LANG_CHARACTER_DELETED_RESTORE);
        HandleCharacterDeletedListHelper(foundList, handler);

        if (newCharName.empty())
        {
            // Drop not existed account cases
            for (DeletedInfoList::iterator itr = foundList.begin(); itr != foundList.end(); ++itr)
                HandleCharacterDeletedRestoreHelper(*itr, handler);
        }
        else if (foundList.size() == 1 && normalizePlayerName(newCharName))
        {
            DeletedInfo delInfo = foundList.front();

            // update name
            delInfo.name = newCharName;

            // if new account provided update deleted info
            if (newAccount && newAccount != delInfo.accountId)
            {
                delInfo.accountId = newAccount;
                AccountMgr::GetName(newAccount, delInfo.accountName);
            }

            HandleCharacterDeletedRestoreHelper(delInfo, handler);
        }
        else
            handler->SendSysMessage(LANG_CHARACTER_DELETED_ERR_RENAME);

        return true;
    }

    /**
     * Handles the '.character deleted delete' command, which completely deletes all deleted characters which matches the given search string
     *
     * @see Player::GetDeletedCharacterGUIDs
     * @see Player::DeleteFromDB
     * @see HandleCharacterDeletedListCommand
     * @see HandleCharacterDeletedRestoreCommand
     *
     * @param args the search string which either contains a player GUID or a part fo the character-name
     */
    static bool HandleCharacterDeletedDeleteCommand(ChatHandler* handler, char const* args)
    {
        // It is required to submit at least one argument
        if (!*args)
            return false;

        DeletedInfoList foundList;
        if (!GetDeletedCharacterInfoList(foundList, args))
            return false;

        if (foundList.empty())
        {
            handler->SendSysMessage(LANG_CHARACTER_DELETED_LIST_EMPTY);
            return false;
        }

        handler->SendSysMessage(LANG_CHARACTER_DELETED_DELETE);
        HandleCharacterDeletedListHelper(foundList, handler);

        // Call the appropriate function to delete them (current account for deleted characters is 0)
        for (DeletedInfoList::const_iterator itr = foundList.begin(); itr != foundList.end(); ++itr)
            Player::DeleteFromDB(itr->lowGuid, 0, false, true);

        return true;
    }

    /**
     * Handles the '.character deleted old' command, which completely deletes all deleted characters deleted with some days ago
     *
     * @see Player::DeleteOldCharacters
     * @see Player::DeleteFromDB
     * @see HandleCharacterDeletedDeleteCommand
     * @see HandleCharacterDeletedListCommand
     * @see HandleCharacterDeletedRestoreCommand
     *
     * @param args the search string which either contains a player GUID or a part fo the character-name
     */
    static bool HandleCharacterDeletedOldCommand(ChatHandler* /*handler*/, char const* args)
    {
        int32 keepDays = sWorld->getIntConfig(CONFIG_CHARDELETE_KEEP_DAYS);

        char* daysStr = strtok((char*)args, " ");
        if (daysStr)
        {
            if (!isNumeric(daysStr))
                return false;

            keepDays = atoi(daysStr);
            if (keepDays < 0)
                return false;
        }
        // config option value 0 -> disabled and can't be used
        else if (keepDays <= 0)
            return false;

        Player::DeleteOldCharacters(uint32(keepDays));

        return true;
    }

    static bool HandleCharacterEraseCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        char* characterName_str = strtok((char*)args, " ");
        if (!characterName_str)
            return false;

        std::string characterName = characterName_str;
        if (!normalizePlayerName(characterName))
            return false;

        uint64 characterGuid;
        uint32 accountId;

        Player* player = sObjectAccessor->FindPlayerByName(characterName.c_str());
        if (player)
        {
            characterGuid = player->GetGUID();
            accountId = player->GetSession()->GetAccountId();
            player->GetSession()->KickPlayer();
        }
        else
        {
            characterGuid = sObjectMgr->GetPlayerGUIDByName(characterName);
            if (!characterGuid)
            {
                handler->PSendSysMessage(LANG_NO_PLAYER, characterName.c_str());
                handler->SetSentErrorMessage(true);
                return false;
            }
            accountId = sObjectMgr->GetPlayerAccountIdByGUID(characterGuid);
        }

        std::string accountName;
        AccountMgr::GetName(accountId, accountName);

        Player::DeleteFromDB(characterGuid, accountId, true, true);
        handler->PSendSysMessage(LANG_CHARACTER_DELETED, characterName.c_str(), GUID_LOPART(characterGuid), accountName.c_str(), accountId);

        return true;
    }

    static bool HandleLevelUpCommand(ChatHandler* handler, char const* args)
    {
        char* nameStr;
        char* levelStr;
        handler->extractOptFirstArg((char*)args, &nameStr, &levelStr);

        // exception opt second arg: .character level $name
        if (levelStr && isalpha(levelStr[0]))
        {
            nameStr = levelStr;
            levelStr = NULL;                                    // current level will used
        }

        Player* target;
        uint64 targetGuid;
        std::string targetName;
        if (!handler->extractPlayerTarget(nameStr, &target, &targetGuid, &targetName))
            return false;

        int32 oldlevel = target ? target->getLevel() : Player::GetLevelFromDB(targetGuid);
        int32 addlevel = levelStr ? atoi(levelStr) : 1;
        int32 newlevel = oldlevel + addlevel;

        if (newlevel < 1)
            newlevel = 1;

        if (newlevel > STRONG_MAX_LEVEL)                         // hardcoded maximum level
            newlevel = STRONG_MAX_LEVEL;

        HandleCharacterLevel(target, targetGuid, oldlevel, newlevel, handler);

        if (!handler->GetSession() || handler->GetSession()->GetPlayer() != target)      // including chr == NULL
        {
            std::string nameLink = handler->playerLink(targetName);
            handler->PSendSysMessage(LANG_YOU_CHANGE_LVL, nameLink.c_str(), newlevel);
        }

        return true;
    }

    static bool HandlePDumpLoadCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        char* fileStr = strtok((char*)args, " ");
        if (!fileStr)
            return false;

        char* accountStr = strtok(NULL, " ");
        if (!accountStr)
            return false;

        std::string accountName = accountStr;
        if (!AccountMgr::normalizeString(accountName))
        {
            handler->PSendSysMessage(LANG_ACCOUNT_NOT_EXIST, accountName.c_str());
            handler->SetSentErrorMessage(true);
            return false;
        }

        uint32 accountId = AccountMgr::GetId(accountName);
        if (!accountId)
        {
            accountId = atoi(accountStr);                             // use original string
            if (!accountId)
            {
                handler->PSendSysMessage(LANG_ACCOUNT_NOT_EXIST, accountName.c_str());
                handler->SetSentErrorMessage(true);
                return false;
            }
        }

        if (!AccountMgr::GetName(accountId, accountName))
        {
            handler->PSendSysMessage(LANG_ACCOUNT_NOT_EXIST, accountName.c_str());
            handler->SetSentErrorMessage(true);
            return false;
        }

        char* guidStr = NULL;
        char* nameStr = strtok(NULL, " ");

        std::string name;
        if (nameStr)
        {
            name = nameStr;
            // normalize the name if specified and check if it exists
            if (!normalizePlayerName(name))
            {
                handler->PSendSysMessage(LANG_INVALID_CHARACTER_NAME);
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (ObjectMgr::CheckPlayerName(name, true) != CHAR_NAME_SUCCESS)
            {
                handler->PSendSysMessage(LANG_INVALID_CHARACTER_NAME);
                handler->SetSentErrorMessage(true);
                return false;
            }

            guidStr = strtok(NULL, " ");
        }

        uint32 guid = 0;

        if (guidStr)
        {
            guid = uint32(atoi(guidStr));
            if (!guid)
            {
                handler->PSendSysMessage(LANG_INVALID_CHARACTER_GUID);
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (sObjectMgr->GetPlayerAccountIdByGUID(guid))
            {
                handler->PSendSysMessage(LANG_CHARACTER_GUID_IN_USE, guid);
                handler->SetSentErrorMessage(true);
                return false;
            }
        }

        switch (PlayerDumpReader().LoadDump(fileStr, accountId, name, guid))
        {
            case DUMP_SUCCESS:
                handler->PSendSysMessage(LANG_COMMAND_IMPORT_SUCCESS);
                break;
            case DUMP_FILE_OPEN_ERROR:
                handler->PSendSysMessage(LANG_FILE_OPEN_FAIL, fileStr);
                handler->SetSentErrorMessage(true);
                return false;
            case DUMP_FILE_BROKEN:
                handler->PSendSysMessage(LANG_DUMP_BROKEN, fileStr);
                handler->SetSentErrorMessage(true);
                return false;
            case DUMP_TOO_MANY_CHARS:
                handler->PSendSysMessage(LANG_ACCOUNT_CHARACTER_LIST_FULL, accountName.c_str(), accountId);
                handler->SetSentErrorMessage(true);
                return false;
            default:
                handler->PSendSysMessage(LANG_COMMAND_IMPORT_FAILED);
                handler->SetSentErrorMessage(true);
                return false;
        }

        return true;
    }

    static bool HandlePDumpWriteCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        char* fileStr = strtok((char*)args, " ");
        char* playerStr = strtok(NULL, " ");

        if (!fileStr || !playerStr)
            return false;

        uint64 guid;
        // character name can't start from number
        if (isNumeric(playerStr))
            guid = MAKE_NEW_GUID(atoi(playerStr), 0, HIGHGUID_PLAYER);
        else
        {
            std::string name = handler->extractPlayerNameFromLink(playerStr);
            if (name.empty())
            {
                handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
                handler->SetSentErrorMessage(true);
                return false;
            }

            guid = sObjectMgr->GetPlayerGUIDByName(name);
        }

        if (!sObjectMgr->GetPlayerAccountIdByGUID(guid))
        {
            handler->PSendSysMessage(LANG_PLAYER_NOT_FOUND);
            handler->SetSentErrorMessage(true);
            return false;
        }

        switch (PlayerDumpWriter().WriteDump(fileStr, uint32(guid)))
        {
            case DUMP_SUCCESS:
                handler->PSendSysMessage(LANG_COMMAND_EXPORT_SUCCESS);
                break;
            case DUMP_FILE_OPEN_ERROR:
                handler->PSendSysMessage(LANG_FILE_OPEN_FAIL, fileStr);
                handler->SetSentErrorMessage(true);
                return false;
            case DUMP_CHARACTER_DELETED:
                handler->PSendSysMessage(LANG_COMMAND_EXPORT_DELETED_CHAR);
                handler->SetSentErrorMessage(true);
                return false;
            default:
                handler->PSendSysMessage(LANG_COMMAND_EXPORT_FAILED);
                handler->SetSentErrorMessage(true);
                return false;
        }

        return true;
    }

	/*      char** ChatHandler::split(char* chaine,const char* delim,int vide)      */
	/*      bool ChatHandler::Levelup(int levelcount)								*/
	/*      bool ChatHandler::AddGold(int gold)										*/
	/*      uint32 ChatHandler::getItemId(const char* args)							*/
	/*      bool ChatHandler::Additem(int itemid)									*/
	/*      int ChatHandler::coutlevel(int level)									*/
	/*      bool ChatHandler::BoutiqueAdditem(const char* args)						*/
	/*      bool ChatHandler::BoutiqueMetier(const char* args)						*/
	/*      bool ChatHandler::BoutiqueGold(const char* args)						*/
	
	/*                          SHOP ONLINE V1.0									*/
	/* tables : db_char.compte : acct,points 										*/
	/*			db_char.shop_log : id,acct,type,valeur1,valeur2,valeur3				*/
	/*			db_char.shop_item : entry,prix,commentaire 							*/
	/*			db_char.shop_level : level,prix 									*/
	/*          db_char.shop_gold : minimum,maximum , prix_po						*/
	/*          db_char.shop_gold : minimum,maximum , prix_po						*/
	/*          db_char.shop_categorie: id,prix ,value,commentaire					*/
	/*          db_char.shop_set : set_id,prix ,data,commentaire					*/
	/*          db_char.shop_creation : id,prix ,data,commentaire				    */
	
	
	// Retour tableau des chaines recupérer. Terminé par NULL.
	// chaine : chaine à splitter
	// delim : delimiteur qui sert à la decoupe
	// vide : 0 : on n'accepte pas les chaines vides
	// 1 : on accepte les chaines vides
	static char** split(const char* chaine,const char* delim,int vide, int* sizeTabRet)
	{
		char** tab=NULL; //tableau de chaine, tableau resultat
		const char *ptr; //pointeur sur une partie de
		int sizeStr; //taille de la chaine à recupérer
		int sizeTab=0; //taille du tableau de chaine
		const char* largestring; //chaine à traiter
	
		int sizeDelim=strlen(delim); //taille du delimiteur
	
	
		largestring = chaine; //comme ca on ne modifie pas le pointeur d'origine
		//(faut ke je verifie si c bien nécessaire)
	
	
		while( (ptr=strstr(largestring, delim))!=NULL ){
			sizeStr=ptr-largestring;
	
			//si la chaine trouvé n'est pas vide ou si on accepte les chaine vide
			if(vide==1 || sizeStr!=0){
				//on alloue une case en plus au tableau de chaines
				sizeTab++;
				tab= (char**) realloc(tab,sizeof(char*)*sizeTab);
	
				//on alloue la chaine du tableau
				tab[sizeTab-1]=(char*) malloc( sizeof(char)*(sizeStr+1) );
				strncpy(tab[sizeTab-1],largestring,sizeStr);
				tab[sizeTab-1][sizeStr]='\0';
			}
	
			//on decale le pointeur largestring pour continuer la boucle apres le premier elément traiter
			ptr=ptr+sizeDelim;
			largestring=ptr;
		}
	
		//si la chaine n'est pas vide, on recupere le dernier "morceau"
		if(strlen(largestring)!=0){
			sizeStr=strlen(largestring);
			sizeTab++;
			tab= (char**) realloc(tab,sizeof(char*)*sizeTab);
			tab[sizeTab-1]=(char*) malloc( sizeof(char)*(sizeStr+1) );
			strncpy(tab[sizeTab-1],largestring,sizeStr);
			tab[sizeTab-1][sizeStr]='\0';
		}
		else if(vide==1){ //si on fini sur un delimiteur et si on accepte les mots vides,on ajoute un mot vide
			sizeTab++;
			tab= (char**) realloc(tab,sizeof(char*)*sizeTab);
			tab[sizeTab-1]=(char*) malloc( sizeof(char)*1 );
			tab[sizeTab-1][0]='\0';
		}
	
		*sizeTabRet = sizeTab;
	
		return tab;
	} 

	static bool Additem(ChatHandler* handler,int itemid, int count, int type, bool remplacement)
	{
		Player* pl = handler->GetSession()->GetPlayer();
	
		if(!pl) return false;
	
		uint64 receiver_guid = pl->GetGUID();
		int levelItem = 0;
	
		if (type == 0)
		{
			int typeRecup;
	
			QueryResult result = CharacterDatabase.PQuery("SELECT `typeRecup`, `level_item` FROM `recup_infos` WHERE name = '%s'",pl->GetName());
			if (!result) return false;
			Field* fields = result->Fetch();
			if (!fields) return false;
			typeRecup = fields[0].GetUInt32();
			levelItem = fields[1].GetUInt32();
	
			if(remplacement)
			{
				QueryResult result = CharacterDatabase.PQuery("SELECT `itemEpique`, `itemRare` FROM `recup_itemRemplacement` WHERE item = '%u'",itemid);
				if (result)
				{
					Field* fields = result->Fetch();
					if (fields)
					{
						if(typeRecup == 0 || typeRecup == 1)
							itemid = fields[typeRecup].GetUInt32();
					}
				}
			}
		}
	
		if(!levelItem)
			levelItem = 200;
	
		QueryResult result = WorldDatabase.PQuery("SELECT `ItemLevel` FROM `item_template` WHERE `entry` = '%d'",itemid);
	
		if (!result)
			return false;
	
		if (result)
		{
			Field* fields = result->Fetch();
	
			if(!fields)
				return false;
	
			if(fields)
			{
				int itemLevel = fields[0].GetUInt32();
	
				if(itemLevel > levelItem)
				{
					return false;
				}
			}
		}
	
		std::string name = pl->GetName();
		if(name.empty())
		{
			handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
			handler->SetSentErrorMessage(true);
			return false;
		}
	
		char* msgSubject;
		char* msgText;
	
	
		switch(type)
		{
		case 0 : msgSubject="Recuperation";sLog->outError(LOG_FILTER_GENERAL, "Recuperation");break;
		case 1 : msgSubject="Achat";sLog->outError(LOG_FILTER_GENERAL, "Achat");break;
		}
	
	
		switch(type)
		{
		case 0 : msgText="Voici les objets de votre recuperation. Nous vous remercions d'avoir choisi Avalon";sLog->outError(LOG_FILTER_GENERAL, "Voici les objets de votre recuperation. Nous vous remercions d'avoir choisi Avalon");break;
		case 1 : msgText="Merci pour votre achat. Bon jeu sur Avalon";sLog->outError(LOG_FILTER_GENERAL, "Merci pour votre achat. Bon jeu sur Avalon");break;
		}
	
		if (!msgSubject)
			return false;
	
		if (!msgText)
			return false;
	
		// msgSubject, msgText isn't NUL after prev. check
		std::string subject = msgSubject;
		std::string text    = msgText;
	
		// extract items
		typedef std::pair<uint32,uint32> ItemPair;
		typedef std::list< ItemPair > ItemPairs;
		ItemPairs items;
	
		uint32 item_id = itemid;
		if (!item_id)
			return false;
	
		ItemTemplate const* item_proto = sObjectMgr->GetItemTemplate(item_id);
		if (!item_proto)
		{
			handler->PSendSysMessage(LANG_COMMAND_ITEMIDINVALID, item_id);
			handler->SetSentErrorMessage(true);
			return false;
		}
	
		uint32 item_count = count;
		if (item_count < 1 || (item_proto->MaxCount > 0 && item_count > uint32(item_proto->MaxCount)))
		{
			handler->PSendSysMessage(LANG_COMMAND_INVALID_ITEM_COUNT, item_count,item_id);
			handler->SetSentErrorMessage(true);
			return false;
		}
	
		while (item_count > item_proto->GetMaxStackSize())
		{
			items.push_back(ItemPair(item_id,item_proto->GetMaxStackSize()));
			item_count -= item_proto->GetMaxStackSize();
		}
	
		items.push_back(ItemPair(item_id,item_count));
	
		if (items.size() > MAX_MAIL_ITEMS)
		{
			handler->PSendSysMessage(LANG_COMMAND_MAIL_ITEMS_LIMIT, MAX_MAIL_ITEMS);
			handler->SetSentErrorMessage(true);
			return false;
		}
	
		// from console show not existed sender
		MailSender sender(MAIL_NORMAL,handler->GetSession() ? handler->GetSession()->GetPlayer()->GetGUIDLow() : 0, MAIL_STATIONERY_GM);
	
		// fill mail
		MailDraft draft(subject, text);
	
		SQLTransaction trans = CharacterDatabase.BeginTransaction();
	
		for (ItemPairs::const_iterator itr = items.begin(); itr != items.end(); ++itr)
		{
			if (Item* item = Item::CreateItem(itr->first,itr->second,handler->GetSession() ? handler->GetSession()->GetPlayer() : 0))
			{
				item->SaveToDB(trans);                               // save for prevent lost at next mail load, if send fail then item will deleted
				draft.AddItem(item);
			}
		}
	
		draft.SendMailTo(trans, MailReceiver(pl,GUID_LOPART(receiver_guid)), sender);
		CharacterDatabase.CommitTransaction(trans);
	
		std::string nameLink = handler->playerLink(name);
		handler->PSendSysMessage(LANG_MAIL_SENT, nameLink.c_str());
	
		return true;
	}
	
	static uint32 getItemId(ChatHandler* handler,const char* args)
	{
	
		uint32 itemId = 0;
	
		if(args[0]=='[')                                        // [name] manual form
		{
			char* citemName = citemName = strtok((char*)args, "]");
	
			if(citemName && citemName[0])
			{
				std::string itemName = citemName+1;
				WorldDatabase.EscapeString(itemName);
				QueryResult result = WorldDatabase.PQuery("SELECT entry FROM item_template WHERE name = '%s'", itemName.c_str());
				if (!result)
				{
					handler->PSendSysMessage(LANG_COMMAND_COULDNOTFIND, citemName+1);
					handler->SetSentErrorMessage(true);
					return 0;
				}
				itemId = result->Fetch()->GetUInt16();
			}
			else
				return 0;
		}
		else                                                    // item_id or [name] Shift-click form |color|Hitem:item_id:0:0:0|h[name]|h|r
		{
			char* cId = handler->extractKeyFromLink((char*)args,"Hitem");
			if(!cId)
				return 0;
			itemId = atol(cId);
		}
	
		return itemId;
	}
	
	static bool Levelup(ChatHandler* handler,int levelcount)
	{
		if(levelcount<1 || levelcount > 79)
			return false;
	
		Player *chr = handler->GetSession()->GetPlayer();
	
		if(chr && chr->getLevel()+levelcount > 80)
			return false;
	
		HandleCharacterLevel(chr,0,chr->getLevel(),chr->getLevel()+levelcount,handler);
	
		return true;
	}
	
	static int coutlevel(ChatHandler* /*handler*/,int level)
	{
		QueryResult result = CharacterDatabase.PQuery("SELECT `prix` FROM `shop_level` WHERE level = '%u'",level);
		if (!result) return 0;
		Field* fields = result->Fetch();
		if (!fields) return 0;
		return fields[0].GetUInt32();
	}
	
	static bool AddGold(ChatHandler* handler,int gold)
	{
		if(gold <0) return false;
	
		int32 addmoney = gold*10000;
		Player * chr = handler->GetSession()->GetPlayer();
		uint32 moneyuser = chr->GetMoney();
	
		if(addmoney < 0)
		{
			int32 newmoney = moneyuser + addmoney;
	
			if(newmoney <= 0 )
			{
				chr->SetMoney(0);
			}
			else
			{
				chr->SetMoney( newmoney );
			}
		}
		else
		{
			chr->ModifyMoney( addmoney );
		}
	
		handler->GetSession()->GetPlayer()->SaveToDB();
		return true;
	}
	
	static bool BoutiqueLevel(ChatHandler* handler,const char* args)
	{
		int asklvl = 0;
		int points = 0;
		int currentlevel = 0;
		int prix_level = 0;
		int i = 0;
		int bilan = 0;
	
		if (!*args)
			return false;
	
		char* px = strtok((char*)args, " ");
		asklvl = atoi(px);
	
		if (asklvl < 0) return false;
	
		Player* character = handler->GetSession()->GetPlayer();
		Player* plTarget = NULL;
		if(!plTarget)
			plTarget = character;
	
		QueryResult result = CharacterDatabase.PQuery("SELECT `points` FROM `compte` WHERE `acct` = '%u'",character->GetSession()->GetAccountId());
		if (!result) return false;
	
		Field* fields = result->Fetch();
		if (!fields) return false;
	
		points = fields[0].GetUInt32();
		currentlevel = character->getLevel();
	
		if ((currentlevel == 80)||(currentlevel+asklvl > 80))
		{
			handler->GetSession()->SendNotification("Erreur sur le level demande");
			return false;
		}
	
	
		while (i<asklvl)
		{
			currentlevel = character->getLevel() + i;
			if(coutlevel(handler,currentlevel) == 0) return true;
			prix_level += coutlevel(handler,currentlevel);
			i++;
		}
	
		if (prix_level > points)
		{
			handler->GetSession()->SendNotification("Vous n'avez pas assez de points.Il vous faut %u points pour votre demande.",prix_level);
			return false;
		}
		bilan = points - prix_level;
		CharacterDatabase.PQuery("UPDATE `compte` SET `points` = '%u' WHERE acct='%u'",bilan,character->GetSession()->GetAccountId());
		CharacterDatabase.PQuery("INSERT INTO `shop_log` (`id`,`acct`,`type`,`valeur1`,`valeur2`) VALUES ('','%u','level','%u','%u')",character->GetSession()->GetAccountId(),character->getLevel(),asklvl);
		Levelup(handler,asklvl);
		handler->GetSession()->SendNotification("Il vous reste %u points sur votre compte",bilan);
		return true;
	
	}
	
	// Boutique Infos
	static bool BoutiqueInfos(ChatHandler* handler,const char* args)
	{
		int points = 0;
		int renommage = 50;
		int customisation = 100;
		int race = 125;
		int faction = 150;
	
		Player* character = handler->GetSession()->GetPlayer();
	
		QueryResult result = CharacterDatabase.PQuery("SELECT `points` FROM `compte` WHERE `acct` = '%u'",character->GetSession()->GetAccountId());
		if (!result) return false;
	
		Field* fields = result->Fetch();
		if (!fields) return false;
	
		points = fields[0].GetUInt32();
	
		handler->PSendSysMessage("Vous avez %u points",points);
		handler->PSendSysMessage("Renommage : %u points",renommage);
		handler->PSendSysMessage("Customisation : %u points",customisation);
		handler->PSendSysMessage("Changement de Race : %u points",race);
		handler->PSendSysMessage("Changement de Faction : %u points",faction);
	
		return true;
	}
	
	//Boutique additem
	static bool BoutiqueAdditem(ChatHandler* handler,const char* args)
	{
		int points;
		int prix;
		int bilan;
		uint32 ItemId = 0;
	
		if (!*args)
			return false;
	
		char** tab;
		int size = 0;
	
		tab=split(args," ",0,&size);
	
		int32 count = size==2?atoi(tab[1]):1;
	
		int i = 0;
	
		for(i=0;i<size;i++) {
			free(tab[i]);
		}
		free(tab);
	
	
	
		if(count < 0){
			handler->GetSession()->SendNotification("La quantité ne peut pas être négative");
			return false;
		}
	
		if (count == 0)
			count = 1;
	
	
		Player* character = handler->GetSession()->GetPlayer();
	
		ItemId = getItemId(handler,args);
	
		QueryResult result = CharacterDatabase.PQuery("SELECT `prix` FROM `shop_item` WHERE `entry` = '%u'",ItemId);
	
		if(!result){
			handler->GetSession()->SendNotification("L'objet n'est pas disponible");
			return false;
		}
	
		Field* fields = result->Fetch();
	
		if(!fields)
			return false;
	
		if (fields[0].GetUInt32() == 0)
			return false;
	
		prix = count*(fields[0].GetUInt32());
	
	
		result = CharacterDatabase.PQuery("SELECT `points` FROM `compte` WHERE `acct` = '%u'",character->GetSession()->GetAccountId());
	
		if(!result)
			return false;
	
		fields = result->Fetch();
	
	
		if(!fields)
			return false;
	
		points = fields[0].GetUInt32();
	
		if(points < prix)
		{
			handler->GetSession()->SendNotification("Vous n'avez pas assez de points.Il vous faut %u points pour votre demande.",prix);
			return false;
		}
	
		bilan = points - prix;
		if(!Additem(handler,ItemId, count,1, false))
		{
			handler->GetSession()->SendNotification("Vous n'avez pas assez de place dans vos sacs");
			return false;
		}
		CharacterDatabase.PQuery("UPDATE `compte` SET `points` = '%u' WHERE acct='%u'",bilan,character->GetSession()->GetAccountId());
		// On veut log, surtout au debut pour verifier les abus sur les prix pas tres juste :/
		CharacterDatabase.PQuery("INSERT INTO `shop_log` (`acct`,`type`,`valeur1`,`valeur2`) VALUES ('%u','item','%u','%u')",character->GetSession()->GetAccountId(),ItemId,count);
		return true;
	}
	
	//boutique métier
	static bool BoutiqueMetier(ChatHandler* handler,const char* args)
	{
	
		int setskill = 0;
		int id = 0;
		int points = 0;
		int value = 0;
		int prix = 0;
		int bilan = 0;
	
		if (!*args)
		       return false;
	
		char* px = strtok((char*)args, " ");
		id = atoi(px);
		
		if (id < 0) return false;
	
		Player* character = handler->GetSession()->GetPlayer();
	
		QueryResult result = CharacterDatabase.PQuery("SELECT `points` FROM `compte` WHERE `acct` = '%u'",character->GetSession()->GetAccountId());
		if (!result) return false;
	
		Field* fields = result->Fetch();
		if (!fields) return false;
	
		points = fields[0].GetUInt32();
	
		if (id < 0) return false;
		result = CharacterDatabase.PQuery("SELECT prix,value,setskill FROM `shop_metier` WHERE `id` = '%u'",id);
		if (!result) return false;
	
		fields = result->Fetch();
		if (!fields) return false;
	
		prix = fields[0].GetUInt32();
		value = fields[1].GetUInt32();
		setskill = fields[2].GetUInt32();
		
		if (prix > points)
		{
			handler->GetSession()->SendNotification("Vous n'avez pas assez de points.Il vous faut %u points pour votre demande.",prix);
			return false;
		}
		bilan = points - prix;
		if(apprendreMetier(handler,character, setskill, value)){
			CharacterDatabase.PQuery("UPDATE `compte` SET `points` = '%u' WHERE acct='%u'",bilan,character->GetSession()->GetAccountId());
			handler->GetSession()->SendNotification("Il vous reste %u points sur votre compte",bilan);
		}
		else return false;
		return true;
	}
	
	static bool BoutiqueGold(ChatHandler* handler,const char* args)
	{
		if (!*args)
			return false;
	
		uint32 bilan = 0;
		uint32 points = 0;
		float prix = 0;
		float Qt = 0;
		float askgold2 = 0;
	
	
		Player* character = handler->GetSession()->GetPlayer();
	
		char* px = strtok((char*)args, " ");
	
		int askgold = atoi(px);
		askgold2 = askgold;
		if ((askgold <= 0)||(askgold>10000))
		{
			handler->GetSession()->SendNotification("Cette Valeur n'est pas autorise");
			return false;
		}
	
		QueryResult result = CharacterDatabase.PQuery("SELECT id FROM shop_gold WHERE minimum <= '%u' AND maximum >= '%u'",askgold,askgold);
		if (!result) return false;
		Field* fields = result->Fetch();
		if (!fields) return false;
		uint32 ui = fields[0].GetUInt32();
		for(uint32 i = 1;i <= ui;i++)
		{
			result = CharacterDatabase.PQuery("SELECT maximum,prix_po,minimum FROM shop_gold WHERE id='%u'",i);
			if (!result) return false;
			Field* fields = result->Fetch();
			if (!fields) return false;
			if(fields[0].GetFloat() < askgold)
			{
				askgold2 = askgold;
				Qt = fields[0].GetFloat() - (fields[2].GetFloat() - 1);
				askgold2 -= fields[0].GetFloat();
			}
			else
			{
				Qt = askgold2;
				askgold2 = 0;
			}
			prix += Qt*(fields[1].GetFloat());
		}
	
		result = CharacterDatabase.PQuery("SELECT `points` FROM `compte` WHERE `acct` = '%u'",character->GetSession()->GetAccountId());
	
		if(!result)
			return false;
	
		fields = result->Fetch();
	
		if(!fields)
			return false;
	
		points = fields[0].GetUInt32();
		uint32 prix2 = (int)prix;
	
		if(points < prix2)
		{
			handler->GetSession()->SendNotification("Il vous manque %u points pour cet achat",prix2 - points);
			return false;
		}
	
	
		bilan = points - prix2;
		CharacterDatabase.PQuery("UPDATE `compte` SET `points` = '%u' WHERE acct='%u'",bilan,character->GetSession()->GetAccountId());
		CharacterDatabase.PQuery("INSERT INTO `shop_log` (`id`,`acct`,`type`,`valeur1`,`valeur2`) VALUES ('','%u','gold','%u','%u')",character->GetSession()->GetAccountId(),askgold,prix2);
		character->SetMoney(character->GetMoney() + askgold*10000);
		handler->GetSession()->SendNotification("Il vous reste %u points sur votre compte",bilan);
	
		return true;
	
	
	}
	
	// customize characters
	static bool BoutiqueCustomize(ChatHandler* handler,const char* args)
	{
		int points = 0;
		int prix_custom = 100;
		int bilan = 0;
		Player* character = handler->GetSession()->GetPlayer();
	
		//TODO : faire la requete voir si l'object à 1 prix
		QueryResult result = CharacterDatabase.PQuery("SELECT `points` FROM `compte` WHERE `acct` = '%u'",character->GetSession()->GetAccountId());
	
		if(!result)
			return false;
	
		Field* fields = result->Fetch();
	
	
		if(!fields)
			return false;
	
		points = fields[0].GetUInt32();
	
		if(points < prix_custom)
		{
			handler->GetSession()->SendNotification("Vous n'avez pas assez de points.Il vous faut %u points pour votre demande.",prix_custom);
			return false;
		}
	
		Player* target = handler->GetSession()->GetPlayer();
		bilan = points - prix_custom;
		if(target)
		{
			handler->PSendSysMessage(LANG_CUSTOMIZE_PLAYER, handler->GetNameLink(target).c_str());
			target->SetAtLoginFlag(AT_LOGIN_CUSTOMIZE);
			CharacterDatabase.PQuery("UPDATE `compte` SET `points` = '%u' WHERE acct='%u'",bilan,character->GetSession()->GetAccountId());
			CharacterDatabase.PExecute("UPDATE characters SET at_login = at_login | '8' WHERE guid = '%u'", target->GetGUIDLow());
		}
		else
		{
			return false;
		}
	
		return true;
	}
	
	// rename characters
	static bool BoutiqueRename(ChatHandler* handler,const char* args)
	{
		int points = 0;
		int prix_rename = 50;
		int bilan = 0;
	
		Player* character = handler->GetSession()->GetPlayer();
	
		//TODO : faire la requete voir si l'object à 1 prix
		QueryResult result = CharacterDatabase.PQuery("SELECT `points` FROM `compte` WHERE `acct` = '%u'",character->GetSession()->GetAccountId());
	
		if(!result)
			return false;
	
		Field* fields = result->Fetch();
	
	
		if(!fields)
			return false;
	
		points = fields[0].GetUInt32();
	
		if(points < prix_rename)
		{
			handler->GetSession()->SendNotification("Vous n'avez pas assez de points.Il vous faut %u points pour votre demande.",prix_rename);
			return false;
		}
	
		Player* target = handler->GetSession()->GetPlayer();
		bilan = points - prix_rename;
		if(target)
		{
			handler->PSendSysMessage(LANG_RENAME_PLAYER, handler->GetNameLink(target).c_str());
			target->SetAtLoginFlag(AT_LOGIN_RENAME);
			CharacterDatabase.PQuery("UPDATE `compte` SET `points` = '%u' WHERE acct='%u'",bilan,character->GetSession()->GetAccountId());
			//CharacterDatabase.PExecute("UPDATE characters SET at_login = at_login | '8' WHERE guid = '%u'", target->GetGUIDLow());
		}
		else
		{
			return false;
		}
	
		return true;
	}
	
	// race characters
	static bool BoutiqueRace(ChatHandler* handler,const char* args)
	{
		int points = 0;
		int prix_race = 125;
		int bilan = 0;
	
		Player* character = handler->GetSession()->GetPlayer();
	
		//TODO : faire la requete voir si l'object à 1 prix
		QueryResult result = CharacterDatabase.PQuery("SELECT `points` FROM `compte` WHERE `acct` = '%u'",character->GetSession()->GetAccountId());
	
		if(!result)
			return false;
	
		Field* fields = result->Fetch();
	
	
		if(!fields)
			return false;
	
		points = fields[0].GetUInt32();
	
		if(points < prix_race)
		{
			handler->GetSession()->SendNotification("Vous n'avez pas assez de points.Il vous faut %u points pour votre demande.",prix_race);
			return false;
		}
	
		Player* target = handler->GetSession()->GetPlayer();
		bilan = points - prix_race;
		if(target)
		{
			handler->PSendSysMessage(LANG_CUSTOMIZE_PLAYER, handler->GetNameLink(target).c_str());
			target->SetAtLoginFlag(AT_LOGIN_CHANGE_RACE);
			CharacterDatabase.PExecute("UPDATE characters SET at_login = at_login | '128' WHERE guid = %u", target->GetGUIDLow());
			CharacterDatabase.PQuery("UPDATE `compte` SET `points` = '%u' WHERE acct='%u'",bilan,character->GetSession()->GetAccountId());
		}
		else
		{
			return false;
		}
	
		return true;
	}
	
	// faction characters
	static bool BoutiqueFaction(ChatHandler* handler,const char* args)
	{
		int points = 0;
		int prix_faction = 150;
		int bilan = 0;
	
		Player* character = handler->GetSession()->GetPlayer();
	
		//TODO : faire la requete voir si l'object à 1 prix
		QueryResult result = CharacterDatabase.PQuery("SELECT `points` FROM `compte` WHERE `acct` = '%u'",character->GetSession()->GetAccountId());
	
		if(!result)
			return false;
	
		Field* fields = result->Fetch();
	
	
		if(!fields)
			return false;
	
		points = fields[0].GetUInt32();
	
		if(points < prix_faction)
		{
			handler->GetSession()->SendNotification("Vous n'avez pas assez de points.Il vous faut %u points pour votre demande.",prix_faction);
			return false;
		}
	
		Player* target = handler->GetSession()->GetPlayer();
		bilan = points - prix_faction;
		if(target)
		{
			handler->PSendSysMessage(LANG_CUSTOMIZE_PLAYER, handler->GetNameLink(target).c_str());
			target->SetAtLoginFlag(AT_LOGIN_CHANGE_FACTION);
			CharacterDatabase.PExecute("UPDATE characters SET at_login = at_login | '64' WHERE guid = %u", target->GetGUIDLow());
			CharacterDatabase.PQuery("UPDATE `compte` SET `points` = '%u' WHERE acct='%u'",bilan,character->GetSession()->GetAccountId());
		}
		else
		{
			return false;
		}
	
		return true;
	}
	
	static bool BoutiqueSet(ChatHandler* handler,const char* args)
	{
		int points = 0;
		int id = 0;
	
		if (!*args)
			return false;
	
		char* px = strtok((char*)args, " ");
	
		id = atoi(px);
	
		Player* character = handler->GetSession()->GetPlayer();
	
		QueryResult result = CharacterDatabase.PQuery("SELECT `points` FROM `compte` WHERE `acct` = '%u'",character->GetSession()->GetAccountId());
	
		if(!result) return false;
	
		Field* fields = result->Fetch();
	
		if (!fields) return false;
	
		points = fields[0].GetUInt32();
	
		result = CharacterDatabase.PQuery("SELECT `data`,`prix` FROM `shop_set` WHERE `set_id` = '%u'",id);
	
		if(!result) return false;
	
		fields = result->Fetch();
	
		if (!fields) return false;
	
		const char* data = fields[0].GetCString();
		int prix = fields[1].GetUInt32();
	
		if(prix>points){
			handler->GetSession()->SendNotification("Il vous manque %u points pour cet achat",prix - points);
			return false;
		}
	
		CharacterDatabase.PQuery("UPDATE `compte` SET `points` = '%u' WHERE acct='%u'",points-prix,character->GetSession()->GetAccountId());
	
		char** tab;
		int size = 0;
	
		tab=split(data," ",0,&size);
	
		int i = 0;
	
		for(i=0;i<size;i++) {
			Additem(handler,atoi(tab[i]),1,1,false);
			free(tab[i]);
		}
		free(tab);
	
		return true;
	}

	static bool apprendreMetier(ChatHandler* handler,Player* character, int id, int value)
	{
	
		int max = 75;
		int nbMetier = 0;
	
		if(value > 75)
			max = 150;
		if(value > 150)
			max = 225;
		if(value > 225)
			max = 300;
		if(value > 300)
			max = 375;
		if(value > 375)
			max = 450;
			
		if(!character) return false;
	
		if(id!=186 && (character->HasSpell(2580)||character->HasSpell(2575)||character->HasSpell(3564)||character->HasSpell(10248)||character->HasSpell(29354) || character->HasSpell(50310))) nbMetier++;
		if(id!=164 && (character->HasSpell(2018)||character->HasSpell(3100)||character->HasSpell(3538)||character->HasSpell(9785)||character->HasSpell(29844) || character->HasSpell(51300))) nbMetier++;
		if(id!=197 && (character->HasSpell(3908)||character->HasSpell(3909)||character->HasSpell(3910)||character->HasSpell(12180)||character->HasSpell(26790) || character->HasSpell(51309))) nbMetier++;
		if(id!=202 && (character->HasSpell(4036)||character->HasSpell(4037)||character->HasSpell(4038)||character->HasSpell(12656)||character->HasSpell(30350) || character->HasSpell(51306))) nbMetier++;
		if(id!=171 && (character->HasSpell(2259)||character->HasSpell(3101)||character->HasSpell(3464)||character->HasSpell(11611)||character->HasSpell(28596) || character->HasSpell(51304))) nbMetier++;
		if(id!=182 && (character->HasSpell(2372)||character->HasSpell(2373)||character->HasSpell(3571)||character->HasSpell(11994)||character->HasSpell(28696) || character->HasSpell(50300))) nbMetier++;
		if(id!=333 && (character->HasSpell(7411)||character->HasSpell(7412)||character->HasSpell(7413)||character->HasSpell(13920)||character->HasSpell(28029) || character->HasSpell(51313))) nbMetier++;
		if(id!=755 && (character->HasSpell(25229)||character->HasSpell(25230)||character->HasSpell(28894)||character->HasSpell(28895)||character->HasSpell(28897) || character->HasSpell(51311))) nbMetier++;
		if(id!=393 && (character->HasSpell(8613)||character->HasSpell(8617)||character->HasSpell(8618)||character->HasSpell(10768)||character->HasSpell(32678) || character->HasSpell(50305))) nbMetier++;
		if(id!=165 && (character->HasSpell(2108)||character->HasSpell(3104)||character->HasSpell(3811)||character->HasSpell(10662)||character->HasSpell(32549) || character->HasSpell(51302))) nbMetier++;
		if(id!=773 && (character->HasSpell(45357)||character->HasSpell(45358)||character->HasSpell(45359)||character->HasSpell(45360)||character->HasSpell(45361) || character->HasSpell(45363))) nbMetier++;
	
		if(id!=356 && id!=185 && id!=129 && nbMetier > 2){
			handler->GetSession()->SendNotification("Vous possèdez déjà 2 métiers");
			return false;
		}
		
		switch(id){
			case 186 : // Mineur
				ApprendreSort(handler,character,2580);
				ApprendreSort(handler,character,2575);
				if(value > 75)
					ApprendreSort(handler,character,2576);
				if(value > 150)
					ApprendreSort(handler,character,3564);
				if(value > 225)
					ApprendreSort(handler,character,10248);
				if(value > 300)
					ApprendreSort(handler,character,29354);
				if(value > 375)
					ApprendreSort(handler,character,50310);
			break;
			case 164 : // Forgeron
				ApprendreSort(handler,character,2018);
				if(value > 75)
					ApprendreSort(handler,character,3100);
				if(value > 150)
					ApprendreSort(handler,character,3538);
				if(value > 225)
					ApprendreSort(handler,character,9785);
				if(value > 300)
					ApprendreSort(handler,character,29844);
				if(value > 375)
					ApprendreSort(handler,character,51300);
			break;
			case 197 : // Couture
				ApprendreSort(handler,character,3908);
				if(value > 75)
					ApprendreSort(handler,character,3909);
				if(value > 150)
					ApprendreSort(handler,character,3910);
				if(value > 225)
					ApprendreSort(handler,character,12180);
				if(value > 300) 
					ApprendreSort(handler,character,26790);
				if(value > 375)
					ApprendreSort(handler,character,51309);
			break;
			case 202 : // Ingénieur
				ApprendreSort(handler,character,4036);
				if(value > 75)
					ApprendreSort(handler,character,4037);
				if(value > 150)
					ApprendreSort(handler,character,4038);
				if(value > 225)
					ApprendreSort(handler,character,12656);
				if(value > 300)
					ApprendreSort(handler,character,30350);
				if(value > 375)
					ApprendreSort(handler,character,51306);
			break;
			case 171 : // Alchimie
				ApprendreSort(handler,character,2259);
				if(value > 75)
					ApprendreSort(handler,character,3101);
				if(value > 150)
					ApprendreSort(handler,character,3464);
				if(value > 225)
					ApprendreSort(handler,character,11611);
				if(value > 300)
					ApprendreSort(handler,character,28596);
				if(value > 375)
					ApprendreSort(handler,character,51304);
			break;
			case 182 : // Herboriste
				ApprendreSort(handler,character,2372);
				if(value > 75)
					ApprendreSort(handler,character,2373);
				if(value > 150)
					ApprendreSort(handler,character,3571);
				if(value > 225)
					ApprendreSort(handler,character,11994);
				if(value > 300)
					ApprendreSort(handler,character,28696);
				if(value > 375)
					ApprendreSort(handler,character,50300);
			break;
			case 333 : // Enchanteur
				ApprendreSort(handler,character,7411);
				if(value > 75)
					ApprendreSort(handler,character,7412);
				if(value > 150)
					ApprendreSort(handler,character,7413);
				if(value > 225)
					ApprendreSort(handler,character,13920);
				if(value > 300)
					ApprendreSort(handler,character,28029);
				if(value > 375)
					ApprendreSort(handler,character,51313);
			break;
			case 755 : // Joaillier
				ApprendreSort(handler,character,25229);
				if(value > 75)
					ApprendreSort(handler,character,25230);
				if(value > 150)
					ApprendreSort(handler,character,28894);
				if(value > 225)
					ApprendreSort(handler,character,28895);
				if(value > 300)
					ApprendreSort(handler,character,28897);
				if(value > 375)
					ApprendreSort(handler,character,51311);
			break;
			case 393 : //dépeceur
				ApprendreSort(handler,character,8613);
				if(value > 75)
					ApprendreSort(handler,character,8617);
				if(value > 150)
					ApprendreSort(handler,character,8618);
				if(value > 225)
					ApprendreSort(handler,character,10768);
				if(value > 300)
					ApprendreSort(handler,character,32678);
				if(value > 375)
					ApprendreSort(handler,character,50305);
				break;
			case 165 : //tdc
				ApprendreSort(handler,character,2108);
				if(value > 75)
					ApprendreSort(handler,character,3104);
				if(value > 150)
					ApprendreSort(handler,character,3811);
				if(value > 225)
					ApprendreSort(handler,character,10662);
				if(value > 300)
					ApprendreSort(handler,character,32549);
				if(value > 375)
					ApprendreSort(handler,character,51302);
				break;
			case 773 : //calligraphie
				ApprendreSort(handler,character,45357);
				if(value > 75)
					ApprendreSort(handler,character,45358);
				if(value > 150)
					ApprendreSort(handler,character,45359);
				if(value > 225)
					ApprendreSort(handler,character,45360);
				if(value > 300)
					ApprendreSort(handler,character,45361);
				if(value > 375)
					ApprendreSort(handler,character,45363);
				break;
			case 356 : //peche
				ApprendreSort(handler,character,7620);
				if(value > 75)
					ApprendreSort(handler,character,7731);
				if(value > 150)
					ApprendreSort(handler,character,7732);
				if(value > 225)
					ApprendreSort(handler,character,18248);
				if(value > 300)
					ApprendreSort(handler,character,33095);
				if(value > 375)
					ApprendreSort(handler,character,51294);
				break;
			case 185 : //cuisine
				ApprendreSort(handler,character,2550);
				if(value > 75)
					ApprendreSort(handler,character,3102);
				if(value > 150)
					ApprendreSort(handler,character,3413);
				if(value > 225)
					ApprendreSort(handler,character,18260);
				if(value > 300)
					ApprendreSort(handler,character,33359);
				if(value > 375)
					ApprendreSort(handler,character,51296);
				break;
			case 129 : //secourisme
				ApprendreSort(handler,character,3273);
				if(value > 75)
					ApprendreSort(handler,character,3274);
				if(value > 150)
					ApprendreSort(handler,character,7924);
				if(value > 225)
					ApprendreSort(handler,character,10846);
				if(value > 300)
					ApprendreSort(handler,character,27028);
				if(value > 375)
					ApprendreSort(handler,character,45542);
				Additem(handler,6454,1,0,false);
				Additem(handler,21993,1,0,false);
				break;
			default : return false;
		}
	
		character->SetSkill(id,character->GetSkillStep(id),value,max);
	
		return true;
	}
	
	static bool ApprendreSort(ChatHandler* /*handler*/,Player* p , int idSort)
	{
		SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(idSort);
		if(spellInfo && sSpellMgr->IsSpellValid(spellInfo) && !p->HasSpell(idSort))
		{
			p->learnSpell(idSort,false);
			uint32 first_spell = sSpellMgr->GetFirstSpellInChain(idSort);
			if(GetTalentSpellCost(first_spell))
				p->SendTalentsInfoData(false);
		}
		else
			return false;
	
		return true;
	}
};

void AddSC_character_commandscript()
{
    new character_commandscript();
}
