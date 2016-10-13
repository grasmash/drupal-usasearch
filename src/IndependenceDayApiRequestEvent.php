<?php

namespace Drupal\usasearch;

use Symfony\Component\EventDispatcher\Event;

/**
 * Wraps an i14y API  event for event listeners.
 */
class IndependenceDayApiRequestEvent extends Event {

  /**
   * Request method.
   *
   * @var string $method
   */
  protected $method;

  /**
   * Request uri.
   *
   * @var string $uri
   */
  protected $uri;

  /**
   * Request options.
   *
   * @var array $options
   */
  protected $options;

  /**
   * Constructs a i14y API event object.
   *
   * @param string $method
   *   The request method.
   * @param string $uri
   *   The request uri.
   * @param array $options
   *   HTTP request options.
   */
  public function __construct($method, $uri, $options) {
    $this->method = $method;
    $this->uri = $uri;
    $this->options = $options;
  }

  /**
   * Gets the request method.
   */
  public function getMethod() {
    return $this->method;
  }

  /**
   * Gets the request uri.
   */
  public function getUri() {
    return $this->uri;
  }

  /**
   * Gets the request options.
   */
  public function getOptions() {
    return $this->options;
  }

}
