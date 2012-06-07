<?php
/**
 * @file
 * Admin pages and forms for connect settings.
 *
 */

/**
 * Implementation of hook_fb_admin().
 */
function fb_connect_fb_admin($op, $data, &$return) {
  if ($op == FB_ADMIN_OP_SET_PROPERTIES) {
    $return['connect_url'] = fb_connect_get_connect_url($data['fb_app']);
    if (variable_get(FB_VAR_SECURE_URLS, FB_SECURE_URLS_SOMETIMES) == FB_SECURE_URLS_ALWAYS) {
      $return['connect_url'] = str_replace('http://', 'https://', $return['connect_url']);
    }
    else {
      $return['connect_url'] = str_replace('https://', 'http://', $return['connect_url']);
    }
  }
  elseif ($op == FB_ADMIN_OP_LIST_PROPERTIES) {
    $return[t('Connect URL')] = 'connect_url';
  }
}

/**
 * Convenience method to get an apps connect URL.
 *
 */
function fb_connect_get_connect_url($fb_app) {
  // absolute URL with no rewriting applied
  global $base_url;

  return $base_url . '/';

}


/**
 * Form builder; Configure settings for this site.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function fb_connect_admin_settings() {
  $options = array(0 => t('<none>')) +
    fb_admin_get_app_options(FALSE);
  if (count($options) == 1) {
    $message = t('You must create an app first!');
    drupal_set_message($message, 'error');
    return array('help' => array('#value' => $message));
  }

  $form[FB_CONNECT_VAR_PRIMARY] = array(
    '#type' => 'select',
    '#options' => $options,
    '#title' => t('Primary Connect Application'),
    '#description' => t('Inialize one application on every page.'),
    '#default_value' => variable_get(FB_CONNECT_VAR_PRIMARY, NULL),
  );

  $form['theme_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Theme overrides'),
  );
  $form['theme_settings'][FB_CONNECT_VAR_THEME_USERNAME_1] = array(
    '#type' => 'checkbox',
    '#title' => t('Override default theme_username() function.'),
    '#default_value' => variable_get(FB_CONNECT_VAR_THEME_USERNAME_1, TRUE),
  );
  $form['theme_settings'][FB_CONNECT_VAR_THEME_USERNAME_2] = array(
    '#type' => 'checkbox',
    '#title' => t('Override phptemplate_username() functions defined in themes.'),
    '#default_value' => variable_get(FB_CONNECT_VAR_THEME_USERNAME_2, TRUE),
  );
  $form['theme_settings'][FB_CONNECT_VAR_THEME_USERPIC_1] = array(
    '#type' => 'checkbox',
    '#title' => t('Override default theme_user_picture() function.'),
    '#default_value' => variable_get(FB_CONNECT_VAR_THEME_USERPIC_1, TRUE),
  );
  $form['theme_settings'][FB_CONNECT_VAR_THEME_USERPIC_2] = array(
    '#type' => 'checkbox',
    '#title' => t('Override phptemplate_user_picture() functions defined in themes.'),
    '#default_value' => variable_get(FB_CONNECT_VAR_THEME_USERPIC_2, TRUE),
  );
  return system_settings_form($form);
}

