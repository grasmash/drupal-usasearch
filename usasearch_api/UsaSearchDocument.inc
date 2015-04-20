<?php

/**
 * Class UsaSearchDocument.
 */
class UsaSearchDocument {
  protected $document_id;
  protected $title;
  protected $content;
  protected $path;
  protected $created;
  protected $description;
  protected $promote;

  /**
   * @return mixed
   */
  public function getDocumentId() {
    return $this->document_id;
  }

  /**
   * @param mixed $document_id
   */
  public function setDocumentId($document_id) {
    $this->document_id = $document_id;
  }

  /**
   * @return mixed
   */
  public function getTitle() {
    return $this->title;
  }

  /**
   * @param mixed $title
   */
  public function setTitle($title) {
    $this->title = $title;
  }

  /**
   * @return mixed
   */
  public function getPath() {
    return $this->path;
  }

  /**
   * @param mixed $path
   */
  public function setPath($path) {
    $this->path = $path;
  }

  /**
   * @return mixed
   */
  public function getCreated() {
    return $this->created;
  }

  /**
   * @param mixed $created
   */
  public function setCreated($created) {
    $this->created = $created;
  }

  /**
   * @return mixed
   */
  public function getDescription() {
    return $this->description;
  }

  /**
   * @param mixed $description
   */
  public function setDescription($description) {
    $this->description = $description;
  }

  /**
   * @return mixed
   */
  public function getPromote() {
    return $this->promote;
  }

  /**
   * @param mixed $promote
   */
  public function setPromote($promote) {
    $this->promote = $promote;
  }

  /**
   * @return mixed
   */
  public function getContent() {
    return $this->content;
  }

  /**
   * @param mixed $content
   */
  public function setContent($content) {
    $this->content = $content;
  }

  public function json() {
    return json_encode(get_object_vars($this));
  }
}
