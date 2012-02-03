DELETE FROM `spell_script_names` WHERE `spell_id` IN (62552,62719,66482);
INSERT INTO `spell_script_names`(`spell_id`,`ScriptName`) VALUES
(62552,'spell_gen_defend'),
(62719,'spell_gen_defend'),
(66482,'spell_gen_defend');
DELETE FROM `spell_script_names` WHERE `spell_id` IN (62874,62960,63661,62563,63003,63010,64591,66481,68282,68284,68321,68498,68501);
INSERT INTO `spell_script_names`(`spell_id`,`ScriptName`) VALUES
(62874,'spell_gen_mounted_charge'),
(62960,'spell_gen_mounted_charge'),
(63661,'spell_gen_mounted_charge'),
(62563,'spell_gen_mounted_charge'),
(63003,'spell_gen_mounted_charge'),
(63010,'spell_gen_mounted_charge'),
(64591,'spell_gen_mounted_charge'),
(66481,'spell_gen_mounted_charge'),
(68282,'spell_gen_mounted_charge'),
(68284,'spell_gen_mounted_charge'),
(68321,'spell_gen_mounted_charge'),
(68498,'spell_gen_mounted_charge'),
(68501,'spell_gen_mounted_charge');
DELETE FROM `spell_script_names` WHERE `spell_id` IN (62575,62626,64342,64507,64590,64595,64686,65147,66480,68504);
INSERT INTO `spell_script_names`(`spell_id`,`ScriptName`) VALUES
(62575,'spell_gen_break_shield'),
(62626,'spell_gen_break_shield'),
(64342,'spell_gen_break_shield'),
(64507,'spell_gen_break_shield'),
(64590,'spell_gen_break_shield'),
(64595,'spell_gen_break_shield'),
(64686,'spell_gen_break_shield'),
(65147,'spell_gen_break_shield'),
(66480,'spell_gen_break_shield'),
(68504,'spell_gen_break_shield');

UPDATE `npc_spellclick_spells` SET `aura_required`=0 WHERE `aura_required`=62853;
DELETE FROM `spell_script_names` WHERE `spell_id` IN (62774,62779,62780,62781,62782,62783,62784,62785,62786,62787,63663,63791,63792);
INSERT INTO `spell_script_names`(`spell_id`,`ScriptName`) VALUES
(62774,'spell_gen_summon_argent_mount'),
(62779,'spell_gen_summon_argent_mount'),
(62780,'spell_gen_summon_argent_mount'),
(62781,'spell_gen_summon_argent_mount'),
(62782,'spell_gen_summon_argent_mount'),
(62783,'spell_gen_summon_argent_mount'),
(62784,'spell_gen_summon_argent_mount'),
(62785,'spell_gen_summon_argent_mount'),
(62786,'spell_gen_summon_argent_mount'),
(62787,'spell_gen_summon_argent_mount'),
(63663,'spell_gen_summon_argent_mount'),
(63791,'spell_gen_summon_argent_mount'),
(63792,'spell_gen_summon_argent_mount');



