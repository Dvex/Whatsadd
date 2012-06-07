// $Id: node_expire.js,v 1.3 2009/01/29 21:02:05 brmassa Exp $

Drupal.behaviors.nodeExpire = function(context) {
  var max = Drupal.settings.maxDate;
  if (max) {
    max = new Date(max[0], max[1] -1, max[2]);
  }
  var min = Drupal.settings.minDate;
  min = new Date(min[0], min[1] -1, min[2]);
  $("#edit-expire").datepicker({
    dateFormat: Drupal.settings.dateFormat,
    minDate:    min,
    maxDate:    max
  });
}
