-- Fix combat range of creature 28860 Sartharion
UPDATE `creature_model_info` SET `bounding_radius` = 8.0, `combat_reach` = 8.0 WHERE `modelid` = 27035;

DELETE FROM `achievement_criteria_data` WHERE `criteria_id` IN (7326, 7327);
INSERT INTO `achievement_criteria_data` (`criteria_id`, `type`, `value1`, `value2`, `ScriptName`) VALUES
(7326, 12, 0, 0, ''),
(7326, 11, 0, 0, 'achievement_volcano_blows'),
(7327, 12, 1, 0, ''),
(7327, 11, 0, 0, 'achievement_volcano_blows');

DELETE FROM `disables` WHERE `SourceType` = 4 AND `entry` IN (7326, 7327);


UPDATE `gameobject_template` SET `data10` = 0, `ScriptName` = 'go_portal_sartharion' WHERE `entry` = 193988;

DELETE FROM `spell_script_target` WHERE `entry` IN (61254, 60430);
INSERT INTO `spell_script_target` (entry, type, targetEntry) VALUES
(61254, 1, 30449),
(61254, 1, 30451),
(61254, 1, 30452),
(60430, 1, 30643);