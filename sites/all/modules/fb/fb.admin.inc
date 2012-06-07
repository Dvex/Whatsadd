<?php
/**
 * @file
 * Admin pages and forms for Drupal for Facebook.
 *
 */


define('FB_ADMIN_HOOK', 'fb_admin');

define('FB_ADMIN_OP_SET_PROPERTIES', 'set_props'); // build props for admin.setAppProperties
define('FB_ADMIN_OP_LIST_PROPERTIES', 'list_props'); // list of known properties for admin.getAppProperties


// Pages which may or may not be supported by the app implementation.
define('FB_ADMIN_OP_LOCAL_LINKS', 'fb_admin_links');

/**
 * Drupal page callback.
 */
function fb_admin_page() {
  $apps = fb_get_all_apps();
  $output = ''; // Avoid PHP notice.

  if (count($apps)) {
    $header = array(
      t('Label'),
      t('Name'),
      t('Canvas'),
      t('Local Operations'),
      t('Remote Settings'),
    );
    $protocol = fb_protocol();

    foreach ($apps as $fb_app) {
      // Get properties from facebook.
      fb_admin_get_app_properties($fb_app);

      $row = array();
      // Title.
      $row[] = $fb_app->label . ($fb_app->status ? '' : ' ' . t('(<em>not enabled</em>)'));

      // New apps no longer have about pages.
      $name = isset($fb_app->application_name) ? $fb_app->application_name : $fb_app->label;
      if ($fb_app->about_url) {
        $row[] = l($name, $fb_app->about_url);
      }
      else {
        $row[] = $name;
      }

      // Canvas Page.
      if (isset($fb_app->canvas_name) &&
          $fb_app->canvas_name != $fb_app->canvas) {
        drupal_set_message(t('Canvas page for %label is out of sync!  Facebook believes it is %fbcanvas, while our database believes %canvas.  Edit and save the application to remedy this.', array('%label' => $fb_app->label, '%fbcanvas' => $fb_app->canvas_name, '%canvas' => $fb_app->canvas)), 'error');
      }
      if ($fb_app->canvas) {
        $row[] = l($fb_app->canvas, $protocol . '://apps.facebook.com/' . $fb_app->canvas);
      }
      else {
        $row[] = t('n/a');
      }
      // Local Ops
      $local_links =
        fb_invoke(FB_ADMIN_OP_LOCAL_LINKS,
                  array('fb_app' => $fb_app),
                  array(t('view') => FB_PATH_ADMIN_APPS . '/' . $fb_app->label,
                        t('sync props') => FB_PATH_ADMIN_APPS . '/' . $fb_app->label . '/fb/set_props',
                  ),
                  FB_ADMIN_HOOK);

      $links = array();
      foreach ($local_links as $title => $href) {
        $links[] = array(
          'title' => $title,
          'href' => $href,
        );
      }
      $row[] = theme('links', $links);

      // Remote Settings
      $row[] = l($fb_app->id, 'https://www.facebook.com/developers/editapp.php?app_id=' . $fb_app->id);

      $rows[] = $row;
    }

    $output .= theme('table', $header, $rows);

  }
  else {
    if (!module_exists('fb_app')) {
      $output = t('Enable the <em>Facebook Apps</em> module (fb_app.module) to manage an application.');
    }
    else {
      $output = t('Click <a href="!url">Add Application</a> to get started.',
                   array('!url' => url('admin/build/fb/fb_app_create')));
    }
  }
  return $output;
}

