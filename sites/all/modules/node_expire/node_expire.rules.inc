<?php
// $Id: node_expire.rules.inc,v 1.1.2.4 2010/11/10 16:36:58 vikramy Exp $

/**
 * @file
 * Rules module integration.
 */

/**
 * Implementation of hook_rules_action_info().
 * @ingroup rules
 */
function node_expire_rules_action_info() {
  return array(
    'node_expire_unset_expired' => array(
      'arguments' => array(
        'node'      => array('type' => 'node', 'label' => t('content expired')),
      ),
      'label'     => t('Unset the expired flag'),
      'module'    => 'Node',
    ),
   'node_expire_update_lastnotify' => array(
      'arguments' => array(
        'node'      => array('type' => 'node', 'label' => t('content expired')),
      ),
      'label'     => t('Update lastnotify'),
      'module'    => 'Node',
    ),
  );
}

/**
 * Implementation of hook_rules_condition_info().
 * @ingroup rules
 */
function node_expire_rules_condition_info() {
  return array(
    'node_expire_rules_expired_check' => array(
      'arguments' => array(
        'node'      => array('type' => 'node', 'label' => t('Content')),
      ),
      'label'     => t('Content is expired'),
      'help'      => 'Evaluates to TRUE, if the given content has one of the selected content types.',
      'module'    => 'Node',
    ),
    'node_expire_rules_expired_check_lastnotify' => array(
      'arguments' => array(
        'node'      => array('type' => 'node', 'label' => t('Content')),
      ),
      'label'     => t('Content is expired: Check lastnotify'),
      'help'      => 'Evaluates to TRUE, if the given content is expired and lastnotified 2 weeks ago.',
      'module'    => 'Node',
    ),
  );
}

/**
 * Implementation of hook_rules_event_info().
 * @ingroup rules
 */
function node_expire_rules_event_info() {
  return array(
    'node_expired' => array(
      'arguments' => rules_events_node_arguments(t('content expired'), t("content's author")),
      'label'     => t('Content expired'),
      'module'    => 'Node',
    ),
  );
}

/**
 * Check if the node has the the "Expired" flag on.
 *
 * @param $node
 *   Object. The Node object.
 */
function node_expire_rules_expired_check($node) {
  return (!empty($node->expire) && $node->expire <= time() && $node->expired == 1);
}

/**
 * Check if the node has the the "Expired" flag on
 * and lastnotify greater than or equal to 2 weeks.
 * @param $node
 *   Object. The Node object.
 */
function node_expire_rules_expired_check_lastnotify($node) {
  return (!empty($node->expire) && $node->expire <= time() && $node->expired == 1 && $node->lastnotify <= (time() - (14 * 24 * 60 * 60)));
}


/**
 * Unset the "Expired" flag on nodes.
 *
 * @param $nids
 *   Object. The Node object with all node IDs
 *   that should unset the expired flag.
 */
function node_expire_unset_expired($nids) {
  db_query("UPDATE {node_expire} SET expired = '0', lastnotify = '0' WHERE nid = '" . $nids->nid . "'");
}

/**
 * Update lastnotify on nodes.
 *
 * @param $nids
 *   Object. The Node object with all node IDs
 *   that should update lastnotify.
 */
function node_expire_update_lastnotify($nids) {
  db_query("UPDATE {node_expire} SET lastnotify = '" . time() . "' WHERE nid = '" . $nids->nid . "'");
}