<?php


/**
 * Form creator -- ask for confirmation of deletion
 */
function fb_graph_delete_confirm_form(&$form_state, $fb_graph_id) {
  extract(fb_vars());
  $token = fb_get_token($fb, $fbu);
  try {
    $result = fb_graph($fb_graph_id, array(
                         'access_token' => $token,
                       ));
  }
  catch (Exception $e) {
    if (fb_verbose() == 'extreme') {
      fb_log_exception($e, t('Open Graph id %id not found.', array('%id' => $fb_graph_id)));
      drupal_not_found();
      exit();
    }
  }
  if (!count($result)) {
    drupal_not_found();
    exit();
  }

  //dpm($result, __FUNCTION__); // debug

  // Get data from facebook.  Structure bound to change.
  list($namespace, $action_type) = explode(':', $result['type']);
  $object = array_shift($result['data']); // Is this always the object???

  $form['fb_graph_id'] = array(
    '#type' => 'value',
    '#value' => $fb_graph_id,
  );

  // @TODO provide a link to the item on facebook.  I'm not sure how to turn the id into a url.

  return confirm_form(
    $form,
    t('Are you sure you want to remove %action_type of %title from Facebook timeline?', array(
        '%title' => $object['title'],
        '%action_type' => $action_type,
      )),
    isset($_GET['destination']) ? $_GET['destination'] : '<front>', // $object['url'] does not work here???
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

function fb_graph_delete_confirm_form_submit($form, &$form_state) {
  extract(fb_vars());
  $token = fb_get_token($fb, $fbu);
  if ($form_state['values']['confirm']) {
    try {
      // Delete from the graph.
      $result = fb_graph($form_state['values']['fb_graph_id'], array(
                           'access_token' => $token,
                           'method' => 'delete',
                         ), 'POST');
      if ($result) {
        drupal_set_message("Deleted action from Facebook timeline.");
      }
    }
    catch (Exception $e) {
      fb_log_exception($e, t('Failed to delete %id from Facebook graph.', array(
                               '%id' => $form_state['values']['fb_graph_id'],
                             )));
    }
  }
}
