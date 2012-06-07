<?php
// $Id: workflow.rules_forms.inc,v 1.1.2.2 2010/10/25 01:56:45 jvandyk Exp $

/**
 * @file
 * Contains configuration forms for workflow rules integration.
 */

/**
 * Configuration form for check transition condition.
 */
function workflow_check_transition_form($settings, &$form) {
  $options = array();
  $options['ANY'] = t('Any state');
  foreach (workflow_get_all() as $wid => $workflow) {
    $options[$workflow] = array();
    foreach (workflow_get_states($wid) as $sid => $state) {
      $options[$workflow][$sid] = $state;
    }
  }
  $form['settings']['from_state'] = array(
    '#type' => 'select',
    '#title' => t('From State'),
    '#options' => $options,
    '#multiple'=> TRUE,
    '#default_value' => isset($settings['from_state']) ? $settings['from_state'] : array(),
    '#required' => TRUE,
  );
  $form['settings']['to_state'] = array(
    '#type' => 'select',
    '#title' => t('To State'),
    '#options' => $options,
    '#multiple'=> TRUE,
    '#default_value' => isset($settings['to_state']) ? $settings['to_state'] : array(),
    '#required' => TRUE,
  );
}

/**
 * Label callback for check transition condition.
 */
function workflow_check_transition_label($settings, $argument_labels) {
    if (in_array('ANY', $settings['from_state'])) $settings['from_state'] = array('ANY');
    if (in_array('ANY', $settings['to_state'])) $settings['to_state'] = array('ANY');
    $from = array();
    $to = array();
   foreach ($settings['from_state'] as $state) {
    if ($state != 'ANY') {
        $fromtemp = workflow_get_state($state);
        $from[] = $fromtemp['state']; 
    } else {
        $from[] = t('Any state');
    }
  }
  foreach ($settings['to_state'] as $state) {
    if ($state != 'ANY') {
        $totemp = workflow_get_state($state);
        $to[] = $totemp['state']; 
    } else {
        $to[] = t('Any state');
    }
  }
  return t('Check workflow transition from @from to @to', array('@from' => implode(', ',$from), '@to' => implode(', ',$to)));
}

/**
 * Configuration form for check state condition.
 */
function workflow_check_state_form($settings, &$form) {
  $options = array();
  $options['ANY'] = t('Any state');
  foreach (workflow_get_all() as $wid => $workflow) {
    $options[$workflow] = array();
    foreach (workflow_get_states($wid) as $sid => $state) {
      $options[$workflow][$sid] = $state;
    }
  }
  $form['settings']['state'] = array(
    '#type' => 'select',
    '#title' => t('State'),
    '#options' => $options,
    '#multiple'=> TRUE,
    '#default_value' => isset($settings['state']) ? $settings['state'] : array(),
    '#required' => TRUE,
  );
}

/**
 * Label callback for check state condition.
 */
function workflow_check_state_label($settings, $argument_labels) {
  if (in_array('ANY', $settings['state'])) {
    $settings['state'] = array('ANY');
  }
  $states = array();
  foreach ($settings['state'] as $state) {
    if ($state != 'ANY') {
        $temp = workflow_get_state($state);
        $states[] = $temp['state']; 
    } else {
        $states[] = t('Any state');
    }
  }
  return t('Check if content workflow state is @state', array('@state' => implode(', ',$states)));
}
