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
    '#description' => t('Please enter your affiliate site handle provided by <a href="@usasearch_affiliates_url">USASearch</a>, eg. "example.gov".', array('@usasearch_affiliates_url' => url('https://search.usa.gov/affiliates'))),
    '#required' => TRUE,
  );
  $form['usasearch_autocomplete'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable autocomplete'),
    '#default_value' => variable_get('usasearch_autocomplete', TRUE),
    '#description' => t('Check this box to load USASearch javascript for autocompletion.'),
  );
  $form['usasearch_action_domain'] = array(
    '#type' => 'textfield',
    '#title' => t('Search domain'),
    '#default_value' => variable_get('usasearch_action_domain', 'search.usa.gov'),
    '#size' => 30,
    '#maxlength' => 50,
    '#description' => t('You may enter a custom search domain, eg. "http://search.commerce.gov", or leave the default "http://search.usa.gov". This will change the search form action to submit search requests to the search domain entered. <strong>NOTE: Only change this if USASearch has configured this option for your search affiliate!</strong>'),
    '#required' => FALSE,
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

  return system_settings_form($form);
}

