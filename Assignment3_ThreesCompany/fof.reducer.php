#!/usr/bin/php
<?php
	error_reporting(0);

	//printFlag for avoiding duplicates
	$printFlag = FALSE;
	$line = "";
	$prevLine = " ";
	while ($line = trim(fgets(STDIN))) {

		//if keys match we have mutual friends
		if($line == $prevLine) {
			if ($printFlag === FALSE) { 
				print $line . "\n";
				$printFlag = TRUE;
			}
		} else {
			$printFlag = FALSE; //reset printFlag
			$prevLine = $line;
		}
	}
?>