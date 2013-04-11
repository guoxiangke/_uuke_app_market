(function ($) {

/**
 * JS related to the tabs in the Panels tabs.
 */
Drupal.behaviors.market = {
  attach: function (context) {
    $("#tabs").tabs();
	$('jcarousel-roundabout').roundabout({
		 shape: 'figure8',
		 autoplay: true,
		 autoplayDuration: 5000,
	});
  }
};

})(jQuery);


