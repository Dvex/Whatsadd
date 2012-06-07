<?php
// $Id: site_map.theme.inc,v 1.1.2.4 2010/07/11 17:49:10 frjo Exp $

/**
 * @file site_map.theme.inc
 *
 * Site map theme functions.
 */


function theme_site_map_rss_legend() {
  $output .= '<p><strong>'. t('Legend:') .'</strong><br />';
  $output .= '<span class="rss">'. theme('site_map_feed_icon', NULL) .'</span> '. t('Link to a content RSS feed');
  $output .= '<br /><span class="rss">'. theme('site_map_feed_icon', NULL, 'comment') .'</span> '. t('Link to a comment RSS feed');
  $output .= '</p>';

  return $output;
}

/**
 * Return a themed site map box.
 *
 * @param $title
 *   The subject of the box.
 * @param $content
 *   The content of the box.
 * @param $class
 *   Optional extra class for the box.
 * @return
 *   A string containing the box output.
 */
function theme_site_map_box($title, $content, $class = '') {
  $output = '';
  if ($title || $content) {
    $class = $class ? 'site-map-box '. $class : 'site-map-box';
    $output .= '<div class="'. $class .'">';
    if ($title) {
      $output .= '<h2 class="title">'. $title .'</h2>';
    }
    if ($content) {
      $output .= '<div class="content">'. $content .'</div>';
    }
    $output .= '</div>';
  }

  return $output;
}

/**
 * Return a themed feed icon.
 *
 * @param $url
 *   The feed URL.
 * @param $type
 *   The type of feed icon.
 * @return
 *   A string containing the linked image.
 */
function theme_site_map_feed_icon($url, $type = 'node') {
  $output = '';

  switch ($type) {
    case 'node':
      $output = theme('image', (drupal_get_path('module', 'site_map') .'/feed-small.png'), t('Syndicate content'), t('Syndicate content'));
      break;
    case 'comment':
      $output = theme('image', (drupal_get_path('module', 'site_map') .'/feed-small-comment.png'), t('Syndicate comments'), t('Syndicate comments'));
      break;
  }

  if ($url) {
    $output = l($output, $url, array('attributes' => array('class' => 'feed-link'), 'html' => TRUE));
  }

  return $output;
}

/**
 * This is a clone of the core theme_menu_tree() function with the exception of
 * the site_map specific class name used in the UL that also allow themers to
 * override the function only for the site map page.
 *
 * Generate the HTML output for a menu tree
 *
 * @ingroup themeable
 */
function theme_site_map_menu_tree($tree) {
  return '<ul class="site-map-menu">'. $tree .'</ul>';
}

/**
 * This is a one by one clone of the core theme_menu_item() function that allows
 * custom theming of the site map page items.
 *
 * Generate the HTML output for a menu item and submenu.
 *
 * @ingroup themeable
 */
function theme_site_map_menu_item($link, $has_children, $menu = '', $in_active_trail = FALSE, $extra_class = NULL) {
  $class = ($menu ? 'expanded' : ($has_children ? 'collapsed' : 'leaf'));
  if (!empty($extra_class)) {
    $class .= ' '. $extra_class;
  }
  if ($in_active_trail) {
    $class .= ' active-trail';
  }
  return '<li class="'. $class .'">'. $link . $menu ."</li>\n";
}

/**
 * Process variables for site-map.tpl.php.
 *
 * @see site-map.tpl.php
 */
function template_preprocess_site_map(&$variables) {
  $variables['message'] = check_markup(variable_get('site_map_message', ''), variable_get('site_map_message_format', FILTER_FORMAT_DEFAULT));

  if ((variable_get('site_map_show_rss_links', 1) != 0) && module_exists('commentrss') && variable_get('commentrss_site', COMMENTRSS_SITE_FRONT_PAGE)) {
    $variables['rss_legend'] = theme('site_map_rss_legend');
  }

  if (variable_get('site_map_show_front', 1)) {
    $variables['front_page'] = _site_map_front_page();
  }

  if (variable_get('site_map_show_blogs', 1)) {
    $variables['blogs'] = _site_map_blogs();
  }

  // Compile the books trees.
  $variables['books'] = _site_map_books();

  // Compile the menu trees.
  $variables['menus'] = _site_map_menus();

  if (variable_get('site_map_show_faq', 0)) {
    $variables['faq'] = _site_map_faq();
  }

  // Compile the vocabulary trees.
  $variables['taxonomys'] = _site_map_taxonomys();

  // Invoke all custom modules and get themed HTML to be integrated into the site map.
  $additional = module_invoke_all('site_map');
  foreach ($additional as $themed_site_map_code) {
    $variables['additional'] .= $themed_site_map_code;
  }
}
