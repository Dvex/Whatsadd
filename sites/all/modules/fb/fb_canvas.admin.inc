<?php
/**
 * @file
 * Admin pages and forms for canvas apps.
 *
 */


/**
 * Implementation of hook_fb_admin().
 */
function fb_canvas_fb_admin($op, $data, &$return) {
  $fb = isset($data['fb']) ? $data['fb'] : NULL;
  $fb_app = isset($data['fb_app']) ? $data['fb_app'] : NULL;

  if ($op == FB_ADMIN_OP_SET_PROPERTIES) {
    // Compute properties which we can set automatically.
    if (function_exists('fb_url_inbound_alter')) {
      $callback_url = url('', array(
                            'absolute' => TRUE,
                            'language' => FALSE,
                          )) . FB_SETTINGS_CB . '/' . $fb_app->id . '/';
    }
    else {
      // Paving the way to make URL alters optional.
      $callback_url = url('', array('absolute' => TRUE));
    }
    $return['callback_url'] = $callback_url;
    if (variable_get(FB_VAR_SECURE_URLS, FB_SECURE_URLS_SOMETIMES) >= FB_SECURE_URLS_SOMETIMES) {
      $return['secure_callback_url'] = str_replace('http://', 'https://', $callback_url);
    }
    if (variable_get(FB_VAR_SECURE_URLS, FB_SECURE_URLS_SOMETIMES) <= FB_SECURE_URLS_SOMETIMES) {
      // Make callback_url HTTP, even if we administer drupal via HTTPS.
      $return['callback_url'] = str_replace('https://', 'http:', $return['callback_url']);
    }
  }
  elseif ($op == FB_ADMIN_OP_LIST_PROPERTIES) {
    $return[t('Callback URL')] = 'callback_url';
    $return[t('Secure Callback URL')] = 'secure_callback_url';
  }
}


/**
 * Form builder; Configure settings for this site.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function fb_canvas_admin_settings() {

  $form['process_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Canvas page URL processing'),
    '#description' => t('This option alters links, so that instead of changing the iframe\'s URL, they change the top frame, to something starting <em>apps.facebook.com/APP/...</em>  This adds some overhead to each canvas page served. Still, most sites will want this enabled.'),
  );
  $form['process_settings'][FB_CANVAS_VAR_PROCESS_IFRAME] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable on iframe canvas pages.'),
    '#default_value' => variable_get(FB_CANVAS_VAR_PROCESS_IFRAME, TRUE),
    '#description' => t('If unchecked, settings below have no effect.'),
  );
  $form['process_settings'][FB_CANVAS_VAR_PROCESS_ABSOLUTE] = array(
    '#type' => 'checkbox',
    '#title' => t('Replace absolute hrefs, not just relative, with canvas page URLs.'),
    '#default_value' => variable_get(FB_CANVAS_VAR_PROCESS_ABSOLUTE, TRUE),
  );
  return system_settings_form($form);
}

/**
 * See fb_canvas_form_alter.
 */
function fb_canvas_admin_form_alter(&$form, &$form_state, $form_id) {
  // Add our settings to the fb_app edit form.
  if (isset($form['fb_app_data']) && is_array($form['fb_app_data'])) {
    $fb_app = $form['#fb_app'];
    $fb_canvas_data = _fb_canvas_get_config($fb_app);

    $form['fb_app_data']['fb_canvas'] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => isset($fb_app->label),
      '#title' => t('Facebook canvas pages'),
      '#description' => t('Settings which apply to <a href=!url target=_blank>canvas pages</a>.',
                          array('!url' => 'http://developers.facebook.com/docs/guides/canvas/')),
    );

    // Override themes
    $themes = system_theme_data();
    ksort($themes);
    $theme_options[0] = t('System default');
    foreach ($themes as $theme) {
      if ($theme->status) { // Only show enabled themes.
        $theme_options[$theme->name] = $theme->name;
      }
    }
    $form['fb_app_data']['fb_canvas']['theme_iframe'] = array(
      '#type' => 'select',
      '#title' => t('Theme for canvas pages'),
      '#description' => t('Choose a theme designed for 760px width iframe canvas.'),
      '#options' => $theme_options,
      '#required' => TRUE,
      '#default_value' => $fb_canvas_data['theme_iframe'],
    );


    if (FALSE) { // @TODO - no require_login in new libs???
    $form['fb_app_data']['fb_canvas']['require_login'] = array(
      '#type' => 'radios',
      '#title' => t('Require authorization'),
      '#description' => t('Require authorization if you want Drupal for Facebook to call require_login() on <strong>every</strong> canvas page.'),
      '#options' => array(
        FB_CANVAS_OPTION_ALLOW_ANON => t('Allow anonymous visitors'),
        FB_CANVAS_OPTION_REQUIRE_LOGIN => t('Require all users to authorize the application'),
      ),
      '#default_value' => $fb_canvas_data['require_login'],
      '#required' => TRUE,
    );
    }

    $form['fb_app_data']['fb_canvas']['front_anonymous'] = array(
      '#type' => 'textfield',
      '#title' => t('Front page when user has not authorized the application'),
      '#description' => t('This is the front page for users who are not logged into facebook, or have not authorized the application.  Leave blank to use the site-wide front page.'),
      '#default_value' => $fb_canvas_data['front_anonymous'],
    );
    $form['fb_app_data']['fb_canvas']['front_added'] = array(
      '#type' => 'textfield',
      '#title' => t('Front page for authorized users of this application'),
      '#description' => t('Leave blank to use the site-wide front page.'),
      '#default_value' => $fb_canvas_data['front_added'],
    );
  }
}
