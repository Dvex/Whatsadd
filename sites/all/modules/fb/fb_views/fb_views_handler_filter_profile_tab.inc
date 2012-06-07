<?php
/**
 * @file
 * Filter handler for profile tab user.
 *
 * Requires the mapping created by fb_user.module.
 * @TODO - Does Views have a way to enforce that dependency?
 */
class fb_views_handler_filter_profile_tab extends views_handler_filter_boolean_operator {
  function construct() {
    parent::construct();
    $this->value_value = t('Is profile tab user');
  }

  function query() {
    $fb_app = $GLOBALS['_fb_app']; // Set on Canvas and Connect pages.

    $profile_fbu = $_REQUEST['fb_sig_profile_id']; // Set by facebook.

    if ($fb_app && $profile_fbu) {
      // @TODO - this code untested since the conversion from authmap to fb_user table.  Does it work?
      $uid = fb_user_get_local_user($profile_fbu, $fb_app);

      if ($uid) {
        $this->ensure_my_table();
        $this->query->add_where(0, $this->table_alias . ".fbu=%d", $profile_fbu);
      }
    }
  }
}
