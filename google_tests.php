<?php
include 'appsapis.inc';

/*THESE FIRST FEW TESTS TEST INTERRACTING WITH GOOGLE DOCUMENTS
*THE file appsapis.inc has the functions being called here and the exceptions should be made to write to a log file or database to log the exceptions
*USAGE:  TO use this test file just uncomment the lines you would like to test and from the command line type php google_tests.php
*/


//add Permission() below worked on 10/5/11
//addPermission();

//createCollection() test below worked on 10/5/11
//$result = createCollection('happy_folder3');
//echo $result;

//addToCollection test below worked on 10/5/11
//$result = addToCollection('0B-a-StXYm5kcNDEwZGQ2ZWItZGY2YS00YTk0LWIxYTUtMGJhZGRjNjU1NDA3','14arxRyvD7-fWyMNsLr7-nWUYL66GFZqib_4ba83b-zs');
//echo $result;

//getACollectionId() to get a list of user's collections
//Test worked on 10/26/2011, returns a google collection id (string)
//$result = getACollectionId('yourgoogleuser@domain.edu','yourpassword','2011Fall-EPS7517-B71');
//echo $result;

printCollection2('yourgoogleuser@domain.edu','yourpassword');

/*USER TESTS ARE BELOW*/


//http://framework.zend.com/manual/en/zend.gdata.gapps.html for info on interracting with users
//worked on 10/18/2011
//retuns an associative array of basic user information if successful or 
//"Caught exception: ERROR: The user object has no data" if the user object does not get set successfully
//this is the best function to run to just test connectivity to google
//$result = retrieveUser('testusername');
//print_r($result);  //to view what's in the array

//worked on 10/18/2011
//result is a boolean and returns true if it works, returns an exception message if false
//$result = changePassword('jbttest','jbtest123456');
//if($result){echo 'password was changed';}else{echo 'there was an error '.$result;}


//worked on 10/18/2011
//result is a boolean and returns true if successful otherwise an exception message is thrown
//$result = createUser('jbtest', 'testfirstname', 'testlastname', 'jbtest212345');
//if($result){echo 'User was created successfully';}else{echo 'there was an error '.$result;}

//worked on 10/18/2011
//result is a boolean and returns true if it works, and returns an exception message if false
//$result = deleteUser('jbtest');
//if($result){echo 'User was deleted successfully';}else{echo 'there was an error '.$result;}




?>

