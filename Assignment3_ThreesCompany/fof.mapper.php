#!/usr/bin/php
<?php
	error_reporting(0);

	$line = "";
	while ($line = trim(fgets(STDIN))) {
		$friends = explode(" ", $line);
		$listSize = sizeof($friends);
		$rootFriend = $friends[0];

		//produce all potential mutal friends from this list
		for($i = 1; $i < $listSize; $i++) {
			for($j = $i+1; $j < $listSize; $j++) {
				echo $friends[$i] . " ";
				if($rootFriend < $friends[$j]) {
					echo $rootFriend . " " . $friends[$j] . "\n";
				} else {
					echo $friends[$j] . " " . $rootFriend . "\n";
				}

				echo $friends[$j] . " ";
				if ( $rootFriend < $friends[$i] ) {
					echo $rootFriend . " " . $friends[$i] . "\n";
				} else {
					echo $friends[$i] . " " . $rootFriend . "\n";
				}
			}
		}
	}
?>