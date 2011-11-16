<?php
/**
 * @file
 * wrappers for zend functions and direct interraction with google apis
 */
$inc_path = get_include_path();
if (!in_array(dirname(__FILE__), split(PATH_SEPARATOR, $inc_path))) {
 set_include_path(dirname(__FILE__) . PATH_SEPARATOR . $inc_path);
}

/**
 * @see Zend_Loader
 */
require_once 'Zend/Loader.php';
//include(libraries_get_path($name) . '/library/Zend/Loader.php');



/**
 * @see Zend_Gdata
 */
Zend_Loader::loadClass('Zend_Gdata', dirname(__FILE__));

/**
 * @see Zend_Gdata_AuthSub
 */
Zend_Loader::loadClass('Zend_Gdata_AuthSub', dirname(__FILE__));

/**
 * @see Zend_Gdata_ClientLogin
 */
Zend_Loader::loadClass('Zend_Gdata_ClientLogin', dirname(__FILE__));

/**
 * @see Zend_Gdata_Docs
 */
Zend_Loader::loadClass('Zend_Gdata_Docs', dirname(__FILE__));
/**
 * Restore include_path
 */


Zend_Loader::loadClass('Zend_Http_Client', dirname(__FILE__));
Zend_Loader::loadClass('Zend_Gdata_Gapps', dirname(__FILE__));

/**
 * Returns a HTTP client object with the appropriate headers for communicating
 * with Google using the ClientLogin credentials supplied.
 *
 * @param  string $user The username, in e-mail address format, to authenticate
 * @param  string $pass The password for the user specified
 * @return Zend_Http_Client
 */


function getClientLoginHttpClient($user, $pass) {
 $service = Zend_Gdata_Docs::AUTH_SERVICE_NAME;
 $client = Zend_Gdata_ClientLogin::getHttpClient($user, $pass, $service);
 return $client;
}


function printDocumentsFeed($feed, $html) {
 //print_r($feed);
 try {
  $content = "";
  if ($html) {
   $content = "<ul>\n";
  }

  // Iterate over the document entries in the feed and display each document's
  // title.
  foreach ($feed->entries as $entry) {

   if ($html) {
    // Find the URL of the HTML view of the document.
    $alternateLink = '';
    foreach ($entry->link as $link) {
     if ($link->getRel() === 'alternate') {
      $alternateLink = $link->getHref();
     }
    }
    // Make the title link to the document on docs.google.com.
    $content .= "<li><a href=\"$alternateLink\">\n";
   }

   $content .= "$entry->title\n";

   if ($html) {
    $content .= "</a></li>\n";}
  }
 } catch (Exception $e) {
  drupal_set_message("Error: Unable to process.\n" . check_plain($e->getMessage()));
  //echo phpinfo();

 }
 return $content;
}


function uploadDocument($user, $password, $docs, $html, $originalFileName, $temporaryFileLocation) {
 $content = 'got to uploadDocumemt';
 try{
  $fileToUpload = $originalFileName;
  if ($temporaryFileLocation) {
   $fileToUpload = $temporaryFileLocation;
  }

  // Upload the file and convert it into a Google Document. The original
  // file name is used as the title of the document and the MIME type
  // is determined based on the extension on the original file name.
  $filenameParts = explode('.', $temporaryFileLocation);
  $fileExtension = end($filenameParts);

  //drupal_set_message($fileExtension);
  $mimeType = Zend_Gdata_Docs::lookupMimeType($fileExtension);
  //$mimeType = Zend_Gdata_Docs::lookupMimeType($fileExtension);
  //$mimeType = Zend_Gdata_Docs::lookupMimeType($fileExtension);
  //drupal_set_message('mimetype is '.$mimeType);
  $newDocumentEntry = $docs->uploadFile($fileToUpload, $originalFileName, $mimeType, Zend_Gdata_Docs::DOCUMENTS_LIST_FEED_URI);

  if ($html) {
   // Find the URL of the HTML view of this document.
   $alternateLink = '';
   foreach ($newDocumentEntry->link as $link) {
    if ($link->getRel() === 'alternate') {
     $alternateLink = $link->getHref();
    }
   }
   // Make the title link to the document on docs.google.com.
   //drupal_set_message('<a href=\"$alternateLink\">\n');
  }
  echo $newDocumentEntry->title . "\n";

  $content = "Success?";
  return $content;
 } catch (Exception $e) {
  return "Error: Unable to process.\n" . $e->getMessage();
  //return 'error';
 }


}

