<?php
/**
 * @file
 *
 * Theme preprocessors for image gallery plugins.
 */

/**
 * Preprocessor for the gallery terms style plugin.
 */
function template_preprocess_image_gallery_view_image_gallery_terms(&$vars) {
  // Common preprocessing for list styles.
  template_preprocess_views_view_unformatted($vars);

  drupal_add_css(drupal_get_path('module', 'image_gallery') .'/image_gallery.css');
}
