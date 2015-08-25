<?php

/**
 * @file
 * Contains \Drupal\usasearch\Plugin\Block\SearchBlock.
 */

namespace Drupal\usasearch\Plugin\Block;

use Drupal\Core\Access\AccessResult;
use Drupal\Core\Session\AccountInterface;
use Drupal\Core\Block\BlockBase;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Provides a 'Search form' block.
 *
 * @Block(
 *   id = "usasearch_search_form_block",
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
    return \Drupal::formBuilder()->getForm('Drupal\usasearch\Form\SearchBlockForm');
  }

}