function addPermission($document_id = '1mKjGghPEU2hfcoU82gPPXT2G0wkBPrIQtPch01-X3BA', $user = 'drupal_development@babson.edu') {
 //To share a collection, provide a collection resource ID instead of a document ID
 //http://code.google.com/apis/documents/docs/3.0/developers_guide_protocol.html#ACLSharingCollections
 $clientAcl = getClientLoginHttpClient('yourgoogleuser@yourdomain.edu', 'yourpassword');
 //$user = 'drupal_development@babson.edu';
 $method = 'POST';
 $headers['GData-Version'] = '3.0';
 $contenttype = 'application/atom+xml';

 //This url can be hardcoded or, as in this example, can be dynamically run via a var $docId and $user for sharing
 //$aclUrl = 'http://docs.google.com/feeds/default/private/full/spreadsheet%3A' . $docId . '/acl';
 //$document_id = '1-qYaniUyBYDPNMIos2Rs60RPruYKv17qzRG8mjR-V88';
 //$document_id = '1mKjGghPEU2hfcoU82gPPXT2G0wkBPrIQtPch01-X3BA';  //get this id by clicking on a url on the droogle page and grab the document id

 //api guide:  http://code.google.com/apis/documents/docs/3.0/developers_guide_protocol.html
 //to share a collection http://code.google.com/apis/documents/docs/3.0/developers_guide_protocol.html#ACLSharingCollections
 //by default user will be emailed they have been granted access ... to suppress the email
 // use POST /feeds/default/private/full/document:12345/acl?send-notification-emails=false
 $aclUrl = "https://docs.google.com/feeds/default/private/full/" . $document_id."/acl";

 $aclEntry = '<entry xmlns="http://www.w3.org/2005/Atom" xmlns:gAcl="http://schemas.google.com/acl/2007">';
 $aclEntry .= "<category scheme='http://schemas.google.com/g/2005#kind' term='http://schemas.google.com/acl/2007#accessRule'/>";
 $aclEntry .= "<gAcl:role value='writer'/>";

 $aclEntry .= "<gAcl:scope type='user' value='drupal_development@babson.edu'/>";
 $aclEntry .= "</entry>";

 $gdataAcl = new Zend_Gdata($clientAcl);

 try {
  $gdataAcl->performHttpRequest( $method, $aclUrl, $headers, $aclEntry, $contenttype );
 } catch(Zend_Gdata_App_Exception $ex) {
  // Report the exception to the user
  echo $ex->getMessage();
 }

}

function createCollection($folder_name = "test_folder") {
 $clientAcl = getClientLoginHttpClient('yourgoogleuser@yourdomain.edu', 'yourpassword');
 //$user = 'drupal_development@babson.edu';
 $method = 'POST';
 $headers['GData-Version'] = '3.0';
 $contenttype = 'application/atom+xml';

 //This url can be hardcoded or, as in this example, can be dynamically run via a var $docId and $user for sharing
 //$aclUrl = 'http://docs.google.com/feeds/default/private/full/spreadsheet%3A' . $docId . '/acl';
 //$document_id = '1-qYaniUyBYDPNMIos2Rs60RPruYKv17qzRG8mjR-V88';
 //$document_id = '1mKjGghPEU2hfcoU82gPPXT2G0wkBPrIQtPch01-X3BA';  //get this id by clicking on a url on the droogle page and grab the document id

 //api guide:  http://code.google.com/apis/documents/docs/3.0/developers_guide_protocol.html
 //to share a collection http://code.google.com/apis/documents/docs/3.0/developers_guide_protocol.html#ACLSharingCollections
 //by default user will be emailed they have been granted access ... to suppress the email
 // use POST /feeds/default/private/full/document:12345/acl?send-notification-emails=false
 $Url = "https://docs.google.com/feeds/default/private/full";

 $Entry = '<entry xmlns="http://www.w3.org/2005/Atom" xmlns:gAcl="http://schemas.google.com/acl/2007">';
 $Entry .= "<category scheme='http://schemas.google.com/g/2005#kind' term='http://schemas.google.com/docs/2007#folder'/>";
 $Entry .= "<title type='text'>" . $folder_name . "</title>";

 $Entry .= "</entry>";

 $gdataAcl = new Zend_Gdata($clientAcl);

 try {
  $gdataAcl->performHttpRequest( $method, $Url, $headers, $Entry, $contenttype );
 } catch(Zend_Gdata_App_Exception $ex) {
  // Report the exception to the user
  echo $ex->getMessage();
 }
 //returning 0 indicates success.
 return 'folder was successfully created';

}

