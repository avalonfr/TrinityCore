DELETE FROM `spell_script_names` WHERE `spell_id` IN (62680,63472);
INSERT INTO `spell_script_names` (`spell_id`,`ScriptName`) VALUES
(62680, 'spell_ignis_flame_jets'),
(63472, 'spell_ignis_flame_jets');

DELETE FROM `spell_script_names` WHERE `spell_id` IN (62717,63477);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(62717, 'spell_ignis_slag_pot'),
(63477, 'spell_ignis_slag_pot');

UPDATE `creature_template` SET `ScriptName`='boss_ignis' WHERE `entry`=33118;

UPDATE `creature_template` SET `ScriptName`='npc_iron_construct' WHERE `entry`=33121;

UPDATE `creature_template` SET `ScriptName`='npc_scorch_ground' WHERE `entry`=33221;
