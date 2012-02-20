-- npc_lake_frog
UPDATE `creature_template` SET `ScriptName` = 'npc_lake_frog' WHERE `entry` IN (33211,33224);
SET @GOSSIP := 33220;
SET @MENUID := 0;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 33220;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(33220,0,0,1,62,0,100,0,@GOSSIP,@MENUID,0,0,72,0,0,0,0,0,0,7,0,0,0,0,0,0,0, 'Maiden of Ashwood Lake - On gossip option 0 select - Close gossip'),
(33220,0,1,0,61,0,100,0,0,0,0,0,85,62554,0,0,0,0,0,7,0,0,0,0,0,0,0, 'Maiden of Ashwood Lake - On gossip option 0 select - Player cast Summon Ashwood Brand on self');
UPDATE `creature_template` SET `gossip_menu_id` = @GOSSIP, `AIName`= 'SmartAI',`ScriptName`= '' WHERE `entry` = 33220;
REPLACE INTO `gossip_menu_option` (`menu_id`, `id`, `option_text`, `option_id`, `npc_option_npcflag`) VALUES (@GOSSIP, @MENUID, 'Do you know, where I can find Ashwood Brand Sword?', '1', '1');