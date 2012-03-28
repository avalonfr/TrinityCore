UPDATE `creature_template` SET `npcflag` = 1, `ScriptName` = 'npc_arena_watcher' WHERE `entry` = 32743; (not blizz)

SET @NPC = 32743;

DELETE FROM `creature_text` WHERE `entry` = @NPC;
INSERT INTO `creature_text` (`entry`, `groupid`, `text`, `type`, `comment`) VALUES
(@NPC, '0', 'Sorry matches at the moment there is no', '15', 'ArenaWatcher'),
(@NPC, '1', 'The match has been completed', '15', 'ArenaWatcher'),
(@NPC, '2', 'Target is offline', '15', 'ArenaWatcher'),
(@NPC, '3', 'Target not in arena', '15', 'ArenaWatcher'),
(@NPC, '4', 'Target is GameMaster', '15', 'ArenaWatcher');

DELETE FROM `locales_creature_text` WHERE `entry` = @NPC;
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc2`) VALUES
(@NPC, '0', '0', 'Aucuns Matchs actuellement en cours'),
(@NPC, '1', '0', 'Le Match est terminé'),
(@NPC, '2', '0', 'le Joueur est déconnecté'),
(@NPC, '3', '0', 'Le Joueur n\'est pas en Arène'),
(@NPC, '4', '0', 'Le Joueur est un Mj, OWNED');

DELETE FROM `trinity_string` WHERE `entry` BETWEEN 11200 AND 11203;
INSERT INTO `trinity_string` (`entry`, `content_default`, `content_loc2`) VALUES
('11200', 'Watch the match 2x2 (now games: %u)', 'Observez un match 2v2 (match(s) en cours: %u)'),
('11201', 'Watch the match 3x3 (now games: %u)', 'Observez un match 3v3 (match(s) en cours: %u)'),
('11202', 'Watch the match 5x5 (now games: %u)', 'Observez un match 5v5 (match(s) en cours: %u)'),
('11203', 'Follow player', 'Suivre un Joueur');