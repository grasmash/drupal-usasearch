<?php

namespace Drupal\usasearch;

/**
 * Defines events for the IndependenceDayApi Service.
 */
final class IndependenceDayApiEvents {

  /**
   * Name of the event fired when making a request to the i14y API.
   *
   * This event allows modules to perform an action whenever a request
   * is made to the i14y API. The event listener method receives a
   * \Drupal\usasearch\IndependenceDayApiRequestEvent instance.
   *
   * @var string
   */
  const REQUEST = 'usasearch.request';

}
