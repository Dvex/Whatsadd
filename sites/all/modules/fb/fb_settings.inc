<?php

/**
 * @file
 * This file is to be included from your sites/.../settings.php file.
 *
 * In this code we set up special session management and url
 * rewriting.  These things must be done before modules are loaded, so
 * the code is here instead of fb.module. And that is why this
 * must be included from settings.php.
 *
 */


// Each of these are things we can learn and store in fb_settings().
// The CB (callback) values are learned via URL rewriting.
// Include fb_url_rewrite.inc in your settings.php to enable this.
define('FB_SETTINGS_CB', 'fb_cb'); // The app id.
define('FB_SETTINGS_CB_PAGE', 'fb_page'); // Page id, for tabs.
define('FB_SETTINGS_CB_TYPE', 'fb_cb_type'); // For iframes within FBML canvas pages, now DEPRECATED.
define('FB_SETTINGS_CB_SESSION', 'fb_sess'); // For embedding session id within URL.

// Things we can learn from the facebook session.
define('FB_SETTINGS_APIKEY', 'apikey');
define('FB_SETTINGS_ID', 'app_id');
define('FB_SETTINGS_PAGE_ID', 'page_id');
define('FB_SETTINGS_FBU', 'fbu');
define('FB_SETTINGS_TOKEN', 'token');
define('FB_SETTINGS_TYPE', 'type'); // page type not same as cb type
define('FB_SETTINGS_COOKIE_DOMAIN', 'cookie_domain');

// Possible values for page type.
define('FB_SETTINGS_TYPE_CANVAS', 'canvas');
define('FB_SETTINGS_TYPE_CONNECT', 'connect');
define('FB_SETTINGS_TYPE_PROFILE', 'profile'); // deprecated, FBML tab
define('FB_SETTINGS_TYPE_PAGE_TAB', 'page_tab'); // iframe tab

/**
 * Helper function to remember values as we learn them.
 */
function fb_settings($key = NULL, $value = NULL) {
  static $cache = array();
  if (isset($value)) {
    $cache[$key] = $value;
  }
  if (isset($key)) {
    return isset($cache[$key]) ? $cache[$key] : NULL;
  }
  else {
    return $cache;
  }
}


/**
 * Helpers to parse signed_session.  Copied from facebook.php.
 */
/**
 * Base64 encoding that doesn't need to be urlencode()ed.
 * Exactly the same as base64_encode except it uses
 *   - instead of +
 *   _ instead of /
 *
 * @param String base64UrlEncodeded string
 */
function _fb_settings_base64_url_decode($input) {
  return base64_decode(strtr($input, '-_', '+/'));
}

/**
 * See https://developers.facebook.com/docs/authentication/signed_request/ for
 * a more reliable version of this function.  We skip validation because we do
 * not yet know the app secret.
 */
function _fb_settings_parse_signed_request($signed_request) {
  list($encoded_sig, $payload) = explode('.', $signed_request, 2);

  // decode the data
  $sig = _fb_settings_base64_url_decode($encoded_sig);
  $data = json_decode(_fb_settings_base64_url_decode($payload), TRUE);

  return $data;
}

/**
 * Get the fb_settings from a parsed signed request.
 * http://developers.facebook.com/docs/authentication/canvas
 */
function _fb_settings_honor_signed_request($sr) {
  if (isset($sr['page'])) {
    // Iframe page tab.
    fb_settings(FB_SETTINGS_CB_PAGE, $sr['page']['id']);
    fb_settings(FB_SETTINGS_PAGE_ID, $sr['page']['id']);
    fb_settings(FB_SETTINGS_TYPE, FB_SETTINGS_TYPE_PAGE_TAB);
    if (isset($sr['user_id'])) {
      fb_settings(FB_SETTINGS_FBU, $sr['user_id']);
    }
  }
  if (isset($sr['profile_id'])) {
    // Only on old FBML tabs.  Deprecated now that iframe tabs are preferred.
    fb_settings(FB_SETTINGS_CB_PAGE, $sr['profile_id']);
    fb_settings(FB_SETTINGS_CB_PAGE, $sr['profile_id']);
    fb_settings(FB_SETTINGS_TYPE, FB_SETTINGS_TYPE_PROFILE);
    if ($sr['user_id'] != $sr['profile_id']) {
      fb_settings(FB_SETTINGS_FBU, $sr['user_id']);
    }
  }
  elseif (isset($sr['user_id'])) {
    // Probably a canvas page.
    fb_settings(FB_SETTINGS_FBU, $sr['user_id']);
  }

  if (isset($sr['oauth_token'])) {
    fb_settings(FB_SETTINGS_TOKEN, $sr['oauth_token']);
    if (!fb_settings(FB_SETTINGS_ID)) {
      // Prefer app id learned from url rewriting over that learned from signed request (because sr may be encrypted).
      $tokens = explode('|', $sr['oauth_token']);
      if (($app_id = $tokens[0]) && !empty($tokens[1])) {
        fb_settings(FB_SETTINGS_ID, $app_id);
      }
    }
  }
}

