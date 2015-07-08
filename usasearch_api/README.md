Overview
--------

This module provides real time updates to USASearch's index when Drupal nodes 
are created, modified, or deleted.

Installation
-----------

1. Install the composer_manager project for drush:
   `drush dl composer-manager`
   `drush cc drush`
1. Enable usasearch_api *via drush* `drush en usasearch_api -y`

Requirements
------------

This module depends on composer_manager, which will manage the PHP library
dependencies. Namely, it will download Guzzle to the sites/all/vendor directory.
