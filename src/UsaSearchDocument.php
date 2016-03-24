<?php

/**
 * @file
 * Contains \Drupal\usajobs\UsaSearchDocument.
 */

namespace Drupal\usasearch;

use Drupal\Core\Url;
use Drupal\node\NodeInterface;
use Drupal\Core\Session\AnonymousUserSession;
use Drupal\Core\Entity;
use Drupal\taxonomy\Entity\Term;

class UsaSearchDocument {

  /**
   * A unique document ID
   */
  public $document_id;
  /**
   * Document title
   */
  public $title;
  /**
   * Document description
   */
  public $description;
  /**
   * Document content
   */
  public $content;
  /**
   * Document link URL
   */
  public $path;
  /**
   * When the document was created (such as ‘2013-02-27T10:00:00Z’)
   */
  public $created;
  /**
   * When the document was updated (such as ‘2013-02-27T10:00:00Z’)
   */
  public $changed;
  /**
   * two letter language locale
   */
  public $language;
  /**
   * Whether to promote the document in the relevance ranking
   */
  public $promote = false;
  /**
   * Comma-separated list of case-insentitive tags
   */
  public $tags;

  private $node = null;
  private $status = 0;

  /**
   *
   * @param object $node
   *   A node object to convert.
   * @param string $force
   *   Force document creation, ignoring node access.
   */
  function __construct(NodeInterface $node, $force = false) {
    $this->node = $node;
    $this->status = $this->node->isPublished();
    if( ($this->status && $this->node->access(new AnonymousUserSession())) || $force == true ){
      $this->document_id = $this->node->id();
      $this->title = $this->node->getTitle();
      $this->path = Url::fromUri('entity:node/' . $this->node->id(), $options = array('absolute' => true))->toString();
      $this->created = \Drupal::service('date.formatter')->format($this->node->getCreatedTime(), $type = 'custom', $format = 'c');
      $this->language = $this->node->language()->getId();
      //TODO: get the view to use from module config
      //TODO: entityManager is depriciated
      $view = \Drupal::entityManager()->getViewBuilder('node')->view($this->node, 'teaser');
      $this->description = \Drupal::service('renderer')->render($view);
      $view = \Drupal::entityManager()->getViewBuilder('node')->view($this->node, 'full');
      $this->content = \Drupal::service('renderer')->render($view);
      $this->tags = $this->getTerms();
      $this->changed = \Drupal::service('date.formatter')->format($this->node->getChangedTime(), $type = 'custom', $format = 'c');
      $this->promote = $this->node->isPromoted() ? true : false;
    }
  }

  public function getTerms() {
    $tids = $terms = array();
    if ( $this->node ){
      $fieldDefinitions = $this->node->getFieldDefinitions();
      foreach( $fieldDefinitions as $fieldDefinition ){
        if( $fieldDefinition->getType() == 'entity_reference' && $fieldDefinition->getSetting('target_type') == 'taxonomy_term' ){
          $field_name= $fieldDefinition->getName();
          $field_values = $this->node->get($field_name)->getValue();
          foreach( $field_values as $value ){
            if( isset($value['target_id']) ){
              $tids[] .= $value['target_id'];
            }
          }
        }
      }
      $taxonomy_terms = Term::loadMultiple($tids);
      foreach ($taxonomy_terms as $term){
        //TODO: make terms keyed by vocabulary (bundle)??
        $terms[] .= $term->getName();
      }
    }
    //return comma-separated tags
    return implode(',', array_unique($terms));
  }

  public function hasRequiredFields(){
    return ($this->document_id && $this->title && $this->path && $this->created) ? true : false;
  }

  public function getStatus(){
    return $this->status;
  }

  /**
   * return an array of public properties
   */
  public function getRawData(){
    $publicProperties = create_function('$obj', 'return get_object_vars($obj);');
    return $publicProperties($this);
  }

  public function getJson() {
    $json = array_filter( $this->getRawData(), 'strlen');
    return json_encode($json);
  }

  public function toString(){
    return $this->getJson();
  }


}