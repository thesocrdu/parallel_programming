THREES COMPANY - README

Author: Kyle Mercer
Date: 04/09/13

Running on AmazonS3:

Streaming parameters:
Hadoop Version: Hadoop 0.20.205 M5 Edition
Type: Streaming
Input Location: s3n://friends.simple/
Output Loaction: s3n://outstuff/myout
Mapper: s3n://myfiles/fof.mapper.php
Reducer: s3n://myfiles/fof.reducer.php
*Everything else should remain default settings

Custom JAR parameters:
Hadoop Version: Hadoop 0.20.205 M5 Edition
Jar Location: s3n://myfiles/fof.jar
Jar Arguments: FoF s3n://friends.simple/ s3n://outstuff/myout2
*Everything else should remain default settings