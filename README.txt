
CONTENTS OF THIS FILE
---------------------

 * Overview
 * Quick setup
 * Requirements
 * Installation
 * Options
 * Features

OVERVIEW
--------

USASearch is an embeddable search engine that can 
be used to search a site. An Affiliate profile and
associated Affiliate ID are required. Read more at
http://usasearch.howto.gov/

QUICK SETUP
-----------

After installing this module, activate it and then configure 
it at admin/settings/usasearch by entering USASearch's unique
affiliate name for your affiliate. Once you have granted
permission for one or more roles to search using the built-in
search module, the the standard search box becomes a USASearch
powered search.

REQUIREMENTS
------------

 * A valid Affiliate profile from USASearch
 * Core Search module must be enabled

Drupal 6 ONLY
 * jQuery Update module (any 6.x-2.x release/dev)

INSTALLATION
------------

Place the usasearch directory in your sites/all/modules directory.  
Enable the USASearch module at admin/modules, and configure it at 
admin/settings/usasearch.

To configure this module, you will need your USASearch's unique
Affiliate name.  Login at https://search.usa.gov/affiliates and
click the "View Current" link next to the affiliate you want to
use. In the URL of the page that loads, look for
"affiliate=example.gov". "example.gov" is the Affiliate name in
this example.

OPTIONS
-------

 * Affiliate name (eg. example.gov)
 * Search domain (only change this if USASearch has configured
   this option for your affiliate)
 * Keep Drupal search page (keeps the Drupal search page
   instead of redirecting it to USASearch
 * Enable Discovery Tag
 * Type-ahead affiliate ID (this is a numeric code, eg. 77)

FEATURES
--------

The module was designed to allow exporting of all the admin
configuration options via the Strongarm and Features modules.
