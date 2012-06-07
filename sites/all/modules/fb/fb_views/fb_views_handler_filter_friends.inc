<?php
/**
 * @file
 * Filter handler for friends of the current user
 */
class fb_views_handler_filter_friends extends views_handler_filter_boolean_operator {
  function construct() {
    parent::construct();
    $this->value_value = t('Is friend of the current user');
  }

  function query() {
    // We can't learn friends if user is not logged in.
    $fbu = fb_facebook_user();
    if ($fbu) {
      $friends = fb_get_friends($fbu);
    }
    if (!$fbu || !count($friends)) {
      // Hide all content
      $friends = array(0);
    }

    $this->ensure_my_table();
    $this->query->add_where(0, $this->table_alias . '.fbu IN (%s)', implode(',', $friends));
  }
}
