<?php

/**
 * @file
 * Non hook implementations for the USA Search Subsites module.
 */

/**
 * Generates a list of subsite links.
 *
 * @return array
 *   A renderable array of subsite links.
 */
function usasearch_subsites_links() {
  $subsites = usasearch_subsites_list();

  $params = drupal_get_query_parameters();
  $subsite_links = array();
  foreach ($subsites as $affiliate_id => $subsite_name) {
    $query = array_merge($params, array('affiliate' => check_plain($affiliate_id)));
    $subsite_links[$affiliate_id] = l(t('@subsite_name', array('@subsite_name' => $subsite_name)), current_path(), array('query' => $query));
  }

  $markup = array(
    '#items' => $subsite_links,
    '#theme' => 'item_list',
  );

  return $markup;
}

/**
 * Returns an array of subsites.
 *
 * @return array
 *   The available subsites.
 */
function usasearch_subsites_list() {
  $subsites_string = variable_get('usasearch_subsite_affiliate_ids', '');

  module_load_include('module', 'list');
  $subsites = list_extract_allowed_values($subsites_string, 'list text', FALSE);

  return $subsites;
}

/**
 * Verifies that a given subsite is in list of configured subsites.
 *
 * @param string $affiliate_id
 *
 * @return bool
 *   TRUE if the given subsite is in the list of configured subsites.
 */
function usasearch_subsites_is_subsite($affiliate_id) {
  $subsites = usasearch_subsites_list();

  return array_key_exists($affiliate_id, $subsites);
}