function addToCollection($collection_id = '0B-a-StXYm5kcZDE0YmExOGEtNDg3MS00M2UwLWE4YWItYTEzZjAxNjcyYjE0',$document_id = '1mKjGghPEU2hfcoU82gPPXT2G0wkBPrIQtPch01-X3BA'){
 $clientAcl = getClientLoginHttpClient('yourgoogleuser@yourdomain.edu', 'yourpassword');
 $method = 'POST';
 $headers['GData-Version'] = '3.0';
 $contenttype = 'application/atom+xml';

 //This url can be hardcoded or, as in this example, can be dynamically run via a var $docId and $user for sharing
 //$aclUrl = 'http://docs.google.com/feeds/default/private/full/spreadsheet%3A' . $docId . '/acl';
 //$document_id = '1-qYaniUyBYDPNMIos2Rs60RPruYKv17qzRG8mjR-V88';
 //$document_id = '1mKjGghPEU2hfcoU82gPPXT2G0wkBPrIQtPch01-X3BA';  //get this id by clicking on a url on the droogle page and grab the document id

 //api guide:  http://code.google.com/apis/documents/docs/3.0/developers_guide_protocol.html
 //to share a collection http://code.google.com/apis/documents/docs/3.0/developers_guide_protocol.html#ACLSharingCollections
 //by default user will be emailed they have been granted access ... to suppress the email
 // use POST /feeds/default/private/full/document:12345/acl?send-notification-emails=false
 $Url = "https://docs.google.com//feeds/default/private/full/folder%3A" . $collection_id . "/contents";

 $Entry = '<entry xmlns="http://www.w3.org/2005/Atom" xmlns:gAcl="http://schemas.google.com/acl/2007">';
 $Entry .= "<category scheme='http://schemas.google.com/g/2005#kind' term='http://schemas.google.com/docs/2007#folder'/>";
 $Entry .= "<id>https://docs.google.com/feeds/default/private/full/" . $document_id . "</id>";

 $Entry .= "</entry>";

 $gdataAcl = new Zend_Gdata($clientAcl);

 try {
  $gdataAcl->performHttpRequest( $method, $Url, $headers, $Entry, $contenttype );
 } catch(Zend_Gdata_App_Exception $ex) {
  // Report the exception to the user
  echo $ex->getMessage();
 }
 //returning 0 indicates success.
 return 'document was successfully added to the collection';
}

