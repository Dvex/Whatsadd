/**
 * @file
 *
 * Javascript specific to iframe tabs.
 */

/**
 * Enable canvas page specific javascript on this page.
 */
Drupal.behaviors.fb_tab = function(context) {
  // Resize if body class includes fb_canvas-resizable.
  $('body.fb_tab-resizable:not(.fb_tab-processed)').each(function () {
    $(this).addClass('fb_tab-processed');
    jQuery(document).bind('fb_init', FB_Tab.setAutoResize);
  });
};

FB_Tab = function(){};

/**
 * Called after Facebook javascript has initialized.  Global FB will be set.
 */
FB_Tab.setAutoResize = function() {
  // Auto resize hopefully works same on tab iframes as it does on canvas pages.
  FB.Canvas.setAutoResize(true, 100); // time in ms, default 100.
};
