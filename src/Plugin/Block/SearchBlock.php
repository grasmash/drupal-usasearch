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

    //get the search form
    $block = \Drupal::formBuilder()->getForm('Drupal\usasearch\Form\SearchBlockForm');
    //add the block's config to drupalSettings
    $config = \Drupal::config('usasearch.settings');
    $affiliate_name = $config->get('affiliate_name');
    $use_type_ahead = $config->get('autocomplete');
    $block['#attached']['drupalSettings']['usasearch']['type_ahead'] = $use_type_ahead ? TRUE : FALSE;
    $block['#attached']['drupalSettings']['usasearch']['affiliate_name'] = $affiliate_name ? $affiliate_name : '';
    if ($use_type_ahead) {
      //add the type_ahead js library
      $block['#attached']['library'][] = 'usasearch/type_ahead';
    }

    return $block;
  }

}