-- remove aura_required for clickspells on tournament mounts and add more cases (Thanks to @Tassader)
DELETE FROM `npc_spellclick_spells` WHERE npc_entry IN (33842,33796,33798,33791,33792,33799,33843,33800,33795,33790,33793,33794);
INSERT INTO `npc_spellclick_spells`(`npc_entry`,`spell_id`,`quest_start`,`quest_start_active`,`quest_end`,`cast_flags`,`aura_required`,`aura_forbidden`,`user_type`) VALUES
(33842,63791,13668,0,13687,1,0,0,0),-- Aspirant
(33799,62783,13691,0,0,1,0,0,0),-- A Valiant Of Orgrimmar
(33799,62783,13707,0,0,1,0,0,0),-- Valiant Of Orgrimmar
(33796,62784,13693,0,0,1,0,0,0),-- A Valiant Of Sen'jin
(33796,62784,13708,0,0,1,0,0,0),-- Valiant Of Sen'jin
(33792,62785,13694,0,0,1,0,0,0),-- A Valiant Of Thunder Bluff
(33792,62785,13709,0,0,1,0,0,0),-- Valiant Of Thunder Bluff
(33791,62786,13696,0,0,1,0,0,0),-- A Valiant Of Silvermoon
(33791,62786,13711,0,0,1,0,0,0),-- Valiant Of Silvermoon
(33798,62787,13695,0,0,1,0,0,0),-- A Valiant Of Undercity
(33798,62787,13710,0,0,1,0,0,0), -- Valiant Of Undercity
(33843,63792,13667,0,13686,1,0,0,0),-- Aspirant
(33800,62774,13593,0,0,1,0,0,0),-- A Valiant Of Stormwind
(33800,62774,13684,0,0,1,0,0,0),-- Valiant Of Stormwind
(33795,62779,13685,0,0,1,0,0,0),-- A Valiant Of Ironforge
(33795,62779,13703,0,0,1,0,0,0),-- Valiant Of Ironforge
(33793,62780,13688,0,0,1,0,0,0),-- A Valiant Of Gnomregan
(33793,62780,13704,0,0,1,0,0,0),-- Valiant Of Gnomregan
(33790,62781,13690,0,0,1,0,0,0),-- A Valiant Of Exodar
(33790,62781,13705,0,0,1,0,0,0),-- Valiant Of Exodar
(33794,62782,13689,0,0,1,0,0,0),-- A Valiant Of Darnassus
(33794,62782,13706,0,0,1,0,0,0); -- Valiant Of Darnassus
UPDATE `npc_spellclick_spells` SET `aura_required`=0 WHERE `aura_required`=62853;

DELETE FROM `spell_script_names` WHERE `spell_id` IN (63034,62774,62779,62780,62781,62782,62783,62784,62785,62786,62787,63663,63791,63792,62595,62596,62594,63394,63395,63396,63397,63398,63401,63402,63403,63404,63405,63406,63421,63422,63423,63425,63426,63427,63428,63429,63430,63431,63432,63433,63434,63435,63436,63606,63500,63501,63607,63608,63609);
INSERT INTO `spell_script_names`(`spell_id`,`ScriptName`) VALUES
(63034,'spell_gen_on_tournament_mount'),
(62595,'spell_gen_tournament_pennant'),
(62596,'spell_gen_tournament_pennant'),
(62594,'spell_gen_tournament_pennant'),
(63394,'spell_gen_tournament_pennant'),
(63395,'spell_gen_tournament_pennant'),
(63396,'spell_gen_tournament_pennant'),
(63397,'spell_gen_tournament_pennant'),
(63398,'spell_gen_tournament_pennant'),
(63401,'spell_gen_tournament_pennant'),
(63402,'spell_gen_tournament_pennant'),
(63403,'spell_gen_tournament_pennant'),
(63404,'spell_gen_tournament_pennant'),
(63405,'spell_gen_tournament_pennant'),
(63406,'spell_gen_tournament_pennant'),
(63421,'spell_gen_tournament_pennant'),
(63422,'spell_gen_tournament_pennant'),
(63423,'spell_gen_tournament_pennant'),
(63425,'spell_gen_tournament_pennant'),
(63426,'spell_gen_tournament_pennant'),
(63427,'spell_gen_tournament_pennant'),
(63428,'spell_gen_tournament_pennant'),
(63429,'spell_gen_tournament_pennant'),
(63430,'spell_gen_tournament_pennant'),
(63431,'spell_gen_tournament_pennant'),
(63432,'spell_gen_tournament_pennant'),
(63433,'spell_gen_tournament_pennant'),
(63434,'spell_gen_tournament_pennant'),
(63435,'spell_gen_tournament_pennant'),
(63436,'spell_gen_tournament_pennant'),
(63606,'spell_gen_tournament_pennant'),
(63500,'spell_gen_tournament_pennant'),
(63501,'spell_gen_tournament_pennant'),
(63607,'spell_gen_tournament_pennant'),
(63608,'spell_gen_tournament_pennant'),
(63609,'spell_gen_tournament_pennant'),
(62774,'spell_gen_summon_tournament_mount'),
(62779,'spell_gen_summon_tournament_mount'),
(62780,'spell_gen_summon_tournament_mount'),
(62781,'spell_gen_summon_tournament_mount'),
(62782,'spell_gen_summon_tournament_mount'),
(62783,'spell_gen_summon_tournament_mount'),
(62784,'spell_gen_summon_tournament_mount'),
(62785,'spell_gen_summon_tournament_mount'),
(62786,'spell_gen_summon_tournament_mount'),
(62787,'spell_gen_summon_tournament_mount'),
(63663,'spell_gen_summon_tournament_mount'),
(63791,'spell_gen_summon_tournament_mount'),
(63792,'spell_gen_summon_tournament_mount');