function printCollection($collection_id = '0B-a-StXYm5kcNDEwZGQ2ZWItZGY2YS00YTk0LWIxYTUtMGJhZGRjNjU1NDA3'){  //happy_folder2 collection
 $clientAcl = getClientLoginHttpClient('yourgoogleuser@yourdomain.edu', 'yourpassword');
 $method = 'GET';
 $headers['GData-Version'] = '3.0';
 $contenttype = 'application/atom+xml';

 //This url can be hardcoded or, as in this example, can be dynamically run via a var $docId and $user for sharing
 //$aclUrl = 'http://docs.google.com/feeds/default/private/full/spreadsheet%3A' . $docId . '/acl';
 //$document_id = '1-qYaniUyBYDPNMIos2Rs60RPruYKv17qzRG8mjR-V88';
 //$document_id = '1mKjGghPEU2hfcoU82gPPXT2G0wkBPrIQtPch01-X3BA';  //get this id by clicking on a url on the droogle page and grab the document id

 //api guide:  http://code.google.com/apis/documents/docs/3.0/developers_guide_protocol.html
 //to share a collection http://code.google.com/apis/documents/docs/3.0/developers_guide_protocol.html#ACLSharingCollections
 //by default user will be emailed they have been granted access ... to suppress the email
 // use POST /feeds/default/private/full/document:12345/acl?send-notification-emails=false
 $Url = "https://docs.google.com//feeds/default/private/full/folder%3A" . $collection_id . "/contents";
 $Entry = NULL;
 $gdataAcl = new Zend_Gdata($clientAcl);

 try {
  $result = $gdataAcl->performHttpRequest( $method, $Url, $headers, $Entry, $contenttype );
 } catch(Zend_Gdata_App_Exception $ex) {
  // Report the exception to the user
  echo $ex->getMessage();
 }
 echo $result;
 //returning 0 indicates success.
 return $result;
}
function printCollection2($user=NULL,$password=NULL,$collection_id = '0B4mZRjHDl6MvZTA3NzBiMDctOTQ1NS00MGM5LWE2ZTUtNWYwZWYzMmJhMWI3'){  //happy_folder2 collection
 // Construct an HTTP POST request

 $clientlogin_url = "https://www.google.com/accounts/ClientLogin";

 $clientlogin_post = array(
    "accountType" => "HOSTED_OR_GOOGLE",
    "Email" => $user,
    "Passwd" => $password,
    "service" => "writely",
    "source" => "your application name"
    );

    // Initialize the curl object
    $curl = curl_init($clientlogin_url);
    // Set some options (some for SHTTP)

    curl_setopt($curl, CURLOPT_POST, true);

    curl_setopt($curl, CURLOPT_POSTFIELDS, $clientlogin_post);

    curl_setopt($curl, CURLOPT_HTTPAUTH, CURLAUTH_ANY);

    curl_setopt($curl, CURLOPT_SSL_VERIFYPEER, false);

    curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);



    // Execute

    $response = curl_exec($curl);

    //print $response;

    // Get the Auth string and save it

    preg_match("/Auth=([a-z0-9_\-]+)/i", $response, $matches);

    $auth = $matches[1];
    //echo "The auth string is: " . $auth;

    // Include the Auth string in the headers
    // Together with the API version being used
    $headers = array(
    "Authorization: GoogleLogin auth=" . $auth,
    "GData-Version: 3.0",
    );

    // Make the request
    $url2 = "https://docs.google.com/feeds/default/private/full";
    $url = "https://docs.google.com//feeds/default/private/full/folder%3A" . $collection_id . "/contents";
    curl_setopt($curl, CURLOPT_URL, $url);
    curl_setopt($curl, CURLOPT_HTTPHEADER, $headers);
    curl_setopt($curl, CURLOPT_POST, false);

    $response = curl_exec($curl);
    curl_close($curl);

    // Parse the response
    $response = simplexml_load_string($response);

    // Output data
    $content = "";
    $content .= theme('droogle_list_docs', $response);
    return $content;

}

/*
 * Will return true if succeeds and will return "Caught exception: ERROR: The user object has no data" if the user object does not get set successfully
 */
