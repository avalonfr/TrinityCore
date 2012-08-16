/* disable  ICC Buff */
UPDATE spell_area SET autocast = 0 WHERE AREA = 4812 AND (spell = 73822 OR spell = 73828);