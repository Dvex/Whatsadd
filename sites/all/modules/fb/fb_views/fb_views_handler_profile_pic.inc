<?php
/**
 * @file
 * Render fbml tag for user's picture.
 */

class fb_views_handler_profile_pic extends views_handler_field {

  function construct() {
    parent::construct();
    $this->additional_fields['fbu'] = 'fbu';
  }

  function render($values) {
    if ($fbu = $values->{$this->field_alias}) {
      return '<div class="picture"><fb:profile-pic uid="' . $fbu . '" linked="yes" size="thumb" /></div>';
    }
  }
}
