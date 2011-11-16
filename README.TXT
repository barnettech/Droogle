The Droogle Module integrates Google Docs with Drupal. It provides a list of a configured google user's documents and allows uploading docs to that account. It also exposes much of the google docs api to Drupal users -- you can create collections, move documents into collections, etc. Droogle also works with Organic Groups and each Organic Group can hook up to a unique user account to manage it's group documents. The module's creation is sponsored by Babson College and is to be used to better manage documents for groups, classes, etc. Integrating documents with google means better access to documents on ipads, iphones, etc, since there are many apps for these devices to sync your google docs account(s). Droogle is not to be confused with BBoogle, which is Blackboards integration with Google Docs -- but Droogle works much the same way and serves the same purpose.

We are looking for ideas on how to improve the module to make it as useful as possible in eduction and to the general community. We are open to having co-maintainers, preferably with another college or university.

INSTALL
------------
Installs like all Drupal Modules, just put it in the modules directory and enable the module

DROOGLE CONFIG AND SETUP
------------------------
Go to admin/settings/droogle and put in usernames and password for the google accounts you would like to use.  You can optionally setup Organic Groups to look for a group cck field to read the google username and password from for each group.  You can also make it so group owners who can edit their own group can manually enter a google username and password to use.  You will have to know how to setup cck fields for the group content type to get this organic group integration feature working.

The module is presented as is under the opensource license that is within the module for you to read, we make no guarantees.  That being said we're looking to make this module great and feel free to submit issues and suggestions to our issue que.

GOOGLE API Exposed for Drupal Developers
-----------------------------------------
Check out the google_tests.php file to check out how to call the api using the command line to create users, delete users, create folders (collections) and lots more.  The Droogle module makes uses the Zend library and it's all bundled up for you to easily use and extend.
