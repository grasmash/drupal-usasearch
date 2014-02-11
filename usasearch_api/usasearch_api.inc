<?php

/**
 * @file
 * Contains non-hook implementations for USA Search API module.
 */


/**
 * @param string $keys
 *   The keyword string, passed by hook_search_execute().
 * @param array $conditions
 *   An associative array of conditions, passed by hook_search_execute().
 * @param stirng $module
 *   The name of the module performing the search.
 * @return array
 *   An array of formatted search results.
 */
function usasearch_api_get_results($keys, $conditions, $module) {

  // Set up vars.
  $dc = isset($conditions['dc']) ? $conditions['dc'] : '';
  $index = isset($conditions['index']) ? $conditions['index'] : 'web';
  $num_per_page = variable_get('usasearch_results_per_page', 10);

  // Drupal starts the page count at 0, whereas USA Search starts it at one.
  // We need to add one to the value before making the API request.
  $page = pager_find_page() + 1;

  // Query the API.
  $client = new UsaSearchClient();
  $response = $client->search($keys, $dc, $page, $index);

  // We temporarily cache the response so that it can be used to in a subsequent
  // hook_form_alter() implementation.
  ctools_include('object-cache');
  ctools_object_cache_set('search_response', 'usasearch_api', $response);

  $results = array();
  if (!empty($response['total'])) {
    module_load_include('inc', 'usasearch_api');

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
 * Converts a USA Search result to a Drupal search result for web index.
 *
 * @param $result
 * @param $module
 * @return array
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
 * @param $string
 *   The string containing unicode to be replaced.
 *
 * @return string
 *   The modified string.
 *
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