function retrieveUser($username){
 $client = Zend_Gdata_ClientLogin::getHttpClient('yourgoogleuser@yourdomain.edu', 'yourpassword', Zend_Gdata_Gapps::AUTH_SERVICE_NAME);
 $domain = "babson.edu";
 try {
  $gdata = new Zend_Gdata_Gapps($client, $domain);
 } catch (Exception $e) {
  echo 'Caught exception: ',  $e->getMessage(), "\n";
 }

 $user = $gdata->retrieveUser($username);
 try{
  if(!$user){
   throw new Exception("ERROR: The user object has no data");
  }
 } catch (Exception $e) {
  echo 'Caught exception: ',  $e->getMessage(), "\n";
  return FALSE;
 }
 $userArray = array();
 $userArray['username'] = $user->login->userName;
 $userArray['givenname'] = $user->name->givenName;
 $userArray['familyname'] = $user->name->familyName;
 $userArray['suspended'] = $user->login->suspended ? 'Yes' : 'No';
 $userArray['admin'] = $user->login->admin ? 'Yes' : 'No';
 $userArray['changePasswordAtNextLogin'] = $user->login->changePasswordAtNextLogin ? 'Yes' : 'No';
 $userArray['agreedToTerms'] = $user->login->agreedToTerms ? 'Yes' : 'No';

 /*echo 'Username: ' . $user->login->userName . "\n";
  echo 'Given Name: ' . $user->name->givenName . "\n";
  echo 'Family Name: ' . $user->name->familyName . "\n";
  echo 'Suspended: ' . ($user->login->suspended ? 'Yes' : 'No') . "\n";
  echo 'Admin: ' . ($user->login->admin ? 'Yes' : 'No') . "\n";
  echo 'Must Change Password: ' . ($user->login->changePasswordAtNextLogin ? 'Yes' : 'No') . "\n";
  echo 'Has Agreed To Terms: ' . ($user->login->agreedToTerms ? 'Yes' : 'No') . "\n";*/
 //print_r($userArray);

 return $userArray;

}

function changePassword($username, $newuserpassword){
 $client = Zend_Gdata_ClientLogin::getHttpClient('yourgoogleuser@yourdomain.edu', 'yourpassword', Zend_Gdata_Gapps::AUTH_SERVICE_NAME);
 $domain = "babson.edu";
 try {
  $gdata = new Zend_Gdata_Gapps($client, $domain);
 } catch (Exception $e) {
  echo 'Caught exception: ',  $e->getMessage(), "\n";
 }

 //Now change the user's password
 $updateUser = $gdata->retrieveUser($username);
 try {
  if(!$updateUser){
   throw new Exception("ERROR: The updateUser object has no data");
  }
 } catch (Exception $e) {
  echo 'Caught exception: ',  $e->getMessage(), "\n";
  return FALSE;
 }
 //echo 'Username: ' . $updateUser->login->userName . "\n";
 //echo 'Given Name: ' . $updateUser->name->givenName . "\n";
 //echo 'Family Name: ' . $updateUser->name->familyName . "\n";
 //echo 'Suspended: ' . ($updateUser->login->suspended ? 'Yes' : 'No') . "\n";
 //echo 'Admin: ' . ($updateUser->login->admin ? 'Yes' : 'No') . "\n";
 //echo 'Must Change Password: ' . ($updateUser->login->changePasswordAtNextLogin ? 'Yes' : 'No') . "\n";
 //echo 'Has Agreed To Terms: ' . ($updateUser->login->agreedToTerms ? 'Yes' : 'No') . "\n";
 $updateUser->login->password = $newuserpassword;
 $updateUser = $updateUser->save();


 return TRUE;

}

function createUser($username,$givenname,$familyname, $password){
 $client = Zend_Gdata_ClientLogin::getHttpClient('yourgoogleuser@yourdomain.edu', 'yourpassword', Zend_Gdata_Gapps::AUTH_SERVICE_NAME);
 $domain = "babson.edu";
 try {
  $gdata = new Zend_Gdata_Gapps($client, $domain);
 } catch (Exception $e) {
  echo 'Caught exception: ',  $e->getMessage(), "\n";
 }

 //Now try creating a user
 try {
  $gdata->createUser($username, $givenname, $familyname, $password);
 } catch (Exception $e) {
  echo 'Caught exception: ',  $e->getMessage(), "\n";
  return FALSE;
 }

 return TRUE;

}

function deleteUser($username){
 $client = Zend_Gdata_ClientLogin::getHttpClient('yourgoogleuser@yourdomain.edu', 'yourpassword', Zend_Gdata_Gapps::AUTH_SERVICE_NAME);
 $domain = "babson.edu";
 try {
  $gdata = new Zend_Gdata_Gapps($client, $domain);
 } catch (Exception $e) {
  echo 'Caught exception: ',  $e->getMessage(), "\n";
 }

 //Now try creating a user
 $user = $gdata->retrieveUser($username);
 try {
  if(!$user){
   throw new Exception("ERROR: The updateUser object has no data");
  }
 } catch (Exception $e) {
  echo 'Caught exception: ',  $e->getMessage(), "\n";
  return FALSE;
 }
 $user->delete();


 return TRUE;

}

