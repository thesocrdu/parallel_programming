#!/usr/bin/php
<?php
	error_reporting(0);
	// Allow input through the pipe
	// $stdin = fopen("php://stdin", "r");
	// $pipeIn = stream_get_contents($stdin);
	// fclose($stdin);
	/////////////////////////////////////
	
	//One element is an entire line
	//$friendMatrix = explode("\n", $pipeIn);

	$line = "";
	while ($line = trim(fgets(STDIN))) {
		$friends = explode(" ", $line);
		$listSize = sizeof($friends);
		$rootFriend = $friends[0];

		for($i = 1; $i < $listSize; $i++) {
			for($j = $i+1; $j < $listSize; $j++) {
				if($rootFriend < $friends[$j]) {
					echo $friends[$i] . " " . $rootFriend . " " . $friends[$j] . "\n";
				} else {
					echo $friends[$i] . " " . $friends[$j] . " " . $rootFriend . "\n";
				}

				if ( $rootFriend < $friends[$i] ) {
					echo $friends[$j] . " " . $rootFriend . " " . $friends[$i] . "\n";
				} else {
					echo $friends[$j] . " " . $friends[$i] . " " . $rootFriend . "\n";
				}
			}
		}
	}
?>