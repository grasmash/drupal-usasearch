<?php

namespace Drupal\digitalgov_search;

use Drupal\Core\State\StateInterface;

class DigitalgovSearchTracker {

  /**
   * @var \Drupal\Core\State\StateInterface
   */
  protected $state;

  public function __construct(StateInterface $state) {
    $this->state = $state;
  }

  public function addHug($target_name) {
    $this->state->set('DigitalgovSearch.last_recipient', $target_name);
    return $this;
  }

  public function getLastRecipient() {
    return $this->state->get('DigitalgovSearch.last_recipient');
  }
}
