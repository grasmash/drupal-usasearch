<?php

/**
 * @file
 * Provides content resource for USA Search Index module.
 */

/**
 * Return an array of optionally paged nids based on a set of criteria.
 *
 * @param int $page
 *   Page number of results to return (in pages of 20).
 * @param int $page_size
 *   Integer number of items to be returned.
 *
 * @return array
 *   An array of node objects.
 */
function _usasearch_index_resource($page, $page_size) {
  module_load_include('inc', 'services', 'services.module');

  $resource = 'node';
  $default_limit = variable_get("services_{$resource}_index_page_size", 100);
  if (!user_access('perform unlimited index queries') && $page_size > $default_limit) {
    $page_size = $default_limit;
  }

  $count_query = db_select('node', 't')
    ->condition('status', 1)
    ->addTag('usasearch_index_count');
  $count = $count_query->countQuery()->execute()->fetchField();

  $fields = array(
    'title',
    'nid',
    'vid',
    'type',
    'language',
    'promote',
  );
  $fields = implode(',', $fields);
  $parameters = '';

  $node_select = db_select('node', 't')
    ->condition('status', 1)
    ->orderBy('changed', 'DESC')
    ->orderBy('created', 'DESC')
    ->addTag('usasearch_index');

  // Add created and changed fields. These are added as expressions so that
  // the timestamp can be formatted as yyyy-mm-dd HH:mm:ss.
  $node_select->addExpression('FROM_UNIXTIME(t.created)', 'created');
  $node_select->addExpression('FROM_UNIXTIME(t.changed)', 'changed');

  services_resource_build_index_query($node_select, $page, $fields, $parameters, $page_size, $resource);

  if (!user_access('administer nodes')) {
    $node_select->condition('status', 1);
  }

  $result = services_resource_execute_index_query($node_select);
  $nodes = services_resource_build_index_list($result, $resource, 'nid');

  // Render the node content.
  foreach ($nodes as $nid => $node) {
    $node = node_load($node->nid);

    // Set node path.
    $uri = entity_uri('node', $node);
    $nodes[$nid]->path = url($uri['path'], array('absolute' => TRUE));
    unset($nodes[$nid]->uri);

    // Set node content property.
    $view = node_view($node);
    $rendered = drupal_render($view);
    $nodes[$nid]->content = $rendered;
  }

  return services_govformat_format(array(
    'results' => $nodes,
    'pagesize' => $page_size,
    'count' => $count,
  ));
}
