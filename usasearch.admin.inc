<?php

/**
 * @file
 * Contains admin settings and callbacks for USA Search module.
 */

/**
 * System settings form for USASearch.
 */
function usasearch_admin() {
  $form = array();

  $form['usasearch_affiliate_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Affiliate name'),
    '#default_value' => variable_get('usasearch_affiliate_name', ''),
    '#size' => 30,
    '#maxlength' => 30,
    '#description' => t('Please enter your affiliate name provided by <a href="@usasearch_affiliates_url">USASearch</a>, eg. "example.gov".', array('@usasearch_affiliates_url' => url('https://search.usa.gov/affiliates'))),
    '#required' => TRUE,
  );
  $form['usasearch_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Search Mode'),
    '#default_value' => variable_get('usasearch_mode', 'api'),
    '#options' => drupal_map_assoc(array(
      'api',
      'redirect',
    )),
    '#description' => t('Choose whether you would like to redirect to USA Search when a search is performed, or use the API to display the content within Drupal.'),
  );
  $form['usasearch_action_domain'] = array(
    '#type' => 'textfield',
    '#title' => t('Search domain'),
    '#default_value' => variable_get('usasearch_action_domain', 'search.usa.gov'),
    '#size' => 30,
    '#maxlength' => 50,
    '#description' => t('You may enter a custom search domain, eg. "search.commerce.gov", or leave the default "search.usa.gov". This will change the search form action to submit search requests to the search domain entered. <strong>NOTE: Only change this if USASearch has configured this option for your search affiliate!</strong>'),
    '#required' => FALSE,
  );
  $form['usasearch_search_page'] = array(
    '#type' => 'checkbox',
    '#title' => t('Keep Drupal search page'),
    '#default_value' => variable_get('usasearch_search_page', 0),
    '#description' => t("To make best use of the USASearch module on your site the 'Search box' option should be enabled in the <a href='@usasearch_theme_settings'>global theme settings</a> or in your default theme settings or the search block placed on your site. Check this option if you want to keep the Drupal search page instead of redirecting this to USASearch. If you do neither of these the USASearch module won't be of much use. <strong>NOTE: You must clear your menu cache if you enable this setting.</strong>", array('@usasearch_theme_settings' => url('admin/build/themes/settings'))),
    '#required' => FALSE,
  );

  return system_settings_form($form);
}
