
Contents of this file
---------------------

 * Overview
 * Installation
 * Requirements
 * Setup Tips
 * Features

Overview
--------

DigitalGov search (formerly USASearch) is an embeddable search engine that can
be used to search a site. An Affiliate profile and associated Affiliate ID are required. 
Read more at http://search.digitalgov.gov/developer/

DigitalGov Search offers several services:
 1. A hosted search platform.
 2. Search Results API.
 3. i14y (beta) real-time indexing API


#### Hosted Search
Provides a custom search block, separate from Drupal’s system search block.
When content is searched, the user will be redirected to the hosted search
solution. When they click on a result, they will be sent back to your site.
The search block has the optional “autocomplete” functionality, using DigitalGov’s
“Type-ahead” API

#### Search Results API
In development.

#### i14y 
In development

Installation
-----------

1. Place the usasearch directory in your modules directory.
2. Enable the DigitalGov Search module at admin/modules.
3. Configure it at admin/config/search/usasearch by entering DigitalGov Search’s unique
affiliate name for your affiliate.



Requirements
------------

 * A valid Affiliate profile from DigitalGov Search.
 * Core Search module must be enabled.

Setup Tips
------------


TODO


Features
--------

The module was designed to allow exporting of all the admin
configuration options via Drupal 8 Configuration Management.
