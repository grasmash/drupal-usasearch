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
    '#title' => t('Affiliate site handle'),
    '#default_value' => variable_get('usasearch_affiliate_name', ''),
    '#size' => 30,
    '#maxlength' => 30,
    '#description' => t('Please enter your affiliate site handle provided by <a href="@usasearch_affiliates_url">USASearch</a>, eg. "example.gov".', array('@usasearch_affiliates_url' => url('https://search.usa.gov/affiliates'))),
    '#required' => TRUE,
  );
  $form['usasearch_drawer_handle'] = array(
    '#type' => 'textfield',
    '#title' => t('Drawer handle'),
    '#default_value' => variable_get('usasearch_drawer_handle', ''),
    '#size' => 30,
    '#maxlength' => 30,
    '#description' => t('Please enter the i14y drawer handle provided by <a href="@usasearch_affiliates_url">USASearch</a>, eg. "mydrawer".'),
    '#required' => TRUE,
  );
  $form['usasearch_api_password'] = array(
    '#type' => 'textfield',
    '#title' => t('i14y API Secret Token'),
    '#default_value' => variable_get('usasearch_api_password', ''),
    '#required' => TRUE,
    '#description' => t('To find this, login to your Digital Search account, navigate to the "i14y Drawers" tab, and click "show" next to the drawer.'),
  );
  $form['usasearch_autocomplete'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable autocomplete'),
    '#default_value' => variable_get('usasearch_autocomplete', TRUE),
    '#description' => t('Check this box to load <a href="@usasearch_affiliates_url">USASearch</a> javascript for autocompletion.'),
  );
  $form['usasearch_action_domain'] = array(
    '#type' => 'textfield',
    '#title' => t('Search domain'),
    '#default_value' => variable_get('usasearch_action_domain', 'https://search.usa.gov'),
    '#size' => 30,
    '#maxlength' => 50,
    '#description' => t('You may enter a custom search domain, eg. "https://search.commerce.gov", or leave the default "https://search.usa.gov". This will change the search form action to submit search requests to the search domain entered. <strong>NOTE: Only change this if USASearch has configured this option for your search affiliate!</strong>'),
    '#required' => FALSE,
  );
  $form['usasearch_view_modes'] = array(
    '#type' => 'select',
    '#title' => t('Description view mode'),
    '#options' => usasearch_entity_view_modes('node'),
    '#default_value' => 'teaser',
    '#description' => t('Select a preferred view mode to define description shown in search results. The view mode will need to be enabled and configured for each content type. <strong>If the view mode is not available for a content type "Teaser" will be used.</strong>'),
  );
  return system_settings_form($form);
}

/**
 * Get entity view modes.
 *
 * @param $entity_type
 *   The name of the entity type.
 */
function usasearch_entity_view_modes($entity_type = NULL) {
  if (!empty($entity_type)) {
    switch ($entity_type) {
      // For taxonomy terms the base table and the entity type are different

      case 'taxonomy_term_data':
        $entity_info = entity_get_info('taxonomy_term');
        break;

      default:
        $entity_info = entity_get_info($entity_type);
        break;
    }
    foreach ($entity_info['view modes'] as $key => $view_mode) {
      $view_mode_options[$key] = $view_mode['label'];
    }
    return $view_mode_options;
  }
}


