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
    '#description' => t('Please enter the handle for the <a href="@usasearch_affiliates_url">DigitalGov</a> Search site you want to send your content to, e.g., "dept-agency.gov"', array('@usasearch_affiliates_url' => url('https://search.usa.gov/affiliates'))),
    '#required' => TRUE,
  );
  $form['usasearch_drawer_handle'] = array(
    '#type' => 'textfield',
    '#title' => t('Drawer handle'),
    '#default_value' => variable_get('usasearch_drawer_handle', ''),
    '#size' => 30,
    '#maxlength' => 30,
    '#description' => t('Please enter the handle for the i14y Drawer you created in the above site, where your content will be indexed, e.g. "agency" (See https://search.usa.gov/sites/YOURSITEID/i14y_drawers)'),
    '#required' => TRUE,
  );
  $form['usasearch_api_password'] = array(
    '#type' => 'textfield',
    '#title' => t('i14y API Secret Token'),
    '#default_value' => variable_get('usasearch_api_password', ''),
    '#required' => TRUE,
    '#description' => t('Enter the Secret Token provided in your search site: go to https://search.usa.gov/sites/YOURSITEID/i14y_drawers and click Show next to the drawer.'),
  );
  $form['usasearch_autocomplete'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable autocomplete'),
    '#default_value' => variable_get('usasearch_autocomplete', TRUE),
    '#description' => t('Check this box to load the DigitalGov Search javascript for type-ahead search suggestions in your site\'s search box'),
  );
  $form['usasearch_action_domain'] = array(
    '#type' => 'textfield',
    '#title' => t('Search domain'),
    '#default_value' => variable_get('usasearch_action_domain', 'https://search.usa.gov'),
    '#size' => 30,
    '#maxlength' => 50,
    '#description' => t('<strong>Only change this field if you have completed set up for a CNAME with DigitalGov Search and in your agency\'s external DNS records!</strong > This will change the search form action to submit search requests to the search domain entered . If you would like a domain mask for your search results page, but have not yet requested a CNAME be set up for your site, send a request to search@support . digitalgov . gov .'),
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


