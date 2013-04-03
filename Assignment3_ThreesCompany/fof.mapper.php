<?php
	// Allow input through the pipe
	$stdin = fopen("php://stdin", "r");
	$pipeIn = stream_get_contents($stdin);
	print $pipeIn;

	fclose($stdin);

?>