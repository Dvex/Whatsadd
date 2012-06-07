<?php

/**
 * @file
 * Views handler for image size filter.
 */

/**
 * Filter by image size.
 */
class image_handler_filter_image_size extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      foreach (image_get_sizes() as $key => $size) {
        $options[$key] = $size['label'];
      }
      $this->value_options = $options;
    }
  }
}

