SET @ENTRY := 15467; -- Omen
SET @GUID := XXX; -- Need 1
SET @EVENT := 7;

DELETE FROM `creature` WHERE `id`=@ENTRY;
INSERT INTO `creature` (`guid`,`id`,`map`,`spawnMask`,`phaseMask`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`MovementType`,`npcflag`,`unit_flags`,`dynamicflags`) VALUES
(@GUID,@ENTRY,1,1,1,0,0,7544.55,-2803.19,448.74,4.0,300,0,0,5578000,0,0,0,0,0);

DELETE FROM `game_event_creature` WHERE `eventEntry` = @EVENT AND `guid` = @GUID;
INSERT INTO `game_event_creature` (`eventEntry`,`guid`) VALUES
(@EVENT,@GUID);




UPDATE `creature_template` SET `spell1` = 26393 WHERE `entry` =15902;

DELETE FROM `spell_script_names` WHERE `spell_id` = 26393;
INSERT INTO `spell_script_names` (`spell_id`,`ScriptName`) VALUES
(26393,'spell_gen_elunes_blessing');


DELETE FROM `spell_script_names` WHERE `spell_id` IN (26374);
INSERT INTO `spell_script_names`(`spell_id`,`ScriptName`) VALUES
(26374,'spell_gen_elune_candle');

-- Set Gigant Spotlight as invisible trigger
UPDATE `creature_template` SET `unit_flags`=33554432 WHERE `entry`=15902;

DELETE FROM `creature_template_addon` WHERE (`entry`IN(15902,15466));
INSERT INTO `creature_template_addon`(`entry`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`auras`) VALUES
(15466,0,0,0,0,0,17327), -- Add spirit particles to Omen minions
(15902,0,0,0,0,0,50236); -- Add Spotlight aura to Gigant Spotlight (ummoned by 26392 on Omen's death)

UPDATE `creature_template` SET `ScriptName` = 'npc_gigant_spotlight' WHERE `entry` = 15902;
UPDATE `creature_template` SET `ScriptName` = 'npc_omen' WHERE `entry` = 15467;
