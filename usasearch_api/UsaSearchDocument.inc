<?php

/**
 * Class UsaSearchDocument.
 *
 * Objects of this class conform to USA Search's standard document model for
 * use with their content API.
 */
class UsaSearchDocument {
  protected $document_id;
  protected $title;
  protected $content;
  protected $path;
  protected $created;
  protected $description;
  protected $promote;
  protected $tags;

  /**
   *
   * @return mixed
   */
  public function getDocumentId() {
    return $this->document_id;
  }

  /**
   *
   * @param mixed $document_id
   */
  public function setDocumentId($document_id) {
    // @todo add validation for web-safe string.
    if (!preg_match('/[A-Za-z0-9_-]+/', $document_id)) {
      throwException("The document id property must be a web safe string, containing only alpanumeric characters and/or the following characters: '-, _'");
    }

    $this->document_id = $document_id;
  }

  /**
   *
   * @return mixed
   */
  public function getTitle() {
    return $this->title;
  }

  /**
   *
   * @param mixed $title
   */
  public function setTitle($title) {
    if (!is_string($title)) {
      throwException("The title property must be a string!");
    }

    $this->title = $title;
  }

  /**
   *
   * @return mixed
   */
  public function getPath() {
    return $this->path;
  }

  /**
   *
   * @param mixed $path
   */
  public function setPath($path) {

    // @todo Validate absolute URL. Relative?

    $this->path = $path;
  }

  /**
   *
   * @return mixed
   */
  public function getCreated() {
    return $this->created;
  }

  /**
   *
   * @param mixed $created
   */
  public function setCreated($created) {
    // Validate ISO format date.

    $this->created = $created;
  }

  /**
   *
   * @return mixed
   */
  public function getDescription() {
    return $this->description;
  }

  /**
   *
   * @param mixed $description
   */
  public function setDescription($description) {
    if (!is_string($description)) {
      throwException("The description property must be a string!");
    }

    $this->description = $description;
  }

  /**
   *
   * @return mixed
   */
  public function getPromote() {
    return $this->promote;
  }

  /**
   *
   * @param mixed $promote
   */
  public function setPromote($promote) {
    if (!is_bool($promote)) {
      throwException("The promote property must be a boolean value, either TRUE or FALSE.");
    }

    $this->promote = $promote;
  }

  /**
   *
   * @return mixed
   */
  public function getContent() {
    return $this->content;
  }

  /**
   *
   * @param mixed $content
   */
  public function setContent($content) {
    if (!is_string($content)) {
      throwException("The content property must be a string!");
    }

    $this->content = $content;
  }

  /**
   *
   * @return mixed
   */
  public function getTags() {
    return $this->tags;
  }

  /**
   *
   * @param mixed $tags
   */
  public function setTags($tag) {
    if (!is_string($tag)) {
      throwException("The tags property must be a string!");
    }
    if (isset($this->tags)) {
      $this->tags .= ', ' . $tag;
    }
    else {
      $this->tags = $tag;
    }
  }

  /**
   *
   * @return string
   *   Returns a JSON object representation of the document, compatible with
   *   USA Search's API.
   */
  public function json() {
    return get_object_vars($this);
  }
}