function fb_admin_app_page($fb_app = NULL) {
  fb_get_app_data($fb_app);
  fb_admin_get_app_properties($fb_app);
  $fb = fb_api_init($fb_app);

  // @TODO use an actual theme function and make render more appealing.
  // Hide a couple things...
  unset($fb_app->secret);
  unset($fb_app->data);

  // Warn user if values have changed since last edit.
  foreach (array('canvas_name' => 'canvas', 'application_name' => 'title') as $prop => $key) {
    if (isset($fb_app->$prop) && $fb_app->$prop != $fb_app->$key) {
      drupal_set_message(t("The property %prop has been changed to %value on facebook.  Go to the Edit tab, confirm values are correct and hit Save button to syncronize the local values.",
                           array('%prop' => $prop, '%value' => $fb_app->$prop)), 'error');
    }
  }

  $props_map = array(
    t('Name') => 'name',
    t('Label') => 'label',
    t('ID') => 'id',
    t('Secret') => 'secret',
  );
  $output = "<dl>\n";

  // Render props learned from facebook and stored in $fb_app object.
  $props_map = fb_invoke(FB_ADMIN_OP_LIST_PROPERTIES, array('fb_app' => $fb_app), $props_map, FB_ADMIN_HOOK);
  foreach ($props_map as $name => $key) {
    if (isset($fb_app->$key)) {
      $output .= "<dt>$name</dt><dd>{$fb_app->$key}</dd>\n";
    }
  }

  if ($fb_app->installable && $fb_app->profile_tab_url) {
    // This app can be added to a page.  Provide a link to do so.
    $add_url = "http://www.facebook.com/dialog/pagetab?app_id=$fb_app->id&next=" . url('<front>', array('absolute' => TRUE, 'language' => FALSE));
    $output .= '<dt>' . t('Add to page link') . '</dt><dd>' . l($add_url, $add_url) . '</dd>';
  }

  // Render additional properties
  $output .= '<dt>' . t('Access Token') . '</dt><dd>' . fb_get_token($fb) . "</dd>\n";

  $output .= "</dl>\n";
  //$output .=  '<pre>' . print_r($fb_app, 1) . '</pre>'; // debug

  return $output;
}

function fb_admin_page_title($fb_app) {
  return $fb_app->label;
}

/**
 * Get properties from Facebook.  Fills in the data that we need to
 * know by querying facebook.
 */
function fb_admin_get_app_properties(&$fb_app) {
  static $cache;
  static $props_map;
  if (!isset($cache)) {
    $cache = array();
    // http://wiki.developers.facebook.com/index.php/ApplicationProperties
    $props_map = array(
      t('About URL') => 'about_url',
      t('Application Name') => 'application_name',
      t('Edit URL') => 'edit_url',
    );
    $props_map = fb_invoke(FB_ADMIN_OP_LIST_PROPERTIES, array('fb_app' => $fb_app), $props_map, FB_ADMIN_HOOK);
  }

  if (!isset($cache[$fb_app->id])) {
    if ($fb = fb_api_init($fb_app)) {
      try {
        $props = fb_call_method($fb, 'admin.getAppProperties', array(
                                  'properties' => implode(',', array_values($props_map)),
                                ));
        $cache[$fb_app->id] = $props;
      } catch (Exception $e) {
        fb_log_exception($e, t('Failed to get application properties (%label) from Facebook', array('%label' => $fb_app->label)));
      }
    }
  }
  else {
    $props = $cache[$fb_app->id];
  }

  // Update $fb_app with the values we got from facebook api.
  foreach ($props_map as $key) {
    if (isset($props[$key])) {
      $fb_app->$key = $props[$key];
    }
  }
}


/**
 * Convenience method to return a list of all known apps, suitable for
 * form elements.
 *
 */
function fb_admin_get_app_options($include_current = FALSE, $key = 'label') {
  $apps = fb_get_all_apps();
  $options = array();
  if ($include_current)
    $options[FB_APP_CURRENT] = t('<current>');
  foreach ($apps as $app) {
    $title = !empty($app->title) ? $app->title : $app->label;
    if ($key == 'id') { // Still needed?
      $options[$app->id] = $title;
    }
    else {
      $options[$app->label] = $title;
    }
  }
  return $options;
}