DELETE FROM `spell_script_names` WHERE `spell_id`=62863;
INSERT INTO `spell_script_names`(`spell_id`,`ScriptName`) VALUES
(62863 ,'spell_gen_tournament_duel');

DELETE FROM `achievement_criteria_data` WHERE `criteria_id`=9798 AND `type`=11; 
DELETE FROM `achievement_criteria_data` WHERE `criteria_id`=9798 AND `type`=6;
INSERT INTO `achievement_criteria_data` (`criteria_id`,`type`,`value1`,`value2`,`ScriptName`) VALUES 
(9798,11,0,0, 'achievement_tilted'),
(9798,6,0,0, 'achievement_tilted');

DELETE FROM `spell_script_names` WHERE `spell_id`=63399;
INSERT INTO `spell_script_names`(`spell_id`,`ScriptName`) VALUES
(63399,'spell_gen_tournament_pennant');

-- Argent Tournament Trainers part
SET @SPELL_ON_ARGENT_MOUNT := 63034;

SET @NPC_JAERAN_LOCKWOOD := 33973;
SET @QUEST_MASTERY_OF_MELEE_A := 13828;
SET @QUEST_MASTERY_OF_MELEE_H := 13829;

SET @GOSSIP_MENU_JERAN_MOUNTED := 10398;
SET @GOSSIP_MENU_JERAN_EXPLANATION := 10399; -- GUESSED

SET @GOSSIP_TEXT_JERAN_MOUNTED := 14431;
SET @GOSSIP_TEXT_JERAN_EXPLANATION := 14434;
SET @SPELL_CREDIT_JERAN := 64113;

SET @NPC_RUGAN_STEELBELLY := 33972;
SET @QUEST_MASTERY_OF_CHARGE_A := 13837;
SET @QUEST_MASTERY_OF_CHARGE_H := 13839;

SET @GOSSIP_MENU_RUGAN_MOUNTED := 10400;
SET @GOSSIP_MENU_RUGAN_EXPLANATION := 10401; -- GUESSED

SET @GOSSIP_TEXT_RUGAN_MOUNTED := 14436;
SET @GOSSIP_TEXT_RUGAN_EXPLANATION := 14437;
SET @SPELL_CREDIT_RUGAN := 64114;

SET @NPC_VALIS_WINDCHASER := 33974;
SET @QUEST_MASTERY_OF_SH_BREAKER_A := 13835;
SET @QUEST_MASTERY_OF_SH_BREAKER_H := 13838;

SET @GOSSIP_MENU_VALIS_MOUNTED := 10402;
SET @GOSSIP_MENU_VALIS_EXPLANATION := 10403; -- GUESSED

SET @GOSSIP_TEXT_VALIS_MOUNTED := 14438;
SET @GOSSIP_TEXT_VALIS_EXPLANATION := 14439;
SET @SPELL_CREDIT_VALIS := 64115;

UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry`IN(@NPC_JAERAN_LOCKWOOD,@NPC_RUGAN_STEELBELLY,@NPC_VALIS_WINDCHASER);
DELETE FROM `smart_scripts` WHERE (`entryorguid`IN(@NPC_JAERAN_LOCKWOOD,@NPC_RUGAN_STEELBELLY) AND `source_type`=0);
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@NPC_JAERAN_LOCKWOOD,0,0,0,64,0,100,0,0,0,0,0,98,@GOSSIP_MENU_JERAN_MOUNTED,@GOSSIP_TEXT_JERAN_MOUNTED,0,0,0,0,7,0,0,0,0,0,0,0,'Jeran Lockwood - Send different gossip when mounted (Requires conditions)'),
(@NPC_JAERAN_LOCKWOOD,0,1,3,62,0,100,0,@GOSSIP_MENU_JERAN_MOUNTED,0,0,0,1,0,0,0,0,0,0,7,0,0,0,0,0,0,0,'Jeran Lockwood - Send text when option clicked'),
(@NPC_JAERAN_LOCKWOOD,0,2,3,62,0,100,0,@GOSSIP_MENU_JERAN_EXPLANATION,0,0,0,1,0,0,0,0,0,0,7,0,0,0,0,0,0,0,'Jeran Lockwood - Send text when option clicked'),
(@NPC_JAERAN_LOCKWOOD,0,3,4,61,0,100,0,0,0,0,0,11,@SPELL_CREDIT_JERAN,0,0,0,0,0,7,0,0,0,0,0,0,0,'Jeran Lockwood - Give Credit'),
(@NPC_JAERAN_LOCKWOOD,0,4,0,61,0,100,0,0,0,0,0,72,0,0,0,0,0,0,7,0,0,0,0,0,0,0,'Jeran Lockwood - Close Gossip'),

(@NPC_RUGAN_STEELBELLY,0,0,0,64,0,100,0,0,0,0,0,98,@GOSSIP_MENU_RUGAN_MOUNTED,@GOSSIP_TEXT_RUGAN_MOUNTED,0,0,0,0,7,0,0,0,0,0,0,0,'Rugan Steelbelly - Send different gossip when mounted (Requires conditions)'),
(@NPC_RUGAN_STEELBELLY,0,1,3,62,0,100,0,@GOSSIP_MENU_RUGAN_MOUNTED,0,0,0,1,0,0,0,0,0,0,7,0,0,0,0,0,0,0,'Rugan Steelbelly - Send text when option clicked'),
(@NPC_RUGAN_STEELBELLY,0,2,3,62,0,100,0,@GOSSIP_MENU_RUGAN_EXPLANATION,0,0,0,1,0,0,0,0,0,0,7,0,0,0,0,0,0,0,'Rugan Steelbelly - Send text when option clicked'),
(@NPC_RUGAN_STEELBELLY,0,3,4,61,0,100,0,0,0,0,0,11,@SPELL_CREDIT_RUGAN,0,0,0,0,0,7,0,0,0,0,0,0,0,'Rugan Steelbelly - Give Credit'),
(@NPC_RUGAN_STEELBELLY,0,4,0,61,0,100,0,0,0,0,0,72,0,0,0,0,0,0,7,0,0,0,0,0,0,0,'Rugan Steelbelly - Close Gossip'),

(@NPC_VALIS_WINDCHASER,0,0,0,64,0,100,0,0,0,0,0,98,@GOSSIP_MENU_VALIS_MOUNTED,@GOSSIP_TEXT_VALIS_MOUNTED,0,0,0,0,7,0,0,0,0,0,0,0,'Valis Windchaser - Send different gossip when mounted (Requires conditions)'),
(@NPC_VALIS_WINDCHASER,0,1,3,62,0,100,0,@GOSSIP_MENU_VALIS_MOUNTED,0,0,0,1,0,0,0,0,0,0,7,0,0,0,0,0,0,0,'Valis Windchaser - Send text when option clicked'),
(@NPC_VALIS_WINDCHASER,0,2,3,62,0,100,0,@GOSSIP_TEXT_VALIS_EXPLANATION,0,0,0,1,0,0,0,0,0,0,7,0,0,0,0,0,0,0,'Valis Windchaser - Send text when option clicked'),
(@NPC_VALIS_WINDCHASER,0,3,4,61,0,100,0,0,0,0,0,11,@SPELL_CREDIT_VALIS,0,0,0,0,0,7,0,0,0,0,0,0,0,'Valis Windchaser - Give Credit'),
(@NPC_VALIS_WINDCHASER,0,4,0,61,0,100,0,0,0,0,0,72,0,0,0,0,0,0,7,0,0,0,0,0,0,0,'Valis Windchaser - Close Gossip');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=22 AND `SourceEntry`IN(@NPC_JAERAN_LOCKWOOD,@NPC_RUGAN_STEELBELLY,@NPC_VALIS_WINDCHASER);
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=15 AND `SourceGroup`IN(@GOSSIP_MENU_JERAN_MOUNTED,@GOSSIP_MENU_RUGAN_MOUNTED,@GOSSIP_MENU_VALIS_MOUNTED);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(22,1,@NPC_JAERAN_LOCKWOOD,0,0,1,@SPELL_ON_ARGENT_MOUNT,0,0,0,'','SAI - Jeran Lockwood show different menu if player mounted'),
(15,@GOSSIP_MENU_JERAN_MOUNTED,0,0,0,9,@QUEST_MASTERY_OF_MELEE_A,0,0,0,'',"Jeran Lockwood - Show gossip if player has quest"),
(15,@GOSSIP_MENU_JERAN_MOUNTED,0,0,0,1,@SPELL_ON_ARGENT_MOUNT,0,0,0,'',"Jeran Lockwood - Show gossip if player has aura"),
(15,@GOSSIP_MENU_JERAN_MOUNTED,0,0,1,9,@QUEST_MASTERY_OF_MELEE_H,0,0,0,'',"Jeran Lockwood - Show gossip if player has quest"),
(15,@GOSSIP_MENU_JERAN_MOUNTED,0,0,1,1,@SPELL_ON_ARGENT_MOUNT,0,0,0,'',"Jeran Lockwood - Show gossip if player has aura"),
(15,@GOSSIP_MENU_JERAN_MOUNTED,1,0,0,9,@QUEST_MASTERY_OF_MELEE_A,0,0,0,'',"Jeran Lockwood - Show gossip if player has quest"),
(15,@GOSSIP_MENU_JERAN_MOUNTED,1,0,0,1,@SPELL_ON_ARGENT_MOUNT,0,0,0,'',"Jeran Lockwood - Show gossip if player has aura"),
(15,@GOSSIP_MENU_JERAN_MOUNTED,1,0,1,9,@QUEST_MASTERY_OF_MELEE_H,0,0,0,'',"Jeran Lockwood - Show gossip if player has quest"),
(15,@GOSSIP_MENU_JERAN_MOUNTED,1,0,1,1,@SPELL_ON_ARGENT_MOUNT,0,0,0,'',"Jeran Lockwood - Show gossip if player has aura"),

(22,1,@NPC_RUGAN_STEELBELLY,0,0,1,@SPELL_ON_ARGENT_MOUNT,0,0,0,'','SAI - Rugan Steelbelly show different menu if player mounted'),
(15,@GOSSIP_MENU_RUGAN_MOUNTED,0,0,0,9,@QUEST_MASTERY_OF_CHARGE_A,0,0,0,'',"Rugan Steelbelly - Show gossip if player has quest"),
(15,@GOSSIP_MENU_RUGAN_MOUNTED,0,0,0,1,@SPELL_ON_ARGENT_MOUNT,0,0,0,'',"Rugan Steelbelly - Show gossip if player has aura"),
(15,@GOSSIP_MENU_RUGAN_MOUNTED,0,0,1,9,@QUEST_MASTERY_OF_CHARGE_H,0,0,0,'',"Rugan Steelbelly - Show gossip if player has quest"),
(15,@GOSSIP_MENU_RUGAN_MOUNTED,0,0,1,1,@SPELL_ON_ARGENT_MOUNT,0,0,0,'',"Rugan Steelbelly - Show gossip if player has aura"),
(15,@GOSSIP_MENU_RUGAN_MOUNTED,1,0,0,9,@QUEST_MASTERY_OF_CHARGE_A,0,0,0,'',"Rugan Steelbelly - Show gossip if player has quest"),
(15,@GOSSIP_MENU_RUGAN_MOUNTED,1,0,0,1,@SPELL_ON_ARGENT_MOUNT,0,0,0,'',"Rugan Steelbelly - Show gossip if player has aura"),
(15,@GOSSIP_MENU_RUGAN_MOUNTED,1,0,1,9,@QUEST_MASTERY_OF_CHARGE_H,0,0,0,'',"Rugan Steelbelly - Show gossip if player has quest"),
(15,@GOSSIP_MENU_RUGAN_MOUNTED,1,0,1,1,@SPELL_ON_ARGENT_MOUNT,0,0,0,'',"Rugan Steelbelly - Show gossip if player has aura"),

(22,1,@NPC_VALIS_WINDCHASER,0,0,1,@SPELL_ON_ARGENT_MOUNT,0,0,0,'','SAI - Valis Windchaser show different menu if player mounted'),
(15,@GOSSIP_MENU_VALIS_MOUNTED,0,0,0,9,@QUEST_MASTERY_OF_SH_BREAKER_A,0,0,0,'',"Valis Windchaser - Show gossip if player has quest"),
(15,@GOSSIP_MENU_VALIS_MOUNTED,0,0,0,1,@SPELL_ON_ARGENT_MOUNT,0,0,0,'',"Valis Windchaser - Show gossip if player has aura"),
(15,@GOSSIP_MENU_VALIS_MOUNTED,0,0,1,9,@QUEST_MASTERY_OF_SH_BREAKER_H,0,0,0,'',"Valis Windchaser - Show gossip if player has quest"),
(15,@GOSSIP_MENU_VALIS_MOUNTED,0,0,1,1,@SPELL_ON_ARGENT_MOUNT,0,0,0,'',"Valis Windchaser - Show gossip if player has aura"),
(15,@GOSSIP_MENU_VALIS_MOUNTED,1,0,0,9,@QUEST_MASTERY_OF_SH_BREAKER_A,0,0,0,'',"Valis Windchaser - Show gossip if player has quest"),
(15,@GOSSIP_MENU_VALIS_MOUNTED,1,0,0,1,@SPELL_ON_ARGENT_MOUNT,0,0,0,'',"Valis Windchaser - Show gossip if player has aura"),
(15,@GOSSIP_MENU_VALIS_MOUNTED,1,0,1,9,@QUEST_MASTERY_OF_SH_BREAKER_H,0,0,0,'',"Valis Windchaser - Show gossip if player has quest"),
(15,@GOSSIP_MENU_VALIS_MOUNTED,1,0,1,1,@SPELL_ON_ARGENT_MOUNT,0,0,0,'',"Valis Windchaser - Show gossip if player has aura");

DELETE FROM `gossip_menu` WHERE `entry` IN (@GOSSIP_MENU_JERAN_EXPLANATION,@GOSSIP_MENU_RUGAN_EXPLANATION,@GOSSIP_MENU_VALIS_EXPLANATION);
INSERT INTO `gossip_menu` (`entry`,`text_id`) VALUES
(@GOSSIP_MENU_JERAN_EXPLANATION,@GOSSIP_TEXT_JERAN_EXPLANATION),
(@GOSSIP_MENU_RUGAN_EXPLANATION,@GOSSIP_TEXT_RUGAN_EXPLANATION),
(@GOSSIP_MENU_VALIS_EXPLANATION,@GOSSIP_TEXT_VALIS_EXPLANATION);

DELETE FROM `gossip_menu_option` WHERE `menu_id` IN (@GOSSIP_MENU_JERAN_MOUNTED,@GOSSIP_MENU_JERAN_EXPLANATION,@GOSSIP_MENU_RUGAN_MOUNTED,@GOSSIP_MENU_RUGAN_EXPLANATION,@GOSSIP_MENU_VALIS_MOUNTED,@GOSSIP_MENU_VALIS_EXPLANATION);
INSERT INTO `gossip_menu_option` (`menu_id`,`id`,`option_icon`,`option_text`,`option_id`,`npc_option_npcflag`,`action_menu_id`,`action_poi_id`,`box_coded`,`box_money`,`box_text`) VALUES
(@GOSSIP_MENU_JERAN_MOUNTED,0,0,'Show me how to train with a Melee Target.',1,1,0,0,0,0,''),
(@GOSSIP_MENU_JERAN_MOUNTED,1,0,'Tell me more about Defend and Thrust!',1,1,@GOSSIP_MENU_JERAN_EXPLANATION,0,0,0,''),
(@GOSSIP_MENU_JERAN_EXPLANATION,0,0,'Show me how to train with a Melee Target.',1,1,0,0,0,0,''),

(@GOSSIP_MENU_RUGAN_MOUNTED,0,0,'Show me how to train with a Charge Target.',1,1,0,0,0,0,''),
(@GOSSIP_MENU_RUGAN_MOUNTED,1,0,'Tell me more about the Charge!',1,1,@GOSSIP_MENU_RUGAN_EXPLANATION,0,0,0,''),
(@GOSSIP_MENU_RUGAN_EXPLANATION,0,0,'Show me how to train with a Charge Target.',1,1,0,0,0,0,''),

(@GOSSIP_MENU_VALIS_MOUNTED,0,0,'Show me how to train with a Ranged Target.',1,1,0,0,0,0,''),
(@GOSSIP_MENU_VALIS_MOUNTED,1,0,'Tell me more about the Shield-Breaker!',1,1,@GOSSIP_MENU_VALIS_EXPLANATION,0,0,0,''),
(@GOSSIP_MENU_VALIS_EXPLANATION,0,0,'Show me how to train with a Ranged Target.',1,1,0,0,0,0,'');

DELETE FROM `creature_text` WHERE `entry` IN (@NPC_JAERAN_LOCKWOOD,@NPC_RUGAN_STEELBELLY,@NPC_VALIS_WINDCHASER);
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(@NPC_JAERAN_LOCKWOOD,0,0,'Put up defend$B|TInterface\\Icons\\ability_warrior_shieldmastery.blp:32|t$BThen use Thrust on a Melee Target$B|TInterface\\Icons\\inv_sword_65.blp:32|t',42,0,0,0,0,0,'Argent Tournament - Melee Tutorial'),
(@NPC_RUGAN_STEELBELLY,0,0,'Use Shield-Breaker on a Charge Target$B|TInterface\\Icons\\ability_warrior_shieldbreak.blp:32|t$BFollow up with Charge while the target is vulnerable$B|TInterface\\Icons\\ability_mount_charger.blp:32|t',42,0,0,0,0,0,'Argent Tournament - Charge Tutorial'),
(@NPC_VALIS_WINDCHASER,0,0,'Use Shield-Breaker on a Ranged Target$B|TInterface\\Icons\\ability_warrior_shieldbreak.blp:32|t$BThen use Shield-Breaker while the target is defenseless$B|TInterface\\Icons\\ability_warrior_shieldbreak.blp:32|t',42,0,0,0,0,0,'Argent Tournament - Ranged Tutorial');

-- Training Dummies Part
UPDATE `creature_template` SET `ScriptName` = 'npc_tournament_training_dummy' WHERE `entry` IN (33272,33229,33243);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceEntry`=62709;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(13,0,62709,0,0,18,1,33845,0,0, '','Counterattack! (Argent Tournament) - Target near aspirant mounts'),
(13,0,62709,0,0,18,1,33323,0,0, '','Counterattack! (Argent Tournament) - Target near aspirant mounts');

DELETE FROM `spell_script_names` WHERE `spell_id`=62709;
INSERT INTO `spell_script_names` VALUES (62709, 'spell_gen_tournament_counterattack');

