Contents of this file
---------------------

 * Overview
 * Installation
 * Requirements
 * Setup Tips
 * Features

Overview
--------

DigitalGov Search (machine name: usasearch) is a search engine that can be used to power 
the search box on any U.S. government site. Read more and sign up for an account at 
http://search.digitalgov.gov.

Requirements
------------

 * A valid search Site in the DigitalGov search Admin Center.
 * Core Search module must be enabled.
 * USASearch submodule USASearch_API requires Composer_Manager and GuzzleHTTP 
 library. Composer_Manager libraries require PHP 5.4.5 or higher.

Pre-Install: the DigitalGov Search Admin Center
-----------------------------------------------

Before configuring this module, set up your search Site in DigitalGov Search’s 
Admin Center (https://search.usa.gov/sites/). 
1. Note the `Site Handle` on the Dashboard > Settings page.
2. Email search@support.digitalgov.gov and ask us to turn on i14y for your site. After 
we've turned it on, go to Content > i14y Drawers. Add an i14y Drawer, giving it a Drawer 
Handle. It will be assigned a Secret Token. Note the Handle and Secret Token by clicking 
Show.
3. Configure your domains, social media, and other content that you want searched, and 
customize the Display settings to brand your results page. 

Learn more at http://search.digitalgov.gov/manual/index.html and 
http://search.digitalgov.gov/manual/training.html

Pre-Install: the Command Line
-----------------------------

Ping our server from the command line to confirm your API requests can reach us. 

See a sample curl request and more information at http://gsa.github.io/slate/#create-a-document. 
If you don't receive a 200 OK response, contact search@support.digitalgov.gov for assistance.

Installation and Setup: Drupal
------------------------------

1. Place the usasearch directory in your sites/all/modules directory.
2. Enable the module and the following submodules:
  * USASearch_Hosted (required to return results): replaces Drupal Core search 
  with USASearch hosted search.
  * USASearch_API (required to update the index): Provides real time updates to 
  USASearch's index when Drupal nodes are created, modified, or deleted.
  * USASearch_field (optional): Makes available a "USASearch" field type, which 
  can be added to Drupal entities to render a USASearch box that will search a 
  specific affiliate ID. This is useful when used in a Fieldable Panel Pane in a 
  microsite.
  * USASearch Index (optional): Generates a machine-readable index of all nodes 
  that can be used to passively populate USASearch's index.
3. Configure it at admin/config/search/usasearch with the following information 
from the DigitalGov Search Admin Center:
  * Site Handle (required)
  * Enable autocomplete to provide type-ahead search suggestions in your site’s 
  search box
  * Search Domain (modify only if using a CNAME for your results page)
  * i14y Drawer Handle (optional but highly recommended - see pre-install info above)
  * i14y Drawer Secret Key (required if a Drawer Handle is entered)
4. Grant permission for one or more roles to search using the built-in search 
module.
5. Configure each content type's snippet on admin/structure/types/manage/CONTENTTYPE/display/search_index
6. After a configuration change, run Re-Index Site on admin/config/search/settings, and then run cron.


Setup Tips
------------

#### API Search - usasearch_api
This submodule handles the API calls that send your content to our index. It depends on 
composer_manager, which will manage PHP library dependencies. Namely, GuzzleHTTP (see 
sites/all/vendor). The composer libraries require PHP version 5.4.5 or higher.

#### Updating the index
The i14y index is updated by Drupal when content is created, modified, or 
deleted. A bulk update can be initialized through the Re-index Site button on 
Drupal search settings admin/config/search/settings.

#### Note re: Content type search settings 
This module contains a legacy checkbox at the content type configuration level that 
suggests you can suppress that content type from being sent for indexing. This checkbox 
doesn't do anything currently, and all content will be indexed. Exclusions by content 
type, or other taxonomy terms, can be done in the DigitalGov Search Admin Center.

Features
--------

The module was designed to allow exporting of all the admin configuration options via the 
Strongarm and Features modules.

All site content is indexed, including title, body, URL, date, tags and taxonomy terms, 
language, and (optionally) description/summary. 