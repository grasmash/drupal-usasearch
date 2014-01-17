<?php

/**
 * @file
 * Contains page callbacks for USA Search module.
 */

/**
 * Callback for search result page.
 *
 * @param string $search_url
 *   The url suffix for the performed search.
 *
 * @return array
 *   A renderable array.
 */
function usasearch_page($search_url = '') {
  module_load_include('inc', 'usasearch');
  $mode = usasearch_get_mode();
  $url_parts = drupal_parse_url($search_url);
  $search_path = urldecode(filter_xss($url_parts['path']));

  // Extract the document collection parameter if it is set.
  $dc = !empty($url['query']['dc']) ? $url['query']['dc'] : '';

  // Render via API or redirect, depending on mode setting.
  if ($mode =='api') {
    return usasearch_api_display($search_path, $dc);
  }
  else {
    usasearch_redirect($search_path);
  }
}

/**
 * Displays search results via the USA Search API.
 *
 * @param string $query
 *   The search query string taken from the request URL.
 *
 * @param string $dc
 *   (optional) The specified document collection.
 *
 * @return array
 *   A renderable array.
 */
function usasearch_api_display($query, $dc = '') {

  // Perform the query, using the requested offset from pager_find_page().
  // This comes from a URL parameter, so here we are assuming that the URL
  // parameter corresponds to an actual page of results that will exist
  // within the set.
  $page = pager_find_page();
  $num_per_page = variable_get('usasearch_results_per_page', 10);
  $offset = $num_per_page * $page;

  // Query the API.
  $client = new UsaSearchClient();
  $response = $client->search($query, $dc, $offset);
  $module = 'usasearch';

  // @todo Implement uses of these optional properties.
  // $response['related'];
  // $response['boosted_results'];

  // Generate the search form.
  $output = array(
    'search_form' => drupal_get_form('search_form', NULL, $query, $module),
  );

  // Include a 'did you mean' suggestion if it was returned.
  if (!empty($response['spelling_suggestion'])) {
    $options = array('query' => array('dc' => $dc));
    $output['search_form']['#suffix'] = t('Did you mean ') . l($response['spelling_suggestion'], 'search/content/' . $response['spelling_suggestion'], $options);
  }

  $results = array();
  if (!empty($response['total'])) {
    // Now that we have the total number of results, initialize the pager.
    pager_default_initialize($response['total'], $num_per_page);
    $output['pager'] = theme('pager');

    // Iterate over results and add to render array.
    foreach ($response['results'] as $key => $result) {
      $results[] = array(
        '#theme' => 'search_result',
        'title' => usasearch_replace_unicode_char($result['title']),
        'link' => $result['unescapedUrl'],
        'snippet' => usasearch_replace_unicode_char($result['content']),
        'module' => 'usasearch',
        'info_split' =>array(
          // Unfortunately, this is often NULL and is not displayed.
          'date' => $result['publishedAt'],
        ),
      );
    }
  }

  // Add search results to the render array.
  $output['search_results'] = array(
    '#theme' => 'search_results',
    '#results' => $results,
    '#module' => $module,
  );

  return $output;
}

/**
 * Page callback function to redirect requests to usasearch.
 *
 * @param string $query
 *   The search query string taken from the request URL.
 */
function usasearch_redirect($query) {
  // Set action_domain from variables table or default to search.usa.gov.
  $action_domain = check_plain(variable_get('usasearch_action_domain', 'search.usa.gov'));
  // Set affiliate_name from variables table, checking for a value using ternary operator.
  $affiliate_name = variable_get('usasearch_affiliate_name', '') ? check_plain(variable_get('usasearch_affiliate_name', '')) : '';
  // Set action_protocol from server globals, checking for a value using ternary operator.
  $action_protocol = isset($_SERVER['HTTPS']) ? 'https' : 'http';
  // Check for query terms and use them in redirect if there.
  if ($query) {
    drupal_goto("$action_protocol://$action_domain/search", array('query' => array('query' => $query, 'affiliate' => $affiliate_name)));
  }
  else {
    drupal_goto("$action_protocol://$action_domain/search", array('query' => array('affiliate' => $affiliate_name)));
  }
}