function retrieveAllUsers(){
 $client = Zend_Gdata_ClientLogin::getHttpClient('yourgoogleuser@yourdomain.edu', 'yourpassword', Zend_Gdata_Gapps::AUTH_SERVICE_NAME);
 $domain = "babson.edu";
 try {
  $gdata = new Zend_Gdata_Gapps($client, $domain);
 } catch (Exception $e) {
  echo 'Caught exception: ',  $e->getMessage(), "\n";
 }

 $feed = $gdata->retrieveAllUsers();
 $count = 0;
 foreach ($feed as $user) {
  $count++;
  echo "  * " . $user->login->username . ' (' . $user->name->givenName .
        ' ' . $user->name->familyName . ")\n";
 }
 echo 'total number of users: '.$count;
}

function getACollectionId($user,$password,$desiredCollectionTitle){
 //feeds/default/private/full/-/folder
 // Construct an HTTP POST request
 $clientAcl = getClientLoginHttpClient($user, $password);
 $method = 'GET';
 $headers['GData-Version'] = '3.0';
 $contenttype = 'application/atom+xml';

 //This url can be hardcoded or, as in this example, can be dynamically run via a var $docId and $user for sharing
 //$aclUrl = 'http://docs.google.com/feeds/default/private/full/spreadsheet%3A' . $docId . '/acl';
 //$document_id = '1-qYaniUyBYDPNMIos2Rs60RPruYKv17qzRG8mjR-V88';
 //$document_id = '1mKjGghPEU2hfcoU82gPPXT2G0wkBPrIQtPch01-X3BA';  //get this id by clicking on a url on the droogle page and grab the document id

 //api guide:  http://code.google.com/apis/documents/docs/3.0/developers_guide_protocol.html
 //to share a collection http://code.google.com/apis/documents/docs/3.0/developers_guide_protocol.html#ACLSharingCollections
 //by default user will be emailed they have been granted access ... to suppress the email
 // use POST /feeds/default/private/full/document:12345/acl?send-notification-emails=false
 $Url = "https://docs.google.com/feeds/default/private/full/-/folder";
 $Entry = NULL;
 $gdataAcl = new Zend_Gdata($clientAcl);

 try {
  $response = $gdataAcl->performHttpRequest( $method, $Url, $headers, $Entry, $contenttype );
 } catch(Zend_Gdata_App_Exception $ex) {
  // Report the exception to the user
  echo $ex->getMessage();
 }
 /*http://www.welmers.net/~bastiaan/ZendFramework-1.5.3/documentation/manual/core/en/zend.http.response.html
  * this url shows how to parse the results
  */
 $body = $response->getBody();
 $xml = simplexml_load_string($body);
 //$docurl = xml_attribute($xml->entry->content,'src');
 //preg_match("/folder%3A(.*)\/contents/i", $docurl, $matches);
 //$collectionID = $matches[1];
 $collectionID = "";
 $collectionTitles = "";
 $resultArray = array();
 //print_r($xml->entry);
 foreach($xml->entry as $object){
  if($object->title != $desiredCollectionTitle){
   continue;
  }
  $collectionTitle = $object->title;
  $docurl = xml_attribute($object->content,'src');
  preg_match("/folder%3A(.*)\/contents/i", $docurl, $matches);
  $collectionID = $matches[1];
  //$resultArray[$collectionTitle] = $collectionID;*/
  //$collectionTitles .= $object->title.',';
  //$collectionTitles .= xml_attribute($object->content,'src');
 }
 //$collectionTitles .= ','.$collectionID;
 return $collectionID;
}

function xml_attribute($object, $attribute)
{
 if(isset($object[$attribute]))
 return (string) $object[$attribute];
}
?>
