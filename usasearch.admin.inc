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
    '#title' => t('Site handle'),
    '#default_value' => variable_get('usasearch_affiliate_name', ''),
    '#size' => 30,
    '#maxlength' => 30,
    '#description' => t('Please enter the handle for the <a href="@usasearch_affiliates_url">DigitalGov</a> Search site you want to send your content to, e.g., "dept-agency.gov"', array('@usasearch_affiliates_url' => url('https://search.usa.gov/affiliates'))),
    '#required' => TRUE,
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
    '#default_value' => variable_get('usasearch_action_domain', 'search.usa.gov'),
    '#size' => 30,
    '#maxlength' => 50,
    '#description' => t('<strong>Only change this field if you have completed set up for a CNAME with DigitalGov Search and in your agency\'s external DNS records!</strong> This will change the search form action to submit search requests to the search domain entered. If you would like a domain mask for your search results page, but have not yet requested a CNAME be set up for your site, send a request to search@support.digitalgov.gov.'),
    '#required' => FALSE,
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

  return system_settings_form($form);
}

