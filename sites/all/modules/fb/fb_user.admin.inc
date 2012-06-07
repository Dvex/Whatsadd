<?php

/**
 * @file
 * Admin pages and forms for user settings.
 *
 */

// TODO: add pages to view data in the fb_user_app table.

/**
 * Form builder; Configure settings for this site.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function fb_user_admin_settings() {
  $options = array(0 => t('<none>')) +
    fb_admin_get_app_options(FALSE);
  if (count($options) == 1) {
    $message = t('You must create an app first!');
    drupal_set_message($message, 'error');
    return array('help' => array('#value' => $message));
  }

  $form[FB_USER_VAR_CHECK_SESSION] = array(
    '#type' => 'checkbox',
    '#title' => t('Validate session'),
    '#description' => t('Before creating a user or mapping an account, verify the connection by calling into facebook.  This adds overhead, requiring server-side request to facebook.com.  The result is stored in local session, so should only happen when user first connects.  A "signed request" is difficult to spoof, so this check may not be necessary.'),
    '#default_value' => variable_get(FB_USER_VAR_CHECK_SESSION, NULL),
  );

  // Name options for automatically created user accounts.
  $default = variable_get(FB_USER_VAR_USERNAME_STYLE, FB_USER_OPTION_USERNAME_FBU);

  $form[FB_USER_VAR_USERNAME_STYLE] = array(
    '#type' => 'radios',
    '#title' => t('Username Style for Automatically Created Accounts'),
    '#description' => t('Machine-friendly names include Facebook user ids to ensure uniqueness.  Thanks to Drupal\'s theme layer, visitors to your site will <em>usually</em> see a proper name.<br/>Human-friendly names are like "John Smith".  Because Drupal requires unique names, you may see "John Smith_2", "John Smith_3" and so on.'),
    '#options' => array(
      FB_USER_OPTION_USERNAME_FBU => t('Machine-friendly, i.e. "1234565789@facebook"'),
      FB_USER_OPTION_USERNAME_FULL => t('Human-friendly, i.e. "John Smith"'),
    ),
    '#default_value' => $default,
  );

  $form['fb_user_alter'] = array(
    '#type' => 'fieldset',
    '#title' => t('Form alters'),
    '#description' => t('Add connect button to forms.  If user has connected, show the user\'s name and profile picture.'),
  );
  $form['fb_user_alter']['registration'] = array(
    '#type' => 'fieldset',
    '#title' => t('Registration page'),
  );
  $form['fb_user_alter']['registration'][FB_USER_VAR_ALTER_REGISTER] = array(
    '#type' => 'checkbox',
    '#title' => t('Do alter registration form'),
    '#default_value' => variable_get(FB_USER_VAR_ALTER_REGISTER, TRUE),
  );
  $form['fb_user_alter']['registration'][FB_USER_VAR_TEXT_REGISTER] = array(
    '#type' => 'textfield',
    '#title' => t('Registration form button text'),
    '#size' => 60,
    '#default_value' => _fb_user_button_text('user_register'),
    '#description' => t('Leave blank for facebook\'s default, currently "Login".'),
  );
  $form['fb_user_alter']['login'] = array(
    '#type' => 'fieldset',
    '#title' => t('Login page'),
  );
  $form['fb_user_alter']['login'][FB_USER_VAR_ALTER_LOGIN] = array(
    '#type' => 'checkbox',
    '#title' => t('Do alter login form'),
    '#default_value' => variable_get(FB_USER_VAR_ALTER_LOGIN, TRUE),
  );
  $form['fb_user_alter']['login'][FB_USER_VAR_TEXT_LOGIN] = array(
    '#type' => 'textfield',
    '#title' => t('Login form button text'),
    '#size' => 60,
    '#default_value' => _fb_user_button_text('user_login'),
  );
  $form['fb_user_alter']['login_block'] = array(
    '#type' => 'fieldset',
    '#title' => t('Login block'),
  );
  $form['fb_user_alter']['login_block'][FB_USER_VAR_ALTER_LOGIN_BLOCK] = array(
    '#type' => 'checkbox',
    '#title' => t('Do alter login block'),
    '#default_value' => variable_get(FB_USER_VAR_ALTER_LOGIN_BLOCK, TRUE),
  );
  $form['fb_user_alter']['login_block'][FB_USER_VAR_TEXT_LOGIN_BLOCK] = array(
    '#type' => 'textfield',
    '#title' => t('Login block button text'),
    '#size' => 60,
    '#default_value' => _fb_user_button_text('user_login_block'),
  );
  $form['fb_user_alter'][FB_USER_VAR_ALTER_CONTACT] = array(
    '#type' => 'checkbox',
    '#title' => t('Do alter contact form'),
    '#default_value' => variable_get(FB_USER_VAR_ALTER_CONTACT, TRUE),
    '#description' => t('Sets name and email (if available) from facebook.'),
  );

  return system_settings_form($form);
}

/**
 * Not truly hook_form_alter(), this is called from fb_user_form_alter().
 */
