<?php

/**
 * @file
 * Contains \Drupal\digitalgov_search\Plugin\Block\SearchBlock.
 */

namespace Drupal\digitalgov_search\Plugin\Block;

use Drupal\Core\Access\AccessResult;
use Drupal\Core\Session\AccountInterface;
use Drupal\Core\Block\BlockBase;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Provides a 'Search form' block.
 *
 * @Block(
 *   id = "digitalgov_search_search_form_block",
 *   admin_label = @Translation("DigitalGov Search form"),
 *   category = @Translation("Forms")
 * )
 */
class SearchBlock extends BlockBase {

  /**
   * {@inheritdoc}
   */
  protected function blockAccess(AccountInterface $account) {
    return AccessResult::allowedIfHasPermission($account, 'search content');
  }

  /**
   * {@inheritdoc}
   */
  public function build() {
    //TODO: look at the Cache API to see if I can get/set the block from cache
    return \Drupal::formBuilder()->getForm('Drupal\digitalgov_search\Form\SearchBlockForm');
  }

}
