/**
 * Devel helpers and sanity checks.
 *
 * This file will be included only when fb_devel.module is enabled and user
 * has 'access devel information' permission.
 */

FB_Devel = function(){};

FB_Devel.sanityCheck = function() {
  if (typeof(FB) != 'undefined' &&
      (!Drupal.settings.fb || FB._apiKey != Drupal.settings.fb.fb_init_settings.appId)) {

    // There is a <script> tag initializing Facebook's Javascript
    // before fb.js has a chance to initilize it!  To fix: use browser
    // to view page source, find all <script> tags that include all.js
    // and get rid of them.
    if (Drupal.settings.fb_devel.verbose) {
      alert("fb_devel.js: Facebook JS SDK initialized witout app id!"); // verbose
    }
    debugger; // Failed sanity check.
  }

  var root = jQuery('#fb-root');
  if (root.length != 1) {
    debugger; // not verbose.
    if (Drupal.settings.fb_devel.verbose) {
      alert("fb_devel.js: no <div id=fb-root> found!"); // verbose
    }
  }
};

/**
 * Called when fb.js triggers the 'fb_init' event.
 */
FB_Devel.initHandler = function() {
  FB_Devel.sanityCheck();

  // Facebook events that may be of interest...
  //FB.Event.subscribe('auth.login', FB_Devel.debugHandler);
  //FB.Event.subscribe('auth.logout', FB_Devel.debugHandler);
  //FB.Event.subscribe('auth.statusChange', FB_Devel.debugHandler);
  //FB.Event.subscribe('auth.sessionChange', FB_Devel.debugHandler);
};

// Helper, for debugging facebook events.
FB_JS.debugHandler = function(response) {
  debugger;
};

/**
 * Implements Drupal javascript behaviors.
 */
Drupal.behaviors.fb_devel = function(context) {
  jQuery(document).bind('fb_init', FB_Devel.initHandler);

  //FB_Devel.sanityCheck(); // This is now done in page footer.
};