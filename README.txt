
Contents of this file
---------------------

 * Overview
 * Installation
 * Requirements
 * Setup Tips
 * Features

Overview
--------

USASearch is an embeddable search engine that can
be used to search a site. An Affiliate profile and
associated Affiliate ID are required. Read more at
http://usasearch.howto.gov/

USA Search offers two ways of using their service:
 1. Via their hosted search platform.
 2. Via their API.

The USASearch module allows either of these search modes to be used. To use
the hosted search, enable the usasearch_hosted module. To use the API, enable
the usasearch_api module. You must enable ONE of these to use this module, but
you may not enable both.

#### Hosted Search - usasearch_hosted
When content is searched, the user will be redirected to the hosted search
solution. When they click on a result, they will be sent back to your site.

#### API Search - usasearch_api
When content is searched, a request is made to the USA Search API and the
reponse is rendered via the core Search module's theming functions. The
following submodules are available for the usasearch_api module:
* usasearch_docs
* usasearch_images
* usasearch_news
* usasearch_videonews

Installation
-----------

1. Place the usasearch directory in your sites/all/modules directory.
2. Enable the USASearch API or USASearch Hosted module at admin/modules.
3. Configure it at admin/config/search/usasearch by entering USASearch's unique
affiliate name for your affiliate.
4. Grant permission for one or more roles to
search using the built-in search module.

#### API Search - usasearch_api
If you've enabled usasearch_api, you must also enable one of the following
submodules:
* usasearch_docs
* usasearch_images
* usasearch_news
* usasearch_videonews

Then, you must visit the core search settings page at
/admin/config/search/settings and enable the USA Search modules
under the 'ACTIVE SEARCH MODULES' heading.


Requirements
------------

 * A valid Affiliate profile from USASearch.
 * Core Search module must be enabled.
 * (usasearch_api) composer_manager and Guzzle library.

Setup Tips
------------

#### API Search - usasearch_api
This module depends on composer_manager, which will manage the PHP library
dependencies. Namely, it will download Guzzle to the sites/all/vendor directory.

#### Configuration
To configure this module, you will need your USASearch's unique
Affiliate name.  Login at https://search.usa.gov/affiliates and
click the "View Current" link next to the affiliate you want to
use. In the URL of the page that loads, look for
"affiliate=example.gov". "example.gov" is the Affiliate name in
this example.

Features
--------

The module was designed to allow exporting of all the admin
configuration options via the Strongarm and Features modules.
