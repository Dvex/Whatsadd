<?php
// $Id: node_expire.nodeapi.inc,v 1.8.2.3 2010/11/30 01:19:01 vikramy Exp $

/**
 * @file
 * Node API integration.
 */

/**
 * Implementation of hook_nodeapi().
 */
function _node_expire_nodeapi(&$ntypes, &$node, $op, $a3 = NULL, $a4 = NULL) {
  switch ($op) {
    case 'load':
      $query = db_query('SELECT expire, expired, lastnotify
        FROM {node_expire} WHERE nid = %d', $node->nid);

      // Use the existing expiration data if present.
      if ($row = db_fetch_object($query)) {
        $node->expire     = $row->expire;
        $node->expired    = $row->expired;
        $node->lastnotify = $row->lastnotify;
      }
      break;

    case 'prepare':
      // To prevent default value 1969-12-31 check also $ntypes['default'].
      if (!isset($node->expire) && $ntypes['default']) {
        $node->expire = format_date(strtotime($ntypes['default']), 'custom', NODE_EXPIRE_FORMAT);
      }
      // This gives a way to users without edit exipration permission to update nodes with default expiration.
      if (isset($node->expire) && !user_access('edit node expire')) {
        $node->expire = format_date(strtotime($ntypes['default']), 'custom', NODE_EXPIRE_FORMAT);
      }
      break;

    case 'validate':
      // The only restriction we have is that the node can't expire in the past.
      if ($node->expire == '') {
        if (!empty($ntypes['required']) && $ntypes['default']) {
          form_set_error('expire_date', t('You must choose an expiration date.'));
        }
      }
      elseif (!$expire = strtotime($node->expire) or $expire <= 0) {
        form_set_error('expire_date', t('You have to specify a valid expiration date.'));
      }
      elseif ($expire <= time()) {
        form_set_error('expire_date', t("You can't expire a node in the past!"));
      }
      elseif (!empty($ntypes['max']) and $expire > strtotime($ntypes['max'], $node->created)) {
        form_set_error('expire_date', t('It must expire before %date.',
        array('%date' => format_date(strtotime($ntypes['max'], $node->created), 'custom', NODE_EXPIRE_FORMAT))));
      }
      break;

    case 'update':
    case 'insert':
      // has the expiration been removed, or does it exist?
      if (isset($node->expire)) {
        db_query('DELETE FROM {node_expire} WHERE nid = %d', $node->nid);
        // should we create a new record?
        if ($node->expire) {
          if (strtotime($node->expire)) {
            $node->expire = strtotime($node->expire);
          }          
          $node->expired = FALSE;
          drupal_write_record('node_expire', $node);
        }
      }
      break;

    case 'delete':
      db_query('DELETE FROM {node_expire} WHERE nid = %d', $node->nid);
      break;
  }
}

function _node_expire_form_alter_nodeform(&$ntypes, &$form, &$form_state, $form_id) {
  // Check if the Node expire feature is enabled for the node type
  $node = isset($form['#node']) ? $form['#node'] : NULL;

  // Convert the timestamp into a human readable date
  if (is_numeric($node->expire)) {
    $node->expire = format_date($node->expire, 'custom', NODE_EXPIRE_FORMAT);
  }
  // This supports node to never expire.
  if (empty($ntypes['default']) && empty($node->expire)) {
    $ntypes['required'] = FALSE;
  }
  if (user_access('edit node expire')) {
    $expire_field = array(
      '#title'         => t('Expiration date'),
      '#description'   => t('Time date to consider the node expired. Format: %time (YYYY-MM-DD).',
        array('%time' => format_date(time(), 'custom', NODE_EXPIRE_FORMAT))),
      '#type'          => 'textfield',
      '#maxlength'     => 25,
      '#required'      => $ntypes['required'],
      '#default_value' => $node->expire,
    );

    // In case jQuery UI module is enabled, use it to
    // get the DatePicker widget.
    if (module_exists('jquery_ui')) {
      jquery_ui_add('ui.datepicker');
      drupal_add_js(drupal_get_path('module', 'node_expire') .'/node_expire.js');
      drupal_add_css(drupal_get_path('module', 'jquery_ui') ."/jquery.ui/themes/default/ui.datepicker.css");
      $min = empty($node->created) ? time() : $node->created;
      $min = array(
        format_date($min, 'custom', 'Y'),
        format_date($min, 'custom', 'm'),
        format_date($min, 'custom', 'd'),
      );
      if (!empty($ntypes['max'])) {
        $max = strtotime($ntypes['max'], (empty($node->created) ? time() : $node->created));
        $max = array(
          format_date($max, 'custom', 'Y'),
          format_date($max, 'custom', 'm'),
          format_date($max, 'custom', 'd'),
        );
      }
      else {
        $max = NULL;
      }
      drupal_add_js(array(
        'dateFormat'  => NODE_EXPIRE_FORMAT_JS,
        'minDate'     => $min,
        'maxDate'     => $max,
      ), 'setting');
    }
  }
  else {
    $expire_field = array(
      '#type'         => 'value',
      '#value'        => $node->expire
    );
  }

  // Put the expire field into 'Publising options' if possible
  if (user_access('administer nodes')) {
    $form['options']['expire']  = &$expire_field;
  }
  else {
    $form['expire']             = &$expire_field;
  }

  if (isset($node->expired)) {
    $form['node_expire'] = array(
      '#type'         => 'value',
      '#value'        => TRUE,
    );
  }
}
