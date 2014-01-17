<?php

/**
 * @file
 * Contains the client class for communicating with the USA Search API.
 */

use Guzzle\Http\Client;

class UsaSearchClient extends Client {
  protected $affiliate;
  protected $api_url;

  public function __construct() {
    $this->api_url = 'http://search.usa.gov/api';
    parent::__construct($this->api_url);

    // Set all requests to include the 'affiliate' parameter.
    $this->affiliate = variable_get('usasearch_affiliate_name', '');
    $this->setDefaultOption('query', array('affiliate' => $this->affiliate));
  }

  /**
   * Sends a search request.
   *
   * @param $query
   *   The string of keywords.
   *
   * @param string $dc
   *   The document collection.
   *
   * @param int $offset
   *   The resultset offset, used for pagination.
   *
   * @return array
   *   The response, including results.
   */
  public function search($query, $dc = '', $offset = 0) {
    $request = $this->get('search.json', array(), array(
      'query' => array(
        'query' => $query,
        'page' => $offset,
        'dc' => $dc,
      ),
    ));

    $response = $request->send();
    $data = $response->json();

    return $data;
  }

}
