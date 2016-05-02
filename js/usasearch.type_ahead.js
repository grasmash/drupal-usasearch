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

      var moduleConfig = drupalSettings.usasearch;

      if(moduleConfig.affiliate_name && typeof moduleConfig.affiliate_name !== 'undefined'
          && moduleConfig.type_ahead && typeof moduleConfig.type_ahead !== 'undefined' ) {
          //load the type ahead script
          window.usasearch_config = { siteHandle : moduleConfig.affiliate_name };
          var script = document.createElement("script");
          script.type = "text/javascript";
          script.src = "//search.usa.gov/javascripts/remote.loader.js";
          $('body').once().append(script);
      } 

    }
  };


})(jQuery, window);
