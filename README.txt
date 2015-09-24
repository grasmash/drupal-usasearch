
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
associated Site ID are required. Read more at
http://search.digitalgov.gov

When content is searched, the user will be redirected to the hosted search
solution. When they click on a result, they will be sent back to your site.

Installation
-----------

1. Place the usasearch directory in your sites/all/modules directory.
2. Enable one of the following submodules:
  * USASearch Hosted (required to return results): replaces Drupal Core search with USASearch hosted search.
  * USASearch API (required to update the index): Provides real time updates to USASearch's index when Drupal
    nodes are created, modified, or deleted.
  * USASearch field (optional): Makes available a "USASearch" field type, which can be
    added to Drupal entities to render a USASearch box that will search a
    specific affiliate ID. This is useful when used in a Fieldable Panel Pane
    in a microsite.
  * USASearch Index (optional): Generates a machine-readable index of all nodes that can
    be used to passively populate USASearch's index.
3. Configure it at admin/config/search/usasearch by entering USASearch's unique
site name, drawer handle, and secret token.
4. Grant permission for one or more roles to search using the built-in search module.

Requirements
------------

 * A valid Affiliate profile from USASearch.
 * Core Search module must be enabled.
 * (usasearch_api) composer_manager and Guzzle library.

Setup Tips
------------

#### API Search - usasearch_api
This module depends on composer_manager, which will manage PHP library
dependencies. Namely, it will download Guzzle to the sites/all/vendor directory.
The composer libraries require Php version 5.4 or higher.

#### Configuration
To use the i14y DigitalGovSearch service three modules should be
enabled, usasearch, usasearch_api, and usesearch hosted.
To configure search you will need your USASearch's unique
site handle. Login at https://search.usa.gov/affiliates and
select Dashboard / Settings in the left side menu in the site you want to
use. In the URL of the page that loads, look for "affiliate=example.gov". "example.gov"
is the site handle name in this example.
For indexing you will need the i14y drawer handle and secret token. Login at https://search.usa.gov/affiliates
and select Content / i14y Drawers in the left side menu. If needed Add i14y Drawer and enter
an i14y drawer handle or select show to display the i14y drawer handle and secret token.

#### Updating the index
Content updated are submitted to indexed when saved, updated, or deleted. Additional a bulk update
can be initialixed by through Drupal search settings admin/config/search/settings.

Features
--------

The module was designed to allow exporting of all the admin
configuration options via the Strongarm and Features modules.
