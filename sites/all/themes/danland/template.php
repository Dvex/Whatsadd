<?php
// $Id: template.php,v 1.10 2010/07/19 22:05:33 danprobo Exp $
function phptemplate_body_class($left, $right) {
	if ($left && $right) {
		$class = 'sidebars-2';
		$id = 'sidebar-side-2';	
	}
	else if ($left || $right) {
		$class = 'sidebars-1';
		$id = 'sidebar-side-1';
	}
	
	if(isset($class)) {
		print ' class="'. $class .'"';
	}
		if(isset($id)) {
		print ' id="'. $id .'"';
	}
}

if (drupal_is_front_page()) {
  drupal_add_js(drupal_get_path('theme', 'danland') . '/scripts/jquery.cycle.all.js');
}