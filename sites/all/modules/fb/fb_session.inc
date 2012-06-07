<?php
/**
 * @file
 * This file is a replacement for Drupal's session.inc.  Although not
 * truly a replacement, as we include the default session.inc to do the heavy
 * lifting.
 *
 * In this file we handle some special cases for iframe canvas pages by faking
 * cookies in cases where browsers do not accept them.  We do this here
 * because we must set session_id early in the bootstrap process, and Drupal
 * gives us no way to easily do that.
 */

// Default session handler functions.
require('includes/session.inc');

// When Drupal's bootstrap includes this file, we have a chance to spoof a
// session cookie.
if (isset($_COOKIE[session_name()])) {
  // Forget anything we thought we knew about session.
  fb_settings(FB_SETTINGS_CB_SESSION, FALSE);
}
elseif (fb_settings(FB_SETTINGS_TYPE) &&
        fb_settings(FB_SETTINGS_TYPE) != 'connect') {
  $session_id = NULL;
  if ($token = fb_settings(FB_SETTINGS_TOKEN)) {
    // Learned token from signed_request or session.
    $session_id = md5($token);
    fb_settings(FB_SETTINGS_CB_SESSION, FALSE);
  }
  elseif (isset($_REQUEST['signed_request']) || isset($_REQUEST['session'])) {
    // Signed request, but no token means not logged in.

    // Parse session from URL.
    if ($session_id === NULL && function_exists('_fb_settings_parse')) {
      $session_id = _fb_settings_parse(FB_SETTINGS_CB_SESSION);
    }

    if (!$session_id) {
      // Generating an id and embedding in the URL will make a session where there would otherwise be none.
      $session_id = uniqid(mt_rand(), TRUE);
      // Embed session in URL.
      fb_settings(FB_SETTINGS_CB_SESSION, $session_id);
    }
  }


  // Spoof a cookie so Drupal's session.inc works as expected.
  if ($session_id) {
    session_id($session_id);
    $_COOKIE[session_name()] = session_id();
    $_COOKIE['_fb_session_cookie_fake'] = TRUE;
    $GLOBALS['_fb_session_id'] = $session_id;
  }
}

/**
 * When spoofing cookies, sess_regenerate causes problems when it changes the
 * session id.  Here we undo that change.  Called from fb_user.module.
 */
function fb_sess_regenerate_hack() {
  if (isset($GLOBALS['_fb_session_id'])) {
    $session_id = $GLOBALS['_fb_session_id'];
    db_query("UPDATE {sessions} SET sid = '%s' WHERE sid = '%s'", $session_id, session_id());
    session_id($session_id);
  }
}