function fb_admin_set_properties_form(&$form_state, $fb_app) {
  $form['fba_id'] = array(
    '#type' => 'value',
    '#value' => $fb_app->fba_id,
  );

  $props = fb_invoke(FB_ADMIN_OP_SET_PROPERTIES, array('fb_app' => $fb_app), array(), FB_ADMIN_HOOK);

  $form['#fb_props'] = $props;

  $form['desc'] = array(
    '#type' => 'markup',
    '#value' => t('This will attempt to set the following application properties on facebook.com.'),
    '#prefix' => '<p>',
    '#suffix' => '</p>',
  );
  // @TODO - beautify display of properties.
  $form['props'] = array(
    '#type' => 'markup',
    '#value' => print_r($props, 1),
    '#prefix' => '<pre>',
    '#suffix' => '</pre>',
  );

  $form['desc2'] = array(
    '#type' => 'markup',
    '#value' => t('Syncing will also update local settings with values learned from facebook (i.e. if you have changed your canvas page).'),
    '#prefix' => '<p>',
    '#suffix' => '</p>',
  );

  return confirm_form($form,
                      t('Are you sure you set properties for %title?', array('%title' => $fb_app->title)),
                      isset($_GET['destination']) ? $_GET['destination'] : FB_PATH_ADMIN_APPS . '/' . $fb_app->label,
                      t('This action cannot be undone.'),
                      t('Sync Properties'),
                      t('Cancel')
  );
}

/**
 * Button submit function.  Use has clicked delete, send them to confirm page.
 */
function fb_admin_set_properties_form_submit($form, &$form_state) {
  $fba_id = $form_state['values']['fba_id'];
  $fb_app = fb_get_app(array('fba_id' => $fba_id));
  $props = $form['#fb_props'];
  if ($fb_app && count($props)) {
    if ($fb = fb_api_init($fb_app)) {
      try {
        $result = fb_call_method($fb, 'admin.setAppProperties', array(
                                   'properties' => json_encode($props),
                                 ));
        // Success.
        $form_state['redirect'] = FB_PATH_ADMIN_APPS . '/' . $fb_app->label;

        drupal_set_message(t('Note that it may take several minutes for property changes to propagate to all facebook servers.'));
        if (fb_verbose()) {
          drupal_set_message(t('Set the following properties for %label application:<br/><pre>!props</pre>', array('%label' => $fb_app->label, '!props' => print_r($props, 1))));
          watchdog('fb_app', 'Set facebook app properties for %label.',
                   array('%label' => $fb_app->label,
                   ),
                   WATCHDOG_NOTICE,
                   l(t('view apps'), FB_PATH_ADMIN));
        }

        // Update local data with values learned from facebook.
        fb_admin_get_app_properties($fb_app);
        if ($fb_app->fba_id && $fb_app->canvas_name && module_exists('fb_app')) {
          // @TODO - clean this up or move it to fb_app.module.  fb.module should have no knowledge of fb_app table.
          db_query("UPDATE {fb_app} SET canvas='%s' WHERE fba_id = %d", array(
                     $fb_app->canvas_name,
                     $fb_app->fba_id,
                   ));
        }

      } catch (Exception $e) {
        drupal_set_message(t('Failed to set the following properties for %label application.  You may need to manually editing remote settings!<br/><pre>!props</pre>', array('%label' => $fb_app->label, '!props' => print_r($props, 1))), 'error');
        fb_log_exception($e, t('Failed to set application properties on Facebook'));
      }
    }
  }

}

/**
 * Form callback for general settings.
 */
