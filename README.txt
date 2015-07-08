
Contents of this file
---------------------

 * Overview
 * Installation
 * Requirements
 * Setup Tips
 * Features

Overview
--------

USASearch is an embeddable search engine that can
be used to search a site. An Affiliate profile and
associated Affiliate ID are required. Read more at
http://usasearch.howto.gov/

When content is searched, the user will be redirected to the hosted search
solution. When they click on a result, they will be sent back to your site.

Installation
-----------

1. Place the usasearch directory in your sites/all/modules directory.
1. Enable one of the following submodules:
  * USASearch Hosted: replaces Drupal Core search with USASearch hosted search.
  * USASearch API: Provides real time updates to USASearch's index when Drupal
    nodes are created, modified, or deleted.
  * USASearch field: Makes available a "USASearch" field type, which can be
    added to Drupal entities to render a USASearch box that will search a 
    specific affiliate ID. This is useful when used in a Fieldable Panel Pane
    in a microsite.
  * USASearch Index: Generates a machine-readable index of all nodes that can
    be used to passively populate USASearch's index.
1. Configure it at admin/config/search/usasearch by entering USASearch's unique
affiliate name for your affiliate.
1. Grant permission for one or more roles to
search using the built-in search module.

Requirements
------------

 * A valid Affiliate profile from USASearch.
 * Core Search module must be enabled.
 * (usasearch_api) composer_manager and Guzzle library.

Setup Tips
------------

#### API Search - usasearch_api
This module depends on composer_manager, which will manage the PHP library
dependencies. Namely, it will download Guzzle to the sites/all/vendor directory.

#### Configuration
To configure this module, you will need your USASearch's unique
Affiliate name.  Login at https://search.usa.gov/affiliates and
click the "View Current" link next to the affiliate you want to
use. In the URL of the page that loads, look for
"affiliate=example.gov". "example.gov" is the Affiliate name in
this example.

Features
--------

The module was designed to allow exporting of all the admin
configuration options via the Strongarm and Features modules.
