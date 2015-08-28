<?php

/**
 * @file
 * Contains \Drupal\usasearch\Form\ConfigForm.
 */

namespace Drupal\usasearch\Form;

use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;

class ConfigForm extends ConfigFormBase {
  public function getFormId() {
    return 'usasearch_settings';
  }

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
      '#description' => $this->t('You may enter a custom search domain, eg. "http://usasearch.fema.gov", or leave the default "http://search.usa.gov/search". 
        This will change the search form action to submit search requests to the search domain entered.
        <em>NOTE: Only change this if DigitalGov has configured this option for your search affiliate</em>'),

    ];
    //TODO: enable "allowed affiliates" after figuring out how to wire up this setting
    $form['allowed_affiliates'] = [
      '#disabled' => TRUE,
      '#type' => 'textarea',
      '#title' => $this->t('Allowed Affliate IDs'),
      '#default_value' => $config->get('allowed_affiliates'),
      '#description' => $this->t('Optional. A pipe-delimited list of affiliate ids that may be used in the DigitalGov Search field, in the form of "affiliate_id|Title"'),
    ];

    return parent::buildForm($form, $form_state);
  }

  public function submitForm(array &$form, FormStateInterface $form_state) {
    parent::submitForm($form, $form_state);
    $config = $this->config('usasearch.settings')
      ->set('affiliate_name', $form_state->getValue('affiliate_name'))
      ->set('autocomplete', $form_state->getValue('autocomplete'))
      ->set('action_domain', $form_state->getValue('action_domain'))
      ->set('allowed_affiliates', $form_state->getValue('allowed_affiliates'))
      ->save();
  }

  public function getEditableConfigNames() {
    return ['usasearch.settings'];
  }
}