function fb_admin_settings() {
  $form = array();

  // @TODO would be nice to automatically test whether HTTPS is supported.
  $form['fb_admin_secure'] = array(
    '#title' => t('Secure URLs'),
    '#type' => 'fieldset',
    '#description' => t('Canvas Pages and Page Tabs require a server that supports SSL encrypted HTTPS.  Without HTTPS, those features will only work in "sandbox mode."'),
  );
  $form['fb_admin_secure'][FB_VAR_SECURE_URLS] = array(
    '#type' => 'radios',
    '#title' => t('Use Secure URLs'),
    '#default_value' => variable_get(FB_VAR_SECURE_URLS, FB_SECURE_URLS_SOMETIMES),
    '#description' => t('This setting affects your applications\' connect_url, callback_url, secure_callback_url, profile_tab_url, and secure_page_tab_url properties.'),
    '#options' => array(
      FB_SECURE_URLS_NEVER => t('Never use HTTPS. (I.e. this server does not support it.)'),
      FB_SECURE_URLS_SOMETIMES => t('Use HTTPS only when Facebook expects a secure URL. (Recommended.)'),
      FB_SECURE_URLS_ALWAYS => t('Always use HTTPS for both secure urls and also other url callbacks.'),
    ),
  );

  $form['fb_admin_session'] = array(
    '#title' => t('Sessions'),
    '#type' => 'fieldset',
    '#description' => t('Settings that control <a href="!auth_url" target=_blank>authentication</a> and session management when connected to Facebook.  These settings are relavant for Apps, but not Social Plugins.', array(
                          '!auth_url' => 'http://developers.facebook.com/docs/authentication/',
                        )),
    '#collapsible' => TRUE,
  );

  $form['fb_admin_session'][FB_VAR_USE_COOKIE] = array(
    '#type' => 'checkbox',
    '#title' => t('Use FB Cookie'),
    '#default_value' => variable_get(FB_VAR_USE_COOKIE, TRUE),
    '#description' => t('Tell Facebook\'s libraries to set and honor a cookie when user connects.  Makes facebook applications run smoother, but may fail when third-party cookies are denied by user\'s browser.'),
  );

  $form['fb_admin_session'][FB_VAR_USE_SESSION] = array(
    '#type' => 'checkbox',
    '#title' => t('Store tokens in session'),
    '#default_value' => variable_get(FB_VAR_USE_SESSION, TRUE),
    '#description' => t('Store tokens and data needed by facebook\'s libraries in Drupal\'s session.  Helps applications work even when third-party cookies are disabled.'),
  );

  $form['fb_admin_js'] = array(
    '#title' => t('Javascript'),
    '#type' => 'fieldset',
    '#description' => t('Settings that control the <a href="!url" target=_blank>Javascript SDK</a>. These settings are relavant for both Apps and not Social Plugins. <br/>The defaults are the recommended values.', array(
                          '!url' => 'http://developers.facebook.com/docs/reference/javascript/',
                        )),
    '#collapsible' => TRUE,
  );

  $form['fb_admin_js'][FB_VAR_JS_OAUTH] = array(
    '#type' => 'checkbox',
    '#title' => t('Use Oauth when initializing javascript.'),
    '#default_value' => variable_get(FB_VAR_JS_OAUTH, TRUE),
    '#description' => t('Pass oauth TRUE to <a href=!url target=_blank>FB.init()</a>.  When checked, javascript SDK looks for new-style cookie named "fbsr_...".  When unchecked, looks for old-style cookie named "fbs_...".  When this and above options are unchecked, user will have to explicitly connect/disconnect from your site, logging in/out of facebook will not be automatically detected.', array(
                          '!url' => 'http://developers.facebook.com/docs/reference/javascript/FB.init/',
                        )),
  );

  $form['fb_admin_js'][FB_VAR_JS_GET_LOGIN_STATUS] = array(
    '#type' => 'checkbox',
    '#title' => t('Get login status when initializing javascript.'),
    '#default_value' => variable_get(FB_VAR_JS_GET_LOGIN_STATUS, TRUE),
    '#description' => t('Detect whether a connected user has logged into or out of facebook.  Uses FB.getLoginStatus().  This technique is recommended by Facebook; however, it has <a href=!url>bugs</a> and <a href=!url2>more bugs</a>.  If those bugs affect your site, you may want to disable this.', array(
                          '!url' => 'http://developers.facebook.com/bugs/173032012783482',
                          '!url2' => 'https://developers.facebook.com/bugs/240058389381072',
                        )),
  );

  $form['fb_admin_js'][FB_VAR_JS_TEST_LOGIN_STATUS] = array(
    '#type' => 'checkbox',
    '#title' => t('Alternative verify login status when initializing javascript.'),
    '#default_value' => variable_get(FB_VAR_JS_TEST_LOGIN_STATUS, FALSE),
    '#description' => t('When not using FB.getLoginStatus() (because it is <a href=!url target=_blank>buggy</a>) detect log out of facebook via a call to FB.api().  Does not detect a login (the user must explicitly click a connect button). This may add overhead on the client side to every page load.   This setting only takes effect when the setting above is unchecked.', array(
                          '!url' => 'http://developers.facebook.com/bugs/173032012783482',
                        )),
  );

  $form['fb_admin_js'][FB_VAR_JS_USE_SESSION] = array(
    '#type' => 'checkbox',
    '#title' => t('Use tokens in session to initialize javascript'),
    '#default_value' => variable_get(FB_VAR_JS_USE_SESSION, FALSE),
    '#description' => t('Attempt to workaround <a href=!url target=_blank>a bug in facebook\'s javascript</a> by passing a signed request in the js settings.  Facebook\'s SDK should use that instead of a cookie. (Only works when storing tokens in session and using oauth.)', array(
                          '!url' => 'http://developers.facebook.com/bugs/173032012783482',
                        )),
  );

  $form['fb_admin_js'][FB_VAR_RELOAD_APPEND_HASH] = array(
    '#type' => 'checkbox',
    '#title' => t('Append hash on javascript page reload'),
    '#default_value' => variable_get(FB_VAR_RELOAD_APPEND_HASH, FALSE),
    '#description' => t('When fb.js reloads (on session change), append a query parameter which serves two purposes.  First, it will break through a cache which may be desireable when facebook session changes.  Second, it prevents infinite reloads when third-party cookies are not enabled.  Enable this if you encounter either of those problems.'),
  );

  $form['fb_admin_languages'] = array(
    '#title' => t('Languages and Locales'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
  );

  $form['fb_admin_languages'][FB_VAR_LANGUAGE_OVERRIDE] = array(
    '#title'  => t("Use locale learned from facebook, if available."),
    '#type'  => 'checkbox',
    '#return_value'  => 'override',
    '#default_value' => variable_get(FB_VAR_LANGUAGE_OVERRIDE, 'override'),
    '#description' => t('Otherwise, use the mapping below when the local user\'s language is known.'),
);

  foreach (language_list() as $language) {
    $form['fb_admin_languages']['fb_language_'. $language->language] = array(
      '#title'  => t("Assigned Facebook's locale to @language", array('@language' => t($language->name))),
      '#type'  => 'select',
      '#options' => fb_admin_i18n_list(),
      '#default_value' => fb_admin_i18n_map($language->language),
    );
  }

  return system_settings_form($form);
}

/**
 * Validates the settings form.
 */
function fb_admin_settings_validate($form, &$form_state) {
  $values = $form_state['values'];
  if (!$values[FB_VAR_USE_COOKIE] && !$values[FB_VAR_USE_SESSION]) {
    form_set_error('fb_admin_session][' . FB_VAR_USE_SESSION, t('You must select either the FB Cookie option, the store token in session option, or both.'));
    form_set_error('fb_admin_session][' . FB_VAR_USE_COOKIE);
  }

}

/*
 * Returns a list of valid Facebook language codes.
 * Helper function
 * see: http://wiki.developers.facebook.com/index.php/Facebook_Locales
*/
function fb_admin_i18n_list() {
  return array(
    'af_ZA' => t('Afrikaans'),
    'sq_AL' => t('Albanian'),
    'ar_AR' => t('Arabic'),
    'hy_AM' => t('Armenian'),
    'ay_BO' => t('Aymara'),
    'az_AZ' => t('Azeri'),
    'eu_ES' => t('Basque'),
    'be_BY' => t('Belarusian'),
    'bn_IN' => t('Bengali'),
    'bs_BA' => t('Bosnian'),
    'bg_BG' => t('Bulgarian'),
    'ca_ES' => t('Catalan'),
    'ck_US' => t('Cherokee'),
    'hr_HR' => t('Croatian'),
    'cs_CZ' => t('Czech'),
    'da_DK' => t('Danish'),
    'nl_NL' => t('Dutch'),
    'nl_BE' => t('Dutch (België)'),
    'en_PI' => t('English (Pirate)'),
    'en_GB' => t('English (UK)'),
    'en_US' => t('English (US)'),
    'en_UD' => t('English (Upside Down)'),
    'eo_EO' => t('Esperanto'),
    'et_EE' => t('Estonian'),
    'fo_FO' => t('Faroese'),
    'tl_PH' => t('Filipino'),
    'fi_FI' => t('Finnish'),
    'fb_FI' => t('Finnish (test)'),
    'fr_FR' => t('French (France)'),
    'fr_CA' => t('French (Canada)'),
    'gl_ES' => t('Galician'),
    'ka_GE' => t('Georgian'),
    'de_DE' => t('German'),
    'el_GR' => t('Greek'),
    'gn_PY' => t('Guaraní'),
    'gu_IN' => t('Gujarati'),
    'he_IL' => t('Hebrew'),
    'hi_IN' => t('Hindi'),
    'hu_HU' => t('Hungarian'),
    'is_IS' => t('Icelandic'),
    'id_ID' => t('Indonesian'),
    'ga_IE' => t('Irish'),
    'it_IT' => t('Italian'),
    'ja_JP' => t('Japanese'),
    'jv_ID' => t('Javanese'),
    'kn_IN' => t('Kannada'),
    'kk_KZ' => t('Kazakh'),
    'km_KH' => t('Khmer'),
    'tl_ST' => t('Klingon'),
    'ko_KR' => t('Korean'),
    'ku_TR' => t('Kurdish'),
    'la_VA' => t('Latin'),
    'lv_LV' => t('Latvian'),
    'fb_LT' => t('Leet Speak'),
    'li_NL' => t('Limburgish'),
    'lt_LT' => t('Lithuanian'),
    'mk_MK' => t('Macedonian'),
    'mg_MG' => t('Malagasy'),
    'ms_MY' => t('Malay'),
    'ml_IN' => t('Malayalam'),
    'mt_MT' => t('Maltese'),
    'mr_IN' => t('Marathi'),
    'mn_MN' => t('Mongolian'),
    'ne_NP' => t('Nepali'),
    'se_NO' => t('Northern Sámi'),
    'nb_NO' => t('Norwegian (bokmal)'),
    'nn_NO' => t('Norwegian (nynorsk)'),
    'ps_AF' => t('Pashto'),
    'fa_IR' => t('Persian'),
    'pl_PL' => t('Polish'),
    'pt_PT' => t('Portuguese (Portugal)'),
    'pt_BR' => t('Portuguese (Brazil)'),
    'pa_IN' => t('Punjabi'),
    'qu_PE' => t('Quechua'),
    'ro_RO' => t('Romanian'),
    'rm_CH' => t('Romansh'),
    'ru_RU' => t('Russian'),
    'sa_IN' => t('Sanskrit'),
    'sr_RS' => t('Serbian'),
    'zh_CN' => t('Simplified Chinese (China)'),
    'sk_SK' => t('Slovak'),
    'sl_SI' => t('Slovenian'),
    'so_SO' => t('Somali'),
    'es_LA' => t('Spanish'),
    'es_CL' => t('Spanish (Chile)'),
    'es_CO' => t('Spanish (Colombia)'),
    'es_MX' => t('Spanish (Mexico)'),
    'es_ES' => t('Spanish (Spain)'),
    'es_VE' => t('Spanish (Venezuela)'),
    'sw_KE' => t('Swahili'),
    'sv_SE' => t('Swedish'),
    'sy_SY' => t('Syriac'),
    'tg_TJ' => t('Tajik'),
    'ta_IN' => t('Tamil'),
    'tt_RU' => t('Tatar'),
    'te_IN' => t('Telugu'),
    'th_TH' => t('Thai'),
    'zh_HK' => t('Traditional Chinese (Hong Kong)'),
    'zh_TW' => t('Traditional Chinese (Taiwan)'),
    'tr_TR' => t('Turkish'),
    'uk_UA' => t('Ukrainian'),
    'ur_PK' => t('Urdu'),
    'uz_UZ' => t('Uzbek'),
    'vi_VN' => t('Vietnamese'),
    'cy_GB' => t('Welsh'),
    'xh_ZA' => t('Xhosa'),
    'yi_DE' => t('Yiddish'),
    'zu_ZA' => t('Zulu'),
  );
}


/*
 * Mapping Drupal language code -> Facebook  locale
 * Helper function
 */
function fb_admin_i18n_map($lang_code) {

  $languages_map = array(
    'af' => 'af_ZA',
    'ar' => 'ar_AR',
    'ay' => 'ay_BO',
    'az' => 'az_AZ',
    'be' => 'be_BY',
    'bg' => 'bg_BG',
    'bn' => 'bn_IN',
    'bs' => 'bs_BA',
    'ca' => 'ca_ES',
    'cs' => 'cs_CZ',
    'cy' => 'cy_GB',
    'da' => 'da_DK',
    'de' => 'de_DE',
    'el' => 'el_GR',
    'en' => 'en_US',
    'eo' => 'eo_EO',
    'es' => 'es_ES',
    'et' => 'et_EE',
    'eu' => 'eu_ES',
    'fa' => 'fa_IR',
    'fi' => 'fi_FI',
    'fo' => 'fo_FO',
    'fr' => 'fr_FR',
    'ga' => 'ga_IE',
    'gl' => 'gl_ES',
    'gn' => 'gn_PY',
    'gu' => 'gu_IN',
    'he' => 'he_IL',
    'hi' => 'hi_IN',
    'hr' => 'hr_HR',
    'hu' => 'hu_HU',
    'hy' => 'hy_AM',
    'id' => 'id_ID',
    'is' => 'is_IS',
    'it' => 'it_IT',
    'ja' => 'ja_JP',
    'jv' => 'jv_ID',
    'ka' => 'ka_GE',
    'kk' => 'kk_KZ',
    'km' => 'km_KH',
    'kn' => 'kn_IN',
    'ko' => 'ko_KR',
    'ku' => 'ku_TR',
    'la' => 'la_VA',
    'lt' => 'lt_LT',
    'lv' => 'lv_LV',
    'mg' => 'mg_MG',
    'mk' => 'mk_MK',
    'ml' => 'ml_IN',
    'mn' => 'mn_MN',
    'mr' => 'mr_IN',
    'ms' => 'ms_MY',
    'mt' => 'mt_MT',
    'nb' => 'nb_NO',
    'ne' => 'ne_NP',
    'nl' => 'nl_BE',
    'nn' => 'nn_NO',
    'pa' => 'pa_IN',
    'pl' => 'pl_PL',
    'ps' => 'ps_AF',
    'qu' => 'qu_PE',
    'rm' => 'rm_CH',
    'ro' => 'ro_RO',
    'ru' => 'ru_RU',
    'sa' => 'sa_IN',
    'se' => 'se_NO',
    'sk' => 'sk_SK',
    'sl' => 'sl_SI',
    'so' => 'so_SO',
    'sq' => 'sq_AL',
    'sr' => 'sr_RS',
    'sv' => 'sv_SE',
    'sw' => 'sw_KE',
    'ta' => 'ta_IN',
    'te' => 'te_IN',
    'tg' => 'tg_TJ',
    'th' => 'th_TH',
    'tl' => 'tl_ST',
    'tr' => 'tr_TR',
    'tt' => 'tt_RU',
    'uk' => 'uk_UA',
    'ur' => 'ur_PK',
    'uz' => 'uz_UZ',
    'vi' => 'vi_VN',
    'xh' => 'xh_ZA',
    'yi' => 'yi_DE',
    'zh-hans' => 'zh_CN',
    'zh-hant' => 'zh_TW',
    'zu' => 'zu_ZA',
  );

  return isset($languages_map[$lang_code]) ? $languages_map[$lang_code] : 'en_US';
}
