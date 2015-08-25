<?php

namespace Drupal\usasearch;

use Drupal\Core\State\StateInterface;

class UsasearchTracker {

  /**
   * @var \Drupal\Core\State\StateInterface
   */
  protected $state;

  public function __construct(StateInterface $state) {
    $this->state = $state;
  }

  public function addHug($target_name) {
    $this->state->set('usasearch.last_recipient', $target_name);
    return $this;
  }

  public function getLastRecipient() {
    return $this->state->get('usasearch.last_recipient');
  }
}
