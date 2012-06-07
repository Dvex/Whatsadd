<?php
/**
 * @file
 * Admin pages and forms for fb_user_app module, which tracks
 * various events within Drupal for Facebook.
 *
 */

/**
 * Form builder; Configure settings for statistic gathering.
 */
function fb_user_app_admin_settings() {
  $form['stats'] = array(
    '#type' => 'fieldset',
    '#title' => t('Statistics'),
    '#description' => t('User tracking statistics are optional.'),
  );
  $form['stats'][FB_USER_APP_VAR_TRACK_USERS] = array(
    '#type' => 'checkbox',
    '#title' => t('Store data for regular Facebook users who authorize your app.'),
    '#default_value' => variable_get(FB_USER_APP_VAR_TRACK_USERS, TRUE),
  );
  $form['stats'][FB_USER_APP_VAR_TRACK_PAGES] = array(
    '#type' => 'checkbox',
    '#title' => t('Store the ids of Facebook pages when your application is added to a page.'),
    '#default_value' => variable_get(FB_USER_APP_VAR_TRACK_PAGES, TRUE),
  );
  $form['stats'][FB_USER_APP_VAR_TRACK_EVERY_PAGE] = array(
    '#type' => 'checkbox',
    '#title' => t("Store time of user's most recent visit to the application. This adds some overhead to every request."),
    '#default_value' => variable_get(FB_USER_APP_VAR_TRACK_EVERY_PAGE, FALSE),
  );
  $form['stats'][FB_USER_APP_VAR_USERS_THAT_GRANT_OFFLINE] = array(
    '#type' => 'checkbox',
    '#title' => t("Store user data <strong>only</strong> when users have granted offline_access extended permission. For example, to perform cron jobs using those users' access tokens."),
    '#default_value' => variable_get(FB_USER_APP_VAR_USERS_THAT_GRANT_OFFLINE, FALSE),
    '#weight' => -1,
  );

  return system_settings_form($form);
}

