<?php

namespace Drupal\usasearch;

use Drupal\Core\Logger\LoggerChannelFactoryInterface;
use Drupal\Core\Extension\ModuleHandlerInterface;
use Drupal\Core\Config\ConfigFactoryInterface;
use Drupal\Component\EventDispatcher\ContainerAwareEventDispatcher;
use GuzzleHttp\Exception\RequestException;
use Drupal\node\Entity\Node;
use Drupal\Component\Utility\Html;

/**
 * UsaSearch i14y API service.
 */
class IndependenceDayApi {

  /**
   * Config Factory Service Object.
   *
   * @var \Drupal\Core\Config\ConfigFactoryInterface
   */
  protected $configFactory;

  /**
   * Logger Factory Service Object.
   *
   * @var \Drupal\Core\Logger\LoggerChannelFactoryInterface
   */
  protected $loggerFactory;

  /**
   * Module handler service.
   *
   * @var \Drupal\Core\Extension\ModuleHandlerInterface
   */
  protected $moduleHandler;

  /**
   * An event dispatcher instance to use for configuration events.
   *
   * @var \Symfony\Component\EventDispatcher\EventDispatcherInterface
   */
  protected $eventDispatcher;

  /**
   * Constructs a new IndependenceDayApi instance.
   *
   * @param \Drupal\Core\Config\ConfigFactoryInterface $config_factory
   *   The config factory.
   * @param \Drupal\Core\Logger\LoggerChannelFactoryInterface $logger_factory
   *   The logger factory.
   * @param \Drupal\Core\Extension\ModuleHandlerInterface $module_handler
   *   The module handler.
   */
  public function __construct(ConfigFactoryInterface $config_factory, LoggerChannelFactoryInterface $logger_factory, ModuleHandlerInterface $module_handler, ContainerAwareEventDispatcher $event_dispatcher) {
    $this->configFactory = $config_factory;
    $this->loggerFactory = $logger_factory;
    $this->moduleHandler = $module_handler;
    $this->eventDispatcher = $event_dispatcher;
  }

  /**
   * Check if i14y API is enabled.
   *
   * @return bool
   *   Return enabled.
   */
  public function apiEnabled() {
    return $this->configFactory->get('usasearch.settings')->get('i14y_enabled');
  }

  /**
   * Check if i14y API is enabled for a particular node.
   *
   * @param \Drupal\node\Entity\Node $node
   *   The Node.
   *
   * @return bool
   *   Return answer.
   */
  public function nodeEnabled(Node $node) {
    if ($this->apiEnabled()) {
      // Check that the current node type is one of the configured types.
      if (in_array($node->getType(), $this->getEnabledContentTypes())) {
        return TRUE;
      }
    }
    return FALSE;
  }

  /**
   * Create UsaSearchDocument and convert to JSON.
   *
   * @param \Drupal\node\Entity\Node $node
   *   The Node.
   *
   * @return string
   *   Return a json string.
   */
  public function createDocument(Node $node) {
    $document = new UsaSearchDocument($node);
    $rawData = $document->getRawData();
    // Let modules alter the document.
    $this->moduleHandler->alter('usasearch_document', $rawData);
    return $document->setRawData($rawData);
  }

  /**
   * Get the enabled content types.
   *
   * @return array
   *   Enabled content type machine names.
   */
  public function getEnabledContentTypes() {
    $content_types = $this->configFactory->get('usasearch.settings')->get('content_types');
    $enabled_content_types = array();
    foreach ($content_types as $type => $label) {
      if ($label) {
        $enabled_content_types[] .= $type;
      }
    }
    return $enabled_content_types;
  }

  /**
   * Make an API request to the i14y API.
   *
   * @param string $method
   *   The HTTP method to be used.
   * @param string $uri
   *   The URI resource to which the HTTP request will be made.
   * @param array $request_options
   *   An array of options passed directly to the request.
   *
   * @see http://gsa.github.io/slate
   * @see http://guzzle.readthedocs.org/en/5.3/quickstart.html
   *
   * @return bool
   *   Return if request successfully
   */
  public function request($method, $uri, $request_options = array()) {
    $method = strtolower($method);
    $config = $this->configFactory->get('usasearch.settings');
    $options = array(
      'base_uri' => 'https://i14y.usa.gov',
      'timeout' => 5,
      'connect_timeout' => 5,
      'auth' => array(
        $config->get('drawer_handle'),
        $config->get('secret_token'),
      ),
      'headers' => array(
        'Content-Type' => 'application/json',
        'Accept' => 'application/json',
      ),
    );
    if (!empty($request_options)) {
      $options = array_merge($options, $request_options);
    }
    // Make request.
    $client = \Drupal::httpClient();
    try {
      $response = $client->request($method, $uri, $options);
      if ($response) {
        // Register event.
        $event = new IndependenceDayApiRequestEvent($method, $uri, $options);
        $this->eventDispatcher->dispatch(IndependenceDayApiEvents::REQUEST, $event);

        $this->loggerFactory->get('usasearch')
          ->notice('Updated DigitalGov Search index via %method request to %uri with options: %options. Got a %response_code response.',
            array(
              '%method' => $method,
              '%uri' => $uri,
              '%options' => '<pre>' . Html::escape(print_r($options, TRUE)) . '</pre>',
              '%response_code' => $response->getStatusCode(),
            ));
        drupal_set_message(t('Updated DigitalGov Search index'), 'status', FALSE);
        return TRUE;
      }
    }
    catch (RequestException $exception) {
      $this->loggerFactory->get('usasearch')
        ->error('Error updating DigitalGov Search index, Code: %code, Message: %message, Body: %body',
          array(
            '%code' => $exception->getCode(),
            '%message' => $exception->getMessage(),
            '%body' => '<pre>' . Html::escape($exception->getResponse()
                ->getBody()) . '</pre>',
          ));
      return FALSE;
    }

  }

}
