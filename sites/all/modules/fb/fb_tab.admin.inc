<?php

/**
 * @file
 * Drupal administration of fb_tab.module.
 */

/**
 * Implementation of hook_fb_admin().
 *
 */
function fb_tab_fb_admin($op, $data, &$return) {
  $fb = isset($data['fb']) ? $data['fb'] : NULL;
  $fb_app = isset($data['fb_app']) ? $data['fb_app'] : NULL;
  $config = _fb_tab_get_app_config($fb_app);
  if ($op == FB_ADMIN_OP_SET_PROPERTIES) {
    $return['tab_default_name'] = $config['tab_default_name'];

    if ($config['tab_default_name']) {
      if (function_exists('fb_url_inbound_alter')) {
        $callback_url = url('', array('absolute' => TRUE)) . FB_SETTINGS_CB . '/' . $fb_app->id . '/';
      }
      else {
        // Paving the way to make URL alters optional.
        $callback_url = url('', array('absolute' => TRUE));
      }
      $return['profile_tab_url'] = $callback_url . FB_TAB_PATH_VIEW;

      if (variable_get(FB_VAR_SECURE_URLS, FB_SECURE_URLS_SOMETIMES) >= FB_SECURE_URLS_SOMETIMES) {
        $return['secure_page_tab_url'] = str_replace('http://', 'https://', $return['profile_tab_url']) . '/'; // Must end with '/' ???
      }
      if (variable_get(FB_VAR_SECURE_URLS, FB_SECURE_URLS_SOMETIMES) <= FB_SECURE_URLS_SOMETIMES) {
        $return['profile_tab_url'] = str_replace('https://', 'http://', $return['profile_tab_url']);
      }
    }
    else {
      $return['profile_tab_url'] = '';
    }

    if ($config['edit_url']) {
      // http://forum.developers.facebook.net/viewtopic.php?id=88920
      $return['edit_url'] =  url($config['edit_url'], array(
                                   'absolute' => TRUE,
                                   'fb_canvas' => $fb_app->label, // @TODO make canvas optional
                                 ));;
    }
    else {
      $return['edit_url'] = '';
    }
  }
  elseif ($op == FB_ADMIN_OP_LIST_PROPERTIES) {
    $return[t('Profile Tab Name')] = 'tab_default_name';
    $return[t('Profile Tab URL')] = 'profile_tab_url';
    $return[t('Secure Page Tab URL')] = 'secure_page_tab_url';
    $return[t('Profile Tab Edit URL')] = 'edit_url';
    $return[t('Installable')] = 'installable';
  }
}


/**
 * Form builder; Configure settings for this site.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function fb_tab_admin_settings() {

  $form['process_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Tab URL processing'),
    '#description' => t('This option alters links, so that instead of changing the iframe\'s URL, they change the top frame.  This adds some overhead to each tab served. Still, most sites will want this enabled.'),
  );
  $form['process_settings'][FB_TAB_VAR_PROCESS_IFRAME] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable processing on iframe tabs.'),
    '#default_value' => variable_get(FB_TAB_VAR_PROCESS_IFRAME, TRUE),
    '#description' => t('Uncheck if you want the user to stay on the tab, instead of linking to canvas page or website. <br/>If unchecked, settings below have no effect.'),
  );
  $form['process_settings'][FB_TAB_VAR_PROCESS_TO_CANVAS] = array(
    '#type' => 'checkbox',
    '#title' => t('Links send user to canvas pages.'),
    '#default_value' => variable_get(FB_TAB_VAR_PROCESS_TO_CANVAS, TRUE),
    '#description' => t('If unchecked, links send user to normal website.'),
  );
  $form['process_settings'][FB_TAB_VAR_PROCESS_ABSOLUTE] = array(
    '#type' => 'checkbox',
    '#title' => t('Replace absolute hrefs, not just relative links.'),
    '#default_value' => variable_get(FB_TAB_VAR_PROCESS_ABSOLUTE, TRUE),
  );

  return system_settings_form($form);
}


/**
 * See fb_tab_form_alter.
 */
function fb_tab_admin_form_alter(&$form, &$form_state, $form_id) {
  // Add our settings to the fb_app edit form.
  if (isset($form['fb_app_data']) && is_array($form['fb_app_data'])) {
    $fb_app = $form['#fb_app'];
    $config = _fb_tab_get_app_config($fb_app);

    $form['fb_app_data']['fb_tab'] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => isset($fb_app->label),
      '#title' => t('Facebook profile tabs'),
      '#description' => t('Settings which apply to <a href=!url target=_blank>profile tabs</a>.',
                          array('!url' => 'http://developers.facebook.com/docs/guides/canvas/#tabs')),
    );

    // Override themes
    $themes = system_theme_data();
    ksort($themes);
    $theme_options[0] = t('System default');
    foreach ($themes as $theme) {
      if ($theme->status) { // Only enabled themes.
        $theme_options[$theme->name] = $theme->name;
      }
    }
    $form['fb_app_data']['fb_tab']['custom_theme'] = array(
      '#type' => 'select',
      '#title' => t('Theme for profile tabs'),
      '#description' => t('Choose a theme designed to return FBML specifically for the 520 pixel wide space allocated to tabs.  <br/>Note that if your tab path is a menu callback which returns FBML, this setting is ignored.'),
      '#options' => $theme_options,
      '#required' => TRUE,
      '#default_value' => $config['custom_theme'],
    );

    // Properties: http://developers.facebook.com/docs/appproperties
    $form['fb_app_data']['fb_tab']['tab_default_name'] = array(
      '#type' => 'textfield',
      '#title' => 'Tab name',
      '#default_value' => $config['tab_default_name'],
      '#description' => t('A very short title.'),
    );

    $form['fb_app_data']['fb_tab']['profile_tab_url'] = array(
      '#type' => 'textfield',
      '#title' => 'View path',
      '#default_value' => $config['profile_tab_url'],
      '#description' => t('Specify a Drupal page (i.e. "node/123") to render specific content in your page tab.  Or leave blank and implement <em>hook_fb_tab()</em> in your custom module.'),
    );

    $form['fb_app_data']['fb_tab']['profile_tab_url_liked'] = array(
      '#type' => 'textfield',
      '#title' => 'View path when page is liked',
      '#default_value' => $config['profile_tab_url_liked'],
      '#description' => t('Optionally specify another path when the user has liked the page where the tab is shown.  Leave blank to use <em>view path</em>.'),
    );

    $form['fb_app_data']['fb_tab']['edit_url'] = array(
      '#type' => 'textfield',
      '#title' => 'Edit path',
      '#default_value' => $config['edit_url'],
      '#description' => t('Recommended value is %edit_url.  Implement <em>hook_fb_tab()</em> to customize the configuration form.', array(
                            '%edit_url' => FB_TAB_PATH_FORM,
                          )),
    );
  }
}
