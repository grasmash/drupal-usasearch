/**
 * @file
 * DigitalGov Search behaviors.
 */

(function ($, window) {

  "use strict";

  /**
   * Load the DigitalGov Search Type Ahead.
   *
   * @type {Drupal~behavior}
   */
  Drupal.behaviors.usasearchTypeAhead = {
    attach: function () {

      var config = drupalSettings.usasearch;

      if(config.affiliate_name && typeof config.affiliate_name !== 'undefined'
          && config.type_ahead && typeof config.type_ahead !== 'undefined' ) {
          //load the type ahead script
          var usasearch_config = { siteHanlde : config.affiliate_name };
          var script = document.createElement("script");
          script.type = "text/javascript";
          script.src = "//search.usa.gov/javascripts/remote.loader.js";
          $('body').once().append(script);
      } 

    }
  };


})(jQuery, window);
