<?php

/**
 * @file
 * Contains the client class for communicating with the USA Search API.
 */

use Guzzle\Http\Client;

class UsaSearchClient extends Client {

  protected $affiliate;
  protected $apiUrl;

  /**
   * Constructor.
   */
  public function __construct($affiliate) {
    $this->apiUrl = 'http://search.usa.gov/api';
    parent::__construct($this->apiUrl);

    // Set all requests to include the 'affiliate' parameter.
    $this->affiliate = $affiliate;
    $this->setDefaultOption('query', array('affiliate' => $this->affiliate));
  }

  /**
   * Sends a search request.
   *
   * @param string $query
   *   The string of keywords.
   *
   * @param string $dc
   *   (optional) The document collection. Defaults to none.
   *
   * @param int $page
   *   (optional). The search result page offset, used for pagination.
   *   Defaults to 1.
   *
   * @param string $index
   *   (optional) The USASearch index to search. Default to 'web'.
   *
   * @return array
   *   The response, including results.
   */
  public function search($query, $dc = '', $page = 1, $index = 'web') {

    // @todo Add ability to include 'sort_by' parameter.
    $request = $this->get('search.json', array(), array(
      'query' => array(
        'query' => $query,
        'page' => $page,
        'dc' => $dc,
        'index' => $index,
      ),
      'timeout' => 5,
      'connect_timeout' => 5,
    ));

    $response = $request->send();
    $data = $response->json();

    return $data;
  }
}
