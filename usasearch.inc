<?php

/**
 * @file
 * Contains utility functions for USA Search module.
 */

/**
 * Gets the search mode.
 *
 * @return string
 *   The search mode.
 */
function usasearch_get_mode() {
  $mode = variable_get('usasearch_mode', 'redirect');
  return $mode;
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
function usasearch_replace_unicode_char($string) {
  // This is awful but I haven't found a better way that works.
  $string = str_replace('', '<b>', $string);
  $string = str_replace('', '</b>', $string);

  return $string;
}
