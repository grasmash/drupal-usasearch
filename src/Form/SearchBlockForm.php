<?php

/**
 * @file
 * Contains \Drupal\usasearch\Form\SearchBlockForm.
 */

namespace Drupal\usasearch\Form;

use Drupal\Core\Config\ConfigFactoryInterface;
use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Render\RendererInterface;
//use Drupal\search\SearchPageRepositoryInterface;
use Drupal\Core\Url;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Builds the search form for the search block.
 */
class SearchBlockForm extends FormBase {


  /**
   * The config factory.
   *
   * @var \Drupal\Core\Config\ConfigFactoryInterface
   */
  protected $configFactory;

  /**
   * The renderer.
   *
   * @var \Drupal\Core\Render\RendererInterface
   */
  protected $renderer;

  /**
   * Constructs a new SearchBlockForm.
   *
   * @param \Drupal\Core\Config\ConfigFactoryInterface $config_factory
   *   The config factory.
   * @param \Drupal\Core\Render\RendererInterface
   *   The renderer.
   */
  public function __construct(ConfigFactoryInterface $config_factory, RendererInterface $renderer) {
    $this->configFactory = $config_factory;
    $this->renderer = $renderer;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container) {
    return new static(
      $container->get('config.factory'),
      $container->get('renderer')
    );
  }

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'usasearch_search_block_form';
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, FormStateInterface $form_state) {

    //TODO: what needs to be done if the config uses multiple affiliates in the "allowed_affiliates" setting???

    $actionUrl = Url::fromUri($this->config('usasearch.settings')->get('action_domain'), $options = array('absolute' => TRUE))->toString();
    $affiliate_name = $this->config('usasearch.settings')->get('affiliate_name');
    $use_type_ahead = $this->config('usasearch.settings')->get('autocomplete');

    $form['#action'] = $actionUrl;
    $form['#method'] = 'GET';

    //the search field
    $form['query'] = array(
      '#type' => 'search',
      '#title' => $this->t('Search'),
      '#title_display' => 'invisible',
      '#default_value' => '',
      '#attributes' => array(
          'id' => 'query',
          'title' => $this->t('Enter the terms you wish to search for.'),
          'placeholder' => $this->t('Search'),
          'class' => array('usagov-search-autocomplete'),
          'autocomplete' => $use_type_ahead ? 'off' : 'on',
          'aria-autocomplete' => 'list',
          'aria-haspopup' => TRUE,
        ),
    );

    //the affiliate name
    $form['affiliate']['#type'] = 'hidden';
    if ($affiliate_name && empty($form['affiliate']['#value'])) {
      $form['affiliate']['#value'] = $affiliate_name;
    }

    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => $this->t('Search'),
      // Prevent op from showing up in the query string.
      '#name' => '',
    );

    // SearchPageRepository::getDefaultSearchPage() depends on search.settings.
    //$this->renderer->addCacheableDependency($form, $this->configFactory->get('search.settings'));

    //dpm($form);

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {
    // This form submits to the search page, so processing happens there.
  }

}