function fb_user_admin_form_alter(&$form, &$form_state, $form_id) {
  // Add our settings to the fb_app edit form.
  if (isset($form['fb_app_data'])) {
    $fb_app = $form['#fb_app'];
    $fb_user_data = _fb_user_get_config($fb_app);

    $form['fb_app_data']['fb_user'] = array(
      '#type' => 'fieldset',
      '#title' => t('Facebook user settings'),
      '#tree' => TRUE,
      '#collapsible' => TRUE,
      '#collapsed' => isset($fb_app->label),
    );

    $form['fb_app_data']['fb_user']['create_account'] = array(
      '#type' => 'radios',
      '#title' => t('Create local account'),
      '#description' => t('This option will create a local account and an entry in the fb_user table when a user authorizes a canvas page or connects using Facebook Connect.  If not, Drupal\'s built in user registration will still work.'),
      '#options' => array(
        FB_USER_OPTION_CREATE_NEVER => t('Do not create accounts automatically'),
        FB_USER_OPTION_CREATE_LOGIN => t('If user has authorized the app'),
      ),
      '#default_value' => $fb_user_data['create_account'],
      '#required' => TRUE,
    );

    $default = $fb_user_data['map_account'];
    if (!is_array($default)) { // This check is for backward compatibility.  Should be removed eventually.
      $default = array($default);
    }
    $form['fb_app_data']['fb_user']['map_account'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Map accounts'),
      '#description' => t('Mapping an account means creating an entry in the fb_user table.  This allows Drupal to know which Facebook id corresponds to which local uid. <br/>Matching based on email works when the email extended permission is requested and only if the user is not already mapped to another account.'),
      '#options' => array(
        FB_USER_OPTION_MAP_ALWAYS => t('Map account when both local uid and Facebook id are known'),
        FB_USER_OPTION_MAP_EMAIL => t('Map account when Facebook email exactly matches local account'),
      ),
      '#default_value' => $default,
    );

    $options = array(0 => t('<none>')) + user_roles(1);
    unset($options[DRUPAL_ANONYMOUS_RID]);
    unset($options[DRUPAL_AUTHENTICATED_RID]);
    // Choose a role to be granted to anyone who authorizes the app.
    $form['fb_app_data']['fb_user']['new_user_rid'] = array(
      '#type' => 'select',
      '#title' => t('Permanent role'),
      '#options' => $options,
      '#description' => t('When a local user has authorized the app, the user will be <em>permanently</em> granted this role, in addition to the default <em>authenticated user</em>.'),
      '#default_value' => $fb_user_data['new_user_rid'],
    );

    // Choose a role to be granted only while connected.
    $form['fb_app_data']['fb_user']['connected_user_rid'] = array(
      '#type' => 'select',
      '#title' => t('Temporary role only when connected <em>(advanced feature, use caution)</em>'),
      '#options' => $options,
      '#description' => t('Role granted <em>temporarily</em> while any user (even <em>Anonymous</em>) is connected to facebook. <br/><strong>Important:</strong> Create a role explicitly for this purpose.  Do not select a role that you assign to users on a permanent basis.'),
      '#default_value' => $fb_user_data['connected_user_rid'],
    );
  }

}