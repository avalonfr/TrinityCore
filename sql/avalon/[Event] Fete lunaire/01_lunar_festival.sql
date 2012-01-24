SET @ENTRY := 15467; -- Omen
SET @GUID := 5261515; -- Need 1
SET @EVENT := 7;

DELETE FROM `creature` WHERE `id`=@ENTRY;
INSERT INTO `creature` (`guid`,`id`,`map`,`spawnMask`,`phaseMask`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`MovementType`,`npcflag`,`unit_flags`,`dynamicflags`) VALUES
(@GUID,@ENTRY,1,1,1,0,0,7544.55,-2803.19,448.74,4.0,300,0,0,5578000,0,0,0,0,0);

DELETE FROM `game_event_creature` WHERE `eventEntry` = @EVENT AND `guid` = @GUID;
INSERT INTO `game_event_creature` (`eventEntry`,`guid`) VALUES
(@EVENT,@GUID);

DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
UPDATE creature_template SET AIName="SmartAI" WHERE entry=@ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES 
(@ENTRY,0,0,0,0,0,100,0,6000,8000,6000,8000,11,40504,2,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,"Omen - Cast Cleave"),
(@ENTRY,0,1,0,0,0,100,0,12500,15000,20000,22500,11,37124,2,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,"Omen - Cast Starfall"),
(@ENTRY,0,2,0,6,0,100,0,0,0,0,0,11,26392,0,0,0,0,0,0,0,0,0,0.0,0.0,0.0,0.0,"Omen - Death - Cast Omen's Moonlight");

SET @ENTRY := 15902;
SET @SPELL := 26393;

UPDATE `creature_template` SET `spell1` = @SPELL WHERE `entry` =@ENTRY;

DELETE FROM `creature_template_addon` WHERE `entry` = @ENTRY;
INSERT INTO `creature_template_addon` (`entry`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES
(@ENTRY,0,0,0,0,0,'50236');

DELETE FROM `spell_script_names` WHERE `spell_id` = @SPELL;
INSERT INTO `spell_script_names` (`spell_id`,`ScriptName`) VALUES
(@SPELL,'spell_gen_elunes_blessing');
