(function ($) {

/**
 * JS related to the tabs in the Panels tabs.
 */
Drupal.behaviors.apptabs = {
  attach: function (context) {
    var ids = ["tabs-1", "tabs-2", "tabs-3", "tabs-4"];
     
    for (var key in ids) {  
      $('#' + ids[key] +':not(.tabs-processed)', context)
        .addClass('tabs-processed')
        .tabs();
    } 
  }
};

})(jQuery);


