-- Fix Frostbrood Vanquisher flying - ID: 28670
UPDATE `creature_template` SET `vehicleid`=264,`InhabitType`=7 WHERE `entry`=28670;
DELETE FROM `creature_template_addon` WHERE `entry`=28670;
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`)
VALUES (28670, 0, 0, 50331648, 1, 0, NULL);