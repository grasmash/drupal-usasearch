USA Search Index
---------------------

This module provides a simple index (via services) for USA Search to consume.
USA Search will consume the Drupal provided index and transform it into a
keyword index that will be used to provide search results.

Installation
---------------------

* Enable usasearch_index module
* Enable usasearch_index's content resource for at least one endpoint by:
    * Navigating to /admin/structure/services
    * Selecting 'edit resources' for an endpoint
    * Checking 'usasearch' checkbox
* Visit resource at /[your-sites-endpoint]/usasearch.json

Modifying Response
---------------------

You may implement hook_query_TAG_alter() to for the usasearch_index tag
and alter the index query, like so:

````
/**
 * Implements hook_query_TAG_alter().
 */
function mymodule_query_usasearch_index_alter(QueryAlterableInterface $query) {
  $query->condition('type', 'some_node_type', '!=');
}

````

This example would remove all nodes of type 'some_node_type' from results.
