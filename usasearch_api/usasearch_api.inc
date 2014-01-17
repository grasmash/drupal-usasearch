<?php

/**
 * @file
 * Contains non-hook implementations for USA Search API module.
 */

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
        'width' => $logo_info['height'],
        'height' => $logo_info['width'],
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
  // This is awful but I haven't found a better way that works.
  $string = str_replace('', '<b>', $string);
  $string = str_replace('', '</b>', $string);

  return $string;
}