/**
 * The current format of the cookie is a signed request.
 *
 * Facebook doesn't document the cookie name or format, so basically we just
 * have to hope this works.
 */
function fb_settings_get_facebook_cookie($app_id, $application_secret = NULL) {
  if (!isset($_COOKIE['fbsr_' . $app_id]))
    return;

  $sr = _fb_settings_parse_signed_request($_COOKIE['fbsr_' . $app_id]);
  return $sr;
}


/**
 * By changing the $cookie_domain, we force drupal to use a different session
 * when a user is logged into a facebook application.  We base the
 * $cookie_domain on the id of the application, if we can learn it.
 *
 * Facebook provides a number of "migrations" and historically has offered
 * different data to applications.  So the code below tries a variety of ways
 * to learn the settings.
 */

if (function_exists('_fb_settings_parse') &&
    ($id = _fb_settings_parse(FB_SETTINGS_CB))) {
  // Learned id from url rewrite.
  // Either canvas page or profile tab.
  fb_settings(FB_SETTINGS_ID, $id);

  if ($page_id = _fb_settings_parse(FB_SETTINGS_CB_PAGE)) {
    fb_settings(FB_SETTINGS_TYPE, FB_SETTINGS_TYPE_PAGE_TAB);
    fb_settings(FB_SETTINGS_PAGE_ID, $page_id);
  }
  else {
    fb_settings(FB_SETTINGS_TYPE, FB_SETTINGS_TYPE_CANVAS);
  }

  if (isset($_REQUEST['signed_request']) &&
      ($sr = _fb_settings_parse_signed_request($_REQUEST['signed_request']))) {
    // Prefer signed request data to cookie data.
    _fb_settings_honor_signed_request($sr);
  }
  else {
    $data = fb_settings_get_facebook_cookie($id);
    if (isset($data)) {
      if (isset($data['uid'])) {
        fb_settings(FB_SETTINGS_FBU, $data['uid']);
      }
    }
  }
}
elseif (isset($_REQUEST['signed_request']) &&
        ($sr = _fb_settings_parse_signed_request($_REQUEST['signed_request']))) {
  // Reach this clause on canvas page when admin has not enabled url_rewrite.
  // http://developers.facebook.com/docs/authentication/canvas

  // We get useful info from signed_request only when user is logged in and
  // therefore oauth_token is set.
  _fb_settings_honor_signed_request($sr);

  // Once upon a time, signed_request was only passed on canvas pages.  No longer true.
  // @TODO - somehow detect whether a signed request indicates canvas page or not.
  //fb_settings(FB_SETTINGS_TYPE, FB_SETTINGS_TYPE_CANVAS);
}
else {
  // We're not in a canvas page.
  // We might be in a facebook connect page.  We have to inspect cookies to make sure.
  $id = isset($conf['fb_id']) ? $conf['fb_id'] : NULL;
  $secret = isset($conf['fb_secret']) ? $conf['fb_secret'] : NULL;
  if ($id) {
    if ($sr = fb_settings_get_facebook_cookie($id, $secret)) {
      _fb_settings_honor_signed_request($sr);
    }
  }
}

if (fb_settings(FB_SETTINGS_TYPE) &&
    fb_settings(FB_SETTINGS_TYPE) != FB_SETTINGS_TYPE_CONNECT) {
  // Cookie domain unique to app and page type.
  $unique_id = fb_settings(FB_SETTINGS_ID);
  $cookie_domain = isset($cookie_domain) ? $cookie_domain : '' .
    fb_settings(FB_SETTINGS_TYPE) . $unique_id;
  fb_settings(FB_SETTINGS_COOKIE_DOMAIN, $cookie_domain); // for debugging.
}

if (fb_settings(FB_SETTINGS_FBU)) {
  // Tells Drupal, don't pull from (or save to) cache when logged into facebook.
  $conf['cache'] = 0;
}
