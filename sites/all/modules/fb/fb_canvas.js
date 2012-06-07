/**
 * @file
 *
 * Javascript specific to canvas pages.
 */

/**
 * Enable canvas page specific javascript on this page.
 */
Drupal.behaviors.fb_canvas = function(context) {
  // Resize if body class includes fb_canvas-resizable.
  $('body.fb_canvas-resizable:not(.fb_canvas-processed)').each(function () {
    $(this).addClass('fb_canvas-processed');
    jQuery(document).bind('fb_init', FB_Canvas.setAutoResize);
  });

  // Logout of facebook when logging out of drupal.
  jQuery("a[href^='https://apps.facebook.com/" + Drupal.settings.fb_canvas.canvas + "/logout']", context).click(FB_Canvas.logout);

  // Change 'user/login' links to popup fb connect dialog.
  jQuery("a[href^='https://apps.facebook.com/" + Drupal.settings.fb_canvas.canvas + "/user/']", context).click(FB_Canvas.login);
};

FB_Canvas = function(){};

/**
 * Called after Facebook javascript has initialized.  Global FB will be set.
 */
FB_Canvas.setAutoResize = function() {
  FB.Canvas.setAutoResize(true, 100); // time in ms, default 100.
};

// click handler
FB_Canvas.logout = function(event) {
  if (typeof(FB) != 'undefined') {
    FB.logout(function () {
      //debugger;
    });
  }
};

FB_Canvas.login = function(event) {
  if (typeof(FB) != 'undefined') {
    FB.login(function() {
      // fb login callback.
    }, Drupal.settings.fb.perms);
  }
};
