<?php

/**
 * @file
 * Contains the client class for communicating with the USA Search API.
 */

use Guzzle\Http\Client;

class UsaSearchClient extends Client {

  protected $affiliate;
  protected $apiUrl;
  protected $password;

  /**
   * Constructor.
   */
  public function __construct($affiliate, $password) {
    $this->apiUrl = 'http://173.203.40.169:8081';
    parent::__construct($this->apiUrl);

    // Set all requests to include the 'affiliate' parameter.
    $this->affiliate = $affiliate;
    $this->password = $password;
    $this->setDefaultOption('auth', array($affiliate, $password));
    $this->setDefaultOption('timeout', 5);
    $this->setDefaultOption('connect_timeout', 5);
    $this->setDefaultOption('headers', array('Content-Type' => 'application/json'));
  }

  /**
   * Creates a document record in USASearch's index.
   *
   * @param \UsaSearchDocument $document
   *   The document to be created.
   *
   * @return array
   *   The JSON response from USASearch.
   */
  public function createDocument(UsaSearchDocument $document) {
    $request = $this->post('api/v1/documents');
    $request->setBody($document->json());
    $response = $request->send();
    $data = $response->json();

    return $data;
  }

  /**
   * Updates a document record in USASearch's index.
   *
   * @param \UsaSearchDocument $document
   *   The document to be updated.
   *
   * @return array
   *   The JSON response from USASearch.
   */
  public function updateDocument(UsaSearchDocument $document) {
    $request = $this->put('api/v1/documents/' . $document->getDocumentId());
    $request->setBody($document->json());
    $response = $request->send();
    $data = $response->json();

    return $data;
  }

  /**
   * Deletes a document record in USASearch's index.
   *
   * @param \UsaSearchDocument $document
   *   The document to be deleted.
   *
   * @return array
   *   The JSON response from USASearch.
   */
  public function deleteDocument(UsaSearchDocument $document) {
    $request = $this->delete('api/v1/documents/' . $document->getDocumentId());
    $request->setBody($document->json());
    $response = $request->send();
    $data = $response->json();

    return $data;
  }
}
