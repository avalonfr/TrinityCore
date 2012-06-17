DELETE FROM `spell_bonus_data` WHERE `entry`=63544;
INSERT INTO `spell_bonus_data` (`entry`,`direct_bonus`,`dot_bonus`,`ap_bonus`,`ap_dot_bonus`,`comments`) VALUES
(63544,0,0,0,0,'Priest - Empowered Renew');

DELETE FROM `spell_script_names` WHERE `spell_id`=-139;
INSERT INTO `spell_script_names` (`spell_id`,`ScriptName`) VALUES
(-139,'spell_priest_renew');