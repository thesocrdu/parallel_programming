<?php
	// Allow input through the pipe
	$stdin = fopen("php://stdin", "r");
	$pipeIn = stream_get_contents($stdin);
	fclose($stdin);
	/////////////////////////////////////

	//One element is an entire line
	$friendMatrix = explode("\n", $pipeIn);

	$printFlag = FALSE;
	$prevKey = -1; //NOTE: There should never be a friend with ID < 1
	$prevFriend1 = -1;
	$prevFriend2 = -1;

	foreach ($friendMatrix as $rootFriendIndex => $friends) {
		//Check if $friends is empty, implies end of input
		if (empty($friends)) {
			continue;
		}
		$triPair = explode(" ", $friends);

		if (($triPair[0] == $prevKey) &&
			($triPair[1] == $prevFriend1) &&
			($triPair[2] == $prevFriend2)) {
			if ($printFlag === FALSE) { 
				print "<" . implode(", ", $triPair) . ">" . "\n";
				$printFlag = TRUE;
			}
		} else {
			$printFlag = FALSE;
			$prevKey = $triPair[0];
			$prevFriend1 = $triPair[1];
			$prevFriend2 = $triPair[2];
		}
	}
?>