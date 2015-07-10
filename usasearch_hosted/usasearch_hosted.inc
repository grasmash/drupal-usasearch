<?php

/**
 * @file
 * Contains non-hook implementations for USA Search Hosted module.
 */

/**
 * Page callback function to redirect requests to usasearch.
 *
 * @param string $query
 *   The search query string taken from the request URL.
 */
function usasearch_hosted_redirect($query = '') {
  $action_domain = check_plain(variable_get('usasearch_action_domain', 'http://search.usa.gov'));
  $affiliate_name = variable_get('usasearch_affiliate_name', '') ? check_plain(variable_get('usasearch_affiliate_name', '')) : '';

  // Check for query terms and use them in redirect if there.
  if ($query) {
    drupal_goto("$action_domain/search", array(
      'query' => array(
        'query' => $query,
        'affiliate' => $affiliate_name,
      ),
    ));
  }
  else {
    drupal_goto("$action_domain/search", array('query' => array('affiliate' => $affiliate_name)));
  }
}
