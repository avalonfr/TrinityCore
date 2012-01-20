SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for quest_bug_list
-- ----------------------------
DROP TABLE IF EXISTS `quest_bug_list`;
CREATE TABLE `quest_bug_list` (
 `quest_id` int(20) unsigned NOT NULL default '0',
 PRIMARY KEY (`quest_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

delete from `trinity_string` where `entry` IN ('13002','13003','13004','13005','13006','13007');
INSERT INTO `trinity_string` (`entry`,`content_default`) VALUES ('13002','Vous ne suivez pas cette qu�te !');
INSERT INTO `trinity_string` (`entry`,`content_default`) VALUES ('13003','Cette qu�te n\'est actuellement pas signal�e bug�e, Utilisez le bug tracker ou cr�er un ticket pour nous la signaler.');
INSERT INTO `trinity_string` (`entry`,`content_default`) VALUES ('13004','La qu�te %u est d�ja dans la liste des qu�tes bugu�es');
INSERT INTO `trinity_string` (`entry`,`content_default`) VALUES ('13005','Qu�te %u ajout�e la liste des qu�tes bug�es');
INSERT INTO `trinity_string` (`entry`,`content_default`) VALUES ('13006','La qu�te %u n\'est pas signal�e bug�e');
INSERT INTO `trinity_string` (`entry`,`content_default`) VALUES ('13007','La qu�te %u est retir�e de la liste des qu�tes bug�es');

INSERT INTO `command` (`name`,`help`) VALUES ('valide','Syntax .valide $queteID , vous pouvez aussi link votre qu�te depuis votre journal de qu�te.');
INSERT INTO `command` (`name`,`security`,`help`) VALUES ('questbug add','2','Syntax .questbug add $queteID , Ajoute la quete � la liste des qu�tes bug�es');
INSERT INTO `command` (`name`,`security`,`help`) VALUES ('questbug remove','2','Syntax .questbug remove $queteID , retire la qu�te de la liste des qu�tes bug�es');
