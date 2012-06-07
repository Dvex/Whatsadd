<?php

/**
 * @file fb_rules.rules.inc
 * Rules integration for the Drupal for Facebook module.
 */

/**
 * Implementation of hook_rules_event_info().
 * @ingroup rules
 */
function fb_rules_rules_event_info() {
  return array(
    'fb_user_connected' => array(
      'label' => t('User has connected his Facebook account'),
      'module' => 'Drupal for Facebook',
      'arguments' => array(
        'account' => array(
          'type' => 'user',
          'label' => t('User allowed Facebook App access')
        ),
      ),
    ),
    'fb_user_disconnected' => array(
      'label' => t('User has disconnected his Facebook account.'),
      'module' => 'Drupal for Facebook',
      'arguments' => array(
        'account' => array(
          'type' => 'user',
          'label' => t('User revoked Facebook App access.')
        ),
      ),
    ),
    'fb_user_login' => array(
      'label' => t('User has logged in via Facebook'),
      'module' => 'Drupal for Facebook',
      'arguments' => array(
        'account' => array(
          'type' => 'user',
          'label' => t('User who has logged in via Facebook.')
        ),
      ),
    ),
    'fb_invite_submitted' => array(
      'label' => t('User submitted Facebook invitation'),
      'module' => 'Drupal for Facebook',
      'arguments' => array(
        'account' => array(
          'type' => 'user',
          'label' => t('User who submitted invitation.')
        ),
      ),
    ),
    'fb_invite_accepted' => array(
      'label' => t('UserA accepted UserB Facebook invitation'),
      'module' => 'Drupal for Facebook',
      'arguments' => array(
        'account' => array(
          'type' => 'user',
          'label' => t('UserB (inviter).')
        ),
      ),
    ),
  );
}

