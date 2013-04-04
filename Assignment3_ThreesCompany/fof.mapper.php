<?php
	// Allow input through the pipe
	$stdin = fopen("php://stdin", "r");
	$pipeIn = stream_get_contents($stdin);
	fclose($stdin);
	/////////////////////////////////////
	
	//One element is an entire line
	$friendMatrix = explode("\n", $pipeIn);

	foreach ($friendMatrix as $rootFriendIndex => $friends) {
		$friends = explode(" ", $friends);
		$listSize = sizeof($friends);
		$rootFriend = $friends[0];

		for($i = 1; $i < $listSize; $i++) {
			for($j = $i+1; $j < $listSize; $j++) {
				if($rootFriend < $friends[$j]) {
					print $friends[$i] . " " . $rootFriend . " " . $friends[$j] . "\n";
				} else {
					print $friends[$i] . " " . $friends[$j] . " " . $rootFriend . "\n";
				}

				if ( $rootFriend < $friends[$i] ) {
					print $friends[$j] . " " . $rootFriend . " " . $friends[$i] . "\n";
				} else {
					print $friends[$j] . " " . $friends[$i] . " " . $rootFriend . "\n";
				}
			}
		}
	}
?>