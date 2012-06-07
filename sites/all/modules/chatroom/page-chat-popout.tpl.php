<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">
  <head>
    <?php print $head ?>
    <title><?php print $head_title ?></title>
    <?php print $styles ?>
    <?php print $scripts ?>
    <!--[if lt IE 7]>
      <?php if (function_exists('phptemplate_get_ie_styles')) print phptemplate_get_ie_styles(); ?>
    <![endif]-->
  </head>
  <body class="chatroom-popout<?php if (function_exists('phptemplate_body_class')) print ' ' . phptemplate_body_class($left, $right); ?>">
  <div id="chatroom-popout-title">
    <?php if ($show_messages && $messages): print $messages; endif; ?>
    <?php if ($title): print '<h2'. ($tabs ? ' class="with-tabs"' : '') .'>'. $title .'</h2>'; endif; ?>
  </div>
  <div class="clear-block" id="content">
    <?php print $content ?>
  </div>
  <?php print $closure ?>
  </body>
</html>
