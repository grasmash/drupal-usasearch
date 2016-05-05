<?php

namespace Drupal\usasearch;

use Drupal\Core\Logger\LoggerChannelFactoryInterface;
use GuzzleHttp\Exception\RequestException;
use Drupal\Core\Config\ConfigFactoryInterface;
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
   * Constructs a new instance.
   */
  public function __construct(ConfigFactoryInterface $config_factory, LoggerChannelFactoryInterface $logger_factory) {
    $this->configFactory = $config_factory;
    $this->loggerFactory = $logger_factory;
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
   */
  public function request($method, $uri, $request_options = array()) {

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
    $client = \Drupal::httpClient();
    try {
      $response = $client->request($method, $uri, $options);
      $this->loggerFactory->get('usasearch')
        ->notice('Updated DigitalGov Search index via %method request to %uri with options: %options. Got a %response_code response with body "%response_body".',
          array(
            '%method' => $method,
            '%uri' => $uri,
            '%options' => '<pre>' . Html::escape(print_r($options, TRUE)) . '</pre>',
            '%response_code' => $response->getStatusCode(),
            '%response_body' => $response->getBody(),
          ));
      drupal_set_message(t('Updated DigitalGov Search index'), 'status', FALSE);
      return TRUE;
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
