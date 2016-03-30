<?php

/**
 * @file
 * Contains \Drupal\usajobs\UsaSearchDocument.
 */

namespace Drupal\usasearch;

use Drupal\Core\Url;
use Drupal\node\NodeInterface;
use Drupal\Core\Session\AnonymousUserSession;
use Drupal\taxonomy\Entity\Term;

/**
 * Provides a document in the format to be used as an i14y document.
 */
class UsaSearchDocument {

  /**
   * A unique document ID.
   */
  public $document_id;
  /**
   * Document title.
   */
  public $title;
  /**
   * Document description.
   */
  public $description;
  /**
   * Document content.
   */
  public $content;
  /**
   * Document link URL.
   */
  public $path;
  /**
   * When the document was created (such as ‘2013-02-27T10:00:00Z’).
   */
  public $created;
  /**
   * When the document was updated (such as ‘2013-02-27T10:00:00Z’).
   */
  public $changed;
  /**
   * Two letter language locale.
   */
  public $language;
  /**
   * Whether to promote the document in the relevance ranking.
   */
  public $promote = FALSE;
  /**
   * Comma-separated list of case-insentitive tags.
   */
  public $tags;

  private $node = NULL;
  private $status = 0;

  /**
   * Constructs a new UsaSearchDocument.
   *
   * @param \Drupal\node\NodeInterface $node
   *   A node object to convert.
   * @param string $force
   *   Force document creation, ignoring node access.
   */
  public function __construct(NodeInterface $node, $force = FALSE) {
    $this->node = $node;
    $this->status = $this->node->isPublished();
    if (($this->status && $this->node->access(new AnonymousUserSession())) || $force == TRUE) {
      $this->document_id = $this->node->id();
      $this->title = $this->node->getTitle();
      $this->path = Url::fromUri('entity:node/' . $this->node->id(), array('absolute' => TRUE))->toString();
      $this->created = \Drupal::service('date.formatter')->format($this->node->getCreatedTime(), 'custom', 'c');
      $this->language = $this->node->language()->getId();
      // Get the view mode to be used for the description.
      $config = \Drupal::config('usasearch.settings');
      $description_view_mode = $config->get('description_view_mode');
      // @todo: replace entityManager (depriciated).
      $view = \Drupal::entityManager()->getViewBuilder('node')->view($this->node, $description_view_mode);
      $this->description = \Drupal::service('renderer')->render($view);
      $view = \Drupal::entityManager()->getViewBuilder('node')->view($this->node, 'full');
      $this->content = \Drupal::service('renderer')->render($view);
      $this->tags = $this->getTerms();
      $this->changed = \Drupal::service('date.formatter')->format($this->node->getChangedTime(), 'custom', 'c');
      $this->promote = $this->node->isPromoted() ? TRUE : FALSE;
    }
  }

  /**
   * Get a list of Taxonomy Terms from the given Node.
   */
  public function getTerms() {
    $tids = $terms = array();
    if ($this->node) {
      $field_definitions = $this->node->getFieldDefinitions();
      foreach ($field_definitions as $field_definition) {
        if ($field_definition->getType() == 'entity_reference' && $field_definition->getSetting('target_type') == 'taxonomy_term') {
          $field_name = $field_definition->getName();
          $field_values = $this->node->get($field_name)->getValue();
          foreach ($field_values as $value) {
            if (isset($value['target_id'])) {
              $tids[] .= $value['target_id'];
            }
          }
        }
      }
      $taxonomy_terms = Term::loadMultiple($tids);
      foreach ($taxonomy_terms as $term) {
        // @todo: make terms keyed by vocabulary (bundle)?
        $terms[] .= $term->getName();
      }
    }
    // Return comma-separated tags.
    return implode(',', array_unique($terms));
  }

  /**
   * Check required fields for a valid UsaSearchDocument.
   */
  public function hasRequiredFields() {
    return ($this->document_id && $this->title && $this->path && $this->created) ? TRUE : FALSE;
  }

  /**
   * Return document status.
   */
  public function getStatus() {
    return $this->status;
  }

  /**
   * Return an array of public properties.
   */
  public function getRawData() {
    $public_properties = create_function('$obj', 'return get_object_vars($obj);');
    return $public_properties($this);
  }

  /**
   * Return document as JSON string.
   */
  public function getJson() {
    $json = array_filter($this->getRawData(), 'strlen');
    return json_encode($json);
  }

  /**
   * Return document as JSON string.
   */
  public function toString() {
    return $this->getJson();
  }

}
