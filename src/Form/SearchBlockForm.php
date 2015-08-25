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
use Drupal\search\SearchPageRepositoryInterface;
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

    //Set the action from module settings
    //$config = \Drupal::config('usasearch.settings');
    $action = $this->config('usasearch.settings')->get('action_domain');
    $use_type_ahead = $this->config('usasearch.settings')->get('autocomplete');

    $form['#action'] = $action;
    $form['#token'] = FALSE;
    $form['#method'] = 'get';
    //turn off default autocomplete if using the Type Ahead API
    $form['#autocomplete'] = $use_type_ahead ? 'off' : 'on';
    $form['query'] = array(
      '#type' => 'search',
      '#title' => $this->t('Search'),
      '#title_display' => 'invisible',
      '#size' => 15,
      '#default_value' => '',
      '#attributes' => array(
          'title' => $this->t('Enter the terms you wish to search for.'),
          'placeholder' => $this->t('Search'),
        ),
    );

    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => $this->t('Search'),
      // Prevent op from showing up in the query string.
      '#name' => '',
    );

    // SearchPageRepository::getDefaultSearchPage() depends on search.settings.
    //$this->renderer->addCacheableDependency($form, $this->configFactory->get('search.settings'));

    dpm($form);

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {
    // This form submits to the search page, so processing happens there.
  }

}
