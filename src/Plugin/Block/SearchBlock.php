<?php

/**
 * @file
 * Contains \Drupal\usasearch\Plugin\Block\SearchBlock.
 */

namespace Drupal\usasearch\Plugin\Block;

use Drupal\Core\Access\AccessResult;
use Drupal\Core\Session\AccountInterface;
use Drupal\Core\Block\BlockBase;

/**
 * Provides a 'Search form' block.
 *
 * @Block(
 *   id = "usasearch_search_form_block",
 *   admin_label = @Translation("USA Search Form"),
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

    // Get the search form.
    $block = \Drupal::formBuilder()->getForm('Drupal\usasearch\Form\SearchBlockForm');
    // Add the block's config to drupalSettings.
    $config = \Drupal::config('usasearch.settings');
    $affiliate_name = $config->get('affiliate_name');
    $use_type_ahead = $config->get('autocomplete');
    $block['#attached']['drupalSettings']['usasearch']['type_ahead'] = $use_type_ahead ? TRUE : FALSE;
    $block['#attached']['drupalSettings']['usasearch']['affiliate_name'] = $affiliate_name ? $affiliate_name : '';
    if ($use_type_ahead) {
      // Add the type_ahead js library.
      $block['#attached']['library'][] = 'usasearch/type_ahead';
    }

    return $block;
  }

}
