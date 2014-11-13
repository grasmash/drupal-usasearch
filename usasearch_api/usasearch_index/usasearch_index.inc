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

  $fields = array(
    'title',
    'nid',
    'vid',
    'type',
    'created',
    'changed',
    'language',
    'promote',
  );
  $fields = implode(',', $fields);
  $parameters = '';

  $node_select = db_select('node', 't')
    ->addTag('usasearch_index')
    ->orderBy('sticky', 'DESC')
    ->orderBy('created', 'DESC');

  $count_query = $node_select;
  $count = $count_query->countQuery()->execute()->fetchField();

  services_resource_build_index_query($node_select, $page, $fields, $parameters, $page_size, 'node');

  if (!user_access('administer nodes')) {
    $node_select->condition('status', 1);
  }

  $result = services_resource_execute_index_query($node_select);
  $nodes = services_resource_build_index_list($result, 'node', 'nid');

  // Render the node content.
  foreach ($nodes as $nid => $node) {
    $node = node_load($node->nid);
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
