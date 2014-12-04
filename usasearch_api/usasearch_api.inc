<?php

/**
 * @file
 * Contains non-hook implementations for USA Search API module.
 */

/**
 * Implements hook_form().
 */
function usasearch_api_advanced_form() {
  $form = array();

  $form['all'] = array(
    '#type' => 'textfield',
    '#title' => t('All of these words'),
    '#default_value' => !empty($_GET['all']) ? $_GET['all'] : '',
  );
  $form['exact'] = array(
    '#type' => 'textfield',
    '#title' => t('This exact phrase'),
    '#default_value' => !empty($_GET['exact']) ? $_GET['exact'] : '',
  );
  $form['any'] = array(
    '#type' => 'textfield',
    '#title' => t('Any of these words'),
    '#default_value' => !empty($_GET['any']) ? $_GET['any'] : '',
  );
  $form['none'] = array(
    '#type' => 'textfield',
    '#title' => t('None of these words'),
    '#default_value' => !empty($_GET['none']) ? $_GET['none'] : '',
  );
  $form['file_type'] = array(
    '#type' => 'textfield',
    '#title' => t('File Type'),
    '#default_value' => !empty($_GET['file_type']) ? $_GET['file_type'] : '',
  );
  $affiliates = usasearch_affiliates_list();
  if ($affiliates) {
    $form['affiliate'] = array(
      '#type' => 'select',
      '#title' => t('Affiliate'),
      '#options' => $affiliates,
      '#default_value' => !empty($_GET['affiliate']) ? $_GET['affiliate'] : variable_get('usasearch_affiliate_name'),
      '#required' => FALSE,
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
  );
  $form['help'] = array(
    '#prefix' => '<div class="advanced-search-help">',
    '#markup' => t('You can also use !link to help limit and focus your searches.', array('!link' => l(t('advanced search operators'), 'http://onlinehelp.microsoft.com/en-us/bing/ff808421.aspx'))),
    '#suffix' => '</div>',
  );

  return $form;
}

/**
 * Submit handler for advanced search form.
 */
function usasearch_api_advanced_form_submit($form, &$form_state) {
  // Create form destination parameters.
  $params = drupal_get_query_parameters();
  $options = array(
    'query' => array(),
  );

  $keys = array();

  if (!empty($form_state['values']['all'])) {
    $all_keys = $form_state['values']['all'];
    $keys[] = $all_keys;
  }
  if (!empty($form_state['values']['exact'])) {
    $exact_keys = '"' . $form_state['values']['exact'] . '"';
    $keys[] = $exact_keys;
  }
  if (!empty($form_state['values']['any'])) {
    $any_keys = $form_state['values']['any'];
    $any_keys = explode(' ', $any_keys);
    $any_keys = '(' . implode(' OR ', $any_keys) . ')';
    $keys[] = $any_keys;
  }
  if (!empty($form_state['values']['none'])) {
    $not_keys = $form_state['values']['none'];
    $not_keys = explode(' ', $not_keys);
    $not_keys = '-' . implode(' -', $not_keys);

    $keys[] = $not_keys;
  }
  if (!empty($form_state['values']['file_type'])) {
    $file_type = $form_state['values']['file_type'];
    $file_type = 'filetype:' . $file_type;
    $keys[] = $file_type;
  }
  if (!empty($form_state['values']['affiliate'])) {
    $affiliate = $form_state['values']['affiliate'];
    $options['query']['affiliate'] = $affiliate;
  }
  elseif (!empty($params['affiliate'])) {
    $options['query']['affiliate'] = $params['affiliate'];
  }

  $advanced_keys = implode(' ', $keys);

  $destination = array('search/all/' . $advanced_keys, $options);
  $form_state['redirect'] = $destination;
}

/**
 * Fetch search results from USA Search API.
 *
 * @param string $keys
 *   The keyword string, passed by hook_search_execute().
 *
 * @param array $conditions
 *   An associative array of conditions, passed by hook_search_execute().
 *
 * @param string $module
 *   The name of the module performing the search.
 *
 * @return array
 *   An array of formatted search results.
 */
function usasearch_api_get_results($keys, $conditions, $module) {

  // Set up vars.
  $affiliate = isset($conditions['affiliate']) ? $conditions['affiliate'] : '';
  $doc_collection = isset($conditions['dc']) ? $conditions['dc'] : '';
  $index = isset($conditions['index']) ? $conditions['index'] : 'web';
  $num_per_page = variable_get('usasearch_results_per_page', 10);

  // Drupal starts the page count at 0, whereas USA Search starts it at one.
  // We need to add one to the value before making the API request.
  $page = pager_find_page() + 1;

  $response = usasearch_api_request($keys, $doc_collection, $page, $index, $affiliate);

  // Build results and pager.
  $results = array();
  if (!empty($response['total'])) {

    // Now that we have the total number of results, initialize the pager.
    pager_default_initialize($response['total'], $num_per_page);

    // Iterate over results and add to render array.
    foreach ($response['results'] as $key => $result) {
      // E.g., usasearch_images_result_images() or usasearch_api_result_web().
      $function = $module . '_result_' . $index;
      $results[] = $function($result, $module);
    }
  }

  return $results;
}

/**
 * Makes a search request to the USA Search API.
 *
 * @param string $keys
 *   The keywords to search.
 *
 * @param int $doc_collection
 *   The document collection ID.
 *
 * @param int $page
 *   The search results page number to return. E.g., results page 1, 2, etc.
 *
 * @param string $index
 *   The index to search. E.g., news, web, images, etc.
 *
 * @param string $affiliate
 *   (optional) The USASearch affiliate id to search. Defaults to value in
 *   usasearch_affiliate_name variable.
 *
 * @return array
 *   The parsed JSON response from USASearch.
 */
function usasearch_api_request($keys, $doc_collection, $page, $index, $affiliate = NULL) {

  $response = &drupal_static(__FUNCTION__);
  if (isset($response)) {
    return $response;
  }

  if (!$affiliate) {
    $affiliate = variable_get('usasearch_affiliate_name', '');
  }

  // Query the API.
  try {
    $client = new UsaSearchClient($affiliate);
    $response = $client->search($keys, $doc_collection, $page, $index);
  }
  catch (Exception $e) {
    watchdog('usasearch_api', 'USA Search host server could not be reached. Error: @error', array('@error' => $e->getMessage()));
    drupal_set_message(t('An error occurred when attempting to process your search request. Please try again.'), 'error');
    // By default, error responses are not cached. This prevents us from caching
    // requests that time out and return no results.
    if (!variable_get('usasearch_api_cache_errors')) {
      drupal_add_http_header('Cache-control', 'no-store');
    }
  }

  return $response;
}

/**
 * Converts a USA Search result to a Drupal search result for web index.
 *
 * @param array $result
 *   A single search result, parsed directly from USA Search response JSON.
 *
 * @param string $module
 *   The drupal module executing the search.
 *
 * @return array
 *   A search result to be passed to Drupal's search-result.tpl.php.
 */
function usasearch_api_result_web($result, $module) {
  return array(
    'title' => usasearch_api_replace_unicode_char($result['title']),
    'link' => $result['unescapedUrl'],
    'snippet' => usasearch_api_replace_unicode_char($result['content']),
    'module' => $module,
    'date' => usasearch_api_utc_to_unix_timestamp($result['publishedAt']),
    'type' => 'web',
  );
}

/**
 * Loads the Bing logo from cache or via remote fetch operation.
 */
function usasearch_api_logo($index = 'web') {
  $logo_info = variable_get('usasearch_logo_info', '');
  $logo = array();

  // Attempt to fetch from remote source.
  if (!$logo_info) {
    $logo_info = usasearch_api_fetch_logo();
  }
  if ($logo_info) {
    $logo = array(
      '#theme' => 'html_tag',
      '#tag' => 'img',
      '#value' => '',
      '#attributes' => array(
        'src' => $logo_info['value'],
        'width' => $logo_info['width'],
        'height' => $logo_info['height'],
        'class' => array('usasearch-logo'),
      ),
    );
  }

  return $logo;
}

/**
 * Fetches information about the Bing logo.
 *
 * This is done dynamically because the logo changes sometimes.
 */
function usasearch_api_fetch_logo() {
  try {
    $client = new Guzzle\Http\Client('http://api.bing.com');
    $request = $client->get('Brandassets.xml', array());
    $response = $request->send();
    $data = $response->xml();

    $logo_info = array(
      'value' => (string) trim($data->Images->LogoSmallDark),
      'width' => (string) $data->Images->LogoSmallDark->Width,
      'height' => (string) $data->Images->LogoSmallDark->Height,
    );

    if ($logo_info) {
      variable_set('usasearch_logo_info', $logo_info);
      return $logo_info;
    }
  }
  catch (Exception $e) {
    watchdog('usasearch_api', 'There was an error attempting to retrieve the Bing logo for USASearch attribution: @error', array('@error' => $e->getMessage()));
  }

  return FALSE;
}

/**
 * Converts the unicode characters "U+E000" and "U+E001" to <b> and </b> tags.
 *
 * @param string $string
 *   The string containing unicode to be replaced.
 *
 * @return string
 *   The modified string.
 */
function usasearch_api_replace_unicode_char($string) {
  $string = preg_replace('/\x{e000}/u', '<strong>', $string);
  $string = preg_replace('/\x{e001}/u', '</strong>', $string);

  return $string;
}

/**
 * Converts a UTC formatted time string to a UNIX timestamp.
 *
 * @param string $utc_time
 *   A UTC time string.
 *
 * @return int
 *   A unix timestamp.
 */
function usasearch_api_utc_to_unix_timestamp($utc_time) {

  // Return immediately if $utc_time is NULL, or it will default to epoch.
  if (!$utc_time) {
    return NULL;
  }

  $unix_timestamp = strtotime($utc_time . ' UTC');
  return $unix_timestamp;
}

/**
 * Generates a list of subsite links.
 *
 * @return array
 *   A renderable array of subsite links.
 */
function usasearch_api_affiliate_links() {
  $subsites = usasearch_affiliates_list();

  $params = drupal_get_query_parameters();
  $subsite_links = array();
  foreach ($subsites as $affiliate_id => $subsite_name) {
    $query = array_merge($params, array('affiliate' => check_plain($affiliate_id)));
    $options = array(
      'query' => $query,
      'attributes' => array(
        'class' => array(),
      ),
    );
    if (!empty($params['affiliate']) && $params['affiliate'] == $affiliate_id) {
      $options['attributes']['class'][] = 'current';
    }
    $subsite_links[$affiliate_id] = l(t('@subsite_name', array('@subsite_name' => $subsite_name)), current_path(), $options);
  }

  $markup = array(
    '#items' => $subsite_links,
    '#theme' => 'item_list',
    '#attributes' => array(
      'class' => 'usasearch-affiliates',
    ),
    '#attached' => array(
      'css' => array(drupal_get_path('module', 'usasearch_api') . '/usasearch_api.css'),
    ),
  );

  return $markup;
}
