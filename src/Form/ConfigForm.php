<?php

/**
 * @file
 * Contains \Drupal\usasearch\Form\ConfigForm.
 */

namespace Drupal\usasearch\Form;

use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;

/**
 * Provides a form for administering usasearch settings.
 */
class ConfigForm extends ConfigFormBase {

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'usasearch_settings';
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, FormStateInterface $form_state) {
    $config = $this->config('usasearch.settings');

    $form['affiliate_name'] = [
      '#type' => 'textfield',
      '#title' => $this->t('Affilate Name'),
      '#size' => 30,
      '#maxlength' => 128,
      '#required' => TRUE,
      '#default_value' => $config->get('affiliate_name'),
      '#description' => $this->t('Please enter your affiliate name provided by <a href="http://search.digitalgov.gov/" target="_blank">DigitalGov</a>, eg. "fema".'),
    ];
    // @todo: make the i14y functionality optional?
    $form['drawer_handle'] = [
      '#type' => 'textfield',
      '#title' => $this->t('Drawer Handle'),
      '#size' => 30,
      '#maxlength' => 128,
      '#required' => TRUE,
      '#default_value' => $config->get('drawer_handle'),
      '#description' => $this->t('Please enter the i14y API "drawer handle". More information about <a href="http://search.digitalgov.gov/developer/i14y.html" target="_blank">drawers</a>'),
    ];
    $form['secret_token'] = [
      '#type' => 'textfield',
      '#title' => $this->t('i14y API Secret Token'),
      '#size' => 60,
      '#maxlength' => 128,
      '#required' => TRUE,
      '#default_value' => $config->get('secret_token'),
      '#description' => $this->t('To find your secret token, <a href="https://search.usa.gov/login" target="_blank">login to your Digital Search account</a>, navigate to the "i14y Drawers" tab, and click "show" next to the drawer.'),
    ];
    $form['autocomplete'] = [
      '#type' => 'checkbox',
      '#title' => $this->t('Enable autocomplete'),
      '#default_value' => $config->get('autocomplete'),
      '#description' => $this->t('Check this box to load javascript for the <a href="http://search.digitalgov.gov/developer/" target="_blank">Type-ahead API</a>.'),
    ];
    $form['action_domain'] = [
      '#type' => 'textfield',
      '#title' => $this->t('Search domain'),
      '#size' => 60,
      '#maxlength' => 128,
      '#required' => TRUE,
      '#default_value' => $config->get('action_domain'),
      '#description' => $this->t('You may enter a custom search domain, eg. "http://search.commerce.gov", or leave the default "http://search.usa.gov". This will change the search form action to submit search requests to the search domain entered. NOTE: Only change this if USASearch has configured this option for your search affiliate!'),
    ];
    $form['description_view_mode'] = [
      '#type' => 'select',
      '#title' => $this->t('Description View Mode'),
      '#options' => $this->getViewModes(),
      '#empty_option' => 'Teaser',
      '#empty_value' => 'node.teaser',
      '#required' => FALSE,
      '#default_value' => $config->get('description_view_mode'),
      '#description' => $this->t('Select a preferred <a href="/admin/structure/display-modes/view">view mode</a> to define description shown in search results. The view mode will need to be enabled and configured for each content type. If the view mode is not available for a content type "Teaser" will be used.'),
    ];

    return parent::buildForm($form, $form_state);
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {
    parent::submitForm($form, $form_state);
    $this->config('usasearch.settings')
      ->set('affiliate_name', $form_state->getValue('affiliate_name'))
      ->set('drawer_handle', $form_state->getValue('drawer_handle'))
      ->set('secret_token', $form_state->getValue('secret_token'))
      ->set('autocomplete', $form_state->getValue('autocomplete'))
      ->set('action_domain', $form_state->getValue('action_domain'))
      ->set('description_view_mode', $form_state->getValue('description_view_mode'))
      ->save();
  }

  /**
   * {@inheritdoc}
   */
  public function getEditableConfigNames() {
    return ['usasearch.settings'];
  }

  /**
   * Get an assoc array of all view modes for node entity.
   */
  public function getViewModes() {
    // @todo: entityManager is depreciated.
    $modes = array();
    $view_modes = \Drupal::entityManager()->getViewModes('node');
    foreach ($view_modes as $mode) {
      $modes[$mode['id']] = $mode['label'];
    }
    return $modes;
  }

}
