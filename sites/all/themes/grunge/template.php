<?php
// $Id: template.php,v 1.1 2010/09/11 20:26:56 atelier Exp $

/**
* Breadcrumb modification
*/
function grunge_breadcrumb($breadcrumb) {
  if (!empty($breadcrumb)) {
    return '<div class="breadcrumb"><span class="sep">' . t('You are here &raquo;') . '</span>' . implode(' &raquo; ', $breadcrumb) . '</div>';
  }
}
